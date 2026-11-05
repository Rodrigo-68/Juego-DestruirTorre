// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Proyectil.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

/** Un solo proyectil para todo: bala (20), bola de fuego y bomba (con radio de explosion). */
UCLASS()
class JUEGOTORRE_API AProyectil : public AActor
{
    GENERATED_BODY()

public:
    AProyectil();

    /** Se llama justo despues de crearlo para configurar dano, velocidad, etc. */
    void Inicializar(float InDano, float Velocidad, float Gravedad, float Radio,
        bool bDelJugador, const FVector& Direccion);

protected:
    UPROPERTY(VisibleAnywhere) USphereComponent* Esfera;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Visual;
    UPROPERTY(VisibleAnywhere) UProjectileMovementComponent* Movimiento;

    float Dano = 20.f;
    float RadioExplosion = 0.f;
    bool bEsDelJugador = false;

    UFUNCTION()
    void AlImpactar(UPrimitiveComponent* HitComp, AActor* Otro, UPrimitiveComponent* OtroComp,
        FVector Impulso, const FHitResult& Hit);
};
