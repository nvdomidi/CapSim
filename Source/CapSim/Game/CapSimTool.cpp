#include "CapSim/Game/CapSimTool.h"
#include "CapSim/Game/Tagger.h"
#include "CapSim/Sensor/SemanticSegmentationCamera.h"
#include "CapSim/Sensor/InstanceSegmentationCamera.h"
#include "CapSim/Sensor/DepthCamera.h"

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

	if (TaggerDelegate != nullptr) {
		TaggerDelegate->RegisterSpawnHandler(GetWorld());
		TaggerDelegate->SetSemanticSegmentationEnabled(true);
	}

	// TODO
	ATagger::TagActorsInLevel(*GetWorld(), true);

}

ASceneCaptureCamera* ACapSimTool::AddSceneCaptureCamera()
{
	auto SceneCaptureCamera = GetWorld()->SpawnActor<ASceneCaptureCamera>(ASceneCaptureCamera::StaticClass());

	CapSimEngine.SensorManager->AddSensor(SceneCaptureCamera);

	return SceneCaptureCamera;
}

ASemanticSegmentationCamera* ACapSimTool::AddSemanticSegmentationCamera()
{
	auto SemanticSegmentationCamera = GetWorld()->SpawnActor<ASemanticSegmentationCamera>(ASemanticSegmentationCamera::StaticClass());

	CapSimEngine.SensorManager->AddSensor(SemanticSegmentationCamera);

	return SemanticSegmentationCamera;
}

AInstanceSegmentationCamera* ACapSimTool::AddInstanceSegmentationCamera()
{
	auto InstanceSegmentationCamera = GetWorld()->SpawnActor<AInstanceSegmentationCamera>(AInstanceSegmentationCamera::StaticClass());

	CapSimEngine.SensorManager->AddSensor(InstanceSegmentationCamera);

	return InstanceSegmentationCamera;
}

ADepthCamera* ACapSimTool::AddDepthCamera()
{
	auto DepthCamera = GetWorld()->SpawnActor<ADepthCamera>(ADepthCamera::StaticClass());

	CapSimEngine.SensorManager->AddSensor(DepthCamera);

	return DepthCamera;
}