// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetReadyState() const;

	UFUNCTION(Server, Unreliable)
	void ToggleReady();

	UFUNCTION(BlueprintImplementableEvent)
	void ReadyStateChanged(bool NewReadyState);

protected:

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ReadyState)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_ReadyState();
};
