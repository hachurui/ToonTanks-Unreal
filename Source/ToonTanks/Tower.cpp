// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
    Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(Tank == nullptr)
    {
        return;
    }
    if(InFireRange() && Tank->bAlive)
    {
        RotateTurretMesh(Tank->GetActorLocation());
    }
}

void ATower::HandleDesctruction()
{
    Super::HandleDesctruction();
    Destroy();
}

void ATower::CheckFireCondition()
{
    if(InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::InFireRange()
{
    if(Tank && Range >= FVector::Dist(GetActorLocation(), Tank->GetActorLocation()))
    {
        return true;
    }
    return false;
}

