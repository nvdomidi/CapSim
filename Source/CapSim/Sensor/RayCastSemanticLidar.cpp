#include "CapSim/Sensor/RayCastSemanticLidar.h"
#include "CapSim/Game/CapSimEngine.h"

#include <PxScene.h>
#include <cmath>

#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"


ARayCastSemanticLidar::ARayCastSemanticLidar() : Super()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ARayCastSemanticLidar::BeginPlay()
{
  Super::BeginPlay();
  //FLidarDescription LidarDescription;
  //Description = LidarDescription;
  //SemanticLidarData = FSemanticLidarData(Description.Channels);
  //CreateLasers();
  //PointsPerChannel.resize(Description.Channels);
}

void ARayCastSemanticLidar::SetSemanticLidarParameters(int Channels,
    float Range,
    int PointsPerSecond,
    float RotationFrequency,
    float UpperFovLimit,
    float LowerFovLimit,
    float HorizontalFov,
    float AtmospAttenRate,
    int RandomSeed,
    float DropOffGenRate,
    float DropOffIntensityLimit,
    float DropOffAtZeroIntensity,
    bool ShowDebugPoints,
    float NoiseStdDev,
    int HorizontalPointsPerLaser)
{
    FLidarDescription LidarDescription(Channels,
        Range,
        PointsPerSecond,
        RotationFrequency,
        UpperFovLimit,
        LowerFovLimit,
        HorizontalFov,
        AtmospAttenRate,
        RandomSeed,
        DropOffGenRate,
        DropOffIntensityLimit,
        DropOffAtZeroIntensity,
        ShowDebugPoints,
        NoiseStdDev,
        HorizontalPointsPerLaser);

    Description = LidarDescription;
}

void ARayCastSemanticLidar::InitializeSemanticLidar()
{
  SemanticLidarData = FSemanticLidarData(Description.Channels);
  CreateLasers();
  PointsPerChannel.resize(Description.Channels);
}


void ARayCastSemanticLidar::CreateLasers()
{
  const auto NumberOfLasers = Description.Channels;
  check(NumberOfLasers > 0u);
  const float DeltaAngle = NumberOfLasers == 1u ? 0.f :
    (Description.UpperFovLimit - Description.LowerFovLimit) /
    static_cast<float>(NumberOfLasers - 1);
  LaserAngles.Empty(NumberOfLasers);
  for(auto i = 0u; i < NumberOfLasers; ++i)
  {
    const float VerticalAngle =
        Description.UpperFovLimit - static_cast<float>(i) * DeltaAngle;
    LaserAngles.Emplace(VerticalAngle);
  }
}

void ARayCastSemanticLidar::PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaTime)
{
  // Use this for partial lidar
  //SimulateLidar(DeltaTime);

  // USE NAVID's CODE FOR FULL 360 DEGREES
  ProcessLidar();

  if (bIsRecording) {

      SemanticLidarData.PrintSemanticLidarDetections();
      
      FString filePathLabel = FString::Printf(TEXT("%s/%d.ply"), *this->folderPath, FCapSimEngine::GetFrameCounter());

      FFileHelper::SaveStringToFile(SemanticLidarData.PointString, *filePathLabel);
  }
  
}

// Scan some points in the current frame
void ARayCastSemanticLidar::SimulateLidar(const float DeltaTime)
{
  //TRACE_CPUPROFILER_EVENT_SCOPE(ARayCastSemanticLidar::SimulateLidar);
  const uint32 ChannelCount = Description.Channels;
  const uint32 PointsToScanWithOneLaser =
    FMath::RoundHalfFromZero(
        Description.PointsPerSecond * DeltaTime / float(ChannelCount));

  if (PointsToScanWithOneLaser <= 0)
  {
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s: no points requested this frame, try increasing the number of points per second."),
        *GetName());
    return;
  }

  check(ChannelCount == LaserAngles.Num());

  const float CurrentHorizontalAngle = FMath::RadiansToDegrees(
      SemanticLidarData.GetHorizontalAngle());
  const float AngleDistanceOfTick = Description.RotationFrequency * Description.HorizontalFov
      * DeltaTime;
  const float AngleDistanceOfLaserMeasure = AngleDistanceOfTick / PointsToScanWithOneLaser;

  ResetRecordedHits(ChannelCount, PointsToScanWithOneLaser);
  PreprocessRays(ChannelCount, PointsToScanWithOneLaser);

  GetWorld()->GetPhysicsScene()->GetPxScene()->lockRead();
  {
    TRACE_CPUPROFILER_EVENT_SCOPE(ParallelFor);
    ParallelFor(ChannelCount, [&](int32 idxChannel) {
      TRACE_CPUPROFILER_EVENT_SCOPE(ParallelForTask);


      FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Laser_Trace")), true, this);
      TraceParams.bTraceComplex = true;
      TraceParams.bReturnPhysicalMaterial = false;

      for (auto idxPtsOneLaser = 0u; idxPtsOneLaser < PointsToScanWithOneLaser; idxPtsOneLaser++) {
        FHitResult HitResult;
        const float VertAngle = LaserAngles[idxChannel];
        const float HorizAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfLaserMeasure
            * idxPtsOneLaser, Description.HorizontalFov) - Description.HorizontalFov / 2;
        const bool PreprocessResult = RayPreprocessCondition[idxChannel][idxPtsOneLaser];

        if (PreprocessResult && ShootLaser(VertAngle, HorizAngle, HitResult, TraceParams)) {
          WritePointAsync(idxChannel, HitResult);
        }
      };
    });
  }
  GetWorld()->GetPhysicsScene()->GetPxScene()->unlockRead();

  FTransform ActorTransf = GetTransform();
  ComputeAndSaveDetections(ActorTransf);

  const float HorizontalAngle = FMath::DegreesToRadians(
      std::fmod(CurrentHorizontalAngle + AngleDistanceOfTick, Description.HorizontalFov));

  UE_LOG(LogTemp, Warning, TEXT("CurrentHorizontalAngle: %f, AngleDistanceOfTick: %f, HorizontalAngle: %f, DeltaTime: %f"),
      CurrentHorizontalAngle, AngleDistanceOfTick, HorizontalAngle, DeltaTime);

  SemanticLidarData.SetHorizontalAngle(HorizontalAngle);
}

void ARayCastSemanticLidar::ResetRecordedHits(uint32_t Channels, uint32_t MaxPointsPerChannel) {
  RecordedHits.resize(Channels);

  for (auto& hits : RecordedHits) {
    hits.clear();
    hits.reserve(MaxPointsPerChannel);
  }
}

void ARayCastSemanticLidar::PreprocessRays(uint32_t Channels, uint32_t MaxPointsPerChannel) {
  RayPreprocessCondition.resize(Channels);

  for (auto& conds : RayPreprocessCondition) {
    conds.clear();
    conds.resize(MaxPointsPerChannel);
    std::fill(conds.begin(), conds.end(), true);
  }
}

void ARayCastSemanticLidar::WritePointAsync(uint32_t channel, FHitResult &detection) {
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__);
  //check(GetChannelCount() > channel);
  RecordedHits[channel].emplace_back(detection);
}

void ARayCastSemanticLidar::ComputeAndSaveDetections(const FTransform& SensorTransform) {
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__);

    for (auto idxChannel = 0u; idxChannel < Description.Channels; ++idxChannel)
    {
        PointsPerChannel[idxChannel] = RecordedHits[idxChannel].size();
    }

  SemanticLidarData.ResetMemory(PointsPerChannel);

  for (auto idxChannel = 0u; idxChannel < Description.Channels; ++idxChannel) {
    for (auto& hit : RecordedHits[idxChannel]) {
      FSemanticDetection detection;
      ComputeRawDetection(hit, SensorTransform, detection);
      SemanticLidarData.WritePointSync(detection);
    }
  }

  SemanticLidarData.WriteChannelCount(PointsPerChannel);
}

void ARayCastSemanticLidar::ComputeRawDetection(const FHitResult& HitInfo, const FTransform& SensorTransf, FSemanticDetection& Detection) const
{
    const FVector HitPoint = HitInfo.ImpactPoint;
    Detection.point = SensorTransf.Inverse().TransformPosition(HitPoint);

    const FVector VecInc = - (HitPoint - SensorTransf.GetLocation()).GetSafeNormal();
    Detection.cos_inc_angle = FVector::DotProduct(VecInc, HitInfo.ImpactNormal);

    //const FActorRegistry &Registry = GetEpisode().GetActorRegistry();

    const AActor* actor = HitInfo.Actor.Get();
    Detection.object_idx = 0;
    Detection.object_tag = static_cast<uint32_t>(HitInfo.Component->CustomDepthStencilValue);

    if (actor != nullptr) {

      /*
      const FCarlaActor* view = Registry.FindCarlaActor(actor);
      if(view)
        Detection.object_idx = view->GetActorId();
      */
        Detection.object_idx = actor->GetUniqueID();

    }
    else {
      UE_LOG(LogTemp, Warning, TEXT("Actor not valid %p!!!!"), actor);
    }
}


bool ARayCastSemanticLidar::ShootLaser(const float VerticalAngle, const float HorizontalAngle, FHitResult& HitResult, FCollisionQueryParams& TraceParams) const
{
  TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__);

  FHitResult HitInfo(ForceInit);

  FTransform ActorTransf = GetTransform();
  FVector LidarBodyLoc = ActorTransf.GetLocation();
  FRotator LidarBodyRot = ActorTransf.Rotator();

  FRotator LaserRot (VerticalAngle, HorizontalAngle, 0);  // float InPitch, float InYaw, float InRoll
  FRotator ResultRot = UKismetMathLibrary::ComposeRotators(
    LaserRot,
    LidarBodyRot
  );

  const auto Range = Description.Range;
  FVector EndTrace = Range * UKismetMathLibrary::GetForwardVector(ResultRot) + LidarBodyLoc;


  GetWorld()->LineTraceSingleByChannel(
    HitInfo,
    LidarBodyLoc,
    EndTrace,
    ECC_GameTraceChannel2,
    TraceParams,
    FCollisionResponseParams::DefaultResponseParam
  );

  if (HitInfo.bBlockingHit) {
    HitResult = HitInfo;
    return true;
  } else {
    return false;
  }
}

void ARayCastSemanticLidar::StartRecording()
{
    bIsRecording = true;
}

void ARayCastSemanticLidar::StopRecording()
{
    bIsRecording = false;
}

void ARayCastSemanticLidar::SetCapturePath(const FString path)
{
    this->folderPath = path;
}


void ARayCastSemanticLidar::ProcessLidar()
{
    const uint32 ChannelCount = Description.Channels;
    const uint32 PointsToScanWithOneLaser = Description.HorizontalPointsPerLaser;
    const float CurrentHorizontalAngle = 0;
    const float AngleDistanceOfTick = Description.HorizontalFov / (static_cast<float>(PointsToScanWithOneLaser - 1));
    const float AngleDistanceOfLaserMeasure = AngleDistanceOfTick;

    ResetRecordedHits(ChannelCount, PointsToScanWithOneLaser);
    PreprocessRays(ChannelCount, PointsToScanWithOneLaser);

    GetWorld()->GetPhysicsScene()->GetPxScene()->lockRead();
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(ParallelFor);
        ParallelFor(ChannelCount, [&](int32 idxChannel) {
            TRACE_CPUPROFILER_EVENT_SCOPE(ParallelForTask);

            FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Laser_Trace")), true, this);
            TraceParams.bTraceComplex = true;
            TraceParams.bReturnPhysicalMaterial = false;

            for (auto idxPtsOneLaser = 0u; idxPtsOneLaser < PointsToScanWithOneLaser; idxPtsOneLaser++) {
                FHitResult HitResult;
                const float VertAngle = LaserAngles[idxChannel];
                const float HorizAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfLaserMeasure
                    * idxPtsOneLaser, Description.HorizontalFov) - Description.HorizontalFov / 2;
                const bool PreprocessResult = RayPreprocessCondition[idxChannel][idxPtsOneLaser];

                if (PreprocessResult && ShootLaser(VertAngle, HorizAngle, HitResult, TraceParams)) {
                    WritePointAsync(idxChannel, HitResult);
                }
            };
            });
    }
    GetWorld()->GetPhysicsScene()->GetPxScene()->unlockRead();

    FTransform ActorTransf = GetTransform();
    ComputeAndSaveDetections(ActorTransf);

}


void ARayCastSemanticLidar::CaptureScene(FString path)
{

    if (path.IsEmpty())
    {
        path = FString::Printf(TEXT("%s/capture.ply"), *this->folderPath);
    }

    ProcessLidar();

    SemanticLidarData.PrintSemanticLidarDetections();

    FFileHelper::SaveStringToFile(SemanticLidarData.PointString, *path);
}
