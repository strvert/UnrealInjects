#pragma once

class FDIWorldSettingTab
{
public:
	FDIWorldSettingTab();
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args);

private:
	void OnMapChanged(uint32 MapChangeFlags) const;
	TSharedPtr<IDetailsView> DetailsView;
};
