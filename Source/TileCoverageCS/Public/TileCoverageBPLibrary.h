// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TileCoverageBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TILECOVERAGECS_API UTileCoverageBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** 
	* Викликає Compute Shader, обчислює покриття тайлів
	* @param Texture - текстура для перевірки
	* @param GridSize - кількість тайлів по одній осі
	* @param SampleDensity - кількість семплів в тайлі
	* @param Threshold - мінімальна кількість покритих семплів
	* @param RotationDeg - обертання текстури навколо AnchorUV
	* @param AnchorUV - точка обертання у UV-просторі
	*/
	UFUNCTION(BlueprintCallable, Category="Tile Coverage")
	static TArray<int32> ComputeTileCoverage(
		UTexture2D* Texture,
		int32 GridSize,
		int32 SampleDensity,
		float Threshold,
		float RotationDeg,
		FVector2f AnchorUV
	);
};
