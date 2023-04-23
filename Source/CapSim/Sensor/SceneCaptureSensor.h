// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapSim/Sensor/Sensor.h"
#include "CapSim/Sensor/PixelReader.h"
#include "Components/SceneCaptureComponent2D.h"
#include "SceneCaptureSensor.generated.h"

UCLASS()
class CAPSIM_API ASceneCaptureSensor : public ASensor
{
	GENERATED_BODY()

	friend class FPixelReader;
	
public:	
	// Sets default values for this actor's properties
	ASceneCaptureSensor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		USceneCaptureComponent2D* GetCaptureComponent2D()
	{
		return CaptureComponent2D;
	}

	UFUNCTION(BlueprintCallable)
		UTextureRenderTarget2D* GetCaptureRenderTarget()
	{
		return CaptureRenderTarget;
	}

	void SetImageSize(uint32 Width, uint32 Height);

	UFUNCTION(BlueprintCallable)
	void SetImageSizeAndUpdate(int Width, int Height);

	uint32 GetImageWidth() const
	{
		return ImageWidth;
	}

	uint32 GetImageHeight() const
	{
		return ImageHeight;
	}

	UFUNCTION(BlueprintCallable)
		void EnablePostProcessingEffects(bool Enable = true)
	{
		bEnablePostProcessingEffects = Enable;

		//InitializePostProcessingEffects();
	}

	UFUNCTION(BlueprintCallable)
		bool ArePostProcessingEffectsEnabled() const
	{
		return bEnablePostProcessingEffects;
	}

	UFUNCTION(BlueprintCallable)
		void Enable16BitFormat(bool Enable = false)
	{
		bEnable16BitFormat = Enable;
	}

	UFUNCTION(BlueprintCallable)
		bool Is16BitFormatEnabled() const
	{
		return bEnable16BitFormat;
	}

	UFUNCTION(BlueprintCallable)
		void SetFOVAngle(float FOVAngle);

	UFUNCTION(BlueprintCallable)
		float GetFOVAngle() const;

	UFUNCTION(BlueprintCallable)
		void SetTargetGamma(float InTargetGamma)
	{
		TargetGamma = InTargetGamma;
	}

	UFUNCTION(BlueprintCallable)
		float GetTargetGamma() const
	{
		return TargetGamma;
	}

	UFUNCTION(BlueprintCallable)
		void SetExposureMethod(EAutoExposureMethod Method);

	UFUNCTION(BlueprintCallable)
		EAutoExposureMethod GetExposureMethod() const;

	UFUNCTION(BlueprintCallable)
		void SetExposureCompensation(float Compensation);

	UFUNCTION(BlueprintCallable)
		float GetExposureCompensation() const;

	UFUNCTION(BlueprintCallable)
		void SetShutterSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
		float GetShutterSpeed() const;

	UFUNCTION(BlueprintCallable)
		void SetISO(float ISO);

	UFUNCTION(BlueprintCallable)
		float GetISO() const;

	UFUNCTION(BlueprintCallable)
		void SetAperture(float Aperture);

	UFUNCTION(BlueprintCallable)
		float GetAperture() const;

	UFUNCTION(BlueprintCallable)
		void SetFocalDistance(float Distance);

	UFUNCTION(BlueprintCallable)
		float GetFocalDistance() const;

	UFUNCTION(BlueprintCallable)
		void SetDepthBlurAmount(float Amount);

	UFUNCTION(BlueprintCallable)
		float GetDepthBlurAmount() const;

	UFUNCTION(BlueprintCallable)
		void SetDepthBlurRadius(float Radius);

	UFUNCTION(BlueprintCallable)
		float GetDepthBlurRadius() const;

	UFUNCTION(BlueprintCallable)
		void SetBladeCount(int Count);

	UFUNCTION(BlueprintCallable)
		int GetBladeCount() const;

	UFUNCTION(BlueprintCallable)
		void SetDepthOfFieldMinFstop(float MinFstop);

	UFUNCTION(BlueprintCallable)
		float GetDepthOfFieldMinFstop() const;

	UFUNCTION(BlueprintCallable)
		void SetFilmSlope(float Slope);

	UFUNCTION(BlueprintCallable)
		float GetFilmSlope() const;

	UFUNCTION(BlueprintCallable)
		void SetFilmToe(float Toe);

	UFUNCTION(BlueprintCallable)
		float GetFilmToe() const;

	UFUNCTION(BlueprintCallable)
		void SetFilmShoulder(float Shoulder);

	UFUNCTION(BlueprintCallable)
		float GetFilmShoulder() const;

	UFUNCTION(BlueprintCallable)
		void SetFilmBlackClip(float BlackClip);

	UFUNCTION(BlueprintCallable)
		float GetFilmBlackClip() const;

	UFUNCTION(BlueprintCallable)
		void SetFilmWhiteClip(float WhiteClip);

	UFUNCTION(BlueprintCallable)
		float GetFilmWhiteClip() const;

	UFUNCTION(BlueprintCallable)
		void SetExposureMinBrightness(float Brightness);

	UFUNCTION(BlueprintCallable)
		float GetExposureMinBrightness() const;

	UFUNCTION(BlueprintCallable)
		void SetExposureMaxBrightness(float Brightness);

	UFUNCTION(BlueprintCallable)
		float GetExposureMaxBrightness() const;

	UFUNCTION(BlueprintCallable)
		void SetExposureSpeedDown(float Speed);

	UFUNCTION(BlueprintCallable)
		float GetExposureSpeedDown() const;

	UFUNCTION(BlueprintCallable)
		void SetExposureSpeedUp(float Speed);

	UFUNCTION(BlueprintCallable)
		float GetExposureSpeedUp() const;

	UFUNCTION(BlueprintCallable)
		void SetExposureCalibrationConstant(float Constant);

	UFUNCTION(BlueprintCallable)
		float GetExposureCalibrationConstant() const;

	UFUNCTION(BlueprintCallable)
		void SetMotionBlurIntensity(float Intensity);

	UFUNCTION(BlueprintCallable)
		float GetMotionBlurIntensity() const;

	UFUNCTION(BlueprintCallable)
		void SetMotionBlurMaxDistortion(float MaxDistortion);

	UFUNCTION(BlueprintCallable)
		float GetMotionBlurMaxDistortion() const;

	UFUNCTION(BlueprintCallable)
		void SetMotionBlurMinObjectScreenSize(float ScreenSize);

	UFUNCTION(BlueprintCallable)
		float GetMotionBlurMinObjectScreenSize() const;

	UFUNCTION(BlueprintCallable)
		void SetLensFlareIntensity(float Intensity);

	UFUNCTION(BlueprintCallable)
		float GetLensFlareIntensity() const;

	UFUNCTION(BlueprintCallable)
		void SetBloomIntensity(float Intensity);

	UFUNCTION(BlueprintCallable)
		float GetBloomIntensity() const;

	UFUNCTION(BlueprintCallable)
		void SetWhiteTemp(float Temp);

	UFUNCTION(BlueprintCallable)
		float GetWhiteTemp() const;

	UFUNCTION(BlueprintCallable)
		void SetWhiteTint(float Tint);

	UFUNCTION(BlueprintCallable)
		float GetWhiteTint() const;

	UFUNCTION(BlueprintCallable)
		void SetChromAberrIntensity(float Intensity);

	UFUNCTION(BlueprintCallable)
		float GetChromAberrIntensity() const;

	UFUNCTION(BlueprintCallable)
		void SetChromAberrOffset(float ChromAberrOffset);

	UFUNCTION(BlueprintCallable)
		float GetChromAberrOffset() const;

	UFUNCTION(BlueprintCallable)
		void ApplyPostProcessing();

	/// Use for debugging purposes only.
	UFUNCTION(BlueprintCallable)
		bool ReadPixels(TArray<FColor>& BitMap) const
	{
		check(CaptureRenderTarget != nullptr);
		// TODO: check if this works
		return FPixelReader::WritePixelsToArray(*CaptureRenderTarget, BitMap);
	}

	/// Use for debugging purposes only.
	UFUNCTION(BlueprintCallable)
		void SaveCaptureToDisk(const FString& FilePath) const
	{
		check(CaptureRenderTarget != nullptr);
		// TODO: FPixelReader::SavePixelsToDisk
		FPixelReader::SavePixelsToDisk(*CaptureRenderTarget, FilePath);
	}

	/// Immediate enqueues render commands of the scene at the current time.
	void EnqueueRenderSceneImmediate();

	/// Blocks until the render thread has finished all it's tasks.
	void WaitForRenderThreadToFinish() {
		TRACE_CPUPROFILER_EVENT_SCOPE(ASceneCaptureSensor::WaitForRenderThreadToFinish);
		// FlushRenderingCommands();
	}

protected:

	/// Render target necessary for scene capture.
	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* CaptureRenderTarget = nullptr;

	/// Scene capture component.
	UPROPERTY(EditAnywhere)
		USceneCaptureComponent2D* CaptureComponent2D = nullptr;

	UPROPERTY(EditAnywhere)
		float TargetGamma = 2.4f;

	/// Image width in pixels.
	UPROPERTY(EditAnywhere)
		uint32 ImageWidth = 800u;

	/// Image height in pixels.
	UPROPERTY(EditAnywhere)
		uint32 ImageHeight = 600u;

	/// Whether to render the post-processing effects present in the scene.
	UPROPERTY(EditAnywhere)
		bool bEnablePostProcessingEffects = true;

	/// Whether to change render target format to PF_A16B16G16R16, offering 16bit / channel
	UPROPERTY(EditAnywhere)
		bool bEnable16BitFormat = false;

	virtual void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds) override;

	virtual void SetUpSceneCaptureComponent(USceneCaptureComponent2D& SceneCapture) {}

	void CaptureSceneExtended();

};
