#pragma once

#include "CapSim/Sensor/SensorManager.h"
#include "CapSim/Sensor/SceneCaptureCamera.h"
#include "CapSim/Sensor/RayCastSemanticLidar.h"
#include "CapSim/Sensor/RayCastLidar.h"
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

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		ASemanticSegmentationCamera* AddSemanticSegmentationCamera();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		AInstanceSegmentationCamera* AddInstanceSegmentationCamera();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		ADepthCamera* AddDepthCamera();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		ARayCastSemanticLidar* AddSemanticLidar();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		ARayCastLidar* AddLidar();



protected:

	void BeginPlay() override;

private:

	FCapSimEngine CapSimEngine;

	UPROPERTY()
		UTaggerDelegate* TaggerDelegate = nullptr;
};