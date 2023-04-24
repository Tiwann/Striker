// (C) Erwann Messoah 2023

#pragma once
#include "GameFramework/Pawn.h"
#include "StrikerCharacter.generated.h"

class UArrowComponent;
class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
struct FInputActionValue;

UCLASS()
class STRIKER_API AStrikerCharacter : public APawn
{
	GENERATED_BODY()

public:
	AStrikerCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Striker Character|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Striker Character|Components")
	TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character|Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character|Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character")
	float MoveSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Striker Character")
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Striker Character")
	uint8 bEnableRotationSmooth : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Striker Character")
	float RotationSmoothSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character")
	uint8 bCanSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character")
	uint8 bSprinting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character")
	float SprintMax = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Striker Character")
	float SprintCurrent = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Striker Character", meta=(UIMin=1.0, UIMax=2.0, ClampMin=1.0, ClampMax=2.0, Units="times"))
	float SprintMultiplier = 1.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Striker Character")
	float SprintProcessed;

	

protected:
	virtual void BeginPlay() override;
	void MoveActionTriggered(const FInputActionValue& ActionValue);
	void MoveActionCompleted(const FInputActionValue& ActionValue);
	void SprintActionTriggered(const FInputActionValue& ActionValue);
	void SprintActionCompleted(const FInputActionValue& ActionValue);
	void SprintActionStarted(const FInputActionValue& ActionValue);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
