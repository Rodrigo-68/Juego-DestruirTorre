// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JugadorTorre.generated.h"

class UArma;
class ABonificadorBase;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

/** Delegados Observer. */
DECLARE_MULTICAST_DELEGATE(FAlMorirJugador);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAlCambiarVida, float /*Vida*/, float /*Escudo*/);

UCLASS()
class JUEGOTORRE_API AJugadorTorre : public ACharacter
{
    GENERATED_BODY()

public:
    AJugadorTorre();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float Dano, struct FDamageEvent const& Evento,
        AController* Instigador, AActor* Causante) override;

    // --- Efectos de los bonificadores ---
    void AgregarEscudo(float Cantidad);   // +50 de escudo
    void PotenciarLaser();                // envuelve el arma con UDecoradorLaser
    void PotenciarEspada();               // envuelve el arma con UDecoradorEspada

    // --- Observer ---
    FAlMorirJugador AlMorir;
    FAlCambiarVida AlCambiarVida;

    /** Bonificador que esta al alcance (lo marca el propio bonificador al solaparse). */
    UPROPERTY() ABonificadorBase* BonificadorCercano = nullptr;

    UPROPERTY(EditAnywhere, Category = "Jugador") float Vida = 100.f;
    UPROPERTY(VisibleAnywhere, Category = "Jugador") float Escudo = 0.f;
    UPROPERTY(EditAnywhere, Category = "Jugador") float CadenciaDisparo = 0.15f;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere) USpringArmComponent* Brazo;
    UPROPERTY(VisibleAnywhere) UCameraComponent* Camara;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Visual;

    /** Arma actual: empieza siendo la ametralladora y se va "decorando". */
    UPROPERTY() UArma* ArmaActual = nullptr;

    bool bMuerto = false;
    float EjeDisparoX = 0.f;
    float EjeDisparoY = 0.f;
    float UltimoDisparo = -100.f;

    /** Bonificador recogido con E, pendiente de usar con Espacio (uno a la vez). */
    UPROPERTY() ABonificadorBase* BonificadorGuardado = nullptr;

    void MoverAdelante(float Valor);
    void MoverDerecha(float Valor);
    void DispararAdelante(float Valor) { EjeDisparoX = Valor; }
    void DispararDerecha(float Valor) { EjeDisparoY = Valor; }
    void Morir();
    void Recoger();            // tecla E
    void UsarBonificador();    // tecla Espacio
};

