// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "ProjectHorde/MenuSystem/MenuInterface.h"

#include "HordeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API UHordeGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UHordeGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenuWidget();

	void Play() override;

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(const uint32 Index) override;

	void LoadMainMenu() override;

	void RefreshServerList() override;

	void StartSession();

private:

	TSubclassOf<class UMenuWidget> MainMenuClass;
	TSubclassOf<class UMenuWidget> InGameMenuClass;

	class UMainMenu* MainMenu;
	class UMenuWidget* InGameMenu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;

	void CreateSession();
	
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
