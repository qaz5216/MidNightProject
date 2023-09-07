// Fill out your copyright notice in the Description page of Project Settings.


#include "instrument.h"

// Sets default values
Ainstrument::Ainstrument()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

}

// Called when the game starts or when spawned
void Ainstrument::BeginPlay()
{
	Super::BeginPlay();
	StartRot=GetActorRotation();
}

// Called every frame
void Ainstrument::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bRot)
	{
		if (bRotYaw)
		{
			plusRot+=60*DeltaTime;
			FRotator GoRot=StartRot;
			GoRot.Yaw+=plusRot;
			SetActorRotation(GoRot);
		}
		else
		{
			plusRot+=30*DeltaTime;
			FRotator GoRot=StartRot;
			GoRot.Roll += plusRot;
			SetActorRotation(GoRot);

		}
		if (plusRot>=360)
		{
			bRot=false;
			SetActorRotation(StartRot);
		}
	}
}

void Ainstrument::FixRot()
{
	if (!bNoRot)
	{
		if (!bRot)
		{
			bRot = true;
			plusRot = 0;
		}
	}
}

