#pragma once

#include "CapSim/Sensor/SensorManager.h"
#include "CapSim/Sensor/SceneCaptureCamera.h"
#include "CapSim/Game/CapSimEngine.h"
#include "CapSim/Game/TaggerDelegate.h"

#include "CoreMinimal.h"
#include "CapSimTool.generated.h"

UCLASS()
class CAPSIM_API ACapSimTool : public AActor
{
	GENERATED_BODY()

public:

	ACapSimTool();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	void InitializeCapSim();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	ASceneCaptureCamera* AddSceneCaptureCamera();

	//TODO: change the output to semanticsegmentationcamera
	UFUNCTION(BlueprintCallable, Category = "CapSim")
	//ASemanticSegmentationCamera* AddSemanticSegmentationCamera();
	void AddSemanticSegmentationCamera();

protected:

	void BeginPlay() override;

private:

	FCapSimEngine CapSimEngine;

	UPROPERTY()
	UTaggerDelegate* TaggerDelegate = nullptr;
};