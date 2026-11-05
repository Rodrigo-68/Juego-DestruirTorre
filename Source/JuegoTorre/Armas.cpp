// Fill out your copyright notice in the Description page of Project Settings.


#include "Armas.h"
#include "JugadorTorre.h"
#include "Proyectil.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

// ---------------- Componente concreto: ametralladora ----------------
void UArmaAmetralladora::Disparar(AJugadorTorre* Jugador, const FVector& Direccion)
{
    if (!Jugador) return;

    const FVector Origen = Jugador->GetActorLocation() + Direccion * 70.f;

    FActorSpawnParameters Param;
    Param.Owner = Jugador;
    Param.Instigator = Jugador;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AProyectil* Bala = Jugador->GetWorld()->SpawnActor<AProyectil>(
        AProyectil::StaticClass(), Origen, Direccion.Rotation(), Param);
    if (Bala)
    {
        Bala->Inicializar(Dano, 2000.f, 0.f, 0.f, true, Direccion);
    }
}

void UArmaDecorador::Envolver(UArma* Interior, float Ahora)
{
    ArmaInterior = Interior;
    Inicio = Ahora;
}

void UArmaDecorador::Disparar(AJugadorTorre* Jugador, const FVector& Direccion)
{
    // 1) Primero hace lo que ya hacia el arma envuelta (la bala, u otros decoradores).
    if (ArmaInterior) ArmaInterior->Disparar(Jugador, Direccion);

    // 2) Luego agrega su efecto, mientras el bonificador siga vigente.
    const float Ahora = Jugador->GetWorld()->GetTimeSeconds();
    if (Ahora - Inicio > Duracion) return;
    if (Ahora - UltimoUso < Enfriamiento) return;

    UltimoUso = Ahora;
    EjecutarExtra(Jugador, Direccion);
}

UDecoradorLaser::UDecoradorLaser()
{
    Duracion = 10.f;
    Enfriamiento = 1.f;
}

void UDecoradorLaser::EjecutarExtra(AJugadorTorre* Jugador, const FVector& Direccion)
{
    UWorld* Mundo = Jugador->GetWorld();
    const FVector Ini = Jugador->GetActorLocation();
    const FVector Fin = Ini + Direccion * Alcance;

    // Trazo por tipo de objeto: atraviesa y golpea a todos los que estan en la linea.
    FCollisionObjectQueryParams Objetos;
    Objetos.AddObjectTypesToQuery(ECC_Pawn);
    Objetos.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams Query;
    Query.AddIgnoredActor(Jugador);

    TArray<FHitResult> Impactos;
    Mundo->LineTraceMultiByObjectType(Impactos, Ini, Fin, Objetos, Query);

    TSet<AActor*> Golpeados;
    for (const FHitResult& H : Impactos)
    {
        AActor* A = H.GetActor();
        if (A && !Golpeados.Contains(A))
        {
            Golpeados.Add(A);
            UGameplayStatics::ApplyDamage(A, Dano, Jugador->GetController(), Jugador,
                UDamageType::StaticClass());
        }
    }
    DrawDebugLine(Mundo, Ini, Fin, FColor::Red, false, 0.3f, 0, 8.f);
}

UDecoradorEspada::UDecoradorEspada()
{
    Duracion = 10.f;
    Enfriamiento = 0.8f;
}

void UDecoradorEspada::EjecutarExtra(AJugadorTorre* Jugador, const FVector& Direccion)
{
    UWorld* Mundo = Jugador->GetWorld();
    const FVector Pos = Jugador->GetActorLocation();

    FCollisionObjectQueryParams Objetos;
    Objetos.AddObjectTypesToQuery(ECC_Pawn);
    Objetos.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams Query;
    Query.AddIgnoredActor(Jugador);

    TArray<FOverlapResult> Resultados;
    Mundo->OverlapMultiByObjectType(Resultados, Pos, FQuat::Identity, Objetos,
        FCollisionShape::MakeSphere(Radio), Query);

    // Abanico: solo los que estan dentro del angulo frente al jugador.
    TSet<AActor*> Golpeados;
    for (const FOverlapResult& R : Resultados)
    {
        AActor* A = R.GetActor();
        if (!A || Golpeados.Contains(A)) continue;

        const FVector Hacia = (A->GetActorLocation() - Pos).GetSafeNormal2D();
        if (FVector::DotProduct(Direccion, Hacia) >= CosMedioAngulo)
        {
            Golpeados.Add(A);
            UGameplayStatics::ApplyDamage(A, Dano, Jugador->GetController(), Jugador,
                UDamageType::StaticClass());
        }
    }

    // Dibujo del abanico (5 lineas cada 30 grados).
    for (int32 i = -2; i <= 2; ++i)
    {
        const FVector D = Direccion.RotateAngleAxis(i * 30.f, FVector::UpVector);
        DrawDebugLine(Mundo, Pos, Pos + D * Radio, FColor::Cyan, false, 0.3f, 0, 4.f);
    }
}
