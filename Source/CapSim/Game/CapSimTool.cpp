#include "CapSim/Game/CapSimTool.h"

ACapSimTool::ACapSimTool() : Super()
{
	
}

void ACapSimTool::BeginPlay()
{
	Super::BeginPlay();
}

void ACapSimTool::InitializeCapSim()
{
	this->SensorManager = GetWorld()->SpawnActor<ASensorManager>(ASensorManager::StaticClass());
	CapSimEngine.NotifyInitGame(SensorManager);
}

ASceneCaptureCamera* ACapSimTool::AddSceneCaptureCamera()
{
	auto SceneCaptureCamera = GetWorld()->SpawnActor<ASceneCaptureCamera>(ASceneCaptureCamera::StaticClass());
	this->SensorManager->AddSensor(SceneCaptureCamera);

	return SceneCaptureCamera;
}