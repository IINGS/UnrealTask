// Fill out your copyright notice in the Description page of Project Settings.

#include <iostream>
#include "MyActor.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	totaldistance = 0.f;
	totalevents = 0;
	path.Add(FVector2D(0, 0));
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	move();

	UE_LOG(LogTemp, Warning, TEXT("Total distance: %f, Total events: %d"), totaldistance, totalevents);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int AMyActor::step()
{
	return FMath::RandRange(-1, 1);
}

void AMyActor::move()
{
	for (int i = 0; i < 10; i++)
	{
		int32 dx = step();
		int32 dy = step();
        FVector2D newlocation;

		newlocation.X = path[i].X + dx;
		newlocation.Y = path[i].Y + dy;

        path.Add(newlocation);

		UE_LOG(LogTemp, Warning, TEXT("New location: X = %f, Y = %f"), newlocation.X, newlocation.Y);
		UE_LOG(LogTemp, Warning, TEXT("Current Step Distance: %f"), difference(path[i], path[i + 1]));

		totaldistance += difference(path[i], path[i + 1]);

		TriggerEventWithProbability(50);
	}
}

void AMyActor::TriggerEventWithProbability(float Probability)
{
    int32 RandomValue = FMath::RandRange(1, 100); // 1 ~ 100의 난수 생성
    if (RandomValue <= Probability)
    {
        UE_LOG(LogTemp, Log, TEXT("Event Triggered!"));
        totalevents++;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Event Not Triggered"));
    }
}

float AMyActor::difference(const FVector2D& a, const FVector2D& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return FMath::Sqrt(dx * dx + dy * dy);
}