#include "DIWorldSettingTab.h"

#include "DIManagedContainer.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "FDIWorldSettingTab"

FDIWorldSettingTab::FDIWorldSettingTab()
{
	FEditorDelegates::MapChange.AddRaw(this, &FDIWorldSettingTab::OnMapChanged);
}

TSharedRef<SDockTab> FDIWorldSettingTab::SpawnTab(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& PropPlugin = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.NotifyHook = GUnrealEd;
	DetailsViewArgs.ColumnWidth = 0.5f;

	DetailsView = PropPlugin.CreateDetailView(DetailsViewArgs);

	OnMapChanged(0);

	return SNew(SDockTab)
		.Label(LOCTEXT("DIWorldSettingsTabTitle", "DI World Settings"))
		[
			DetailsView.ToSharedRef()
		];
}

void FDIWorldSettingTab::OnMapChanged(uint32 MapChangeFlags) const
{
	if (GEditor != nullptr)
	{
		const UWorld* World = GEditor->GetEditorWorldContext(true).World();

		AWorldSettings* WorldSettings = World->GetWorldSettings();
		if (!WorldSettings)
		{
			return;
		}

		UClass* UserDataClass = UDIWorldSettingUserData::StaticClass();
		if (UDIWorldSettingUserData* UserData = Cast<UDIWorldSettingUserData>(WorldSettings->GetAssetUserDataOfClass(UserDataClass)))
		{
			DetailsView->SetObject(UserData);
		} else
		{
			UDIWorldSettingUserData* NewUserData = NewObject<UDIWorldSettingUserData>(WorldSettings, UserDataClass);
			WorldSettings->AddAssetUserData(NewUserData);
			DetailsView->SetObject(NewUserData);
		}
	}
}

#undef LOCTEXT_NAMESPACE
