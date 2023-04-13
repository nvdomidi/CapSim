// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorManager.h"
//#include "CapSim/Sensor/SceneCaptureSensor.h"
//#include "CapSim/Sensor/SceneCaptureCameraTest1.h"

// Sets default values
ASensorManager::ASensorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASensorManager::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("Ticking in SensorManager"));
	UE_LOG(LogTemp, Warning, TEXT("%d"), SensorList.Num());
	for (ASensor* Sensor : SensorList)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ticking in Snesor"));
		Sensor->PostPhysTickInternal(World, TickType, DeltaSeconds);
	}
}

// Called when the game starts or when spawned
void ASensorManager::BeginPlay()
{
	Super::BeginPlay();
	
	//auto SceneCaptureSensor = GetWorld()->SpawnActor<ASceneCaptureSensor>(ASceneCaptureSensor::StaticClass());
	////////auto SceneCaptureCameraTest1 = GetWorld()->SpawnActor<ASceneCaptureCameraTest1>(ASceneCaptureCameraTest1::StaticClass());
	////////SensorList.Add(SceneCaptureCameraTest1);
}

// Called every frame
void ASensorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
