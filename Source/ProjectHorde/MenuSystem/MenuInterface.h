// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "LobbyPlayerState.h"

#include "MenuInterface.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString ServerName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTHORDE_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Play() = 0;
	virtual void Host(FString ServerName) = 0;
	virtual void Join(const uint32 Index) = 0;
	virtual void LoadMainMenu() = 0;
	virtual void RefreshServerList() = 0;
	virtual void UpdateLobbyList(const TArray<ALobbyPlayerState*> PlayerArray) = 0;
	virtual void TeardownLobbyMenu() = 0;
};
