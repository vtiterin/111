// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "TileCoverageSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TILECOVERAGECS_API UTileCoverageSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	void DispatchCompute(
		UTexture2D* Texture,
		int32 GridSize,
		int32 SampleDensity,
		float Threshold,
		float RotationDeg,
		FVector2f AnchorUV,
		TArray<int32>& OutResult
	);
};
