// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MidNightProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AMidNightProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	
public:
	AMidNightProjectCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick( float DeltaSeconds );

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EnterAction;


	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
	TSubclassOf<class UChatWidget> Chat_Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
	class UChatWidget* Chat_UI;
	UPROPERTY()
	bool bUishow=false;

	UPROPERTY()
	bool bSearching=false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
	class Ainstrument* Targetinstrument;

	UFUNCTION(BlueprintCallable)
	void AttachUI();

	UFUNCTION(BlueprintCallable)
	void DetachUI();

	UFUNCTION(BlueprintCallable)
	void Enter();
	UFUNCTION(BlueprintCallable)
	void StartSound();
	UFUNCTION(BlueprintCallable)
	void StopSound();
	
	UFUNCTION(BlueprintCallable)
	void VolumeSound(float Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=info)
	USoundBase* BGSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=info)
	UAudioComponent* SoundPlayer;
	float SoundMakeTime=0;
	float SoundKillTime=0;
	float SoundVolume=0;
	bool bSoundkill=false;
	bool bSoundMake=false;

	UPROPERTY()
   class AHttpRequestActor* httpReqActor;

   class Ainstrument* instrument;

   FString serverurl = "http://192.168.1.14:8000";

};

