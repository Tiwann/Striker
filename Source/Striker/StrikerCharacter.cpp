// (C) Erwann Messoah 2023
#include "StrikerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"


AStrikerCharacter::AStrikerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Forward");

	ArrowComponent->SetupAttachment(CapsuleComponent);
	bEnableRotationSmooth = true;

	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->BodyInstance.bLockXRotation = true;
	CapsuleComponent->BodyInstance.bLockYRotation = true;
	
	CapsuleComponent->SetEnableGravity(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	CapsuleComponent->SetLinearDamping(0.0f);
	CapsuleComponent->SetAngularDamping(10.0f);


	bSprinting = false;
	bCanSprint = true;
}

void AStrikerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AStrikerCharacter::MoveActionTriggered(const FInputActionValue& ActionValue)
{
	Direction = ActionValue.Get<FVector>();
}

void AStrikerCharacter::MoveActionCompleted(const FInputActionValue& ActionValue)
{
	Direction = FVector::ZeroVector;
}

void AStrikerCharacter::SprintActionTriggered(const FInputActionValue& ActionValue)
{
	if(SprintCurrent >= 0.0f)
	{
		SprintCurrent -= GetWorld()->GetDeltaSeconds();
		bSprinting = true;
		SprintProcessed = FMath::Lerp(1.0f, SprintMultiplier, ActionValue.Get<float>());
	} else
	{
		SprintCurrent = 0.0f;
		bSprinting = false;
	}
}

void AStrikerCharacter::SprintActionCompleted(const FInputActionValue& ActionValue)
{
	bSprinting = false;
	if(SprintCurrent <= SprintMax)
	{
		SprintCurrent += GetWorld()->GetDeltaSeconds();
	} else
	{
		SprintCurrent = SprintMax;
	}
	
}

void AStrikerCharacter::SprintActionStarted(const FInputActionValue& ActionValue)
{
	
}

void AStrikerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!Direction.IsZero())
	{
		// Movement
		FVector Movement(Direction.Y, Direction.X, Direction.Z);
		Movement.Normalize();
		Movement *= bSprinting ? MoveSpeed * SprintProcessed : MoveSpeed;
		CapsuleComponent->SetPhysicsLinearVelocity(Movement);


		GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Blue, Movement.ToString());

		// Rotation
		const float TargetAngle = FMath::RadiansToDegrees(FMath::Atan2(Direction.X, Direction.Y));
		GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Blue, *FString::Printf(TEXT("%.1f"), TargetAngle));

		
		const FQuat TargetRotation = FQuat::MakeFromEuler(FVector(0.0f, 0.0, TargetAngle));
		const FQuat CurrentRotation = GetActorRotation().Quaternion();
		const FQuat SmootRotation = FMath::QInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSmoothSpeed);
		const FQuat FinalRotation = bEnableRotationSmooth ? SmootRotation : TargetRotation;
		SetActorRotation(FinalRotation);
	}

	
}

void AStrikerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
	const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(MappingContext, 0);

	if(const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStrikerCharacter::MoveActionTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AStrikerCharacter::MoveActionCompleted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AStrikerCharacter::SprintActionTriggered);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AStrikerCharacter::SprintActionCompleted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AStrikerCharacter::SprintActionStarted);
	}
}

