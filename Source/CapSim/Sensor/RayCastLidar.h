#pragma once

#include "CapSim/Sensor/LidarDescription.h"
#include "CapSim/Sensor/Sensor.h"
#include "CapSim/Sensor/RayCastSemanticLidar.h"
#include "CapSim/Sensor/LidarData.h"

#include "RayCastLidar.generated.h"

/// A ray-cast based Lidar sensor.
UCLASS()
class CAPSIM_API ARayCastLidar : public ARayCastSemanticLidar
{
	GENERATED_BODY()

	using FLidarData = LidarData;
	using FDetection = LidarDetection;

public:
	ARayCastLidar();

	void BeginPlay() override;

	void InitializeLidar() override;

	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaTime) override;

	void StartRecording() override;

	void StopRecording() override;

	void SetCapturePath(const FString path) override;

	void CaptureScene(FString path) override;

private:
	/// Compute the received intensity of the point
	float ComputeIntensity(const FSemanticDetection& RawDetection) const;
	FDetection ComputeDetection(const FHitResult& HitInfo, const FTransform& SensorTransf) const;

	void PreprocessRays(uint32_t Channels, uint32_t MaxPointsPerChannel) override;
	bool PostprocessDetection(FDetection& Detection) const;

	void ComputeAndSaveDetections(const FTransform& SensorTransform) override;

	/// Enable/Disable general dropoff of lidar points
	bool DropOffGenActive;

	/// Slope for the intensity dropoff of lidar points, it is calculated
	/// throught the dropoff limit and the dropoff at zero intensity
	/// The points is kept with a probality alpha*Intensity + beta where
	/// alpha = (1 - dropoff_zero_intensity) / droppoff_limit
	/// beta = (1 - dropoff_zero_intensity)
	float DropOffAlpha;
	float DropOffBeta;

	// Added by Navid
	FLidarData LidarData;

	bool bIsRecording = false;

	FString folderPath = "D:/CapSim_Images/Lidar";
};
