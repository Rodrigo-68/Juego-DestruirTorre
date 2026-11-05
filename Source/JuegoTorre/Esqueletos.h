// Esqueletos.h  --  los 3 enemigos concretos (cada uno define su ataque)
#pragma once

#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "Esqueletos.generated.h"

/** Esqueleto mago: lanza bolas de fuego desde lejos. */
UCLASS()
class JUEGOTORRE_API AEsqueletoMago : public AEnemigoBase
{
	GENERATED_BODY()

public:
	AEsqueletoMago();

	UPROPERTY(EditAnywhere, Category = "Ataque") float DanoBolaFuego = 20.f;

protected:
	virtual void Atacar(AActor* Objetivo) override;
};

/** Esqueleto cuerpo a cuerpo: persigue y golpea. */
UCLASS()
class JUEGOTORRE_API AEsqueletoMelee : public AEnemigoBase
{
	GENERATED_BODY()

public:
	AEsqueletoMelee();

	UPROPERTY(EditAnywhere, Category = "Ataque") float DanoGolpe = 10.f;

protected:
	virtual void Atacar(AActor* Objetivo) override;
};

/** Esqueleto aereo: vuela bajo y suelta bombas. */
UCLASS()
class JUEGOTORRE_API AEsqueletoAereo : public AEnemigoBase
{
	GENERATED_BODY()

public:
	AEsqueletoAereo();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Enemigo") float AlturaVuelo = 80.f;
	UPROPERTY(EditAnywhere, Category = "Ataque") float DanoBomba = 30.f;
	UPROPERTY(EditAnywhere, Category = "Ataque") float RadioExplosion = 350.f;

protected:
	virtual void BeginPlay() override;
	virtual void Atacar(AActor* Objetivo) override;
};