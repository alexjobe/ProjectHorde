// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyRow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API ULobbyRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyStatus;
};
