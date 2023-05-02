#include "CapSim/Game/CapSimTool.h"

ACapSimTool::ACapSimTool() : Super()
{
	TaggerDelegate = CreateDefaultSubobject<UTaggerDelegate>(TEXT("TaggerDelegate"));
}

void ACapSimTool::BeginPlay()
{
	Super::BeginPlay();
}

void ACapSimTool::InitializeCapSim()
{
	auto SensorManager = GetWorld()->SpawnActor<ASensorManager>(ASensorManager::StaticClass());
	
	CapSimEngine.NotifyInitGame(SensorManager);

	if (TaggerDelegate != nullptr){
		TaggerDelegate->RegisterSpawnHandler(GetWorld());
	}

}

ASceneCaptureCamera* ACapSimTool::AddSceneCaptureCamera()
{
	auto SceneCaptureCamera = GetWorld()->SpawnActor<ASceneCaptureCamera>(ASceneCaptureCamera::StaticClass());
	
	CapSimEngine.SensorManager->AddSensor(SceneCaptureCamera);

	return SceneCaptureCamera;
}

void ACapSimTool::AddSemanticSegmentationCamera()
{
	//auto SemanticSegmentationCamera = GetWorld()->SpawnActor<ASemanticSegmentationCamera>(ASemanticSegmentationCamera::StaticClass());

	if (TaggerDelegate != nullptr) {
		TaggerDelegate->SetSemanticSegmentationEnabled(true);
	}

	//ATagger::TagActorsInLevel(*World, true);
}