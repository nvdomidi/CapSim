#pragma once

#include "CapSim/Sensor/ShaderBasedSensor.h"

#include "SemanticSegmentationCamera.generated.h"

/// Sensor that produces "semantic segmentation" images.
UCLASS()
class CAPSIM_API ASemanticSegmentationCamera : public AShaderBasedSensor
{
	GENERATED_BODY()

public:
	ASemanticSegmentationCamera();

protected:
	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		void StopRecording();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		void SetCapturePath(const FString path);

	UFUNCTION(BlueprintCallable, Category = "CapSim")
		void CaptureScene(const FString path);

private:

	bool bIsRecording = false;

	FString folderPath = "D:/CapSim_Images/Semantic";
};
