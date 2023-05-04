#include "CapSim/Sensor/SemanticSegmentationCamera.h"

#include "CapSim/Game/CapSimEngine.h"
#include "CapSim/Sensor/PixelReader.h"

ASemanticSegmentationCamera::ASemanticSegmentationCamera()
    : Super()
{
    AddPostProcessingMaterial(
        TEXT("Material'/Game/PostProcessingMaterials/PhysicLensDistortion'"));
    AddPostProcessingMaterial(
        TEXT("Material'/Game/PostProcessingMaterials/GTMaterial.GTMaterial'"));
}

void ASemanticSegmentationCamera::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    if (bIsRecording)
    {
        FString filePath = FString::Printf(TEXT("%s/%d.png"), *this->folderPath, FCapSimEngine::GetFrameCounter());

        FPixelReader::SendPixelsInRenderThread<ASemanticSegmentationCamera, FColor>(*this, filePath);

    }
}

void ASemanticSegmentationCamera::StartRecording()
{
    bIsRecording = true;
}

void ASemanticSegmentationCamera::StopRecording()
{
    bIsRecording = false;
}

void ASemanticSegmentationCamera::SetCapturePath(const FString path)
{
    this->folderPath = path;
}

void ASemanticSegmentationCamera::CaptureScene(FString path)
{
    if (path.IsEmpty())
    {
        path = FString::Printf(TEXT("%s/capture.png"), *this->folderPath);
    }
    FPixelReader::SendPixelsInRenderThread<ASemanticSegmentationCamera, FColor>(*this, path);
}

