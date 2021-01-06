// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "ProjectHorde/MenuSystem/MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UMenuButton* QuitButton;

	UFUNCTION()
	void CancelPressed();

	UFUNCTION()
	void QuitPressed();
};
