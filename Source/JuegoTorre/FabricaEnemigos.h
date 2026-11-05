// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TiposJuego.h"
#include "Esqueletos.h"
#include "Bonificadores.h"
#include "FabricaEnemigos.generated.h"

/** Configuracion de un "tema": que clases (o Blueprints hijos) usa su familia de productos. */
USTRUCT(BlueprintType)
struct FConfigTema
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AEsqueletoMago>    ClaseMago;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AEsqueletoMelee>   ClaseMelee;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AEsqueletoAereo>   ClaseAereo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<ABonificadorLaser>  ClaseLaser;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<ABonificadorEscudo> ClaseEscudo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<ABonificadorEspada> ClaseEspada;
};

/** FABRICA ABSTRACTA: crea una familia completa de productos relacionados. */
class IFabricaEnemigos
{
public:
    virtual ~IFabricaEnemigos() {}

    virtual AEnemigoBase* CrearMago(UWorld* Mundo, const FVector& Pos) = 0;
    virtual AEnemigoBase* CrearMelee(UWorld* Mundo, const FVector& Pos) = 0;
    virtual AEnemigoBase* CrearAereo(UWorld* Mundo, const FVector& Pos) = 0;
    virtual ABonificadorBase* CrearBonificador(UWorld* Mundo, ETipoBonificador Tipo,
        const FVector& Pos) = 0;
};

/** Parte comun de las fabricas concretas: crea el actor y llama a Personalizar(). */
class FFabricaBase : public IFabricaEnemigos
{
public:
    explicit FFabricaBase(const FConfigTema& InConfig) : Config(InConfig) {}

    virtual AEnemigoBase* CrearMago(UWorld* Mundo, const FVector& Pos) override;
    virtual AEnemigoBase* CrearMelee(UWorld* Mundo, const FVector& Pos) override;
    virtual AEnemigoBase* CrearAereo(UWorld* Mundo, const FVector& Pos) override;
    virtual ABonificadorBase* CrearBonificador(UWorld* Mundo, ETipoBonificador Tipo,
        const FVector& Pos) override;

protected:
    /** Lo que diferencia a cada familia (tema). */
    virtual void Personalizar(AEnemigoBase* Enemigo) = 0;

    FConfigTema Config;

private:
    template <typename T>
    T* Crear(UWorld* Mundo, TSubclassOf<T> Clase, const FVector& Pos);
};

/** FABRICA CONCRETA 1: esqueletos de fuego (+25% de dano). */
class FFabricaFuego : public FFabricaBase
{
public:
    using FFabricaBase::FFabricaBase;

protected:
    virtual void Personalizar(AEnemigoBase* Enemigo) override;
};

/** FABRICA CONCRETA 2: esqueletos de hielo (30% mas lentos). */
class FFabricaHielo : public FFabricaBase
{
public:
    using FFabricaBase::FFabricaBase;

protected:
    virtual void Personalizar(AEnemigoBase* Enemigo) override;
};

