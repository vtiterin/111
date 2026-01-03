#include "TileCoverageShader.h"

static void ForceLinkTileCoverageCSShader()
{
}

IMPLEMENT_GLOBAL_SHADER(
	FTileCoverageCS,
	"/Plugin/TileCoverageCS/TileCoverageCS.usf",
	"MainCS",
	SF_Compute
);

// гарантований виклик при завантаженні TU
struct FTileCoverageCSShaderLinker
{
	FTileCoverageCSShaderLinker()
	{
		ForceLinkTileCoverageCSShader();
	}
};

// глобальний обʼєкт → static init
static FTileCoverageCSShaderLinker GTileCoverageCSShaderLinker;