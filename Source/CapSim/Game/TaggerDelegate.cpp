#include "TaggerDelegate.h"

#include "CapSim/Game/Tagger.h"

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
        UE_LOG(LogTemp, Warning, TEXT("Actor spawned TaggerDelegate: %s"), *(InActor->GetFName().ToString()));
        ATagger::TagActor(*InActor, bSemanticSegmentationEnabled);
    }
}
