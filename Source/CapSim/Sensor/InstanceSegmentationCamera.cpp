#include "CapSim/Sensor/InstanceSegmentationCamera.h"
#include "CapSim/Game/TaggedComponent.h"
#include "CapSim/Game/CapSimEngine.h"
#include "CapSim/Sensor/PixelReader.h"
#include "ShowFlags.h"

#include "Components/SceneCaptureComponent2D.h"

AInstanceSegmentationCamera::AInstanceSegmentationCamera() : Super()
{
    AddPostProcessingMaterial(TEXT("Material'/Game/PostProcessingMaterials/PhysicLensDistortion'"));
    // TODO: Setup OnActorSpawnHandler so we can refresh components
    // World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateRaw(this, &AInstanceSegmentationCamera::OnActorSpawned));
}

void AInstanceSegmentationCamera::SetUpSceneCaptureComponent(USceneCaptureComponent2D& SceneCapture)
{
    Super::SetUpSceneCaptureComponent(SceneCapture);

    ApplyViewMode(VMI_Unlit, true, SceneCapture.ShowFlags);

    // TODO: NotDrawTaggedComponents
    //SceneCapture.ShowFlags.SetNotDrawTaggedComponents(false); // TaggedComponent detects this and sets view relevance for proxy material
    //FCapSimEngine::SetNotDrawTaggedComponents(false);

    //SceneCapture.ShowFlags.RegisterCustomShowFlag(TEXT("NotDrawTaggedComponents"), true, EShowFlagGroup::SFG_Custom, FText::FromString(TEXT("Dont draw tagged components if enabled")));

    SceneCapture.ShowFlags.SetAtmosphere(false);

    SceneCapture.PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

    TArray<UObject*> TaggedComponents;
    GetObjectsOfClass(UTaggedComponent::StaticClass(), TaggedComponents, false, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::AllFlags);

    
    TArray<UPrimitiveComponent*> ShowOnlyComponents;
    for (UObject* Object : TaggedComponents) {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Object);
        SceneCapture.ShowOnlyComponents.Emplace(Component);
    }
    
}

void AInstanceSegmentationCamera::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    //TRACE_CPUPROFILER_EVENT_SCOPE(AInstanceSegmentationCamera::PostPhysTick);

    USceneCaptureComponent2D* SceneCapture = GetCaptureComponent2D();
    TArray<UObject*> TaggedComponents;
    GetObjectsOfClass(UTaggedComponent::StaticClass(), TaggedComponents, false, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::AllFlags);
    
    SceneCapture->ClearShowOnlyComponents();
    for (UObject* Object : TaggedComponents) {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Object);
        SceneCapture->ShowOnlyComponents.Emplace(Component);
    }
    
    
    if (bIsRecording)
    {
        FString filePath = FString::Printf(TEXT("%s/%d.jpg"), *this->folderPath, FCapSimEngine::GetFrameCounter());

        
        // Sets the view relevance for the tagged components
        FCapSimEngine::SetNotDrawTaggedComponents(false);
        FPixelReader::SendPixelsInRenderThread<AInstanceSegmentationCamera, FColor>(*this, filePath);
        FCapSimEngine::SetNotDrawTaggedComponents(true);
    }
}

void AInstanceSegmentationCamera::StartRecording()
{
    bIsRecording = true;
}

void AInstanceSegmentationCamera::StopRecording()
{
    bIsRecording = false;
}

void AInstanceSegmentationCamera::SetCapturePath(const FString path)
{
    this->folderPath = path;
}

void AInstanceSegmentationCamera::CaptureScene(FString path)
{
    if (path.IsEmpty())
    {
        path = FString::Printf(TEXT("%s/capture.png"), *this->folderPath);
    }

    USceneCaptureComponent2D* SceneCapture = GetCaptureComponent2D();
    TArray<UObject*> TaggedComponents;
    GetObjectsOfClass(UTaggedComponent::StaticClass(), TaggedComponents, false, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::AllFlags);

    SceneCapture->ClearShowOnlyComponents();
    for (UObject* Object : TaggedComponents) {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Object);
        SceneCapture->ShowOnlyComponents.Emplace(Component);
    }

    FCapSimEngine::SetNotDrawTaggedComponents(false);
    FPixelReader::SendPixelsInRenderThread<AInstanceSegmentationCamera, FColor>(*this, path);
    FCapSimEngine::SetNotDrawTaggedComponents(true);
}



