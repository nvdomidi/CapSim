#include "TaggerDelegate.h"

//#include "Game/Tagger.h"

#include "Engine/World.h"

UTaggerDelegate::UTaggerDelegate() :
    ActorSpawnedDelegate(FOnActorSpawned::FDelegate::CreateUObject(this, &UTaggerDelegate::OnActorSpawned)) {}

void UTaggerDelegate::RegisterSpawnHandler(UWorld* InWorld)
{
    InWorld->AddOnActorSpawnedHandler(ActorSpawnedDelegate);
}

void UTaggerDelegate::OnActorSpawned(AActor* InActor)
{
    if (InActor != nullptr) {
        //ATagger::TagActor(*InActor, bSemanticSegmentationEnabled);
    }
}
