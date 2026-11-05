#include "FachadaJuego.h"
#include "JuegoTorreGameMode.h"
#include "Torre.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

FFachadaJuego::FFachadaJuego(AJuegoTorreGameMode* InGameMode)
	: GameMode(InGameMode)
{
}

void FFachadaJuego::IniciarPartida()
{
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Facade] IniciarPartida(): una sola llamada coordina fabrica, builder y torre"));

	CrearFabrica();
	PoblarNivel();
}

void FFachadaJuego::CrearFabrica()
{
	// ABSTRACT FACTORY: se elige la familia de enemigos.
	if (GameMode->bUsarTemaHielo)
	{
		Fabrica = MakeUnique<FFabricaHielo>(GameMode->TemaHielo);
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Abstract Factory] Fabrica concreta elegida: FFabricaHielo"));
	}
	else
	{
		Fabrica = MakeUnique<FFabricaFuego>(GameMode->TemaFuego);
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Abstract Factory] Fabrica concreta elegida: FFabricaFuego"));
	}
}

void FFachadaJuego::PoblarNivel()
{
	UWorld* Mundo = GameMode->GetWorld();
	if (!Mundo || !Fabrica) return;

	const float ZS = GameMode->ZSuelo;
	const float Largo = GameMode->LargoNivel;

	// FACTORY METHOD: el GameMode (o su subclase) decide que torre crear.
	ATorre* Torre = GameMode->CrearTorre(FVector(Largo, 0.f, ZS));
	if (Torre)
	{
		Torre->AlDestruirTorre.AddUObject(GameMode, &AJuegoTorreGameMode::Victoria);
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El GameMode se suscribio a AlDestruirTorre"));
	}

	const ETipoBonificador Ciclo[] = { ETipoBonificador::Laser, ETipoBonificador::Escudo,
		ETipoBonificador::Espada, ETipoBonificador::Escudo };

	const float Ancho = GameMode->AnchoNivel * 0.4f;

	// Zonas del camino (posicion X) donde aparecen enemigos segun el nivel.
	TArray<float> Zonas;
	if (GameMode->bZonaInicio) Zonas.Add(1700.f);
	if (GameMode->bZonaMedio)  Zonas.Add((1700.f + (Largo - 800.f)) * 0.5f);
	if (GameMode->bZonaTorre)  Zonas.Add(Largo - 800.f);

	UE_LOG(LogTemp, Warning, TEXT("[NIVEL %d] Vida jugador=%.0f | Zonas: Inicio=%d Medio=%d Torre=%d"),
		GameMode->NumeroNivel, GameMode->VidaInicialJugador,
		GameMode->bZonaInicio ? 1 : 0, GameMode->bZonaMedio ? 1 : 0, GameMode->bZonaTorre ? 1 : 0);

	// BUILDER: el Director arma el enemigo con la receta indicada.
	auto Armar = [this](AEnemigoBase* E, IConstructorEnemigo& Receta)
		{
			if (!E) return;
			Director.Construir(Receta, E);
			E->MultiplicadorDano *= GameMode->MultiplicadorDano;
			E->GetCharacterMovement()->MaxWalkSpeed *= GameMode->MultiplicadorVelocidad;
			E->GetCharacterMovement()->MaxFlySpeed *= GameMode->MultiplicadorVelocidad;
		};

	for (int32 z = 0; z < Zonas.Num(); ++z)
	{
		// La zona mas cercana a la torre tiene enemigos mas fuertes segun el nivel.
		const bool bUltimaZona = (z == Zonas.Num() - 1);
		IConstructorEnemigo* Receta = &Comun;
		if (bUltimaZona && GameMode->NumeroNivel >= 3)      Receta = &Jefe;
		else if (bUltimaZona && GameMode->NumeroNivel >= 2) Receta = &Elite;

		UE_LOG(LogTemp, Warning, TEXT("[NIVEL %d] Zona %d en X=%.0f"),
			GameMode->NumeroNivel, z + 1, Zonas[z]);

		// Un bonificador antes de cada zona.
		Fabrica->CrearBonificador(Mundo, Ciclo[z % 4],
			FVector(Zonas[z] - 400.f, FMath::RandRange(-Ancho, Ancho), ZS + 100.f));

		for (int32 g = 0; g < FMath::Max(GameMode->GruposPorZona, 1); ++g)
		{
			const float X = Zonas[z] + g * 300.f;

			Armar(Fabrica->CrearMago(Mundo,
				FVector(X, FMath::RandRange(-Ancho, Ancho), ZS + 100.f)), *Receta);
			Armar(Fabrica->CrearMelee(Mundo,
				FVector(X - 200.f, FMath::RandRange(-Ancho, Ancho), ZS + 100.f)), *Receta);
			Armar(Fabrica->CrearAereo(Mundo,
				FVector(X, FMath::RandRange(-Ancho, Ancho), ZS + 170.f)), *Receta);
		}
	}
}

void FFachadaJuego::MostrarMensaje(int32 Clave, float Tiempo, const FColor& Color,
	const FString& Texto, float Escala)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Clave, Tiempo, Color, Texto, true,
			FVector2D(Escala, Escala));
	}
}

void FFachadaJuego::MostrarVida(float Vida, float Escudo)
{
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Facade] MostrarVida(): Vida=%.0f Escudo=%.0f"), Vida, Escudo);

	MostrarMensaje(2, 10.f, FColor::White,
		FString::Printf(TEXT("Vida: %.0f   Escudo: %.0f"), Vida, Escudo));
}

void FFachadaJuego::RegistrarBaja()
{
	++EnemigosEliminados;
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Facade] RegistrarBaja(): eliminados = %d"), EnemigosEliminados);

	MostrarMensaje(3, 10.f, FColor::Cyan,
		FString::Printf(TEXT("Esqueletos eliminados: %d"), EnemigosEliminados));
}

void FFachadaJuego::FinalizarPartida(bool bGano)
{
	if (bTerminado) return;
	bTerminado = true;

	const bool bHaySiguiente = bGano && !GameMode->SiguienteNivel.IsNone();

	FString Texto = TEXT("DERROTA");
	if (bGano)
	{
		Texto = bHaySiguiente
			? FString::Printf(TEXT("NIVEL %d SUPERADO"), GameMode->NumeroNivel)
			: FString(TEXT("VICTORIA - JUEGO COMPLETADO"));
	}
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Facade] FinalizarPartida(): %s"), *Texto);
	MostrarMensaje(-1, 5.f, bGano ? FColor::Green : FColor::Red, Texto, 3.f);

	if (bHaySiguiente)
	{
		GameMode->GetWorldTimerManager().SetTimer(Temporizador, GameMode,
			&AJuegoTorreGameMode::IrAlSiguienteNivel, 5.f, false);
	}
	else
	{
		GameMode->GetWorldTimerManager().SetTimer(Temporizador, GameMode,
			&AJuegoTorreGameMode::Reiniciar, 5.f, false);
	}
}