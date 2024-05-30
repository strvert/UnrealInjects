#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class IAssetTypeActions;

class FUnrealInjectsEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    inline static FName DIWorldSettingsTabName = "DIWorldSettings";

	static EAssetTypeCategories::Type GetInputAssetsCategory();

private:
    void RegisterMenus();
    void RegisterTabs(TSharedPtr<FTabManager> InTabManager);
    TSharedPtr<class FDIWorldSettingTab> DIWorldSettingTab;
	
	inline static EAssetTypeCategories::Type InputAssetsCategory;
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
