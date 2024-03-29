#pragma once

#include "CapSim/Sensor/ShaderBasedSensor.h"

#include "InstanceSegmentationCamera.generated.h"

/// Sensor that produces "Instance segmentation" images.
UCLASS()
class CAPSIM_API AInstanceSegmentationCamera : public AShaderBasedSensor
{
	GENERATED_BODY()

public:

	AInstanceSegmentationCamera();

protected:

	void BeginPlay() override;

	void SetUpSceneCaptureComponent(USceneCaptureComponent2D& SceneCapture) override;
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

	FString folderPath = "D:/CapSim_Images/Instance";
};
