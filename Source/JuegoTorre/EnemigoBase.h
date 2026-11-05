// EnemigoBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemigoBase.generated.h"

class UStaticMeshComponent;
class UAnimSequenceBase;

/** Enemigo base (100 de vida). Cada subclase define COMO ataca en Atacar(). */
UCLASS(Abstract)
class JUEGOTORRE_API AEnemigoBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemigoBase();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Dano, struct FDamageEvent const& Evento,
		AController* Instigador, AActor* Causante) override;

	UPROPERTY(EditAnywhere, Category = "Enemigo") float Vida = 100.f;
	UPROPERTY(EditAnywhere, Category = "Enemigo") float AlcanceAtaque = 150.f;
	UPROPERTY(EditAnywhere, Category = "Enemigo") float IntervaloAtaque = 2.f;
	UPROPERTY(EditAnywhere, Category = "Enemigo") float DistanciaActivacion = 2500.f;
	UPROPERTY(EditAnywhere, Category = "Enemigo") float MultiplicadorDano = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemigo")
	UAnimSequenceBase* AnimMuerte = nullptr;

protected:
	virtual void BeginPlay() override;

	/** Cada tipo de esqueleto implementa su propio ataque. */
	virtual void Atacar(AActor* Objetivo) {}

	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Visual;

	TWeakObjectPtr<AActor> Jugador;
	FTimerHandle TimerAtaque;

	void IntentarAtacar();
	void Morir();
};