
#include "WayPoint.h"

AWayPoint::AWayPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void AWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

