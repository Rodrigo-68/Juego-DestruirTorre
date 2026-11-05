#include "TorreBlindada.h"

ATorreBlindada::ATorreBlindada()
{
	Vida = 450.f;
}

float ATorreBlindada::TakeDamage(float Dano, FDamageEvent const& Evento,
	AController* Instigador, AActor* Causante)
{
	// Cada bala hace 30% menos de dano.
	return Super::TakeDamage(Dano * (1.f - ReduccionDano), Evento, Instigador, Causante);
}