// JuegoTorreGameMode.h
//  SINGLETON (Get), FACTORY METHOD (CrearTorre), FACADE (Fachada), OBSERVER.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FabricaEnemigos.h"
#include "FachadaJuego.h"
#include "Torre.h"
#include "JuegoTorreGameMode.generated.h"

UCLASS()
class JUEGOTORRE_API AJuegoTorreGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJuegoTorreGameMode();

	/** SINGLETON: acceso global al unico GameMode. */
	static AJuegoTorreGameMode* Get(const UObject* ContextoMundo);

	/** FACTORY METHOD: las subclases deciden que torre crear. */
	virtual ATorre* CrearTorre(const FVector& Posicion);

	/** Lo llaman los enemigos al morir. */
	void RegistrarBaja();

	// Callbacks de los observadores.
	void Victoria();
	void Derrota();
	void MostrarVida(float Vida, float Escudo);

	void Reiniciar();
	void IrAlSiguienteNivel();

	// --- Configuracion del nivel ---
	UPROPERTY(EditAnywhere, Category = "Nivel") float LargoNivel = 9000.f;
	UPROPERTY(EditAnywhere, Category = "Nivel") float AnchoNivel = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Nivel") float ZSuelo = 0.f;
	UPROPERTY(EditAnywhere, Category = "Nivel") int32 NumGrupos = 4;
	UPROPERTY(EditAnywhere, Category = "Nivel") bool bUsarTemaHielo = false;
	UPROPERTY(EditAnywhere, Category = "Nivel") TSubclassOf<ATorre> ClaseTorre;

	UPROPERTY(EditAnywhere, Category = "Temas") FConfigTema TemaFuego;
	UPROPERTY(EditAnywhere, Category = "Temas") FConfigTema TemaHielo;

	// --- Dificultad ---
	UPROPERTY(EditAnywhere, Category = "Dificultad") int32 NumeroNivel = 1;
	UPROPERTY(EditAnywhere, Category = "Dificultad") float MultiplicadorDano = 1.f;
	UPROPERTY(EditAnywhere, Category = "Dificultad") float MultiplicadorVelocidad = 1.f;
	/** Mapa siguiente. Vacio (None) = ultimo nivel. Ej: NivelTorre2 */
	UPROPERTY(EditAnywhere, Category = "Dificultad") FName SiguienteNivel = NAME_None;
	UPROPERTY(EditAnywhere, Category = "Dificultad") float VidaInicialJugador = 500.f;

	// --- Zonas donde aparecen enemigos ---
	UPROPERTY(EditAnywhere, Category = "Zonas") bool bZonaInicio = true;
	UPROPERTY(EditAnywhere, Category = "Zonas") bool bZonaMedio = false;
	UPROPERTY(EditAnywhere, Category = "Zonas") bool bZonaTorre = false;
	UPROPERTY(EditAnywhere, Category = "Zonas") int32 GruposPorZona = 1;

protected:
	virtual void BeginPlay() override;

private:
	TUniquePtr<FFachadaJuego> Fachada;
};