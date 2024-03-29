#pragma once

#include <functional>
#include "CapSim/Game/TaggedComponent.h"

#include "CoreGlobals.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Runtime/ImageWriteQueue/Public/ImagePixelData.h"



class FPixelReader
{
public:

    /// Copy the pixels in @a RenderTarget into @a BitMap.
    ///
    /// @pre To be called from game-thread.
    static bool WritePixelsToArray(
        UTextureRenderTarget2D& RenderTarget,
        TArray<FColor>& BitMap);

    /// Dump the pixels in @a RenderTarget.
    ///
    /// @pre To be called from game-thread.
    static TUniquePtr<TImagePixelData<FColor>> DumpPixels(
        UTextureRenderTarget2D& RenderTarget);

    /// Asynchronously save the pixels in @a RenderTarget to disk.
    ///
    /// @pre To be called from game-thread.
    static TFuture<bool> SavePixelsToDisk(
        UTextureRenderTarget2D& RenderTarget,
        const FString& FilePath);

    /// Asynchronously save the pixels in @a RenderTarget to disk.
    ///
    /// @pre To be called from game-thread.
    static TFuture<bool> SavePixelsToDisk2(
        UTextureRenderTarget2D& RenderTarget,
        const FString& FilePath);


    /// Asynchronously save the pixels in @a PixelData to disk.
    ///
    /// @pre To be called from game-thread.
    static TFuture<bool> SavePixelsToDisk(
        TUniquePtr<TImagePixelData<FColor>> PixelData,
        const FString& FilePath);

    /// Asynchronously save the pixels in @a PixelData to disk.
    ///
    /// @pre To be called from game-thread.
    static TFuture<bool> SavePixelsToDisk2(
        TUniquePtr<TImagePixelData<FColor>> PixelData,
        const FString& FilePath);

    /// Convenience function to enqueue a render command that sends the pixels
    /// down the @a Sensor's data stream. It expects a sensor derived from
    /// ASceneCaptureSensor or compatible.
    ///
    /// Note that the serializer needs to define a "header_offset" that it's
    /// allocated in front of the buffer.
    ///
    /// @pre To be called from game-thread.
    template <typename TSensor, typename TPixel>
    static void SendPixelsInRenderThread(TSensor& Sensor, FString filePath, bool use16BitFormat = false, std::function<TArray<TPixel>(void*, uint32)> Conversor = {});

    template <typename TSensor, typename TPixel>
    static void SendPixelsInRenderThread2(TSensor& Sensor, FString filePath, bool use16BitFormat = false, std::function<TArray<TPixel>(void*, uint32)> Conversor = {});

};

template<typename TSensor, typename TPixel>
void FPixelReader::SendPixelsInRenderThread(TSensor& Sensor, FString filePath, bool use16BitFormat, std::function<TArray<TPixel>(void*, uint32)> Conversor)
{
    check(Sensor.CaptureRenderTarget != nullptr);

    if (!Sensor.HasActorBegunPlay())
    {
        return;
    }

    /// Blocks until the render thread has finished all it's tasks.
    // TODO: Sensor.enqueue...

    Sensor.EnqueueRenderSceneImmediate();

    // TODO: SavePixelsToDisk, GetCaptureRenderTarget
    SavePixelsToDisk(*Sensor.GetCaptureRenderTarget(), filePath);
    UE_LOG(LogTemp, Warning, TEXT("saving to disk: %s"), *filePath);

    // TODO: Sensor.WaitForRenderThreadToFinish
    // Blocks until the render thread has finished all it's tasks
    Sensor.WaitForRenderThreadToFinish();
}

template<typename TSensor, typename TPixel>
void FPixelReader::SendPixelsInRenderThread2(TSensor& Sensor, FString filePath, bool use16BitFormat, std::function<TArray<TPixel>(void*, uint32)> Conversor)
{
    check(Sensor.CaptureRenderTarget != nullptr);

    if (!Sensor.HasActorBegunPlay())
    {
        return;
    }

    /// Blocks until the render thread has finished all it's tasks.
    // TODO: Sensor.enqueue...

    Sensor.EnqueueRenderSceneImmediate();

    UE_LOG(LogTemp, Warning, TEXT("something1234"));

    // TODO: SavePixelsToDisk, GetCaptureRenderTarget
    SavePixelsToDisk2(*Sensor.GetCaptureRenderTarget(), filePath);
    UE_LOG(LogTemp, Warning, TEXT("saving to disk: %s"), *filePath);

    // TODO: Sensor.WaitForRenderThreadToFinish
    // Blocks until the render thread has finished all it's tasks
    Sensor.WaitForRenderThreadToFinish();
}
