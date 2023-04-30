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
	auto SensorManager = GetWorld()->SpawnActor<ASensorManager>(ASensorManager::StaticClass());
	
	//UE_LOG(LogTemp, Warning, TEXT("SensorManager in CapSim ADDRESS: %p\n"), (void*)&(SensorManager));
	
	CapSimEngine.NotifyInitGame(SensorManager);

}

ASceneCaptureCamera* ACapSimTool::AddSceneCaptureCamera()
{
	auto SceneCaptureCamera = GetWorld()->SpawnActor<ASceneCaptureCamera>(ASceneCaptureCamera::StaticClass());
	
	CapSimEngine.SensorManager->AddSensor(SceneCaptureCamera);

	return SceneCaptureCamera;
}