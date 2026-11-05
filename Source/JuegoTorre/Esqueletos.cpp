// Esqueletos.cpp
#include "Esqueletos.h"
#include "Proyectil.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

// ---------------- Mago ----------------
AEsqueletoMago::AEsqueletoMago()
{
	AlcanceAtaque = 1200.f;
	IntervaloAtaque = 2.f;
	Visual->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.9f));
}

void AEsqueletoMago::Atacar(AActor* Objetivo)
{
	if (!Objetivo) return;

	const FVector Origen = GetActorLocation() + GetActorForwardVector() * 80.f;
	const FVector Dir = (Objetivo->GetActorLocation() - Origen).GetSafeNormal();

	FActorSpawnParameters Param;
	Param.Owner = this;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProyectil* Bola = GetWorld()->SpawnActor<AProyectil>(
		AProyectil::StaticClass(), Origen, Dir.Rotation(), Param);
	if (Bola)
	{
		Bola->Inicializar(DanoBolaFuego * MultiplicadorDano, 1200.f, 0.f, 0.f, false, Dir);
	}
}

// ---------------- Cuerpo a cuerpo ----------------
AEsqueletoMelee::AEsqueletoMelee()
{
	AlcanceAtaque = 150.f;
	IntervaloAtaque = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void AEsqueletoMelee::Atacar(AActor* Objetivo)
{
	if (!Objetivo) return;

	UGameplayStatics::ApplyDamage(Objetivo, DanoGolpe * MultiplicadorDano,
		GetController(), this, UDamageType::StaticClass());
}

// ---------------- Aereo ----------------
AEsqueletoAereo::AEsqueletoAereo()
{
	AlcanceAtaque = 250.f;
	IntervaloAtaque = 3.f;

	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->MaxFlySpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1500.f;
	Visual->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.6f));
}

void AEsqueletoAereo::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void AEsqueletoAereo::Atacar(AActor* Objetivo)
{
	const FVector Origen = GetActorLocation() - FVector(0.f, 0.f, 120.f);

	FActorSpawnParameters Param;
	Param.Owner = this;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProyectil* Bomba = GetWorld()->SpawnActor<AProyectil>(
		AProyectil::StaticClass(), Origen, FRotator(-90.f, 0.f, 0.f), Param);
	if (Bomba)
	{
		Bomba->Inicializar(DanoBomba * MultiplicadorDano, 200.f, 1.f, RadioExplosion,
			false, FVector(0.f, 0.f, -1.f));
	}
}

void AEsqueletoAereo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Jugador.IsValid()) return;

	const float ObjetivoZ = Jugador->GetActorLocation().Z + AlturaVuelo;
	const float DeltaZ = ObjetivoZ - GetActorLocation().Z;
	AddMovementInput(FVector::UpVector, FMath::Clamp(DeltaZ / 100.f, -1.f, 1.f));
}