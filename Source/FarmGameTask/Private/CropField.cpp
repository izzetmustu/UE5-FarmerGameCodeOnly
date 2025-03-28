// Fill out your copyright notice in the Description page of Project Settings.


#include "CropField.h"

// Sets default values
ACropField::ACropField()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACropField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACropField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

