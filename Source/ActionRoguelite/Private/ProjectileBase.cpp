
#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "SAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnActorHit);
	SphereComp->SetNotifyRigidBodyCollision(true);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

void AProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = OtherActor->GetComponentByClass<USAttributeComponent>();
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
		}

		Explode();
	}
}

// _Implementation from it being marked as BlueprintNativeEvent
void AProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(!IsActorBeingDestroyed()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}
