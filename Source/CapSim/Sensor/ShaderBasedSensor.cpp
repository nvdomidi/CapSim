#include "CapSim/Sensor/ShaderBasedSensor.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

bool AShaderBasedSensor::AddPostProcessingMaterial(const FString &Path)
{
  ConstructorHelpers::FObjectFinder<UMaterial> Loader(*Path);
  if (Loader.Succeeded())
  {
      UE_LOG(LogTemp, Warning, TEXT("MatLoad: Found!"));
      MaterialsFound.Add(Loader.Object);
  }
  else {
      UE_LOG(LogTemp, Warning, TEXT("MatLoad: Not Found!"));
  }
  return Loader.Succeeded();
}

void AShaderBasedSensor::SetUpSceneCaptureComponent(USceneCaptureComponent2D &SceneCapture)
{
  for (const auto &MaterialFound : MaterialsFound)
  {
          // Create a dynamic instance of the Material (Shader)
          AddShader({ UMaterialInstanceDynamic::Create(MaterialFound, this), 1.0 });
          UE_LOG(LogTemp, Warning, TEXT("Postprocess material: %s"), *(MaterialFound->GetFName().ToString()));
  }

  for (const auto &Shader : Shaders)
  {
    // Attach the instance into the blendables
    SceneCapture.PostProcessSettings.AddBlendable(Shader.PostProcessMaterial, Shader.Weight);
  }

  // Set the value for each Float parameter in the shader
  for (const auto &ParameterValue : FloatShaderParams)
  {
    Shaders[ParameterValue.ShaderIndex].PostProcessMaterial->SetScalarParameterValue(
        ParameterValue.ParameterName,
        ParameterValue.Value);
  }
}


// Blueprint callable functions to edit shader parameters
void AShaderBasedSensor::SetLensCircleFallOff(float val)
{
    if (IsDistortionNegative()) {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("CircleFalloff_NState"),
            val);
    }
    else {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("CircleFalloff_PState"),
            val);
    }
}

void AShaderBasedSensor::SetLensCircleMultiplier(float val)
{
    if (IsDistortionNegative()) {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("CircleMultiplier_NState"),
            val);
    }
    else {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("CircleMultiplier_PState"),
            val);
    }
}

void AShaderBasedSensor::SetLensK(float val)
{
    if (IsDistortionNegative()) {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("K_NState"),
            val);
    }
    else {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("K_PState"),
            val);
    }
}

void AShaderBasedSensor::SetLensKCube(float val)
{
    Shaders[0].PostProcessMaterial->SetScalarParameterValue(
        TEXT("kcube"),
        val);
}

void AShaderBasedSensor::SetLensXSize(float val)
{
    if (IsDistortionNegative()) {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("XSize_NState"),
            val);
    }
    else {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("XSize_PState"),
            val);
    }
}

void AShaderBasedSensor::SetLensYSize(float val)
{
    if (IsDistortionNegative()) {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("YSize_NState"),
            val);
    }
    else {
        Shaders[0].PostProcessMaterial->SetScalarParameterValue(
            TEXT("YSize_PState"),
            val);
    }
}

void AShaderBasedSensor::SetDistortionNegative()
{
    Shaders[0].PostProcessMaterial->SetScalarParameterValue(
        TEXT("IsNegative"),
        1);
}

void AShaderBasedSensor::SetDistortionPositive()
{
    Shaders[0].PostProcessMaterial->SetScalarParameterValue(
        TEXT("IsNegative"),
        0);
}

/*  ------------------------------------------------------------------   */

void AShaderBasedSensor::SetFloatShaderParameter(
    uint8_t ShaderIndex,
    const FName &ParameterName,
    float Value)
{
  FloatShaderParams.Add({ShaderIndex, ParameterName, Value});
}

void AShaderBasedSensor::UpdateFloatShaderParameter(uint8_t ShaderIndex, const FName& ParameterName, float Value)
{
    for (auto& FloatParam : FloatShaderParams)
    {
        if (FloatParam.ParameterName == ParameterName)
        {
            FloatParam.Value = Value;
        }
    }
}

// helper function to check distortion negative or not
bool AShaderBasedSensor::IsDistortionNegative()
{
    float IsNegative;
    // check for negative or positive state
    Shaders[0].PostProcessMaterial->GetScalarParameterValue(TEXT("IsNegative"), IsNegative);

    if (IsNegative > 0.5f)
        return true;
    else
        return false;
}
