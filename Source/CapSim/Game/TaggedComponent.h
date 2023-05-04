#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/InstancedStaticMesh.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "TaggedComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class CAPSIM_API UTaggedComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UTaggedComponent(const FObjectInitializer& ObjectInitializer) {};

};