#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "DashProjectile.generated.h"

UCLASS()
class ACTIONROGUELITE_API ADashProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ADashProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle TimerHandle_DelayedDetonate;
	void Explode();

	FTimerHandle TimerHandle_DelayedTeleport;
	void TeleportInstigator();
};