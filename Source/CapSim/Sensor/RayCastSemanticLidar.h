#pragma once

#include "CapSim/Sensor/Sensor.h"

#include "CapSim/Sensor/LidarDescription.h"
#include "CapSim/Sensor/SemanticLidarData.h"

#include "RayCastSemanticLidar.generated.h"


/// A ray-cast based Lidar sensor.
UCLASS()
class CAPSIM_API ARayCastSemanticLidar : public ASensor
{
  GENERATED_BODY()

protected:

  using FSemanticLidarData = SemanticLidarData;
  using FSemanticDetection = SemanticLidarDetection;

public:

  ARayCastSemanticLidar();

  UFUNCTION(BlueprintCallable, Category = "CapSim")
      virtual void StartRecording();

  UFUNCTION(BlueprintCallable, Category = "CapSim")
      virtual void StopRecording();

  UFUNCTION(BlueprintCallable, Category = "CapSim")
      virtual void SetCapturePath(const FString path);

  UFUNCTION(BlueprintCallable, Category = "CapSim")
      virtual void CaptureScene(FString path);

  UFUNCTION(BlueprintCallable, Category = "CapSim")
      void SetLidarParameters(int Channels = 32,
          float Range = 1000.0f,
          int PointsPerSecond = 56000,
          float RotationFrequency = 10.0f,
          float UpperFovLimit = 30.0f,
          float LowerFovLimit = -30.0f,
          float HorizontalFov = 360.0f,
          float AtmospAttenRate = 0.004f,
          int RandomSeed = 0,
          float DropOffGenRate = 0.45f,
          float DropOffIntensityLimit = 0.8f,
          float DropOffAtZeroIntensity = 0.4f,
          bool ShowDebugPoints = false,
          float NoiseStdDev = 0.0f,
          int HorizontalPointsPerLaser = 1000);

  UFUNCTION(BlueprintCallable, Category = "CapSim")
      virtual void InitializeLidar();


protected:

  virtual void BeginPlay() override;

  /// Creates a Laser for each channel.
  void CreateLasers();


  // PostPhysTick
  virtual void PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaTime) override;


  /// Updates LidarMeasurement with the points read in DeltaTime.
  void SimulateLidar(const float DeltaTime);

  /// Shoot a laser ray-trace, return whether the laser hit something.
  bool ShootLaser(const float VerticalAngle, float HorizontalAngle, FHitResult &HitResult, FCollisionQueryParams& TraceParams) const;

  /// Method that allow to preprocess if the rays will be traced.
  virtual void PreprocessRays(uint32_t Channels, uint32_t MaxPointsPerChannel);

  /// Compute all raw detection information
  void ComputeRawDetection(const FHitResult &HitInfo, const FTransform &SensorTransf, FSemanticDetection &Detection) const;

  /// Saving the hits the raycast returns per channel
  void WritePointAsync(uint32_t Channel, FHitResult &Detection);

  /// Clear the recorded data structure
  void ResetRecordedHits(uint32_t Channels, uint32_t MaxPointsPerChannel);

  /// This method uses all the saved FHitResults, compute the
  /// RawDetections and then send it to the LidarData structure.
  virtual void ComputeAndSaveDetections(const FTransform &SensorTransform);
  
  void ProcessLidar();
  

  UPROPERTY(EditAnywhere)
  FLidarDescription Description;

  TArray<float> LaserAngles;

  std::vector<std::vector<FHitResult>> RecordedHits;
  std::vector<std::vector<bool>> RayPreprocessCondition;
  std::vector<uint32_t> PointsPerChannel;

  

private:

  bool bIsRecording = false;

  FSemanticLidarData SemanticLidarData;

  FString folderPath = "D:/CapSim_Images/SemanticLidar";

};
