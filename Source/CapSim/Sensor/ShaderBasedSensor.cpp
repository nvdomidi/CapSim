#include "CapSim/Sensor/ShaderBasedSensor.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

bool AShaderBasedSensor::AddPostProcessingMaterial(const FString &Path)
{
  UE_LOG(LogTemp, Warning, TEXT("MatLoad: %s"), *Path);

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
    UE_LOG(LogTemp, Warning, TEXT("callingfunction SetupSceneCaptureComponent, Materials found: %d"), MaterialsFound.Num());

  for (const auto &MaterialFound : MaterialsFound)
  {
    // Create a dynamic instance of the Material (Shader)
    AddShader({UMaterialInstanceDynamic::Create(MaterialFound, this), 1.0});


    UE_LOG(LogTemp, Warning, TEXT("MatLoader: %s"), *(MaterialFound->GetFName().ToString()));
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
    this->SetFloatShaderParameter(0, TEXT("CircleFalloff_NState"), val);
}

void AShaderBasedSensor::SetLensCircleMultiplier(float val)
{
    //this->SetFloatShaderParameter(0, TEXT("CircleMultiplier_NState"), val);

    Shaders[0].PostProcessMaterial->SetScalarParameterValue(
        TEXT("CircleMultiplier_NState"),
        val);

    //CaptureComponent2D->PostProcessSettings.RemoveBlendable(Shaders[0].PostProcessMaterial);
    UE_LOG(LogTemp, Warning, TEXT("MatLoader: SetLensCircleMultiplier"));

}

void AShaderBasedSensor::SetLensK(float val)
{
    this->SetFloatShaderParameter(0, TEXT("K_NState"), val);
}

void AShaderBasedSensor::SetLensKCube(float val)
{
    this->SetFloatShaderParameter(0, TEXT("kcube"), val);
}

void AShaderBasedSensor::SetLensXSize(float val)
{
    this->SetFloatShaderParameter(0, TEXT("XSize_NState"), val);
}

void AShaderBasedSensor::SetLensYSize(float val)
{
    this->SetFloatShaderParameter(0, TEXT("YSize_NState"), val);
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
