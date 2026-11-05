// ConstructorEnemigo.h  --  PATRON BUILDER
#pragma once

#include "CoreMinimal.h"

class AEnemigoBase;

/** BUILDER: define los pasos para armar un enemigo. */
class IConstructorEnemigo
{
public:
	virtual ~IConstructorEnemigo() {}

	void Iniciar(AEnemigoBase* Enemigo) { Producto = Enemigo; }

	virtual void AsignarVida() = 0;
	virtual void AsignarDano() = 0;
	virtual void AsignarVelocidad() = 0;
	virtual void AsignarTamano() = 0;

	AEnemigoBase* ObtenerEnemigo() const { return Producto; }

protected:
	AEnemigoBase* Producto = nullptr;
};

/** Enemigo comun: 100 de vida, valores base. */
class FConstructorComun : public IConstructorEnemigo
{
public:
	virtual void AsignarVida() override;
	virtual void AsignarDano() override;
	virtual void AsignarVelocidad() override;
	virtual void AsignarTamano() override;
};

/** Enemigo elite: 150 de vida, +25% dano, +15% velocidad, 25% mas grande. */
class FConstructorElite : public IConstructorEnemigo
{
public:
	virtual void AsignarVida() override;
	virtual void AsignarDano() override;
	virtual void AsignarVelocidad() override;
	virtual void AsignarTamano() override;
};

/** Jefe: 300 de vida, +50% dano, mas lento, casi el doble de grande. */
class FConstructorJefe : public IConstructorEnemigo
{
public:
	virtual void AsignarVida() override;
	virtual void AsignarDano() override;
	virtual void AsignarVelocidad() override;
	virtual void AsignarTamano() override;
};

/** DIRECTOR: conoce el ORDEN de los pasos; no sabe que tipo de enemigo arma. */
class FDirectorEnemigos
{
public:
	AEnemigoBase* Construir(IConstructorEnemigo& Constructor, AEnemigoBase* Enemigo) const;
};