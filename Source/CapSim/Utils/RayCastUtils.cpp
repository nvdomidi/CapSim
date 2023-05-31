#include "CapSim/Utils/RayCastUtils.h"

FRayCastUtils::~FRayCastUtils()
{

}

bool FRayCastUtils::ParallelRaycastSingle(const UWorld* World, struct FHitResult& OutHit, const FVector Start,
			const FVector End, ECollisionChannel TraceChannel, const struct FCollisionQueryParams& Params,
			const struct FCollisionResponseParams& ResponseParams, const struct FCollisionObjectQueryParams& ObjectParams)
{
	/*
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__);

	SCOPE_CYCLE_COUNTER(STAT_Collision_SceneQueryTotal);
	SCOPE_CYCLE_COUNTER(STAT_Collision_RaycastSingle);
	CSV_SCOPED_TIMING_STAT(SceneQuery, RaycastSingle);
*/
	//using TCastTraits = TSQTraits<FHitRaycast, ESweepOrRay::Raycast, ESingleMultiOrTest::Single>;
	//return TSceneCastCommon<TCastTraits, false>(World, OutHit, FRaycastSQAdditionalInputs(), Start, End, TraceChannel, Params, ResponseParams, ObjectParams);
	return true;
}

bool FRayCastUtils::ParallelLineTraceSingleByChannel(const UWorld* World, struct FHitResult& OutHit,const FVector& Start,const FVector& End,
			ECollisionChannel TraceChannel,const FCollisionQueryParams& Params /* = FCollisionQueryParams::DefaultQueryParam */,
			const FCollisionResponseParams& ResponseParam /* = FCollisionResponseParams::DefaultResponseParam */)
{
	return ParallelRaycastSingle(World, OutHit, Start, End, TraceChannel, Params, ResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);
}