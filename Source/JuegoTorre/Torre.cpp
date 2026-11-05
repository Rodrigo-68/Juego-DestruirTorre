// Fill out your copyright notice in the Description page of Project Settings.


#include "Torre.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ATorre::ATorre()
{
    Raiz = CreateDefaultSubobject<USceneComponent>(TEXT("Raiz"));
    RootComponent = Raiz;

    // Torre provisional: cubo alto de 300 x 300 x 800. El origen del actor es la base.
    Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Visual->SetupAttachment(Raiz);
    Visual->SetRelativeLocation(FVector(0.f, 0.f, 400.f));
    Visual->SetRelativeScale3D(FVector(3.f, 3.f, 8.f));
    Visual->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cubo(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cubo.Succeeded()) Visual->SetStaticMesh(Cubo.Object);
}

float ATorre::TakeDamage(float Dano, FDamageEvent const& Evento,
    AController* Instigador, AActor* Causante)
{
    if (Vida <= 0.f) return 0.f;

    const float Real = Super::TakeDamage(Dano, Evento, Instigador, Causante);
    Vida -= Real;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Yellow,
            FString::Printf(TEXT("Torre: %.0f"), FMath::Max(Vida, 0.f)));
    }

    if (Vida <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] La torre (sujeto) notifica: AlDestruirTorre"));
        AlDestruirTorre.Broadcast();   // Observer: notifica a todos los suscritos
        Destroy();
    }
    return Real;
}


