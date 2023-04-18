#include "CapSim/Sensor/SceneCaptureCamera.h"
#include "CapSim/Game/CapSimEngine.h"

#include "Runtime/RenderCore/Public/RenderingThread.h"


ASceneCaptureCamera::ASceneCaptureCamera() : Super()
{
    // TODO: Add post-processing material
    
    AddPostProcessingMaterial(
        TEXT("Material'/CapSim/Content/PostProcessingMaterials/PhysicLensDistortion.PhysicLensDistortion'"));
}

void ASceneCaptureCamera::BeginPlay()
{
  Super::BeginPlay();
}


void ASceneCaptureCamera::PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaSeconds)
{
  UE_LOG(LogTemp, Warning, TEXT("PostPhysTick in SceneCaptureCamera, deltaSeconds: %f"), DeltaSeconds);

  FPixelReader::SendPixelsInRenderThread<ASceneCaptureCamera, FColor>(*this);
}
