// Made by Alex Jobe


#include "ServerRow.h"
#include "Components/Button.h"

#include "MainMenu.h"

void UServerRow::Setup(class UMainMenu* NewParent, uint32 NewIndex)
{
	Parent = NewParent;
	Index = NewIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	if (!ensure(Parent != nullptr)) return;
	Parent->SelectIndex(Index);
}