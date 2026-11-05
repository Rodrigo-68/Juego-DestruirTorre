// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Armas.generated.h"

class AJugadorTorre;

/** COMPONENTE: toda arma sabe disparar. */
UCLASS(Abstract)
class JUEGOTORRE_API UArma : public UObject
{
    GENERATED_BODY()

public:
    virtual void Disparar(AJugadorTorre* Jugador, const FVector& Direccion) {}
};

/** COMPONENTE CONCRETO: la ametralladora base (20 de dano por bala). */
UCLASS()
class JUEGOTORRE_API UArmaAmetralladora : public UArma
{
    GENERATED_BODY()

public:
    float Dano = 20.f;
    virtual void Disparar(AJugadorTorre* Jugador, const FVector& Direccion) override;
};

/** DECORADOR BASE: envuelve a otra arma y le agrega un efecto temporal. */
UCLASS(Abstract)
class JUEGOTORRE_API UArmaDecorador : public UArma
{
    GENERATED_BODY()

public:
    void Envolver(UArma* Interior, float Ahora);
    virtual void Disparar(AJugadorTorre* Jugador, const FVector& Direccion) override;

protected:
    /** Efecto extra que aporta cada decorador concreto. */
    virtual void EjecutarExtra(AJugadorTorre* Jugador, const FVector& Direccion) {}

    UPROPERTY() UArma* ArmaInterior = nullptr;
    float Duracion = 10.f;        // segundos que dura el bonificador
    float Enfriamiento = 1.f;     // segundos minimos entre usos del extra
    float Inicio = 0.f;
    float UltimoUso = -100.f;
};

/** DECORADOR CONCRETO: rayo laser, 70 de dano en linea recta (atraviesa a todos). */
UCLASS()
class JUEGOTORRE_API UDecoradorLaser : public UArmaDecorador
{
    GENERATED_BODY()

public:
    UDecoradorLaser();

protected:
    virtual void EjecutarExtra(AJugadorTorre* Jugador, const FVector& Direccion) override;

    float Dano = 70.f;
    float Alcance = 2500.f;
};

/** DECORADOR CONCRETO: espada, 50 de dano en abanico de 120 grados. */
UCLASS()
class JUEGOTORRE_API UDecoradorEspada : public UArmaDecorador
{
    GENERATED_BODY()

public:
    UDecoradorEspada();

protected:
    virtual void EjecutarExtra(AJugadorTorre* Jugador, const FVector& Direccion) override;

    float Dano = 50.f;
    float Radio = 350.f;
    float CosMedioAngulo = 0.5f;   // cos(60 grados) -> abanico total de 120 grados
};

