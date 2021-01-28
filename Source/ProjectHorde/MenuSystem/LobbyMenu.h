// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "ProjectHorde/MenuSystem/MenuWidget.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API ULobbyMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	ULobbyMenu(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetLobbyList(const TArray<class ALobbyPlayerState*> PlayerArray);

	UFUNCTION(BlueprintImplementableEvent)
	void BeginCountdown();

private:

	TSubclassOf<class UUserWidget> LobbyRowClass;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* LobbyMenu_LeaveButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* LobbyMenu_ReadyButton;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* LobbyList;

	UFUNCTION()
	void LeaveLobby();

	UFUNCTION()
	void ToggleReady();
};
