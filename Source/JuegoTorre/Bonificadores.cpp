// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonificadores.h"
#include "JugadorTorre.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ABonificadorBase::ABonificadorBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Zona = CreateDefaultSubobject<USphereComponent>(TEXT("Zona"));
    Zona->InitSphereRadius(100.f);
    Zona->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Zona->OnComponentBeginOverlap.AddDynamic(this, &ABonificadorBase::AlSolapar);
    Zona->OnComponentEndOverlap.AddDynamic(this, &ABonificadorBase::AlSalir);
    RootComponent = Zona;

    Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Visual->SetupAttachment(Zona);
    Visual->SetCollisionProfileName(TEXT("NoCollision"));
    Visual->SetRelativeScale3D(FVector(0.6f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cubo(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cubo.Succeeded()) Visual->SetStaticMesh(Cubo.Object);
}

void ABonificadorBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.f, 90.f * DeltaTime, 0.f));   // gira para llamar la atencion
}

// Al acercarse, el jugador queda "marcado" como cercano; NO se aplica nada todavia.
void ABonificadorBase::AlSolapar(UPrimitiveComponent* Comp, AActor* Otro,
    UPrimitiveComponent* OtroComp, int32 Indice, bool bDesdeBarrido, const FHitResult& Barrido)
{
    AJugadorTorre* Jugador = Cast<AJugadorTorre>(Otro);
    if (Jugador)
    {
        Jugador->BonificadorCercano = this;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Yellow,
                TEXT("Presiona E para recoger"));
        }
    }
}

void ABonificadorBase::AlSalir(UPrimitiveComponent* Comp, AActor* Otro,
    UPrimitiveComponent* OtroComp, int32 Indice)
{
    AJugadorTorre* Jugador = Cast<AJugadorTorre>(Otro);
    if (Jugador && Jugador->BonificadorCercano == this)
    {
        Jugador->BonificadorCercano = nullptr;
    }
}

void ABonificadorLaser::Aplicar(AJugadorTorre* Jugador) { Jugador->PotenciarLaser(); }
void ABonificadorEscudo::Aplicar(AJugadorTorre* Jugador) { Jugador->AgregarEscudo(50.f); }
void ABonificadorEspada::Aplicar(AJugadorTorre* Jugador) { Jugador->PotenciarEspada(); }


