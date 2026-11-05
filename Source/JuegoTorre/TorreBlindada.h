// TorreBlindada.h  --  PRODUCTO del Factory Method
#pragma once

#include "CoreMinimal.h"
#include "Torre.h"
#include "TorreBlindada.generated.h"

/** Torre mas resistente: 450 de vida y recibe 30% menos de dano. */
UCLASS()
class JUEGOTORRE_API ATorreBlindada : public ATorre
{
	GENERATED_BODY()

public:
	ATorreBlindada();

	virtual float TakeDamage(float Dano, struct FDamageEvent const& Evento,
		AController* Instigador, AActor* Causante) override;

	UPROPERTY(EditAnywhere, Category = "Torre") float ReduccionDano = 0.3f;
};