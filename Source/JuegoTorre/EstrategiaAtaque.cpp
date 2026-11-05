// EstrategiaAtaque.cpp
#include "EstrategiaAtaque.h"
#include "EnemigoBase.h"
#include "Proyectil.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

// ---------- Estrategia 1: bola de fuego (mago) ----------
void UEstrategiaBolaFuego::Atacar(AEnemigoBase* Atacante, AActor* Objetivo)
{
    if (!Atacante || !Objetivo) return;

    const FVector Origen = Atacante->GetActorLocation()
        + Atacante->GetActorForwardVector() * 80.f;
    const FVector Dir = (Objetivo->GetActorLocation() - Origen).GetSafeNormal();

    FActorSpawnParameters Param;
    Param.Owner = Atacante;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AProyectil* Bola = Atacante->GetWorld()->SpawnActor<AProyectil>(
        AProyectil::StaticClass(), Origen, Dir.Rotation(), Param);
    if (Bola)
    {
        // (dano, velocidad, gravedad, radio, delJugador, direccion)
        Bola->Inicializar(Dano * Atacante->MultiplicadorDano, 1200.f, 0.f, 0.f, false, Dir);
    }
}

// ---------- Estrategia 2: golpe cuerpo a cuerpo ----------
void UEstrategiaGolpeMelee::Atacar(AEnemigoBase* Atacante, AActor* Objetivo)
{
    if (!Atacante || !Objetivo) return;

    UGameplayStatics::ApplyDamage(Objetivo, Dano * Atacante->MultiplicadorDano,
        Atacante->GetController(), Atacante, UDamageType::StaticClass());
}

// ---------- Estrategia 3: bomba desde la nave ----------
void UEstrategiaBomba::Atacar(AEnemigoBase* Atacante, AActor* Objetivo)
{
    if (!Atacante) return;

    const FVector Origen = Atacante->GetActorLocation() - FVector(0.f, 0.f, 120.f);

    FActorSpawnParameters Param;
    Param.Owner = Atacante;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AProyectil* Bomba = Atacante->GetWorld()->SpawnActor<AProyectil>(
        AProyectil::StaticClass(), Origen, FRotator(-90.f, 0.f, 0.f), Param);
    if (Bomba)
    {
        // Cae por gravedad (escala 1) y explota en area al tocar algo.
        Bomba->Inicializar(Dano * Atacante->MultiplicadorDano, 200.f, 1.f, RadioExplosion,
            false, FVector(0.f, 0.f, -1.f));
    }
}
