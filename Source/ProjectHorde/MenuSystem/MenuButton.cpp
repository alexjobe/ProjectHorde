// Made by Alex Jobe


#include "MenuButton.h"
#include "Components/Button.h"

bool UMenuButton::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(Button != nullptr)) return false;
	Button->OnClicked.AddDynamic(this, &UMenuButton::ButtonPressed);

	return true;
}

void UMenuButton::ButtonPressed()
{
	OnClicked.Broadcast();
}
