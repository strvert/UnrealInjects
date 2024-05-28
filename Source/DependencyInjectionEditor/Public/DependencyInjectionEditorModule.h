#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDependencyInjectionEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    inline static FName DIWorldSettingsTabName = "DIWorldSettings";

private:
    void RegisterMenus();
    void RegisterTabs(TSharedPtr<FTabManager> InTabManager);
    TSharedPtr<class FDIWorldSettingTab> DIWorldSettingTab;
};
