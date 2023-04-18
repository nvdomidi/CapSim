// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CapSim/Sensor/Sensor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SensorManager.generated.h"

UCLASS()
class CAPSIM_API ASensorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorManager();

	// Tick to tick all sensors
	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Use this to add sensor from blueprint
	
	//UFUNCTION(BlueprintCallable)
	//void AddSensor(ASensor& Sensor);


private:
	
	UPROPERTY(EditAnywhere)
	TArray<ASensor*> SensorList;

};
