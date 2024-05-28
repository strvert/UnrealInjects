#include "DependencyInjectionEditorModule.h"

#include "FDIWorldSettingTab.h"
#include "LevelEditor.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "WorkspaceMenuStructureModule.h"

#define LOCTEXT_NAMESPACE "FDependencyInjectionEditorModule"

void FDependencyInjectionEditorModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDependencyInjectionEditorModule::RegisterMenus));
}

void FDependencyInjectionEditorModule::ShutdownModule()
{
}

void FDependencyInjectionEditorModule::RegisterMenus()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	LevelEditorModule.OnRegisterTabs().AddRaw(this, &FDependencyInjectionEditorModule::RegisterTabs);

	FToolMenuOwnerScoped OwnerScoped(this);
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	const FText SectionName = LOCTEXT("DependencyInjectionMenuSection", "Dependency Injection");
	FToolMenuSection& Section = Menu->FindOrAddSection("DependencyInjection", SectionName);
	Section.AddEntry(FToolMenuEntry::InitMenuEntry(
		"DependencyInjectionWorldSettings",
		LOCTEXT("LogDependencyInjectionWorldSettingsTitle", "DI World Settings"),
		LOCTEXT("LogDependencyInjectionWorldSettingsToolTip", "Open the Dependency Injection World Settings"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			FLevelEditorModule& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>(
				"LevelEditor");
			LevelEditorModule.GetLevelEditorTabManager()->TryInvokeTab(DIWorldSettingsTabName);
		}))));
}

void FDependencyInjectionEditorModule::RegisterTabs(TSharedPtr<FTabManager> InTabManager)
{
	DIWorldSettingTab = MakeShared<FDIWorldSettingTab>();

	InTabManager->RegisterTabSpawner(DIWorldSettingsTabName,
	                                 FOnSpawnTab::CreateSP(DIWorldSettingTab.ToSharedRef(), &FDIWorldSettingTab::SpawnTab))
	            .SetDisplayName(LOCTEXT("DIWorldSettingsTabTitle", "DI World Settings"))
	            .SetGroup(WorkspaceMenu::GetMenuStructure().GetLevelEditorCategory())
	            .SetIcon(FSlateIcon());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDependencyInjectionEditorModule, DependencyInjectionEditor)
