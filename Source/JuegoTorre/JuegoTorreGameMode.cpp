// JuegoTorreGameMode.cpp
#include "JuegoTorreGameMode.h"
#include "JugadorTorre.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AJuegoTorreGameMode::AJuegoTorreGameMode()
{
	DefaultPawnClass = AJugadorTorre::StaticClass();
}

// ---------------- SINGLETON ----------------
AJuegoTorreGameMode* AJuegoTorreGameMode::Get(const UObject* ContextoMundo)
{
	AJuegoTorreGameMode* GM = ContextoMundo
		? Cast<AJuegoTorreGameMode>(UGameplayStatics::GetGameMode(ContextoMundo))
		: nullptr;

	if (GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Singleton] Get() dio acceso al GameMode unico: %s"), *GM->GetName());
	}
	return GM;
}

void AJuegoTorreGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Singleton] Instancia unica de GameMode: %s"), *GetName());

	// FACADE: un solo llamado arma todo el nivel.
	Fachada = MakeUnique<FFachadaJuego>(this);
	Fachada->IniciarPartida();

	// OBSERVER: el GameMode escucha al jugador.
	if (AJugadorTorre* Jugador = Cast<AJugadorTorre>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Jugador->Vida = VidaInicialJugador;   // vida segun el nivel
		Jugador->AlMorir.AddUObject(this, &AJuegoTorreGameMode::Derrota);
		Jugador->AlCambiarVida.AddUObject(this, &AJuegoTorreGameMode::MostrarVida);
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El GameMode se suscribio a AlMorir y AlCambiarVida del jugador"));

		MostrarVida(Jugador->Vida, Jugador->Escudo);
	}
}

// ---------------- FACTORY METHOD (version base) ----------------
ATorre* AJuegoTorreGameMode::CrearTorre(const FVector& Posicion)
{
	UClass* Clase = ClaseTorre.Get() ? ClaseTorre.Get() : ATorre::StaticClass();
	ATorre* Torre = GetWorld()->SpawnActor<ATorre>(Clase, Posicion, FRotator::ZeroRotator);

	if (Torre)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PATRON Factory Method] CrearTorre() version base -> %s"), *Torre->GetName());
	}
	return Torre;
}

// ---------------- Observer: el GameMode reacciona ----------------
void AJuegoTorreGameMode::Victoria()
{
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El GameMode (observador) recibio AlDestruirTorre -> VICTORIA"));
	if (Fachada) Fachada->FinalizarPartida(true);
}

void AJuegoTorreGameMode::Derrota()
{
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El GameMode (observador) recibio AlMorir -> DERROTA"));
	if (Fachada) Fachada->FinalizarPartida(false);
}

void AJuegoTorreGameMode::MostrarVida(float Vida, float Escudo)
{
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El GameMode (observador) recibio AlCambiarVida: Vida=%.0f"), Vida);
	if (Fachada) Fachada->MostrarVida(Vida, Escudo);
}

void AJuegoTorreGameMode::RegistrarBaja()
{
	if (Fachada) Fachada->RegistrarBaja();
}

void AJuegoTorreGameMode::Reiniciar()
{
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void AJuegoTorreGameMode::IrAlSiguienteNivel()
{
	UGameplayStatics::OpenLevel(this, SiguienteNivel);
}