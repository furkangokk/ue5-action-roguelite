// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "DrawDebugHelpers.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);

}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);

}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	// ---------------------------------------------------------
	// 1. ADIM: Iþýný Doðrudan Kameranýn Lokasyonundan ve Rotasyonundan Baþlat
	// (Önceki GetActorEyesViewPoint TPS için yanlýþ veri veriyor)
	// ---------------------------------------------------------

	FVector CameraLocation;
	FRotator CameraRotation;

	// Karakter sýnýfýnýn içindeki Kamera Komponentini kullan:
	// 'FollowCamera' üçüncü þahýs þablonlarýndaki varsayýlan isimdir.
	if (CameraComp)
	{
		CameraLocation = CameraComp->GetComponentLocation();
		CameraRotation = CameraComp->GetComponentRotation();
	}
	else
	{
		// Güvenlik: Eðer kamera yoksa (garip), varsayýlana dön
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
	}

	// ---------------------------------------------------------
	// 2. ADIM: Trace Mesafesini Artýr
	// ---------------------------------------------------------
	float TraceDistance = 10000.0f; // Mesafeyi 10 metre yerine ~100 metreye çýkar
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * TraceDistance);

	FHitResult HitResult;

	// ---------------------------------------------------------
	// 3. ADIM: Çarpýþma Kanalýný Deðiþtir
	// ---------------------------------------------------------
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Karakterin kendisini yoksay

	// WorldDynamic yerine ECC_Visibility kullan.
	// Visibility, sadece ekranda görünen objelere çarpar.
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECollisionChannel::ECC_Visibility,
		QueryParams
	);

	// ---------------------------------------------------------
	// 4. ADIM: Hedef Lokasyonu Belirle
	// ---------------------------------------------------------
	FVector TargetLocation = bHit ? HitResult.ImpactPoint : TraceEnd;

	// ---------------------------------------------------------
	// 5. ADIM: Mermiyi Namludan Çýkart
	// ---------------------------------------------------------
	FVector SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	// ---------------------------------------------------------
	// 6. ADIM: Merminin Rotasyonunu Hesapla (Bu kýsým slayttakiyle ayný)
	// ---------------------------------------------------------
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	// ---------------------------------------------------------
	// QA Check: Debug Line Ekle (image_2.png'dekini düzeltmek için)
	// ---------------------------------------------------------
	// Kýrmýzý çizgi tam crosshair'ýn ortasýna gitmelidir.
	DrawDebugLine(GetWorld(), TraceStart, TargetLocation, FColor::Red, false, 2.0f, 0, 2.0f);
}


void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}



// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::StopJumping);
}
