#pragma once

#include "Engine/World.h"

#include "TaggerDelegate.generated.h"

/// Used to tag every actor that is spawned into the world.
UCLASS()
class CAPSIM_API UTaggerDelegate : public UObject
{
    GENERATED_BODY()

public:

    UTaggerDelegate();

    ~UTaggerDelegate();

    void RegisterSpawnHandler(UWorld* World);

    void SetSemanticSegmentationEnabled(bool Enable = true)
    {
        bSemanticSegmentationEnabled = Enable;
    }

    void OnActorSpawned(AActor* Actor);

private:

    FOnActorSpawned::FDelegate ActorSpawnedDelegate;

    bool bSemanticSegmentationEnabled = false;
};
