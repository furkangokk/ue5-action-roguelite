// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UProjectileMovementComponent;
class USAttributeComponent;

UCLASS(Abstract) // 'Abstract' marks this class as incomplete, keeping this out of certain dropdowns windows like SpawnActor in Unreal Editor
class ACTIONROGUELITE_API AProjectileBase : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	// 'virtual' so we can override this in child-classes
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = C++ base implementation, can be expanded in Blueprints
	// BlueprintCallable to allow child classes to trigger explosions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;

public:

	AProjectileBase();

};