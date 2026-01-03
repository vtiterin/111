#include "TileCoverageSubsystem.h"
#include "TileCoverageShader.h"

#include "Engine/Texture2D.h"
#include "RenderGraphUtils.h"
#include "RHICommandList.h"
#include "RHIGPUReadback.h"

// --- Rotate point around anchor ---
static FVector2f RotateUV(const FVector2f& Point, const FVector2f& Anchor, float RotationDeg)
{
    float Rad = FMath::DegreesToRadians(-RotationDeg);
    float CosA = FMath::Cos(Rad);
    float SinA = FMath::Sin(Rad);
    FVector2f Delta = Point - Anchor;
    return FVector2f(
        Delta.X * CosA - Delta.Y * SinA,
        Delta.X * SinA + Delta.Y * CosA
    ) + Anchor;
}

void UTileCoverageSubsystem::DispatchCompute(
    UTexture2D* Texture,
    int32 GridSize,
    int32 SampleDensity,
    float Threshold,
    float RotationDeg,
    FVector2f AnchorUV,
    TArray<int32>& OutResult)
{
    OutResult.Reset();

    if (!Texture || !Texture->GetResource())
        return;
    
    // --- CPU: compute rotated bounding box with dynamic AnchorUV ---
    FVector2f Corners[4] =
    {
        RotateUV(FVector2f(0,0), AnchorUV, RotationDeg),
        RotateUV(FVector2f(1,0), AnchorUV, RotationDeg),
        RotateUV(FVector2f(0,1), AnchorUV, RotationDeg),
        RotateUV(FVector2f(1,1), AnchorUV, RotationDeg)
    };

    FVector2f UVMin = Corners[0];
    FVector2f UVMax = Corners[0];
    
    for(int i=1; i<4; i++)
    {
        UVMin.X = FMath::Min(UVMin.X, Corners[i].X);
        UVMin.Y = FMath::Min(UVMin.Y, Corners[i].Y);
        UVMax.X = FMath::Max(UVMax.X, Corners[i].X);
        UVMax.Y = FMath::Max(UVMax.Y, Corners[i].Y);
    }

    // Clamp to [0,1] для безпеки
    UVMin.X = FMath::Clamp(UVMin.X, 0.f, 1.f);
    UVMin.Y = FMath::Clamp(UVMin.Y, 0.f, 1.f);
    UVMax.X = FMath::Clamp(UVMax.X, 0.f, 1.f);
    UVMax.Y = FMath::Clamp(UVMax.Y, 0.f, 1.f);

    // --- Compute SizeUV ---
    FVector2D TextureSize(Texture->GetSizeX(), Texture->GetSizeY());
    FVector2f SizeUV(
        TextureSize.X / (GridSize * 200.f),
        TextureSize.Y / (GridSize * 200.f)
    );
    
    ENQUEUE_RENDER_COMMAND(TileCoverageCS_Dispatch)(
        [=, &OutResult](FRHICommandListImmediate& RHICmdList)
        {
            const uint32 MaxTiles = GridSize * GridSize;
            const uint32 BufferSize = MaxTiles * sizeof(uint32);
            const uint32 ThreadsPerGroup = 64;
            const uint32 Groups =
                (MaxTiles + ThreadsPerGroup - 1) / ThreadsPerGroup;

            FRHIResourceCreateInfo Info(TEXT("TileCoverageBuffer"));
            FBufferRHIRef Buffer =
                RHICmdList.CreateStructuredBuffer(
                    sizeof(uint32),
                    BufferSize,
                    BUF_UnorderedAccess | BUF_ShaderResource,
                    Info
                );

            FUnorderedAccessViewRHIRef UAV =
                RHICmdList.CreateUnorderedAccessView(Buffer, true, true);

            TShaderMapRef<FTileCoverageCS>
                Shader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

            FTileCoverageCS::FParameters Params;
            Params.GridSize = GridSize;
            Params.SampleDensity = SampleDensity;
            Params.CoveringThreshold = Threshold;
            Params.SizeUV = SizeUV;
            Params.AnchorUV = AnchorUV;
            Params.RotationDeg = RotationDeg;
            Params.SourceTexture = Texture->GetResource()->TextureRHI;
            Params.SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
            Params.OutCoveredTiles = UAV;
            Params.UVMin = UVMin;
            Params.UVMax = UVMax;

            SetShaderParameters(
                RHICmdList,
                Shader,
                Shader.GetComputeShader(),
                Params
            );

            FComputeShaderUtils::Dispatch(
                RHICmdList,
                Shader,
                Params,
                FIntVector(Groups, 1, 1)
            );

            FRHIGPUBufferReadback Readback(TEXT("TileCoverageReadback"));
            Readback.EnqueueCopy(RHICmdList, Buffer);
            RHICmdList.SubmitCommandsAndFlushGPU();

            if (Readback.IsReady())
            {
                const uint32* Data =
                    static_cast<const uint32*>(
                        Readback.Lock(BufferSize));

                for (uint32 i = 0; i < MaxTiles; ++i)
                    OutResult.Add(Data[i]);

                Readback.Unlock();
            }
        }
    );

    FlushRenderingCommands();
}