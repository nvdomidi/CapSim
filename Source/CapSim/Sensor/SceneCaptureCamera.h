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
};
