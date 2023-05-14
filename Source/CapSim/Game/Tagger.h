#pragma once

#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#include <CapSim/Game/ObjectLabel.h>

#include "Tagger.generated.h"

UCLASS()
class CAPSIM_API ATagger : public AActor
{
	GENERATED_BODY()

public:

	/// Set the tag of an actor.
	///
	/// If bTagForSemanticSegmentation true, activate the custom depth pass. This
	/// pass is necessary for rendering the semantic segmentation. However, it may
	/// add a performance penalty since occlusion doesn't seem to be applied to
	/// objects having this value active.
	static void TagActor(const AActor& Actor, bool bTagForSemanticSegmentation);

	/// Set the tag of every actor in level.
	///
	/// If bTagForSemanticSegmentation true, activate the custom depth pass. This
	/// pass is necessary for rendering the semantic segmentation. However, it may
	/// add a performance penalty since occlusion doesn't seem to be applied to
	/// objects having this value active.
	static void TagActorsInLevel(UWorld& World, bool bTagForSemanticSegmentation);

	static void TagActorsInLevel(ULevel& Level, bool bTagForSemanticSegmentation);

	//Added by navid
	static void RemoveTagActorsInLevel(UWorld& World);
	static void RemoveTagActorsInLevel(ULevel& Level);


	/// Retrieve the tag of an already tagged component.
	static CapSimObjectLabel GetTagOfTaggedComponent(const UPrimitiveComponent& Component)
	{
		return static_cast<CapSimObjectLabel>(Component.CustomDepthStencilValue);
	}

	/// Retrieve the tags of an already tagged actor. CapSimObjectLabel::None is
	/// not added to the array.
	static void GetTagsOfTaggedActor(const AActor& Actor, TSet<CapSimObjectLabel>& Tags);

	/// Return true if @a Component has been tagged with the given @a Tag.
	static bool MatchComponent(const UPrimitiveComponent& Component, CapSimObjectLabel Tag)
	{
		return (Tag == GetTagOfTaggedComponent(Component));
	}

	/// Retrieve the tags of an already tagged actor. CityObjectLabel::None is
	/// not added to the array.
	static FString GetTagAsString(CapSimObjectLabel Tag);

	/// Method that computes the label corresponding to a folder path
	static CapSimObjectLabel GetLabelByFolderName(const FString& String);

	/// Method that computes the label corresponding to an specific object
	/// using the folder path in which it is stored
	template <typename T>
	static CapSimObjectLabel GetLabelByPath(const T* Object) {
		const FString Path = Object->GetPathName();
		TArray<FString> StringArray;
		Path.ParseIntoArray(StringArray, TEXT("/"), false);
		return (StringArray.Num() > 4 ? GetLabelByFolderName(StringArray[4]) : CapSimObjectLabel::None);
	}

	static void SetStencilValue(UPrimitiveComponent& Component,
		const CapSimObjectLabel& Label, const bool bSetRenderCustomDepth);

	static FLinearColor GetActorLabelColor(const AActor& Actor, const CapSimObjectLabel& Label);

	static bool IsThing(const CapSimObjectLabel& Label);
	
	ATagger();

// TODO: check what this is for

protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR


private:
	UPROPERTY(Category = "Tagger", EditAnywhere)
	bool bTriggerTagObjects = false;

	UPROPERTY(Category = "Tagger", EditAnywhere)
	bool bTagForSemanticSegmentation = false;
};