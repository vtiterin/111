#pragma once
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

class TILECOVERAGECSSHADERS_API FTileCoverageCS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FTileCoverageCS);
	SHADER_USE_PARAMETER_STRUCT(FTileCoverageCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(int32, GridSize)
		SHADER_PARAMETER(int32, SampleDensity)
		SHADER_PARAMETER(float, CoveringThreshold)
		SHADER_PARAMETER(FVector2f, SizeUV)
		SHADER_PARAMETER(FVector2f, AnchorUV)
		SHADER_PARAMETER(float, RotationDeg)
		SHADER_PARAMETER_TEXTURE(Texture2D, SourceTexture)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)
		SHADER_PARAMETER_UAV(RWStructuredBuffer<uint>, OutCoveredTiles)
		SHADER_PARAMETER(FVector2f, UVMin)
		SHADER_PARAMETER(FVector2f, UVMax)
	END_SHADER_PARAMETER_STRUCT()
	
	/*static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters&)
	{
		return true;
	}*/
};
