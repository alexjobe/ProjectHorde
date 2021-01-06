// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonClickedEvent);

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMenuButtonClickedEvent OnClicked;

private:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	class UButton* Button;

	UFUNCTION()
	void ButtonPressed();
};
