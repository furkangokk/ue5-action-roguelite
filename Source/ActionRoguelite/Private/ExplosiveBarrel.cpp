#include "ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/CollisionProfile.h"
#include "SMagicProjectile.h"
// Yeni eklenen kütüphaneler:
#include "NiagaraComponent.h" 
#include "TimerManager.h" 

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	bHasExploded = false;
	bIsIgnited = false; // Baþlangýçta fitil ateþlenmedi

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	MeshComp->SetNotifyRigidBodyCollision(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	ForceComp->AddCollisionChannelToAffect(ECC_PhysicsBody);

	ExplosionComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionComp"));
	ExplosionComp->bAutoActivate = false;
	ExplosionComp->SetupAttachment(MeshComp);

	FlamesFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlamesFXComp"));
	FlamesFXComp->bAutoActivate = false;
	FlamesFXComp->SetupAttachment(MeshComp);
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Zaten patladýysa hiçbir þey yapma
	if (bHasExploded) return;

	// Sadece bizim Büyü Mermimiz (SMagicProjectile) çarptýysa iþlemi baþlat
	if (OtherActor && OtherActor->IsA(ASMagicProjectile::StaticClass()))
	{
		if (!bIsIgnited)
		{
			// 1. DURUM: Varil ilk defa vuruldu
			bIsIgnited = true;

			// Zamanlayýcýyý baþlat: 2.0 saniye sonra Explode() fonksiyonunu çaðýr
			GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &AExplosiveBarrel::Explode, 2.0f, false);

			// Opsiyonel: Fitilin ateþlendiðini göstermek için varilin rengini deðiþtirebilir veya duman çýkarabilirsin.
		}
		else
		{
			// 2. DURUM: Varil zaten vurulmuþtu, süre dolmadan BÝR DAHA vuruldu

			// Bekleyen zamanlayýcýyý iptal et
			GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);

			// Anýnda patlat
			Explode();
		}
	}
}

void AExplosiveBarrel::Explode()
{
	if (bHasExploded) return;
	bHasExploded = true;

	// Fiziksel itme gücünü uygula
	ForceComp->FireImpulse();

	// 1. Ana Patlama Efektini oynat
	if (ExplosionComp)
	{
		ExplosionComp->Activate(true);
	}

	// 2. Yerde kalacak Alev Efektini baþlat
	if (FlamesFXComp)
	{
		FlamesFXComp->Activate(true);
	}

	// Varilin kendisini gizle ve çarpýþmasýný kapat (Böylece mermiler veya oyuncular içinden geçebilir)
	MeshComp->SetVisibility(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ÖNEMLÝ: Aktörü hemen dünyadan silmiyoruz. Alevlerin yanmaya devam etmesi için 10 saniye ömür biçiyoruz.
	// 10 saniye sonra varil aktörü tamamen silinecek ve alevler de kaybolacaktýr.
	SetLifeSpan(10.0f);
}