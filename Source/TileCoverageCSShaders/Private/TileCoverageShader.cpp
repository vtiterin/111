#include "TileCoverageShader.h"

IMPLEMENT_GLOBAL_SHADER(
	FTileCoverageCS,
	"/Plugin/TileCoverageCS/TileCoverageCS.usf",
	"MainCS",
	SF_Compute
);
