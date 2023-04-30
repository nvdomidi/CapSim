// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorManager.h"
#include "CapSim/Sensor/PixelReader.h"
#include "CapSim/Sensor/SceneCaptureSensor.h"
#include "CapSim/Sensor/SceneCaptureCamera.h"

// Sets default values
ASensorManager::ASensorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASensorManager::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	
	for (ASensor* Sensor : SensorList)
	{
		Sensor->PostPhysTickInternal(World, TickType, DeltaSeconds);
	}
}

// Called when the game starts or when spawned
void ASensorManager::BeginPlay()
{
	Super::BeginPlay();
	
	//auto SceneCaptureCamera = GetWorld()->SpawnActor<ASceneCaptureCamera>(ASceneCaptureCamera::StaticClass());
	//SensorList.Add(SceneCaptureCamera);
}

// Called every frame
void ASensorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// adding sensor to sensormanager
void ASensorManager::AddSensor(ASensor *Sensor)
{
	SensorList.Add(Sensor);
}

