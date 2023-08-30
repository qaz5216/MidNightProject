// Copyright Epic Games, Inc. All Rights Reserved.

#include "MidNightProjectCharacter.h"
#include "MidNightProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "instrument.h"
#include "ChatWidget.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include "HttpRequestActor.h"
#include "Components/EditableText.h"

//////////////////////////////////////////////////////////////////////////
// AMidNightProjectCharacter

AMidNightProjectCharacter::AMidNightProjectCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AMidNightProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	bSoundkill=false;
	bSoundMake=false;
	if (BGSound!=nullptr)
	{
		SoundPlayer = UGameplayStatics::CreateSound2D(GetWorld(), BGSound);
		SoundPlayer->Play();
		StartSound();
	}
	instrument = Cast<Ainstrument>(UGameplayStatics::GetActorOfClass(this, Ainstrument::StaticClass()));
	httpReqActor = Cast<AHttpRequestActor>(UGameplayStatics::GetActorOfClass(this, AHttpRequestActor::StaticClass()));
}

void AMidNightProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FHitResult HitResult;
	FVector StartV=FirstPersonCameraComponent->GetComponentLocation();
	FVector EndV = StartV+(FirstPersonCameraComponent->GetForwardVector()*1000);

	if (GetWorld()->LineTraceSingleByChannel(HitResult,StartV,EndV,ECollisionChannel::ECC_Visibility))
	{
		AActor* HitActor=HitResult.GetActor();
		Ainstrument* Hitinstrument=Cast<Ainstrument>(HitActor);
		if (Hitinstrument!=nullptr)
		{
			if (Targetinstrument!=Hitinstrument)
			{
				bUishow = false;
				DetachUI();
				Targetinstrument=Hitinstrument;
			}
			if (!bUishow)
			{
				bUishow = true;
				AttachUI();
				UE_LOG(LogTemp, Warning, TEXT("Id=%d"), Hitinstrument->id);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Hitinstrument->infoString);
			}
			if (!bSoundkill && !bSoundMake)
			{
				StopSound();
			}
		}
		else
		{
			Targetinstrument=nullptr;
			bUishow=false;
			DetachUI();
			if (!bSoundkill && !bSoundMake)
			{
				StopSound();
			}
		}
	}
	if (bSoundkill)
	{
		SoundKillTime+=DeltaSeconds;
		VolumeSound(1-(SoundKillTime/2));
		if (SoundKillTime>2)
		{
			VolumeSound(0);
			bSoundkill=false;
			SoundKillTime=0;
			SoundPlayer->Stop();
			if (Targetinstrument!=nullptr&&Targetinstrument->instrumentSound!=nullptr)
			{	
				SoundPlayer = UGameplayStatics::CreateSound2D(GetWorld(), Targetinstrument->instrumentSound);
				Chat_UI->text_sound->SetVisibility(ESlateVisibility::Visible);
				Chat_UI->text_sound->SetText(FText::FromString(FText::AsNumber(Targetinstrument->id).ToString()+FString("Playing..")));

			}
			else
			{
				SoundPlayer = UGameplayStatics::CreateSound2D(GetWorld(), BGSound);
				Chat_UI->text_sound->SetVisibility(ESlateVisibility::Hidden);
			}
			SoundPlayer->Play();
			StartSound();
		}
	}
	if (bSoundMake)
	{
		SoundMakeTime+=DeltaSeconds;
		VolumeSound((SoundMakeTime / 2));
		if (SoundMakeTime>2)
		{
			VolumeSound(1);
			bSoundMake=false;
			SoundMakeTime=0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AMidNightProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMidNightProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMidNightProjectCharacter::Look);

		//Enter
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Triggered, this, &AMidNightProjectCharacter::Enter);
	}
}


void AMidNightProjectCharacter::AttachUI()
{
	if (Chat_Widget != nullptr)
	{
		Chat_UI = CreateWidget<UChatWidget>(GetWorld(), Chat_Widget);
		if (Chat_UI!=nullptr)
		{
			Chat_UI->text_id->SetText(FText::AsNumber(Targetinstrument->id));
			Chat_UI->text_Answer->SetVisibility(ESlateVisibility::Hidden);
			Chat_UI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("UIAttach"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UINullzz"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetNullzz"));
	}
}

void AMidNightProjectCharacter::DetachUI()
{
	if (Chat_UI!=nullptr)
	{
		Chat_UI->RemoveFromParent();
		APlayerController* PlayerC = Cast<APlayerController>(Controller);
		if (PlayerC != nullptr)
		{
			PlayerC->SetInputMode(FInputModeGameOnly());
			PlayerC->SetShowMouseCursor(false);
			bSearching = false;
		}
		UE_LOG(LogTemp, Warning, TEXT("UIDetach"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UINullzzdetach"));
	}
}

void AMidNightProjectCharacter::Enter()
{
	if (!bSearching)
	{	
		APlayerController* PlayerC=Cast<APlayerController>(Controller);
		if (PlayerC!=nullptr)
		{
			
			PlayerC->SetShowMouseCursor(true);
			bSearching = true;
		}
	}
	else
	{
		//서버에 edit_text에 있는 텍스트를 보낸다.
		//Chat_UI->text_edit->
		APlayerController* PlayerC = Cast<APlayerController>(Controller);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, FString::Printf(TEXT("else")));
		if (PlayerC != nullptr && httpReqActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, FString::Printf(TEXT("Inif")));
			PlayerC->SetInputMode(FInputModeGameAndUI());
			PlayerC->SetShowMouseCursor(false);
			bSearching = false;

			FString fullPath = serverurl + "/answer" + "?id=" + FText::AsNumber(instrument->id).ToString() + "&content=" + Chat_UI->Etext_Q->GetText().ToString();

			
			Chat_UI->text_waiting->SetVisibility(ESlateVisibility::Visible);
			httpReqActor->PostRequset(fullPath);

		}
	}
}

void AMidNightProjectCharacter::StartSound()
{
	SoundMakeTime=0;
	bSoundMake = true;
}

void AMidNightProjectCharacter::StopSound()
{
	SoundKillTime=0;
	bSoundkill=true;
}

void AMidNightProjectCharacter::VolumeSound(float Value)
{
	SoundPlayer->SetVolumeMultiplier(Value);
}

void AMidNightProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMidNightProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	if (!bSearching)
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void AMidNightProjectCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMidNightProjectCharacter::GetHasRifle()
{
	return bHasRifle;
}