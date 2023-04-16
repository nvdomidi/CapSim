// Fill out your copyright notice in the Description page of Project Settings.


#include "CapSim/Sensor/SceneCaptureSensor.h"

#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

// Sets default values
ASceneCaptureSensor::ASceneCaptureSensor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	CaptureRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(
		FName(*FString::Printf(TEXT("CaptureRenderTarget"))));

	CaptureRenderTarget->CompressionSettings = TextureCompressionSettings::TC_Default;
	CaptureRenderTarget->SRGB = false;
	CaptureRenderTarget->bAutoGenerateMips = false;
	CaptureRenderTarget->bGPUSharedFlag = true;
	CaptureRenderTarget->AddressX = TextureAddress::TA_Clamp;
	CaptureRenderTarget->AddressY = TextureAddress::TA_Clamp;

	CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(
		FName(*FString::Printf(TEXT("USceneCaptureComponent2D"))));
	check(CaptureComponent2D != nullptr);
	//CaptureComponent2D->ViewActor = this;
	CaptureComponent2D->SetupAttachment(RootComponent);
	CaptureComponent2D->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	CaptureComponent2D->bCaptureOnMovement = false;
	CaptureComponent2D->bCaptureEveryFrame = false;
	CaptureComponent2D->bAlwaysPersistRenderingState = true;

	//SceneCaptureSensor_local_ns::SetCameraDefaultOverrides(*CaptureComponent2D);

}

void ASceneCaptureSensor::EnqueueRenderSceneImmediate() {
	TRACE_CPUPROFILER_EVENT_SCOPE(ASceneCaptureSensor::EnqueueRenderSceneImmediate);
	// Equivalent to "CaptureComponent2D->CaptureScene" + (optional) GBuffer extraction.
	CaptureSceneExtended();
}


void ASceneCaptureSensor::CaptureSceneExtended()
{

	// Creates an snapshot of the scene, requieres bCaptureEveryFrame = false.

	UE_LOG(LogTemp, Warning, TEXT("CAPTURING SCENE NORMALLY...."));

	CaptureComponent2D->CaptureScene();

	//////////////////////////////// Added by Navid
	FRenderTarget* RenderTargetResource = CaptureRenderTarget->GameThread_GetRenderTargetResource();
	FReadSurfaceDataFlags ReadSurfaceDataFlags;
	ReadSurfaceDataFlags.SetLinearToGamma(true); // adjust for sRGB encoding, if necessary
	ReadSurfaceDataFlags.SetMip(0); // get the top mip level

	FIntRect Rect(0, 0, CaptureRenderTarget->SizeX, CaptureRenderTarget->SizeY); // read the entire texture
	TArray<FColor> Pixels;

	RenderTargetResource->ReadPixels(Pixels, ReadSurfaceDataFlags, Rect);

	UE_LOG(LogTemp, Warning, TEXT("DATA HAS BEEN CAPTURED: %d"), Pixels.Num());

	////////////////////////////////////////////////

	/// <summary> Added by Navid: Lets save some stuff to the disk
	/// 
	/// </summary>

}

// Called when the game starts or when spawned
void ASceneCaptureSensor::BeginPlay()
{
	// Determine the gamma of the player.
	const bool bInForceLinearGamma = !bEnablePostProcessingEffects;

	CaptureRenderTarget->InitCustomFormat(ImageWidth, ImageHeight, bEnable16BitFormat ? PF_FloatRGBA : PF_B8G8R8A8,
		bInForceLinearGamma);

	if (bEnablePostProcessingEffects)
	{
		CaptureRenderTarget->TargetGamma = TargetGamma;
	}

	check(IsValid(CaptureComponent2D) && !CaptureComponent2D->IsPendingKill());

	CaptureComponent2D->Deactivate();
	CaptureComponent2D->TextureTarget = CaptureRenderTarget;

	// Call derived classes to set up their things.
	SetUpSceneCaptureComponent(*CaptureComponent2D);
	
	CaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	CaptureComponent2D->UpdateContent();
	CaptureComponent2D->Activate();

	// Make sure that there is enough time in the render queue.
	UKismetSystemLibrary::ExecuteConsoleCommand(
		GetWorld(),
		FString("g.TimeoutForBlockOnRenderFence 300000"));

	Super::BeginPlay();
}

void ASceneCaptureSensor::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaTime)
{
	Super::PostPhysTick(World, TickType, DeltaTime);
}
