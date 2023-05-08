#include "CapSim/Game/CapSimEngine.h"

int FCapSimEngine::frame = 0;
bool FCapSimEngine::NotDrawTaggedComponents = true;

void FCapSimEngine::NotifyInitGame(ASensorManager* _SensorManager)
{
	if(!bIsRunning){
		bIsRunning = true;

		this->SensorManager = _SensorManager;
		
		OnPreTickHandle = FWorldDelegates::OnWorldTickStart.AddRaw(
			this,
			&FCapSimEngine::OnPreTick);

		OnPostTickHandle = FWorldDelegates::OnWorldPostActorTick.AddRaw(
			this,
			&FCapSimEngine::OnPostTick);
	}
}

FCapSimEngine::~FCapSimEngine()
{
	if (bIsRunning) {
		FWorldDelegates::OnWorldTickStart.Remove(OnPreTickHandle);
		FWorldDelegates::OnWorldPostActorTick.Remove(OnPostTickHandle);
	}
}

void FCapSimEngine::OnPreTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{

}

void FCapSimEngine::OnPostTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (this->SensorManager == nullptr)
		return;

	this->SensorManager->PostPhysTick(World, TickType, DeltaSeconds);
	frame++;
}


