#include "UnrealInjectsEditorModule.h"

#include "DITypeRegistrationAsset.h"
#include "DIWorldSettingTab.h"
#include "LevelEditor.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "AssetTypeActions/AssetTypeActions_DataAsset.h"

#define LOCTEXT_NAMESPACE "FUnrealInjectsEditorModule"

class FAssetTypeActions_DITypeRegistrationAsset : public FAssetTypeActions_DataAsset
{
public:
	virtual FText GetName() const override
	{
		return NSLOCTEXT("AssetTypeActions",
		                 "AssetTypeActions_DITypeRegistrationAsset",
		                 "DI Type Registration");
	}

	virtual uint32 GetCategories() override { return FUnrealInjectsEditorModule::GetInputAssetsCategory(); }
	virtual FColor GetTypeColor() const override { return FColor(82, 47, 237); }

	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		return NSLOCTEXT("AssetTypeActions",
		                 "AssetTypeActions_DITypeRegistrationAssetDescription",
		                 "An asset that contains type information to be registered in a DI container. Provides information needed when building a DI container.");
	}

	virtual UClass* GetSupportedClass() const override { return UDITypeRegistrationAsset::StaticClass(); }
};


void FUnrealInjectsEditorModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUnrealInjectsEditorModule::RegisterMenus));

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	InputAssetsCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Dependency Injection")), LOCTEXT("DependencyInjectionAssetCategory", "Dependency Injection"));
	{
		auto RegisterAssetTypeAction = [this](IAssetTools& AssetTools, const TSharedRef<IAssetTypeActions>& Action)
		{
			AssetTools.RegisterAssetTypeActions(Action);
			CreatedAssetTypeActions.Add(Action);
		};
		RegisterAssetTypeAction(AssetTools, MakeShared<FAssetTypeActions_DITypeRegistrationAsset>());
	}
}

void FUnrealInjectsEditorModule::ShutdownModule()
{
}

EAssetTypeCategories::Type FUnrealInjectsEditorModule::GetInputAssetsCategory()
{
	return InputAssetsCategory;
}

void FUnrealInjectsEditorModule::RegisterMenus()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	LevelEditorModule.OnRegisterTabs().AddRaw(this, &FUnrealInjectsEditorModule::RegisterTabs);

	FToolMenuOwnerScoped OwnerScoped(this);
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	const FText SectionName = LOCTEXT("UnrealInjectsMenuSection", "Dependency Injection");
	FToolMenuSection& Section = Menu->FindOrAddSection("UnrealInjects", SectionName);
	Section.AddEntry(FToolMenuEntry::InitMenuEntry(
		"UnrealInjectsWorldSettings",
		LOCTEXT("LogUnrealInjectsWorldSettingsTitle", "DI World Settings"),
		LOCTEXT("LogUnrealInjectsWorldSettingsToolTip", "Open the Dependency Injection World Settings"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]
		{
			const FLevelEditorModule& LevelEditorModule
				= FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");
			LevelEditorModule.GetLevelEditorTabManager()->TryInvokeTab(DIWorldSettingsTabName);
		}))));
}

void FUnrealInjectsEditorModule::RegisterTabs(TSharedPtr<FTabManager> InTabManager)
{
	DIWorldSettingTab = MakeShared<FDIWorldSettingTab>();

	InTabManager->RegisterTabSpawner(DIWorldSettingsTabName,
	                                 FOnSpawnTab::CreateSP(DIWorldSettingTab.ToSharedRef(),
	                                                       &FDIWorldSettingTab::SpawnTab))
	            .SetDisplayName(LOCTEXT("DIWorldSettingsTabTitle", "DI World Settings"))
	            .SetGroup(WorkspaceMenu::GetMenuStructure().GetLevelEditorCategory())
	            .SetIcon(FSlateIcon());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealInjectsEditorModule, UnrealInjectsEditor)
