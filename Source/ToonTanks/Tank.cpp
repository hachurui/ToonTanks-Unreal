// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 15.0, 8, FColor::Red, false, -1.0);
        RotateTurretMesh(HitResult.ImpactPoint);
    }
}

APlayerController* ATank::GetTankPlayerController() const {return TankPlayerController;}

void ATank::HandleDestruction()
{
    Super::HandleDesctruction();
        //Sets tank to be invisible
    SetActorHiddenInGame(true);
        //Turns off Tank's tick function for optimization
    SetActorTickEnabled(false);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
        //Sets up InputMapping for Tank
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
        //Binds InputActions selected in blueprint, a triggerevent, and relevent function to each other in PlayerInputComponent
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if(EnhancedInputComponent)
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
        EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
        EnhancedInputComponent->BindAction(RotateTurretAction, ETriggerEvent::Triggered, this, &ATank::RotateTurret);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
    }
}
void ATank::Move(const FInputActionValue& pValue)
{
        //Get -1, 0, or 1 from pValue depending on if a/what key is pressed
    float MoveDirection = pValue.Get<float>();
        //gets deltatime for movement calculation
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
        //Create empty vector and changes its X to how much change in location (What direction times speed times change in time between frames)
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = MoveDirection*Speed*DeltaTime;
        //AddActorLocalOffset changes transform based on relative positioning, so increasing/descreasing DeltaLocation.X with move the tank forward/backwards regardless of location
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(const FInputActionValue& pValue)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this); 
        //Like ATank::Move() but for rotating the tank
    DeltaRotation.Yaw = pValue.Get<float>()*DeltaTime*TurnRate;
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::RotateTurret(const FInputActionValue& pValue)
{
    
    /*float DeltaYaw = pValue.Get<float>();
    FRotator NewRotation = TurretMesh->GetComponentRotation();
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    NewRotation.Yaw += DeltaYaw*TurretTurnRate*DeltaTime;
    TurretMesh->SetWorldRotation(NewRotation);*/
}
