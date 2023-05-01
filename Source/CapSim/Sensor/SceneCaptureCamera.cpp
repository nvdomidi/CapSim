#include "CapSim/Sensor/SceneCaptureCamera.h"
#include "CapSim/Game/CapSimEngine.h"
#include "Kismet/GameplayStatics.h"

#include "Runtime/RenderCore/Public/RenderingThread.h"


ASceneCaptureCamera::ASceneCaptureCamera() : Super()
{
    // TODO: Add post-processing material
    PrimaryActorTick.bCanEverTick = true;

    AddPostProcessingMaterial(
        TEXT("Material'/Game/PostProcessingMaterials/PhysicLensDistortion'"));
}

void ASceneCaptureCamera::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
    
    for (auto& act : FoundActors)
    {
        if (act->GetFName() == FName("Sphere3_Blueprint")) {
            Ball = act;
        }
    }
}

void ASceneCaptureCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ASceneCaptureCamera::PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaSeconds)
{
    if (bIsRecording)
    {
        FString filePath = FString::Printf(TEXT("%s/%d.png"), *this->folderPath, FCapSimEngine::GetFrameCounter());

        FPixelReader::SendPixelsInRenderThread<ASceneCaptureCamera, FColor>(*this, filePath);

        if (Ball != nullptr)
        {
            FString filePathLabel = FString::Printf(TEXT("%s/%d.txt"), *this->folderPath, FCapSimEngine::GetFrameCounter());

            FFileHelper::SaveStringToFile(Ball->GetActorLocation().ToString(), *filePathLabel);
        }
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
