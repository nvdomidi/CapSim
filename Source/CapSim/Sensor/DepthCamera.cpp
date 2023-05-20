#include "CapSim/Sensor/DepthCamera.h"

#include "CapSim/Game/CapSimEngine.h"
#include "CapSim/Sensor/PixelReader.h"


ADepthCamera::ADepthCamera() : Super()
{
    AddPostProcessingMaterial(TEXT("Material'/Game/PostProcessingMaterials/PhysicLensDistortion'"));
    AddPostProcessingMaterial(
#if PLATFORM_LINUX
        TEXT("Material'/Game/PostProcessingMaterials/DepthEffectMaterial_GLSL.DepthEffectMaterial_GLSL'")
#else
        TEXT("Material'/Game/PostProcessingMaterials/DepthEffectMaterial.DepthEffectMaterial'")
#endif
    );
}


void ADepthCamera::BeginPlay()
{
    bEnablePostProcessingEffects = false;
    Super::BeginPlay();
}

void ADepthCamera::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{

    if (bIsRecording)
    {
        FString filePath = FString::Printf(TEXT("%s/%d.jpg"), *this->folderPath, FCapSimEngine::GetFrameCounter());

        FPixelReader::SendPixelsInRenderThread<ADepthCamera, FColor>(*this, filePath);
    }
}

void ADepthCamera::StartRecording()
{
    bIsRecording = true;
}

void ADepthCamera::StopRecording()
{
    bIsRecording = false;
}
    
void ADepthCamera::SetCapturePath(const FString path)
{
    this->folderPath = path;
}

void ADepthCamera::CaptureScene(FString path)
{
    if (path.IsEmpty())
    {
        path = FString::Printf(TEXT("%s/capture.jpg"), *this->folderPath);
    }
    FPixelReader::SendPixelsInRenderThread<ADepthCamera, FColor>(*this, path);
}

float ADepthCamera::CalculateFarValue()
{
    TArray<UObject*> AllActors;

    GetObjectsOfClass(AActor::StaticClass(), AllActors, true, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::AllFlags);

    float MaxDistance{ 0.0f };

    for (UObject* ActorObject : AllActors) {
        AActor* Actor = Cast<AActor>(ActorObject);
        if (GetDistanceTo(Actor) > MaxDistance) {
            MaxDistance = GetDistanceTo(Actor);
        }
    }

    int i = 1;
    while (i * 1000.0f <= MaxDistance * 2) {
        i++;
    }

    UE_LOG(LogTemp, Warning, TEXT("Setting Far at: %f"), (i * 1000.0f));

    return i * 1000.0f;
}

void ADepthCamera::SetFarValue(float Far)
{
    Shaders[1].PostProcessMaterial->SetScalarParameterValue(TEXT("Far_1"), Far);
}