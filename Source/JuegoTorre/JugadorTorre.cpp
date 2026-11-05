// Fill out your copyright notice in the Description page of Project Settings.

#include "JugadorTorre.h"
#include "Armas.h"
#include "Bonificadores.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

AJugadorTorre::AJugadorTorre()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
    bUseControllerRotationYaw = false;

    // Malla provisional (esfera).
    Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Visual->SetupAttachment(RootComponent);
    Visual->SetCollisionProfileName(TEXT("NoCollision"));
    Visual->SetRelativeScale3D(FVector(0.7f, 0.7f, 1.2f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Esfera(
        TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (Esfera.Succeeded()) Visual->SetStaticMesh(Esfera.Object);

    // Camara cenital fija: el norte (+X) queda hacia arriba de la pantalla.
    Brazo = CreateDefaultSubobject<USpringArmComponent>(TEXT("Brazo"));
    Brazo->SetupAttachment(RootComponent);
    Brazo->SetUsingAbsoluteRotation(true);
    Brazo->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
    Brazo->TargetArmLength = 1600.f;
    Brazo->bDoCollisionTest = false;

    Camara = CreateDefaultSubobject<UCameraComponent>(TEXT("Camara"));
    Camara->SetupAttachment(Brazo, USpringArmComponent::SocketName);
    Camara->bUsePawnControlRotation = false;
}

void AJugadorTorre::BeginPlay()
{
    Super::BeginPlay();
    ArmaActual = NewObject<UArmaAmetralladora>(this);   // arma base sin decorar
}

void AJugadorTorre::SetupPlayerInputComponent(UInputComponent* Entrada)
{
    Super::SetupPlayerInputComponent(Entrada);

    // Mismos ejes que ya define la plantilla Twin Stick: WASD mover, flechas disparar.
    Entrada->BindAxis("MoveForward", this, &AJugadorTorre::MoverAdelante);
    Entrada->BindAxis("MoveRight", this, &AJugadorTorre::MoverDerecha);
    Entrada->BindAxis("FireForward", this, &AJugadorTorre::DispararAdelante);
    Entrada->BindAxis("FireRight", this, &AJugadorTorre::DispararDerecha);

    // Teclas directas (no hace falta tocar la configuracion de Input del editor).
    Entrada->BindAction("Recoger", IE_Pressed, this, &AJugadorTorre::Recoger);
    Entrada->BindAction("UsarBonificador", IE_Pressed, this, &AJugadorTorre::UsarBonificador);
}

void AJugadorTorre::MoverAdelante(float Valor)
{
    if (!bMuerto && Valor != 0.f) AddMovementInput(FVector::ForwardVector, Valor);
}

void AJugadorTorre::MoverDerecha(float Valor)
{
    if (!bMuerto && Valor != 0.f) AddMovementInput(FVector::RightVector, Valor);
}

void AJugadorTorre::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bMuerto) return;

    const FVector Dir(EjeDisparoX, EjeDisparoY, 0.f);
    if (Dir.SizeSquared() < 0.01f) return;

    SetActorRotation(Dir.Rotation());

    const float Ahora = GetWorld()->GetTimeSeconds();
    if (ArmaActual && Ahora - UltimoDisparo >= CadenciaDisparo)
    {
        UltimoDisparo = Ahora;
        ArmaActual->Disparar(this, Dir.GetSafeNormal());   // ejecuta toda la cadena Decorator
    }
}

float AJugadorTorre::TakeDamage(float Dano, FDamageEvent const& Evento,
    AController* Instigador, AActor* Causante)
{
    if (bMuerto || Dano <= 0.f) return 0.f;

    // El escudo absorbe el dano antes que la vida.
    float Resto = Dano;
    if (Escudo > 0.f)
    {
        const float Absorbido = FMath::Min(Escudo, Resto);
        Escudo -= Absorbido;
        Resto -= Absorbido;
    }
    Vida = FMath::Max(0.f, Vida - Resto);

    UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El jugador (sujeto) notifica: AlCambiarVida"));
    AlCambiarVida.Broadcast(Vida, Escudo);   // Observer
    if (Vida <= 0.f) Morir();
    return Dano;
}

void AJugadorTorre::Morir()
{
    bMuerto = true;
    SetActorHiddenInGame(true);
    UE_LOG(LogTemp, Warning, TEXT("[PATRON Observer] El jugador (sujeto) notifica: AlMorir"));
    AlMorir.Broadcast();                     // Observer: el GameMode muestra "DERROTA"
}

// ---------------- Recoger (E) y usar (Espacio) ----------------
void AJugadorTorre::Recoger()
{
    if (bMuerto || !BonificadorCercano || BonificadorGuardado) return;

    BonificadorGuardado = BonificadorCercano;
    BonificadorCercano = nullptr;
    BonificadorGuardado->SetActorHiddenInGame(true);
    BonificadorGuardado->SetActorEnableCollision(false);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green,
            TEXT("Bonificador guardado: presiona ESPACIO para usarlo"));
    }
}

void AJugadorTorre::UsarBonificador()
{
    if (bMuerto || !BonificadorGuardado) return;

    BonificadorGuardado->Aplicar(this);   // laser, escudo o espada
    BonificadorGuardado->Destroy();
    BonificadorGuardado = nullptr;
}

// ---------------- Efectos de los bonificadores ----------------
void AJugadorTorre::AgregarEscudo(float Cantidad)
{
    Escudo += Cantidad;
    AlCambiarVida.Broadcast(Vida, Escudo);
}

void AJugadorTorre::PotenciarLaser()
{
    UDecoradorLaser* Deco = NewObject<UDecoradorLaser>(this);
    Deco->Envolver(ArmaActual, GetWorld()->GetTimeSeconds());
    ArmaActual = Deco;                       // el arma ahora es: Laser( Ametralladora )
}

void AJugadorTorre::PotenciarEspada()
{
    UDecoradorEspada* Deco = NewObject<UDecoradorEspada>(this);
    Deco->Envolver(ArmaActual, GetWorld()->GetTimeSeconds());
    ArmaActual = Deco;                       // Espada( Laser( Ametralladora ) ) si ya tenia laser
}


