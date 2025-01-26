// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

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
        EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
        EnhancedInputComponent->BindAction(RotateTurretAction, ETriggerEvent::Triggered, this, &ATank::RotateTurret);
    }
}
void ATank::Move(const FInputActionValue& pValue)
{
    FVector DeltaLocation(0.f);
    float MoveDirection = pValue.Get<float>();
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaLocation.X = MoveDirection*Speed*DeltaTime;
    AddActorLocalOffset(DeltaLocation, true);

    /*FVector2D MovementVector = pValue.Get<FVector2D>();
    if(Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }*/
}

void ATank::Turn(const FInputActionValue& pValue)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaRotation.Yaw = pValue.Get<float>()*DeltaTime*TurnRate;
    AddActorLocalRotation(DeltaRotation, true);
    /*float DeltaYaw = pValue.Get<float>()*TurnRate;
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += DeltaYaw*UGameplayStatics::GetWorldDeltaSeconds(this);
    SetActorRotation(NewRotation);*/
}

void ATank::RotateTurret(const FInputActionValue& pValue)
{
    float DeltaYaw = pValue.Get<float>();
    FRotator NewRotation = TurretMesh->GetComponentRotation();
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    NewRotation.Yaw += DeltaYaw*TurretTurnRate*DeltaTime;
    TurretMesh->SetWorldRotation(NewRotation);
}