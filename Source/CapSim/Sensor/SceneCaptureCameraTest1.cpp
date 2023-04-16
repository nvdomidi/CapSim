// Fill out your copyright notice in the Description page of Project Settings.


#include "CapSim/Sensor/SceneCaptureCameraTest1.h"
#include "CapSim/Sensor/PixelReader.h"

#include "Runtime/RenderCore/Public/RenderingThread.h"



void ASceneCaptureCameraTest1::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    UE_LOG(LogTemp, Warning, TEXT("Ticking in SceneCaptureCameraTest1"));
    this->time += DeltaSeconds;

    auto* This = this;

    FPixelReader::SendPixelsInRenderThread<ASceneCaptureCameraTest1, FColor>(*this);

    //FString imgPath = FString::Printf(TEXT("%f.jpg"), this->time);
    //SaveCaptureToDisk(imgPath);
    //UE_LOG(LogTemp, Warning, TEXT("Saved %s to disk"), *imgPath);
}
