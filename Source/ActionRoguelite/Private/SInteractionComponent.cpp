// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include <SGameplayInterface.h>
#include <DrawDebugHelpers.h>



// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;

	// 1. Karakterin bakýþ açýsýný alýyoruz
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// 2. Start ve End noktalarýný DOÐRU HESAPLIYORUZ
	FVector Start = EyeLocation; // Çizgi gözden çýkacak
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f); // Baktýðý yöne doðru 1000 birim ileri gidecek

	FHitResult Hit;

	// 3. Çizgiyi (LineTrace) oluþturduðumuz Start ve End ile atýyoruz
	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		if (HitActor->Implements<USGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);

			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}

	// 4. Debug çizgisini de doðru End noktasýna çizdiriyoruz
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);
}