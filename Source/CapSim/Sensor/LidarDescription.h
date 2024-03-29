#pragma once

#include "LidarDescription.generated.h"


USTRUCT()
struct CAPSIM_API FLidarDescription
{
  GENERATED_BODY()

  /// Number of lasers.
  UPROPERTY(EditAnywhere)
  uint32 Channels = 32u;

  /// Measure distance in centimeters.
  UPROPERTY(EditAnywhere)
  float Range = 1000.0f;

  /// Points generated by all lasers per second.
  UPROPERTY(EditAnywhere)
  uint32 PointsPerSecond = 56000u;

  /// Lidar rotation frequency.
  UPROPERTY(EditAnywhere)
  float RotationFrequency = 10.0f;

  /// Upper laser angle, counts from horizontal, positive values means above
  /// horizontal line.
  UPROPERTY(EditAnywhere)
  float UpperFovLimit = 30.0f;

  /// Lower laser angle, counts from horizontal, negative values means under
  /// horizontal line.
  UPROPERTY(EditAnywhere)
  float LowerFovLimit = -30.0f;
  
  /// Horizontal field of view
  UPROPERTY(EditAnywhere)
  float HorizontalFov = 360.0f;

  /// Attenuation Rate in the atmosphere in m^-1.
  UPROPERTY(EditAnywhere)
  float AtmospAttenRate = 0.004f;

  /// Random seed for the noise/dropoff used by this sensor.
  UPROPERTY(EditAnywhere)
  int RandomSeed = 0;

  /// General drop off rate.
  UPROPERTY(EditAnywhere)
  float DropOffGenRate = 0.45f;

  /// General drop off rate.
  UPROPERTY(EditAnywhere)
  float DropOffIntensityLimit = 0.8f;

  /// General drop off rate.
  UPROPERTY(EditAnywhere)
  float DropOffAtZeroIntensity = 0.4f;

  /// Wether to show debug points of laser hits in simulator.
  UPROPERTY(EditAnywhere)
  bool ShowDebugPoints = false;

  UPROPERTY(EditAnywhere)
  float NoiseStdDev = 0.0f;

  UPROPERTY(EditAnywhere)
  uint32 HorizontalPointsPerLaser = 1000;

  FLidarDescription(int Channels,
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
      int HorizontalPointsPerLaser) : Channels(Channels),
      Range(Range),
      PointsPerSecond(PointsPerSecond),
      RotationFrequency(RotationFrequency),
      UpperFovLimit(UpperFovLimit),
      LowerFovLimit(LowerFovLimit),
      HorizontalFov(HorizontalFov),
      AtmospAttenRate(AtmospAttenRate),
      RandomSeed(RandomSeed),
      DropOffGenRate(DropOffGenRate),
      DropOffIntensityLimit(DropOffIntensityLimit),
      DropOffAtZeroIntensity(DropOffAtZeroIntensity),
      ShowDebugPoints(ShowDebugPoints),
      NoiseStdDev(NoiseStdDev),
      HorizontalPointsPerLaser(HorizontalPointsPerLaser){}

   FLidarDescription(){}


};
