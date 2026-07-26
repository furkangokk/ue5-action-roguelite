#include "DashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ADashProjectile::ADashProjectile()
{
}

void ADashProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Çarpýþma algýlayýcýsýný (OnHit) delegeye baðla
	SphereComp->OnComponentHit.AddDynamic(this, &ADashProjectile::OnHit);

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADashProjectile::Explode, 0.2f, false);
}

void ADashProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	Explode();
}

void ADashProjectile::Explode()
{

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false); 

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADashProjectile::TeleportInstigator, 0.2f, false);
}

void ADashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();

	if (ActorToTeleport)
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}

	Destroy();
}