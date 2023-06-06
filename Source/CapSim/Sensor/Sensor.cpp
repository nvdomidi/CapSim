// Fill out your copyright notice in the Description page of Project Settings.


#include "CapSim/Sensor/Sensor.h"

// Sets default values
ASensor::ASensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASensor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bClientsAreListening) {
		if (!AreClientsListening()) {
			bClientsAreListening = false;
		}
	}
	else {
		if (AreClientsListening()) {
			bClientsAreListening = true;
		}
	}
	if (!bClientsAreListening) {
		return;
	}
	ReadyToTick = true;
	PrePhysTick(DeltaTime);

}

void ASensor::PostPhysTickInternal(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (ReadyToTick) {
		PostPhysTick(World, TickType, DeltaSeconds);
		ReadyToTick = false;
	}
}

// Checks it clients are connected
bool ASensor::AreClientsListening()
{
	// TODO: implement check for clients connection
	return true;
}

void ASensor::SetSeed(const int32 InSeed)
{
	check(RandomEngine != nullptr);
	Seed = InSeed;
	RandomEngine->Seed(InSeed);
}