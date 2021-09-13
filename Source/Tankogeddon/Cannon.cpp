// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Engine/Engine.h>
#include <TimerManager.h>
#include <Engine/World.h>

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootComp;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
    Mesh->SetupAttachment(RootComponent);

    ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
    ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{ 
    if (!IsReadyToFire())
    {
        return;
    }
    
    bReadyToFire = false;

    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
    }
    Ammo--;
    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}

void ACannon::SpecialFire()
{
    if (!IsReadyToFire())
    {
        return;
    }
    bReadyToFire = false;

    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Special fire - projectile");
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Special fire - trace");
    }
    Ammo--;
    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
    if (Ammo <= 0)
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "No ammo");
        return false;
    }
    return bReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
    bReadyToFire = true;
}

void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void ACannon::Reload()
{
    bReadyToFire = true;
}
