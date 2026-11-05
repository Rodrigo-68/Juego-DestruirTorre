// EstrategiaAtaque.h  --  PATRON STRATEGY
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EstrategiaAtaque.generated.h"

class AEnemigoBase;

/** Interfaz comun: el enemigo delega "como ataca" en una estrategia intercambiable. */
UCLASS(Abstract)
class JUEGOTORRE_API UEstrategiaAtaque : public UObject
{
    GENERATED_BODY()

public:
    virtual void Atacar(AEnemigoBase* Atacante, AActor* Objetivo) {}
};

/** Mago: lanza una bola de fuego en linea recta hacia el jugador. */
UCLASS()
class JUEGOTORRE_API UEstrategiaBolaFuego : public UEstrategiaAtaque
{
    GENERATED_BODY()

public:
    float Dano = 20.f;
    virtual void Atacar(AEnemigoBase* Atacante, AActor* Objetivo) override;
};

/** Cuerpo a cuerpo: golpea directamente al objetivo. */
UCLASS()
class JUEGOTORRE_API UEstrategiaGolpeMelee : public UEstrategiaAtaque
{
    GENERATED_BODY()

public:
    float Dano = 10.f;
    virtual void Atacar(AEnemigoBase* Atacante, AActor* Objetivo) override;
};

/** Aereo: suelta una bomba que cae y explota con dano en area. */
UCLASS()
class JUEGOTORRE_API UEstrategiaBomba : public UEstrategiaAtaque
{
    GENERATED_BODY()

public:
    float Dano = 30.f;
    float RadioExplosion = 350.f;
    virtual void Atacar(AEnemigoBase* Atacante, AActor* Objetivo) override;
};
