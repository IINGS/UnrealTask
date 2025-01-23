// Fill out your copyright notice in the Description page of Project Settings.


#include "movingplatform.h"

// Sets default values
Amovingplatform::Amovingplatform()
{
	MovingRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MovingRoot"));
	SetRootComponent(MovingRoot);

	MovingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovingMesh"));
	MovingMesh->SetupAttachment(MovingRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MovingPf(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Props/SM_AssetPlatform.SM_AssetPlatform'"));
	if (MovingPf.Succeeded())
	{
		MovingMesh->SetStaticMesh(MovingPf.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MovingMt0(TEXT("/Script/Engine.Material'/Game/Resources/Materials/M_Ground_Grass.M_Ground_Grass'"));
	if (MovingMt0.Succeeded())
	{
		MovingMesh->SetMaterial(0, MovingMt0.Object);
	}

	PrimaryActorTick.bCanEverTick = true;

	MovingSpeed = 0.5f;
	Amplitude = 200.f;
	AccumTime = 0.f;

	Disappearable = false;
	Frequency = 5.f;
}

// Called when the game starts or when spawned
void Amovingplatform::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();

	if (Disappearable)
	{
		GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &Amovingplatform::Disappear, Frequency, false);
	}
}

// Called every frame
void Amovingplatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MovingSpeed))
	{
		AccumTime += DeltaTime * MovingSpeed;
		float XOffset = FMath::Sin( 2 * PI * AccumTime) * Amplitude;//200Sin(2¤Ðt)²Ã
		SetActorLocation(InitialLocation + FVector(XOffset, 0.f, 0.f));
	}
}

void Amovingplatform::Disappear()
{
	MovingMesh->SetVisibility(false);
	MovingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(AppearTimerHandle, this, &Amovingplatform::Appear, 1.0f, false);
}

void Amovingplatform::Appear()
{
	MovingMesh->SetVisibility(true);
	MovingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &Amovingplatform::Disappear, Frequency, false);
}