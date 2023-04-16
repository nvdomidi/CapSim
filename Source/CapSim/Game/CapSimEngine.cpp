#include "CapSim/Game/CapSimEngine.h"

int FCapSimEngine::frame = 0;

void FCapSimEngine::NotifyInitGame(ASensorManager* Manager)
{
	if(!bIsRunning){
		bIsRunning = true;
		
		this->SensorManager = Manager;

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
	SensorManager->PostPhysTick(World, TickType, DeltaSeconds);
	frame++;
}


