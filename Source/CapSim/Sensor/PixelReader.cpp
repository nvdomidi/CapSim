#include "CapSim/Sensor/PixelReader.h"

#include "HighResScreenshot.h"
#include "Runtime/ImageWriteQueue/Public/ImageWriteQueue.h"

bool FPixelReader::WritePixelsToArray(UTextureRenderTarget2D& RenderTarget, TArray<FColor>& BitMap)
{
	// Added by Navid
	UE_LOG(LogTemp, Warning, TEXT("Writing Pixels to Array, IsInGameThread?: %d"), IsInGameThread());

	check(IsInGameThread());
	FTextureRenderTargetResource* RTResource =
		RenderTarget.GameThread_GetRenderTargetResource();
	if (RTResource == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FPixelReader: UTextureRenderTarget2D missing render target"));
		return false;
	}
	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(true);

	// Added by Navid
	//auto smth = RTResource->ReadPixels(BitMap, ReadPixelFlags);
	//int a = BitMap.Num();

	//UE_LOG(LogTemp, Warning, TEXT("RTResource Pixels number: %d"), a);

	//return smth;

	return RTResource->ReadPixels(BitMap, ReadPixelFlags);
}

TUniquePtr<TImagePixelData<FColor>> FPixelReader::DumpPixels(UTextureRenderTarget2D& RenderTarget)
{
	const FIntPoint DestSize(RenderTarget.GetSurfaceWidth(), RenderTarget.GetSurfaceHeight());
	TUniquePtr<TImagePixelData<FColor>> PixelData = MakeUnique<TImagePixelData<FColor>>(DestSize);
	TArray<FColor> Pixels(PixelData->Pixels.GetData(), PixelData->Pixels.Num());
	if (!WritePixelsToArray(RenderTarget, Pixels))
	{
		// Added by Navid
		UE_LOG(LogTemp, Warning, TEXT("Returning nullptr"));
		return nullptr;
	}


	UE_LOG(LogTemp, Warning, TEXT("In DumpPixels, the number of pixels in Pixels: %d"), Pixels.Num());

	// Added by Navid
	UE_LOG(LogTemp, Warning, TEXT("NOT NULL, Returning PixelData"));

	UE_LOG(LogTemp, Warning, TEXT("In DumpPixels, the number of pixels in pixeldata BEFORE COPY: %d"), PixelData->Pixels.Num());

	PixelData->Pixels.Empty();
	PixelData->Pixels.Append(Pixels);

	UE_LOG(LogTemp, Warning, TEXT("In DumpPixels, the number of pixels in pixeldata AFTER COPY: %d"), PixelData->Pixels.Num());
	return PixelData;
}

TFuture<bool> FPixelReader::SavePixelsToDisk(UTextureRenderTarget2D& RenderTarget, const FString& FilePath)
{
	return SavePixelsToDisk(DumpPixels(RenderTarget), FilePath);
}

TFuture<bool> FPixelReader::SavePixelsToDisk(TUniquePtr<TImagePixelData<FColor>> PixelData, const FString& FilePath)
{
	
	// Added by Navid
	UE_LOG(LogTemp, Warning, TEXT("In SavePixelsToDisk, the num pixels for pixelData: %d"), PixelData.Get()->Pixels.Num());

	TUniquePtr<FImageWriteTask> ImageTask = MakeUnique<FImageWriteTask>();
	ImageTask->PixelData = MoveTemp(PixelData);
	ImageTask->Filename = FilePath;
	ImageTask->Format = EImageFormat::PNG;
	ImageTask->CompressionQuality = (int32)EImageCompressionQuality::Default;
	ImageTask->bOverwriteFile = true;
	ImageTask->PixelPreProcessors.Add(TAsyncAlphaWrite<FColor>(255));

	FHighResScreenshotConfig& HighResScreenshotConfig = GetHighResScreenshotConfig();
	return HighResScreenshotConfig.ImageWriteQueue->Enqueue(MoveTemp(ImageTask));
	

	return TFuture<bool>();
}
