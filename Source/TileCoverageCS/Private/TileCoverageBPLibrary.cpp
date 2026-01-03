#include "TileCoverageBPLibrary.h"
#include "TileCoverageSubsystem.h"
#include "Engine/Engine.h"

TArray<int32> UTileCoverageBPLibrary::ComputeTileCoverage(
	UTexture2D* Texture,
	int32 GridSize,
	int32 SampleDensity,
	float Threshold,
	float RotationDeg,
	FVector2f AnchorUV)
{
	TArray<int32> Result;

	if (!GEngine)
		return Result;

	auto* Subsystem =
		GEngine->GetEngineSubsystem<UTileCoverageSubsystem>();

	if (!Subsystem)
		return Result;

	Subsystem->DispatchCompute(
		Texture,
		GridSize,
		SampleDensity,
		Threshold,
		RotationDeg,
		AnchorUV,
		Result
	);

	return Result;
}
