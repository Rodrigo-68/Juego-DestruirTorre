#include "GameModeNivelDificil.h"
#include "Engine/World.h"

ATorre* AGameModeNivelDificil::CrearTorre(const FVector& Posicion)
{
	UClass* Clase = ClaseTorreBlindada.Get()
		? ClaseTorreBlindada.Get() : ATorreBlindada::StaticClass();
	ATorre* Torre = GetWorld()->SpawnActor<ATorre>(Clase, Posicion, FRotator::ZeroRotator);

	if (Torre)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Factory Method] CrearTorre() SOBRESCRITO en nivel dificil -> %s"), *Torre->GetName());
	}
	return Torre;
}