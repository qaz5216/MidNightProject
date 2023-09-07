// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTPROJECT_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = info)
	class UEditableText* Etext_Q;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = info)
	class UTextBlock* text_Answer;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = info)
	class UTextBlock* text_sound;
	

};
