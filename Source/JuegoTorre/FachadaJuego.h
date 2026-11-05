// PATRON FACADE
#pragma once

#include "CoreMinimal.h"
#include "FabricaEnemigos.h"
#include "ConstructorEnemigo.h"
#include "TimerManager.h"

class AJuegoTorreGameMode;

/**
 * FACADE: interfaz simple. Por dentro coordina la fabrica (Abstract Factory),
 * el Director y los constructores (Builder), la torre (Factory Method),
 * los mensajes en pantalla y el cambio de nivel.
 */
class FFachadaJuego
{
public:
	explicit FFachadaJuego(AJuegoTorreGameMode* InGameMode);

	void IniciarPartida();
	void MostrarVida(float Vida, float Escudo);
	void RegistrarBaja();
	void FinalizarPartida(bool bGano);

private:
	AJuegoTorreGameMode* GameMode;

	// Subsistemas ocultos
	TUniquePtr<IFabricaEnemigos> Fabrica;
	FDirectorEnemigos Director;
	FConstructorComun Comun;
	FConstructorElite Elite;
	FConstructorJefe Jefe;

	int32 EnemigosEliminados = 0;
	bool bTerminado = false;
	FTimerHandle Temporizador;

	void CrearFabrica();
	void PoblarNivel();
	void MostrarMensaje(int32 Clave, float Tiempo, const FColor& Color,
		const FString& Texto, float Escala = 1.f);
};