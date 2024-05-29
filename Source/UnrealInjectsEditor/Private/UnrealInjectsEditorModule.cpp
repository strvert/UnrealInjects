#include "UnrealInjectsEditorModule.h"

#include "DIWorldSettingTab.h"
#include "LevelEditor.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "WorkspaceMenuStructureModule.h"

#define LOCTEXT_NAMESPACE "FUnrealInjectsEditorModule"

void FUnrealInjectsEditorModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUnrealInjectsEditorModule::RegisterMenus));
}

void FUnrealInjectsEditorModule::ShutdownModule()
{
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
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			FLevelEditorModule& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>(
				"LevelEditor");
			LevelEditorModule.GetLevelEditorTabManager()->TryInvokeTab(DIWorldSettingsTabName);
		}))));
}

void FUnrealInjectsEditorModule::RegisterTabs(TSharedPtr<FTabManager> InTabManager)
{
	DIWorldSettingTab = MakeShared<FDIWorldSettingTab>();

	InTabManager->RegisterTabSpawner(DIWorldSettingsTabName,
	                                 FOnSpawnTab::CreateSP(DIWorldSettingTab.ToSharedRef(), &FDIWorldSettingTab::SpawnTab))
	            .SetDisplayName(LOCTEXT("DIWorldSettingsTabTitle", "DI World Settings"))
	            .SetGroup(WorkspaceMenu::GetMenuStructure().GetLevelEditorCategory())
	            .SetIcon(FSlateIcon());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealInjectsEditorModule, UnrealInjectsEditor)
