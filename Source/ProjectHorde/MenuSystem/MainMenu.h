// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "ProjectHorde/MenuSystem/MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:

	UMainMenu(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void SetServerList(const TArray<FServerData> ServerDataArray);

	void SelectIndex(uint32 Index);

private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* MainMenu_PlayButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* MainMenu_HostButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* MainMenu_JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* MainMenu_QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* JoinMenu_BackButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* JoinMenu_JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* HostMenu_BackButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* HostMenu_HostButton;

	TOptional<uint32> SelectedIndex;

	UFUNCTION()
	void Play();

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void QuitGame();

	void UpdateChildren();
};
