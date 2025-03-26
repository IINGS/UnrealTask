// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyTestCPPLib.generated.h"


USTRUCT(BlueprintType)
struct FGameResult
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GameResult")
	bool bIsAnswer;
	UPROPERTY(BlueprintReadOnly, Category = "GameResult")
	int32 BallCount;
	UPROPERTY(BlueprintReadOnly, Category = "GameResult")
	int32 StrikeCount;
	UPROPERTY(BlueprintReadOnly, Category = "GameResult")
	int32 OutCount;
};

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UMyTestCPPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Custom")
	static TArray<int32> GetRandom();
	UFUNCTION(BlueprintCallable, Category = "Judge")
	static TArray<int32> ConvertToNumber(FString StringInput);
	UFUNCTION(BlueprintCallable, Category = "Judge")
	static FString ToFStringFromTArray(const TArray<int32>& PlayerNumber);
	UFUNCTION(BlueprintCallable, Category = "Judge")
	static FGameResult JudgeNumbers(const TArray<int32>& SecretNumber, const TArray<int32>& PlayerNumber);
};
