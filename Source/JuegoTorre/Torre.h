// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Torre.generated.h"

class UStaticMeshComponent;

/** Delegado Observer: quien se suscriba sera avisado cuando la torre caiga. */
DECLARE_MULTICAST_DELEGATE(FAlDestruirTorre);

UCLASS()
class JUEGOTORRE_API ATorre : public AActor
{
    GENERATED_BODY()

public:
    ATorre();

    virtual float TakeDamage(float Dano, struct FDamageEvent const& Evento,
        AController* Instigador, AActor* Causante) override;

    /** Los observadores (el GameMode) se suscriben aqui. */
    FAlDestruirTorre AlDestruirTorre;

    UPROPERTY(EditAnywhere, Category = "Torre") float Vida = 300.f;

protected:
    UPROPERTY(VisibleAnywhere) USceneComponent* Raiz;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Visual;
};
