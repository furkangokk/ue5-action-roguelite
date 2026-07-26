// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELITE_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};