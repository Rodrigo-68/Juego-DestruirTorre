// GameModeNivelDificil.h  --  CREADOR CONCRETO del Factory Method
#pragma once

#include "CoreMinimal.h"
#include "JuegoTorreGameMode.h"
#include "TorreBlindada.h"
#include "GameModeNivelDificil.generated.h"

UCLASS()
class JUEGOTORRE_API AGameModeNivelDificil : public AJuegoTorreGameMode
{
	GENERATED_BODY()

public:
	/** Sobrescribe el Factory Method: aqui la torre es la blindada. */
	virtual ATorre* CrearTorre(const FVector& Posicion) override;

	UPROPERTY(EditAnywhere, Category = "Nivel") TSubclassOf<ATorreBlindada> ClaseTorreBlindada;
};