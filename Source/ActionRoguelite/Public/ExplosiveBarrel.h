#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class UNiagaraComponent; // Niagara sýnýfý geri eklendi

UCLASS()
class ACTIONROGUELITE_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<URadialForceComponent> ForceComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UNiagaraComponent> ExplosionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UNiagaraComponent> FlamesFXComp;

	// Patlama durumlarý
	bool bHasExploded;
	bool bIsIgnited; // Ýlk vuruþun alýnýp alýnmadýðýný kontrol eder

	// Gecikmeli patlama için zamanlayýcý (Timer) yöneticisi
	FTimerHandle ExplodeTimerHandle;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Explode();

public:

	AExplosiveBarrel();
	virtual void PostInitializeComponents() override;

};