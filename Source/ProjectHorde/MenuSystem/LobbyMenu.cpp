// Made by Alex Jobe


#include "LobbyMenu.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"

#include "LobbyPlayerState.h"
#include "LobbyRow.h"
#include "MenuButton.h"

ULobbyMenu::ULobbyMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> LobbyRowBPClass(TEXT("/Game/MenuSystem/WBP_LobbyRow"));
	if (!ensure(LobbyRowBPClass.Class != nullptr)) return;

	LobbyRowClass = LobbyRowBPClass.Class;
}

bool ULobbyMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(LobbyMenu_LeaveButton != nullptr)) return false;
	LobbyMenu_LeaveButton->OnClicked.AddDynamic(this, &ULobbyMenu::LeaveLobby);

	if (!ensure(LobbyMenu_ReadyButton != nullptr)) return false;
	LobbyMenu_ReadyButton->OnClicked.AddDynamic(this, &ULobbyMenu::ToggleReady);

	return true;
}

void ULobbyMenu::SetLobbyList(const TArray<ALobbyPlayerState*> PlayerArray)
{
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;
	if (!ensure(LobbyRowClass != nullptr)) return;

	LobbyList->ClearChildren();

	for (const ALobbyPlayerState* Player : PlayerArray)
	{
		ULobbyRow* Row = CreateWidget<ULobbyRow>(World, LobbyRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->PlayerName->SetText(FText::FromString(Player->GetPlayerName()));
		
		if (Player->GetReadyState())
		{
			Row->ReadyStatus->SetText(FText::FromString("Ready"));
		}
		else
		{
			Row->ReadyStatus->SetText(FText::FromString("Not Ready"));
		}

		LobbyList->AddChild(Row);
	}
}

void ULobbyMenu::LeaveLobby()
{

}

void ULobbyMenu::ToggleReady()
{
	ALobbyPlayerState* PlayerState = PlayerController->GetPlayerState<ALobbyPlayerState>();
	if (!ensure(PlayerState != nullptr)) return;

	PlayerState->ToggleReady();
}
