#include "CapSim/Sensor/SceneCaptureCamera.h"
#include "CapSim/Game/CapSimEngine.h"

#include "Runtime/RenderCore/Public/RenderingThread.h"


ASceneCaptureCamera::ASceneCaptureCamera() : Super()
{
    // TODO: Add post-processing material
    
    AddPostProcessingMaterial(
        TEXT("Material'/Game/PostProcessingMaterials/PhysicLensDistortion'"));
}

void ASceneCaptureCamera::BeginPlay()
{
  
}


void ASceneCaptureCamera::PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaSeconds)
{
    if (bIsRecording)
    {
        UE_LOG(LogTemp, Warning, TEXT("PostPhysTick in SceneCaptureCamera, deltaSeconds: %f"), DeltaSeconds);

        FString filePath = FString::Printf(TEXT("%s/%d.png"), *this->folderPath, FCapSimEngine::GetFrameCounter());

        FPixelReader::SendPixelsInRenderThread<ASceneCaptureCamera, FColor>(*this, filePath);
    }
}


void ASceneCaptureCamera::StartRecording()
{
    bIsRecording = true;
}

void ASceneCaptureCamera::StopRecording()
{
    bIsRecording = false;
}

void ASceneCaptureCamera::SetCapturePath(const FString path)
{
    this->folderPath = path;
}

void ASceneCaptureCamera::CaptureScene(FString path)
{
    if (path.IsEmpty())
    {
        path = FString::Printf(TEXT("%s/capture.png"), *this->folderPath);
    }
    FPixelReader::SendPixelsInRenderThread<ASceneCaptureCamera, FColor>(*this, path);
}
