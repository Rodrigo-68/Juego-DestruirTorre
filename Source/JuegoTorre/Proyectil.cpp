// Fill out your copyright notice in the Description page of Project Settings.


#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AProyectil::AProyectil()
{
    Esfera = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera"));
    Esfera->InitSphereRadius(20.f);
    Esfera->SetCollisionProfileName(TEXT("Projectile"));
    Esfera->OnComponentHit.AddDynamic(this, &AProyectil::AlImpactar);
    RootComponent = Esfera;

    Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Visual->SetupAttachment(Esfera);
    Visual->SetCollisionProfileName(TEXT("NoCollision"));
    Visual->SetRelativeScale3D(FVector(0.4f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Malla(
        TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (Malla.Succeeded()) Visual->SetStaticMesh(Malla.Object);

    Movimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movimiento"));
    Movimiento->UpdatedComponent = Esfera;
    Movimiento->InitialSpeed = 1500.f;
    Movimiento->MaxSpeed = 3000.f;
    Movimiento->bRotationFollowsVelocity = true;
    Movimiento->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 5.f;
}

void AProyectil::Inicializar(float InDano, float Velocidad, float Gravedad, float Radio,
    bool bDelJugador, const FVector& Direccion)
{
    Dano = InDano;
    RadioExplosion = Radio;
    bEsDelJugador = bDelJugador;

    Movimiento->ProjectileGravityScale = Gravedad;
    Movimiento->Velocity = Direccion.GetSafeNormal() * Velocidad;

    if (GetOwner()) Esfera->IgnoreActorWhenMoving(GetOwner(), true);
}

void AProyectil::AlImpactar(UPrimitiveComponent* HitComp, AActor* Otro,
    UPrimitiveComponent* OtroComp, FVector Impulso, const FHitResult& Hit)
{
    if (Otro == GetOwner()) return;

    // "Amigo": bala del jugador -> el propio jugador; proyectil enemigo -> otros enemigos.
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(this, 0);
    const bool bAmigo = bEsDelJugador
        ? (Otro == Jugador)
        : (Otro && Otro->IsA<APawn>() && Otro != Jugador);

    if (!bAmigo)
    {
        if (RadioExplosion > 0.f)
        {
            // Bomba: dano en area (sin atenuacion).
            TArray<AActor*> Ignorar;
            if (GetOwner()) Ignorar.Add(GetOwner());
            UGameplayStatics::ApplyRadialDamage(this, Dano, GetActorLocation(), RadioExplosion,
                UDamageType::StaticClass(), Ignorar, this, nullptr, true);
        }
        else if (Otro)
        {
            // Bala o bola de fuego: dano directo -> llama a TakeDamage del actor golpeado.
            UGameplayStatics::ApplyDamage(Otro, Dano, nullptr, this, UDamageType::StaticClass());
        }
    }
    Destroy();
}

