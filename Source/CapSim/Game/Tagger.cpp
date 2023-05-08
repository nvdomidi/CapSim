#include "Tagger.h"
#include "TaggedComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "PhysicsEngine/PhysicsAsset.h"

template <typename T>
static auto CastEnum(T label)
{
	return static_cast<typename std::underlying_type<T>::type>(label);
}

/* CARLA
* 
crp::CityObjectLabel ATagger::GetLabelByFolderName(const FString& String) {
	if (String == "Building")     return crp::CityObjectLabel::Buildings;
	else if (String == "Fence")        return crp::CityObjectLabel::Fences;
	else if (String == "Pedestrian")   return crp::CityObjectLabel::Pedestrians;
	else if (String == "Pole")         return crp::CityObjectLabel::Poles;
	else if (String == "Other")        return crp::CityObjectLabel::Other;
	else if (String == "Road")         return crp::CityObjectLabel::Roads;
	else if (String == "RoadLine")     return crp::CityObjectLabel::RoadLines;
	else if (String == "SideWalk")     return crp::CityObjectLabel::Sidewalks;
	else if (String == "TrafficSign")  return crp::CityObjectLabel::TrafficSigns;
	else if (String == "Vegetation")   return crp::CityObjectLabel::Vegetation;
	else if (String == "Car")          return crp::CityObjectLabel::Car;
	else if (String == "Wall")         return crp::CityObjectLabel::Walls;
	else if (String == "Sky")          return crp::CityObjectLabel::Sky;
	else if (String == "Ground")       return crp::CityObjectLabel::Ground;
	else if (String == "Bridge")       return crp::CityObjectLabel::Bridge;
	else if (String == "RailTrack")    return crp::CityObjectLabel::RailTrack;
	else if (String == "GuardRail")    return crp::CityObjectLabel::GuardRail;
	else if (String == "TrafficLight") return crp::CityObjectLabel::TrafficLight;
	else if (String == "Static")       return crp::CityObjectLabel::Static;
	else if (String == "Dynamic")      return crp::CityObjectLabel::Dynamic;
	else if (String == "Water")        return crp::CityObjectLabel::Water;
	else if (String == "Terrain")      return crp::CityObjectLabel::Terrain;
	else if (String == "Truck")        return crp::CityObjectLabel::Truck;
	else if (String == "Motorcycle")   return crp::CityObjectLabel::Motorcycle;
	else if (String == "Bicycle")      return crp::CityObjectLabel::Bicycle;
	else if (String == "Bus")          return crp::CityObjectLabel::Bus;
	else if (String == "Rider")        return crp::CityObjectLabel::Rider;
	else if (String == "Train")        return crp::CityObjectLabel::Train;
	else                               return crp::CityObjectLabel::None;
}
*/

CapSimObjectLabel ATagger::GetLabelByFolderName(const FString& String) {
	if (String == "Buildings")     return CapSimObjectLabel::Buildings;
	else if (String == "Road")         return CapSimObjectLabel::Roads;
	else if (String == "Car")          return CapSimObjectLabel::Car;
	else if (String == "Sky")          return CapSimObjectLabel::Sky;
	else if (String == "Ground")       return CapSimObjectLabel::Ground;
	else                               return CapSimObjectLabel::None;
}

void ATagger::SetStencilValue(
	UPrimitiveComponent& Component,
	const CapSimObjectLabel& Label,
	const bool bSetRenderCustomDepth) {
	Component.SetCustomDepthStencilValue(CastEnum(Label));
	Component.SetRenderCustomDepth(
		bSetRenderCustomDepth &&
		(Label != CapSimObjectLabel::None));
}

/* CARLA's
bool ATagger::IsThing(const crp::CityObjectLabel& Label)
{
	return (Label == crp::CityObjectLabel::Pedestrians ||
		Label == crp::CityObjectLabel::TrafficSigns ||
		Label == crp::CityObjectLabel::Car ||
		Label == crp::CityObjectLabel::Train ||
		Label == crp::CityObjectLabel::Bicycle ||
		Label == crp::CityObjectLabel::Motorcycle ||
		Label == crp::CityObjectLabel::Bus ||
		Label == crp::CityObjectLabel::Rider ||
		Label == crp::CityObjectLabel::Truck ||
		Label == crp::CityObjectLabel::TrafficLight);
}
*/

bool ATagger::IsThing(const CapSimObjectLabel& Label)
{
	return (Label == CapSimObjectLabel::Car);
}

FLinearColor ATagger::GetActorLabelColor(const AActor& Actor, const CapSimObjectLabel& Label)
{
	uint32 id = Actor.GetUniqueID();
	// TODO: Warn if id > 0xffff.

	// Encode label and id like semantic segmentation does
	// TODO: Steal bits from R channel and maybe A channel?
	FLinearColor Color(0.0f, 0.0f, 0.0f, 1.0f);
	Color.R = CastEnum(Label) / 255.0f;
	Color.G = ((id & 0x00ff) >> 0) / 255.0f;
	Color.B = ((id & 0xff00) >> 8) / 255.0f;

	return Color;
}


// =============================================================================
// -- static ATagger functions -------------------------------------------------
// =============================================================================

void ATagger::TagActor(const AActor& Actor, bool bTagForSemanticSegmentation)
{
    UE_LOG(LogTemp, Warning, TEXT("Tagging Actor: %s"), *Actor.GetName());

    // Iterate static meshes.
    TArray<UStaticMeshComponent*> StaticMeshComponents;
    Actor.GetComponents<UStaticMeshComponent>(StaticMeshComponents);
    for (UStaticMeshComponent* Component : StaticMeshComponents) {
        auto Label = GetLabelByPath(Component->GetStaticMesh());

        /* checks for motorcycles
        if (Label == CapSimObjectLabel::Pedestrians &&
            Cast<ACarlaWheeledVehicle>(&Actor))
        {
            Label = crp::CityObjectLabel::Rider;
        }
        */
        SetStencilValue(*Component, Label, bTagForSemanticSegmentation);

        UE_LOG(LogTemp, Warning, TEXT("  + StaticMeshComponent: %s"), *Component->GetName());
        UE_LOG(LogTemp, Warning, TEXT("    - Label: \"%s\""), *GetTagAsString(Label));

        if (!Component->IsVisible() || !Component->GetStaticMesh())
        {
            continue;
        }

        
        // Find a tagged component that is attached to this component
        UTaggedComponent* TaggedComponent = NULL;
        TArray<USceneComponent*> AttachedComponents = Component->GetAttachChildren();
        for (USceneComponent* SceneComponent : AttachedComponents) {
            UTaggedComponent* TaggedSceneComponent = Cast<UTaggedComponent>(SceneComponent);
            if (IsValid(TaggedSceneComponent)) {
                TaggedComponent = TaggedSceneComponent;

                UE_LOG(LogTemp, Warning, TEXT("    - Found Tag"));

                break;
            }
        }

        // If not found, then create new tagged component and attach it to this component
        if (!TaggedComponent) {
            TaggedComponent = NewObject<UTaggedComponent>(Component);
            TaggedComponent->SetupAttachment(Component);
            TaggedComponent->RegisterComponent();
            
            UE_LOG(LogTemp, Warning, TEXT("    - Added Tag"));

        }

        // Set tagged component color
        FLinearColor Color = GetActorLabelColor(Actor, Label);

        UE_LOG(LogTemp, Warning, TEXT("    - Color: %s"), *Color.ToString());

        TaggedComponent->SetColor(Color);
        TaggedComponent->MarkRenderStateDirty();
        
    }
    

    // Iterate skeletal meshes.
    TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
    Actor.GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
    for (USkeletalMeshComponent* Component : SkeletalMeshComponents) {
        auto Label = GetLabelByPath(Component->GetPhysicsAsset());
        /*
        if (Label == crp::CityObjectLabel::Pedestrians &&
            Cast<ACarlaWheeledVehicle>(&Actor))
        {
            Label = crp::CityObjectLabel::Rider;
        }
        */
        SetStencilValue(*Component, Label, bTagForSemanticSegmentation);

        UE_LOG(LogTemp, Warning, TEXT("  + SkeletalMeshComponent: %s"), *Component->GetName());
        UE_LOG(LogTemp, Warning, TEXT("    - Label: \"%s\""), *GetTagAsString(Label));

        if (!Component->IsVisible() || !Component->GetSkeletalMeshRenderData())
        {
            continue;
        }

        
        // Find a tagged component that is attached to this component
        UTaggedComponent* TaggedComponent = NULL;
        TArray<USceneComponent*> AttachedComponents = Component->GetAttachChildren();
        for (USceneComponent* SceneComponent : AttachedComponents) {
            UTaggedComponent* TaggedSceneComponent = Cast<UTaggedComponent>(SceneComponent);
            if (IsValid(TaggedSceneComponent)) {
                TaggedComponent = TaggedSceneComponent;

                UE_LOG(LogTemp, Warning, TEXT("    - Found Tag"));

                break;
            }
        }

        // If not found, then create new tagged component and attach it to this component
        if (!TaggedComponent) {
            TaggedComponent = NewObject<UTaggedComponent>(Component);
            TaggedComponent->SetupAttachment(Component);
            TaggedComponent->RegisterComponent();

            UE_LOG(LogTemp, Warning, TEXT("    - Added Tag"));

        }

        // Set tagged component color
        FLinearColor Color = GetActorLabelColor(Actor, Label);

        UE_LOG(LogTemp, Warning, TEXT("    - Color: %s"), *Color.ToString());


        TaggedComponent->SetColor(Color);
        TaggedComponent->MarkRenderStateDirty();
        
    }
}


void ATagger::TagActorsInLevel(UWorld& World, bool bTagForSemanticSegmentation)
{
    for (TActorIterator<AActor> it(&World); it; ++it) {
        TagActor(**it, bTagForSemanticSegmentation);
    }
}

void ATagger::TagActorsInLevel(ULevel& Level, bool bTagForSemanticSegmentation)
{
    for (AActor* Actor : Level.Actors) {
        TagActor(*Actor, bTagForSemanticSegmentation);
    }
}


void ATagger::GetTagsOfTaggedActor(const AActor& Actor, TSet<CapSimObjectLabel>& Tags)
{
    TArray<UPrimitiveComponent*> Components;
    Actor.GetComponents<UPrimitiveComponent>(Components);
    for (auto* Component : Components) {
        if (Component != nullptr) {
            const auto Tag = GetTagOfTaggedComponent(*Component);
            if (Tag != CapSimObjectLabel::None) {
                Tags.Add(Tag);
            }
        }
    }
}

/*
FString ATagger::GetTagAsString(const crp::CityObjectLabel Label)
{
  switch (Label) {
#define CARLA_GET_LABEL_STR(lbl) case crp::CityObjectLabel:: lbl : return TEXT(#lbl);
    default:
    CARLA_GET_LABEL_STR(None)
    CARLA_GET_LABEL_STR(Buildings)
    CARLA_GET_LABEL_STR(Fences)
    CARLA_GET_LABEL_STR(Other)
    CARLA_GET_LABEL_STR(Pedestrians)
    CARLA_GET_LABEL_STR(Poles)
    CARLA_GET_LABEL_STR(RoadLines)
    CARLA_GET_LABEL_STR(Roads)
    CARLA_GET_LABEL_STR(Sidewalks)
    CARLA_GET_LABEL_STR(TrafficSigns)
    CARLA_GET_LABEL_STR(Vegetation)
    CARLA_GET_LABEL_STR(Car)
    CARLA_GET_LABEL_STR(Walls)
    CARLA_GET_LABEL_STR(Sky)
    CARLA_GET_LABEL_STR(Ground)
    CARLA_GET_LABEL_STR(Bridge)
    CARLA_GET_LABEL_STR(RailTrack)
    CARLA_GET_LABEL_STR(GuardRail)
    CARLA_GET_LABEL_STR(TrafficLight)
    CARLA_GET_LABEL_STR(Static)
    CARLA_GET_LABEL_STR(Dynamic)
    CARLA_GET_LABEL_STR(Water)
    CARLA_GET_LABEL_STR(Terrain)
    CARLA_GET_LABEL_STR(Truck)
    CARLA_GET_LABEL_STR(Motorcycle)
    CARLA_GET_LABEL_STR(Bicycle)
    CARLA_GET_LABEL_STR(Bus)
    CARLA_GET_LABEL_STR(Train)
    CARLA_GET_LABEL_STR(Rider)

#undef CARLA_GET_LABEL_STR
  }
}

*/


FString ATagger::GetTagAsString(const CapSimObjectLabel Label)
{
    switch (Label) {
#define CAPSIM_GET_LABEL_STR(lbl) case CapSimObjectLabel:: lbl : return TEXT(#lbl);
    default:
        CAPSIM_GET_LABEL_STR(None)
            CAPSIM_GET_LABEL_STR(Buildings)
            CAPSIM_GET_LABEL_STR(Roads)
            CAPSIM_GET_LABEL_STR(Car)
            CAPSIM_GET_LABEL_STR(Sky)
            CAPSIM_GET_LABEL_STR(Ground)

#undef CAPSIM_GET_LABEL_STR
    }
}


// =============================================================================
// -- non-static ATagger functions ---------------------------------------------
// =============================================================================

ATagger::ATagger()
{
    PrimaryActorTick.bCanEverTick = false;
}



#if WITH_EDITOR
void ATagger::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
  Super::PostEditChangeProperty(PropertyChangedEvent);
  if (PropertyChangedEvent.Property) {
    if (bTriggerTagObjects && (GetWorld() != nullptr)) {
      TagActorsInLevel(*GetWorld(), bTagForSemanticSegmentation);
    }
  }
  bTriggerTagObjects = false;
}
#endif // WITH_EDITOR

