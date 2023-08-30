// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "instrument.generated.h"

UCLASS()
class MIDNIGHTPROJECT_API Ainstrument : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Ainstrument();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=info)
	int32 id=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=info)
	FString infoString="";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=info)
	class UStaticMeshComponent* instrumentMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=info)
	class USoundBase* instrumentSound;


};
