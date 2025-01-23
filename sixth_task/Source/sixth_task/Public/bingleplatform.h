// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "bingleplatform.generated.h"

UCLASS()
class SIXTH_TASK_API Abingleplatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Abingleplatform();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BinglePlatform|Components")
	USceneComponent* BingleRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BinglePlatform|Components")
	UStaticMeshComponent* BingleMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BinglePlatform|Properties")
	float BingleSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FTimerHandle DisappearTimerHandle;
	FTimerHandle AppearTimerHandle;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "BinglePlatform|Properties")
	bool Disappearable;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "BinglePlatform|Properties")
	float Frequency;

	void Disappear();
	void Appear();
};
