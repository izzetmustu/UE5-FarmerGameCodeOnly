// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CropField.generated.h"

UCLASS()
class FARMGAMETASK_API ACropField : public AActor
{
	GENERATED_BODY()
	
public:	
	ACropField();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
