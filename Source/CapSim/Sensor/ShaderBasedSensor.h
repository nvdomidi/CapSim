#pragma once

#include "CapSim/Sensor/SceneCaptureSensor.h"

#include "ShaderBasedSensor.generated.h"

/// A shader parameter value to change when the material
/// instance is available.
USTRUCT(BlueprintType)
struct CAPSIM_API FShaderFloatParameterValue
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int ShaderIndex;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName ParameterName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float Value = 0.0f;
};

/// A shader in AShaderBasedSensor.
USTRUCT(BlueprintType)
struct CAPSIM_API FSensorShader
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UMaterialInstanceDynamic *PostProcessMaterial = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float Weight = 1.0f;
};

/// A sensor that produces data by applying post-process materials (shaders) to
/// a scene capture image.
///
/// @warning Shaders must be added before BeginPlay.
UCLASS(Abstract)
class CAPSIM_API AShaderBasedSensor : public ASceneCaptureSensor
{
  GENERATED_BODY()

public:

  AShaderBasedSensor() : Super()
  {
      PrimaryActorTick.bCanEverTick = true;
  }

  /// Load the UMaterialInstanceDynamic at the given @a Path and
  /// append it to the list of shaders with @a Weight.
  ///
  /// @return Whether it succeeded.
  UFUNCTION(BlueprintCallable)
  // bool LoadPostProcessingMaterial(const FString &Path, float Weight = 1.0f);
  bool AddPostProcessingMaterial(const FString &Path);

  /// Add a post-processing shader.
  UFUNCTION(BlueprintCallable)
  void AddShader(const FSensorShader &Shader)
  {
    Shaders.Add(Shader);
  }

  /* Functions added by Navid to change shader parameters */
  UFUNCTION(BlueprintCallable)
  void SetLensCircleFallOff(float val = 5.0f);

  UFUNCTION(BlueprintCallable)
  void SetLensCircleMultiplier(float val = 0.0f);

  UFUNCTION(BlueprintCallable)
  void SetLensK(float val = -1.0f);

  UFUNCTION(BlueprintCallable)
  void SetLensKCube(float val = 0.0f);

  UFUNCTION(BlueprintCallable)
  void SetLensXSize(float val = 0.08f);

  UFUNCTION(BlueprintCallable)
  void SetLensYSize(float val = 0.08f);

  UFUNCTION(BlueprintCallable)
  void SetDistortionNegative();

  UFUNCTION(BlueprintCallable)
  void SetDistortionPositive();


  void SetFloatShaderParameter(uint8_t ShaderIndex, const FName &ParameterName, float Value);

  void UpdateFloatShaderParameter(uint8_t ShaderIndex, const FName& ParameterName, float Value);

protected:

  void SetUpSceneCaptureComponent(USceneCaptureComponent2D &SceneCapture) override;

  bool IsDistortionNegative();

private:

  UPROPERTY()
  TArray<UMaterial*> MaterialsFound;

  UPROPERTY()
  TArray<FSensorShader> Shaders;

  UPROPERTY()
  TArray<FShaderFloatParameterValue> FloatShaderParams;

  bool bIsSetUp = false;
};
