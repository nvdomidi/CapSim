#pragma once


class CAPSIM_API FRayCastUtils{

public:

  ~FRayCastUtils();

  
  /** Trace a ray against the world without blocking the physics scene and return the first blocking hit */
  static bool ParallelRaycastSingle(const UWorld* World, struct FHitResult& OutHit, const FVector Start, const FVector End,
	ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParams,
	const FCollisionObjectQueryParams& ObjectParams = FCollisionObjectQueryParams::DefaultObjectQueryParam);

  static bool ParallelLineTraceSingleByChannel(const UWorld* World, struct FHitResult& OutHit,const FVector& Start,const FVector& End,ECollisionChannel TraceChannel,
  	const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,
  	const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam);

};

