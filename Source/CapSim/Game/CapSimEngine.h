#pragma once

#include "CapSim/Utils/NonCopyable.h"
#include "CapSim/Sensor/SensorManager.h"

#include "Misc/CoreDelegates.h"

class FCapSimEngine : private NonCopyable
{
public:

	~FCapSimEngine();

	void NotifyInitGame(ASensorManager* _SensorManager);

	static int GetFrameCounter() { return frame; }

	static bool GetNotDrawTaggedComponents() { return NotDrawTaggedComponents; }
	static void SetNotDrawTaggedComponents(bool bNotDrawTaggedComponents) { NotDrawTaggedComponents = bNotDrawTaggedComponents; }

	ASensorManager* SensorManager;

private:

	bool bIsRunning = false;
	static int frame;
	static bool NotDrawTaggedComponents;

	FDelegateHandle OnPreTickHandle;
	FDelegateHandle OnPostTickHandle;

	void OnPreTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
	void OnPostTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);

};