#include "EnemigoBase.h"
#include "JuegoTorreGameMode.h"
#include "AIController.h"
#include "Animation/AnimSequenceBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AEnemigoBase::AEnemigoBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(40.f, 88.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	bUseControllerRotationYaw = false;

	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	Visual->SetupAttachment(RootComponent);
	Visual->SetCollisionProfileName(TEXT("NoCollision"));
	Visual->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.7f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cubo(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cubo.Succeeded()) Visual->SetStaticMesh(Cubo.Object);
}

void AEnemigoBase::BeginPlay()
{
	Super::BeginPlay();

	Jugador = UGameplayStatics::GetPlayerPawn(this, 0);

	GetWorldTimerManager().SetTimer(TimerAtaque, this, &AEnemigoBase::IntentarAtacar,
		IntervaloAtaque, true, IntervaloAtaque);
}

void AEnemigoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Jugador.IsValid()) return;

	const FVector Hacia = Jugador->GetActorLocation() - GetActorLocation();
	if (Hacia.Size2D() > DistanciaActivacion) return;

	SetActorRotation(FRotator(0.f, Hacia.Rotation().Yaw, 0.f));
	if (Hacia.Size2D() > AlcanceAtaque)
	{
		AddMovementInput(FVector(Hacia.X, Hacia.Y, 0.f).GetSafeNormal());
	}
}

void AEnemigoBase::IntentarAtacar()
{
	if (!Jugador.IsValid()) return;

	const float Dist = FVector::Dist2D(GetActorLocation(), Jugador->GetActorLocation());
	if (Dist <= AlcanceAtaque * 1.1f)
	{
		Atacar(Jugador.Get());
	}
}

float AEnemigoBase::TakeDamage(float Dano, FDamageEvent const& Evento,
	AController* Instigador, AActor* Causante)
{
	if (Vida <= 0.f) return 0.f;

	const float Real = Super::TakeDamage(Dano, Evento, Instigador, Causante);
	Vida -= Real;

	if (Vida <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(TimerAtaque);

		// Singleton: acceso global al unico GameMode.
		if (AJuegoTorreGameMode* GM = AJuegoTorreGameMode::Get(this))
		{
			GM->RegistrarBaja();
		}
		Morir();
	}
	return Real;
}

void AEnemigoBase::Morir()
{
	SetActorTickEnabled(false);

	// Ya no estorba: ignora balas y jugador, pero sigue apoyado en el piso.
	UCapsuleComponent* Capsula = GetCapsuleComponent();
	Capsula->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsula->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// El aereo cae al piso al morir.
	GetCharacterMovement()->GravityScale = 1.f;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	if (AnimMuerte && GetMesh())
	{
		GetMesh()->PlayAnimation(AnimMuerte, false);
		SetLifeSpan(AnimMuerte->GetPlayLength() + 2.f);
	}
	else
	{
		Destroy();
	}
}