// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapSim/Sensor/SceneCaptureSensor.h"
#include "SceneCaptureCameraTest1.generated.h"

/**
 * 
 */
UCLASS()
class CAPSIM_API ASceneCaptureCameraTest1 : public ASceneCaptureSensor
{
	GENERATED_BODY()
	
	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds) override;

public:
	
	float time = 0;

};
