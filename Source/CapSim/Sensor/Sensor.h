// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CapSim/Utils/RandomEngine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sensor.generated.h"

UCLASS()
class CAPSIM_API ASensor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PrePhysTick(float DeltaSeconds) {}
	virtual void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds) {}

	void PostPhysTickInternal(UWorld* World, ELevelTick TickType, float DeltaSeconds);
	
	// TODO Fix this
	bool HasActorBegunPlay() { return true; };

	/// Random Engine used to provide noise for sensor output.
	UPROPERTY()
	URandomEngine* RandomEngine = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetSeed(int32 InSeed);

protected:
	/// Seed of the pseudo-random engine.
	UPROPERTY(Category = "Random Engine", EditAnywhere)
	int32 Seed = 123456789;

private:
	
	bool ReadyToTick = false;
	bool bClientsAreListening = false;

	bool AreClientsListening();

};
