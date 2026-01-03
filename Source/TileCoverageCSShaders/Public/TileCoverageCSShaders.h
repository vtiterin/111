#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTileCoverageCSShadersModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
