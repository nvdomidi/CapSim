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
