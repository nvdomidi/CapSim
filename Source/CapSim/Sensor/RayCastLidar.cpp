#include "CapSim/Sensor/RayCastLidar.h"
#include "CapSim/Game/CapSimEngine.h"

#include <PxScene.h>
#include <cmath>

#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"


ARayCastLidar::ARayCastLidar() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	RandomEngine = CreateDefaultSubobject<URandomEngine>(TEXT("RandomEngine"));
	SetSeed(Description.RandomSeed);
}

void ARayCastLidar::BeginPlay()
{
	Super::BeginPlay();
}

void ARayCastLidar::InitializeLidar()
{
	LidarData = FLidarData(Description.Channels);
	CreateLasers();
	PointsPerChannel.resize(Description.Channels);

	// Compute drop off model parameters
	DropOffBeta = 1.0f - Description.DropOffAtZeroIntensity;
	DropOffAlpha = Description.DropOffAtZeroIntensity / Description.DropOffIntensityLimit;
	DropOffGenActive = Description.DropOffGenRate > std::numeric_limits<float>::epsilon();
}

void ARayCastLidar::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaTime)
{
	
	// Use this for partial lidar
	//SimulateLidar(DeltaTime);

	// USE NAVID's CODE FOR FULL 360 DEGREES
	ProcessLidar();

	if (bIsRecording) {

		UE_LOG(LogTemp, Warning, TEXT("LidarSensor started"));

		LidarData.PrintLidarDetections();

		FString filePathLabel = FString::Printf(TEXT("%s/%d.ply"), *this->folderPath, FCapSimEngine::GetFrameCounter());

		UE_LOG(LogTemp, Warning, TEXT("path: %s"), *filePathLabel);

		FFileHelper::SaveStringToFile(LidarData.PointString, *filePathLabel);
	}
}

void ARayCastLidar::StartRecording()
{
	bIsRecording = true;
}

void ARayCastLidar::StopRecording()
{
	bIsRecording = false;
}

void ARayCastLidar::SetCapturePath(const FString path)
{
	UE_LOG(LogTemp, Warning, TEXT("SX123: %s"), *path);
	this->folderPath = path;
}


float ARayCastLidar::ComputeIntensity(const FSemanticDetection& RawDetection) const
{
	const FVector HitPoint = RawDetection.point;

	const float Distance = HitPoint.Size();

	const float AttenAtm = Description.AtmospAttenRate;
	const float AbsAtm = exp(-AttenAtm * Distance);

	const float IntRec = AbsAtm;

	return IntRec;
}

ARayCastLidar::FDetection ARayCastLidar::ComputeDetection(const FHitResult& HitInfo, const FTransform& SensorTransf) const
{
	FDetection Detection;
	const FVector HitPoint = HitInfo.ImpactPoint;
	Detection.point = SensorTransf.Inverse().TransformPosition(HitPoint);

	const float Distance = Detection.point.Size();

	const float AttenAtm = Description.AtmospAttenRate;
	const float AbsAtm = exp(-AttenAtm * Distance);

	const float IntRec = AbsAtm;

	Detection.intensity = IntRec;

	return Detection;
}

void ARayCastLidar::PreprocessRays(uint32_t Channels, uint32_t MaxPointsPerChannel)
{
	Super::PreprocessRays(Channels, MaxPointsPerChannel);

	for (auto ch = 0u; ch < Channels; ch++) {
		for (auto p = 0u; p < MaxPointsPerChannel; p++) {
			RayPreprocessCondition[ch][p] = !(DropOffGenActive && RandomEngine->GetUniformFloat() < Description.DropOffGenRate);
		}
	}
}

bool ARayCastLidar::PostprocessDetection(FDetection& Detection) const
{
	if (Description.NoiseStdDev > std::numeric_limits<float>::epsilon()) {
		
		//const auto ForwardVector = Detection.point.MakeUnitVector();
		const auto ForwardVector = Detection.point.Normalize();
		const auto Noise = ForwardVector * RandomEngine->GetNormalDistribution(0.0f, Description.NoiseStdDev);
		Detection.point += FVector(Noise);
		
	}

	const float Intensity = Detection.intensity;
	if (Intensity > Description.DropOffIntensityLimit)
		return true;
	else
		return RandomEngine->GetUniformFloat() < DropOffAlpha * Intensity + DropOffBeta;
}

void ARayCastLidar::ComputeAndSaveDetections(const FTransform& SensorTransform)
{
	for (auto idxChannel = 0u; idxChannel < Description.Channels; ++idxChannel)
		PointsPerChannel[idxChannel] = RecordedHits[idxChannel].size();

	LidarData.ResetMemory(PointsPerChannel);

	for (auto idxChannel = 0u; idxChannel < Description.Channels; ++idxChannel) {
		for (auto& hit : RecordedHits[idxChannel]) {
			FDetection Detection = ComputeDetection(hit, SensorTransform);
			if (PostprocessDetection(Detection))
				LidarData.WritePointSync(Detection);
			else
				PointsPerChannel[idxChannel]--;
		}
	}

	LidarData.WriteChannelCount(PointsPerChannel);
}



void ARayCastLidar::CaptureScene(FString path)
{

	if (path.IsEmpty())
	{
		path = FString::Printf(TEXT("%s/capture.ply"), *this->folderPath);
	}

	ProcessLidar();

	LidarData.PrintLidarDetections();

	FFileHelper::SaveStringToFile(LidarData.PointString, *path);
}