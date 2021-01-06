// Made by Alex Jobe


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuButton.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(MainMenu_PlayButton != nullptr)) return false;
	MainMenu_PlayButton->OnClicked.AddDynamic(this, &UMainMenu::Play);

	if (!ensure(MainMenu_HostButton != nullptr)) return false;
	MainMenu_HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(MainMenu_JoinButton != nullptr)) return false;
	MainMenu_JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(JoinMenu_BackButton != nullptr)) return false;
	JoinMenu_BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinMenu_JoinButton != nullptr)) return false;
	JoinMenu_JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(MainMenu_QuitButton != nullptr)) return false;
	MainMenu_QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	if (!ensure(HostMenu_BackButton != nullptr)) return false;
	HostMenu_BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(HostMenu_HostButton != nullptr)) return false;
	HostMenu_HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	Teardown();
}

void UMainMenu::SetServerList(const TArray<FServerData> ServerDataArray)
{
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;
	if (!ensure(ServerRowClass != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerDataArray)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.ServerName));
		Row->HostName->SetText(FText::FromString(ServerData.HostUserName));

		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->PlayersFraction->SetText(FText::FromString(FractionText));

		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row)
		{
			Row->IsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UMainMenu::Play()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Play();
	}
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr && ServerHostName != nullptr)
	{
		FString ServerName = ServerHostName->GetText().ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index: %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set!"));
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::QuitGame()
{
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ConsoleCommand("quit");
}
