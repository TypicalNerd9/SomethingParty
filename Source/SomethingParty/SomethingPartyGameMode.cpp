// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomethingPartyGameMode.h"
#include "SomethingPartyPlayerController.h"
#include "SomethingPartyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include <SomethingPartyGameState.h>
#include "GameFramework/PlayerState.h"

ASomethingPartyGameMode::ASomethingPartyGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASomethingPartyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default state to our Blueprinted state
	static ConstructorHelpers::FClassFinder<AGameStateBase> SomethingPartyGameState(TEXT("/Game/TopDown/Blueprints/SomethingPartyGameStateBP"));
	if (SomethingPartyGameState.Class != NULL)
	{
		GameStateClass = SomethingPartyGameState.Class;
	}

	// set default state to our Blueprinted player state
	static ConstructorHelpers::FClassFinder<APlayerState> SomethingPartyPlayerState(TEXT("/Game/TopDown/Blueprints/SomethingPartyPlayerStateBP"));
	if (SomethingPartyPlayerState.Class != NULL)
	{
		PlayerStateClass = SomethingPartyPlayerState.Class;
	}

	static ConstructorHelpers::FClassFinder<ADice> Dice(TEXT("/Game/TopDown/Blueprints/DiceBP"));
	if (Dice.Class != NULL)
	{
		DiceActor = Dice.Class;
	}
}

void ASomethingPartyGameMode::NextTurn()
{
	ASomethingPartyGameState* MainGameState = Cast<ASomethingPartyGameState>(GameState);
	if (MainGameState) {
		MainGameState->NextTurn();
	}
}

void ASomethingPartyGameMode::SetTurnOrder(TArray<FUniqueNetIdRepl> IDOrder)
{
	ASomethingPartyGameState* MainGameState = Cast<ASomethingPartyGameState>(GameState);
	if (MainGameState) {
		MainGameState->SetTurnOrder(IDOrder);
	}
}

void ASomethingPartyGameMode::RollDice(ASomethingPartyCharacter* Character, ADice* Dice)
{
	GetGameState<ASomethingPartyGameState>()->RollDice(Character, Dice);
}





void ASomethingPartyGameMode::StartPlay()
{

	

	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATileActor::StaticClass(), FName("StartingTile"), ActorsToFind);

	if (ActorsToFind.IsEmpty()) {
		UE_LOG(LogTemp, Fatal, TEXT("Missing start tile..."));
	}
	for (AActor* TileActor : ActorsToFind) {
		ATileActor* Tile = Cast<ATileActor>(TileActor);
		if (Tile) {
			GetGameState<ASomethingPartyGameState>()->StartTile = Tile;
		}
	}
	GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer = GetGameState<ASomethingPartyGameState>()->PlayerArray[0];
	if (DiceActor) {
		UE_LOG(LogTemp, Warning, TEXT("CURRENT PLAYER: %s"), *GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPlayerName());
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPawn();
		GetWorld()->SpawnActor<ADice>(DiceActor, GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPawn()->GetActorLocation() + FVector(0, 0, 150), GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPawn()->GetActorRotation(), spawnParams);
	}
	AGameModeBase::StartPlay();
}
