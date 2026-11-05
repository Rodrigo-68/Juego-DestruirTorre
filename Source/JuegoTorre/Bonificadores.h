// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bonificadores.generated.h"

class AJugadorTorre;
class USphereComponent;
class UStaticMeshComponent;

/** Bonificador base: el jugador lo recoge con E y lo usa con Espacio. */
UCLASS(Abstract)
class JUEGOTORRE_API ABonificadorBase : public AActor
{
    GENERATED_BODY()

public:
    ABonificadorBase();
    virtual void Tick(float DeltaTime) override;

    /** Efecto del bonificador. Lo llama el jugador al usarlo (tecla Espacio). */
    virtual void Aplicar(AJugadorTorre* Jugador) {}

protected:
    UPROPERTY(VisibleAnywhere) USphereComponent* Zona;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Visual;

    UFUNCTION()
    void AlSolapar(UPrimitiveComponent* Comp, AActor* Otro, UPrimitiveComponent* OtroComp,
        int32 Indice, bool bDesdeBarrido, const FHitResult& Barrido);

    UFUNCTION()
    void AlSalir(UPrimitiveComponent* Comp, AActor* Otro, UPrimitiveComponent* OtroComp,
        int32 Indice);
};

/** Rayo laser: 70 de dano en linea recta (usa el decorador UDecoradorLaser). */
UCLASS()
class JUEGOTORRE_API ABonificadorLaser : public ABonificadorBase
{
    GENERATED_BODY()

public:
    virtual void Aplicar(AJugadorTorre* Jugador) override;
};

/** Escudo: +50 puntos que absorben dano antes que la vida. */
UCLASS()
class JUEGOTORRE_API ABonificadorEscudo : public ABonificadorBase
{
    GENERATED_BODY()

public:
    virtual void Aplicar(AJugadorTorre* Jugador) override;
};

/** Espada: 50 de dano en abanico (usa el decorador UDecoradorEspada). */
UCLASS()
class JUEGOTORRE_API ABonificadorEspada : public ABonificadorBase
{
    GENERATED_BODY()

public:
    virtual void Aplicar(AJugadorTorre* Jugador) override;
};

