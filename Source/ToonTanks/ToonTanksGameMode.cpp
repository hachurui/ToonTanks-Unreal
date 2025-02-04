// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if(DeadActor == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("DeadActor nullptr ToonTanksGameMode.cpp"));
        return;
    }
    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if(ToonTanksPlayerController)
        {    
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDesctruction();
        TargetTowersCount--;
        if(TargetTowersCount <= 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowersCount = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();
    
    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, InputDelegate, StartDelay, false);
    }
}

int AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> TowerList;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerList);
    return TowerList.Num();
}

/*void AToonTanksGameMode::GameOver(bool bGameWon)
{
    if(bGameWon)
    {
        UE_LOG(LogTemp, Warning, TEXT("You won!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("You false!"));
    }
}*/