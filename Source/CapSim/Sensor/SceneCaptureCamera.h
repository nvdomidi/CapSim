#pragma once

#include "CapSim/Sensor/PixelReader.h"
#include "CapSim/Sensor/ShaderBasedSensor.h"

#include "SceneCaptureCamera.generated.h"

/// A sensor that captures images from the scene.
UCLASS()
class CAPSIM_API ASceneCaptureCamera : public AShaderBasedSensor
{
  GENERATED_BODY()

public:

  ASceneCaptureCamera();

protected:

  void BeginPlay() override;
  void PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaSeconds) override;

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

	FString folderPath = "D:/Carla_Images";
};
