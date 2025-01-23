// Fill out your copyright notice in the Description page of Project Settings.


#include "bingleplatform.h"

// Sets default values
Abingleplatform::Abingleplatform()
{
	BingleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BingleRoot"));
	SetRootComponent(BingleRoot);

	BingleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BingleMesh"));
	BingleMesh->SetupAttachment(BingleRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BinglePf(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Props/SM_AssetPlatform.SM_AssetPlatform'"));
	if (BinglePf.Succeeded())
	{
		BingleMesh->SetStaticMesh(BinglePf.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> BingleMt0(TEXT("/Script/Engine.Material'/Game/Resources/Materials/M_CobbleStone_Rough.M_CobbleStone_Rough'"));
	if (BingleMt0.Succeeded())
	{
		BingleMesh->SetMaterial(0, BingleMt0.Object);
	}

	PrimaryActorTick.bCanEverTick = true;
	BingleSpeed = 90.f;
	
	Disappearable = false;
	Frequency = 5.f;
}

// Called when the game starts or when spawned
void Abingleplatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(GetActorLocation());

	if (Disappearable)
	{
		GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &Abingleplatform::Disappear, Frequency, false);
	}
}

// Called every frame
void Abingleplatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyZero(BingleSpeed))
	{
		AddActorLocalRotation(FRotator(0.f, BingleSpeed * DeltaTime, 0.f));
	}

}

void Abingleplatform::Disappear()
{
	BingleMesh->SetVisibility(false);
	BingleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(AppearTimerHandle, this, &Abingleplatform::Appear, 1.f, false);
}

void Abingleplatform::Appear()
{
	BingleMesh->SetVisibility(true);
	BingleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &Abingleplatform::Disappear, Frequency, false);
}

