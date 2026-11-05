// Fill out your copyright notice in the Description page of Project Settings.


#include "FabricaEnemigos.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

template <typename T>
T* FFabricaBase::Crear(UWorld* Mundo, TSubclassOf<T> Clase, const FVector& Pos)
{
    if (!Mundo) return nullptr;

    // Si no se asigno un Blueprint, usa la clase C++ por defecto.
    UClass* Cual = Clase.Get() ? Clase.Get() : T::StaticClass();

    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    return Mundo->SpawnActor<T>(Cual, Pos, FRotator::ZeroRotator, Param);
}

AEnemigoBase* FFabricaBase::CrearMago(UWorld* Mundo, const FVector& Pos)
{
    AEnemigoBase* E = Crear(Mundo, Config.ClaseMago, Pos);
    if (E) Personalizar(E);
    return E;
}

AEnemigoBase* FFabricaBase::CrearMelee(UWorld* Mundo, const FVector& Pos)
{
    AEnemigoBase* E = Crear(Mundo, Config.ClaseMelee, Pos);
    if (E) Personalizar(E);
    return E;
}

AEnemigoBase* FFabricaBase::CrearAereo(UWorld* Mundo, const FVector& Pos)
{
    AEnemigoBase* E = Crear(Mundo, Config.ClaseAereo, Pos);
    if (E) Personalizar(E);
    return E;
}

ABonificadorBase* FFabricaBase::CrearBonificador(UWorld* Mundo, ETipoBonificador Tipo,
    const FVector& Pos)
{
    switch (Tipo)
    {
    case ETipoBonificador::Laser:  return Crear(Mundo, Config.ClaseLaser, Pos);
    case ETipoBonificador::Escudo: return Crear(Mundo, Config.ClaseEscudo, Pos);
    default:                       return Crear(Mundo, Config.ClaseEspada, Pos);
    }
}

// ---------------- Familia FUEGO ----------------
void FFabricaFuego::Personalizar(AEnemigoBase* Enemigo)
{
    Enemigo->MultiplicadorDano = 1.25f;
    UE_LOG(LogTemp, Warning, TEXT("[PATRON Abstract Factory] FFabricaFuego creo a %s (+25%% dano)"), *Enemigo->GetName());
}

// ---------------- Familia HIELO ----------------
void FFabricaHielo::Personalizar(AEnemigoBase* Enemigo)
{
    UCharacterMovementComponent* Mov = Enemigo->GetCharacterMovement();
    Mov->MaxWalkSpeed *= 0.7f;
    Mov->MaxFlySpeed *= 0.7f;
    UE_LOG(LogTemp, Warning, TEXT("[PATRON Abstract Factory] FFabricaHielo creo a %s (30%% mas lento)"), *Enemigo->GetName());
}


