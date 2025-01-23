// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "movingplatform.generated.h"

UCLASS()
class SIXTH_TASK_API Amovingplatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Amovingplatform();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MovingPlatform|Components")
	USceneComponent* MovingRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingPlatform|Components")
	UStaticMeshComponent* MovingMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovingPlatform|Properties")
	float MovingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingPlatform|Properties")
	float Amplitude;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FTimerHandle DisappearTimerHandle;
	FTimerHandle AppearTimerHandle;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "MovingPlatform|Properties")
	bool Disappearable;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "MovingPlatform|Properties")
	float Frequency;

	void Disappear();
	void Appear();

private:

	float AccumTime;
	FVector InitialLocation;

};
