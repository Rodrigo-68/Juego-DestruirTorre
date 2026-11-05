#include "ConstructorEnemigo.h"
#include "EnemigoBase.h"
#include "GameFramework/CharacterMovementComponent.h"

static void EscalarVelocidad(AEnemigoBase* E, float Factor)
{
	UCharacterMovementComponent* Mov = E->GetCharacterMovement();
	Mov->MaxWalkSpeed *= Factor;
	Mov->MaxFlySpeed *= Factor;
}

// ---------------- Comun ----------------
void FConstructorComun::AsignarVida() { Producto->Vida = 100.f; }
void FConstructorComun::AsignarDano() { /* usa el dano base */ }
void FConstructorComun::AsignarVelocidad() { /* usa la velocidad base */ }
void FConstructorComun::AsignarTamano() { Producto->SetActorScale3D(FVector(1.f)); }

// ---------------- Elite ----------------
void FConstructorElite::AsignarVida() { Producto->Vida = 150.f; }
void FConstructorElite::AsignarDano() { Producto->MultiplicadorDano *= 1.25f; }
void FConstructorElite::AsignarVelocidad() { EscalarVelocidad(Producto, 1.15f); }
void FConstructorElite::AsignarTamano() { Producto->SetActorScale3D(FVector(1.25f)); }

// ---------------- Jefe ----------------
void FConstructorJefe::AsignarVida() { Producto->Vida = 300.f; }
void FConstructorJefe::AsignarDano() { Producto->MultiplicadorDano *= 1.5f; }
void FConstructorJefe::AsignarVelocidad() { EscalarVelocidad(Producto, 0.85f); }
void FConstructorJefe::AsignarTamano() { Producto->SetActorScale3D(FVector(1.8f)); }

// ---------------- Director ----------------
AEnemigoBase* FDirectorEnemigos::Construir(IConstructorEnemigo& Constructor,
	AEnemigoBase* Enemigo) const
{
	if (!Enemigo) return nullptr;
	UE_LOG(LogTemp, Warning, TEXT("[PATRON Builder] El Director arma a %s paso a paso: vida, dano, velocidad y tamano"), *Enemigo->GetName());
	Constructor.Iniciar(Enemigo);
	Constructor.AsignarVida();
	Constructor.AsignarDano();
	Constructor.AsignarVelocidad();
	Constructor.AsignarTamano();
	return Constructor.ObtenerEnemigo();
}