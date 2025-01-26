// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

    Super::SetupPlayerInputComponent(PlayerInputComponent);
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if(PlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if(EnhancedInputComponent)
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
    }
}
void ATank::Move(const FInputActionValue& pValue)
{
    if (pValue.Get<bool>())
    {
        UE_LOG(LogTemp, Display, TEXT("Tank Move Value: True"));
    }
}