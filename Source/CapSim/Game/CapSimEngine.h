#pragma once

#include "CapSim/Utils/NonCopyable.h"
#include "CapSim/Sensor/SensorManager.h"

#include "Misc/CoreDelegates.h"

class FCapSimEngine : private NonCopyable
{
public:

	void NotifyInitGame(ASensorManager* Manager);

	~FCapSimEngine();

	ASensorManager* SensorManager;

private:

	bool bIsRunning = false;

	FDelegateHandle OnPreTickHandle;
	FDelegateHandle OnPostTickHandle;

	void OnPreTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
	void OnPostTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);

	
	
};