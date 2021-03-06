// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JHero.generated.h"

UCLASS()
class PLAYGROUNDHEROES_API AJHero : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Helper for LockCamera function, only runs when the camera can be locked onto an actor
	void LockCameraHelper();

public:
	// Sets default values for this character's properties
	AJHero();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float LockCamRate;

	FORCEINLINE bool GetAttacking() const { return bAttacking; }

	FORCEINLINE bool GetDodging() const { return bDodging; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(Category = "Combat")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool AttackHelper();

	UFUNCTION(Category = "Combat")
	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool DodgeHelper();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// Handler for when the camera is to be locked
	void LockCamera();

	// True when the camera is locked
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JCamera")
	bool bIsLocked;

	/*
		True when the last player input was attack
		Set to false after InputQueueTime seconds after input
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bInputtingAttack;

	/*
	True when the player is currently in an attack animation
	and is consequently invulnerable
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bAttacking;

	/*
		True when the last player input was dodge
		Set to false after InputQueueTime seconds after input
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bInputtingDodge;

	/*
	True when the player is currently in the dodge animation
	and is consequently invulnerable
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bDodging;

	/*
		The amount of time, in seconds, since the last player input
		-1 when there has been no input in the last InputQueueTime seconds
			OR if the input has been handled
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float TimeSinceLastInput;

	/*
		The maximum amount of time, in seconds, that we will keep
		track of player input
		default = .3f (.3 seconds)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InputQueueTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Stamina;

	/*
		The amount of stamina generated every second
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float StaminaGen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCost;

	/*
		The direction the player is currently inputting on the left stick
		Note: This value is not normalized. If the player is inputting left: x = -1.0, y = 0, z = 0
			z is always 0
			The amount is dependent on how much they are tilting. .5 would be 50% tilt. Half tilt towards NE is x = .5, y = .5
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FVector InputDirection;

	/*
		A pointer to the target you are currently locked on to
		NULL when you are not locked on
	*/
	class AJEnemy* lockTarget;

};
