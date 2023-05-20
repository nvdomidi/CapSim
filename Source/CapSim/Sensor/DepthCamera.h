#pragma once

#include "CapSim/Sensor/ShaderBasedSensor.h"

#include "DepthCamera.generated.h"

/// Sensor that produces "depth" images.
UCLASS()
class CAPSIM_API ADepthCamera : public AShaderBasedSensor
{
	GENERATED_BODY()

public:

	ADepthCamera();

protected:

	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds) override;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	void StopRecording();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	void SetCapturePath(const FString path);

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	void CaptureScene(const FString path);

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	float CalculateFarValue();

	UFUNCTION(BlueprintCallable, Category = "CapSim")
	void SetFarValue(float Far);

private:

	bool bIsRecording = false;

	FString folderPath = "D:/CapSim_Images/Depth";
};
