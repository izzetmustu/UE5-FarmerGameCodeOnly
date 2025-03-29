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
	UPROPERTY(Replicated)
	TArray<AActor*> Slots;
	UPROPERTY()
	TSubclassOf<AActor> SlotClass;
	virtual void PostInitializeComponents() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
