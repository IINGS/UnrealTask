// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestCPPLib.h"
#include "algorithm"
#include "vector"
#include "string"

using namespace std;

TArray<int32> UMyTestCPPLib::GetRandom()
{
	TArray<int32> Numbers;

	for (int32 i = 1; i <= 9; i++)
	{
		Numbers.Add(i);
	}

	TArray<int32> Result;

	for (int32 i = 0; i < 3; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Add(Numbers[RandomIndex]);
		Numbers.Remove(RandomIndex);
	}

	return Result;
}

TArray<int32> UMyTestCPPLib::ConvertToNumber(FString StringInput)
{
	string ToString = string(TCHAR_TO_UTF8(*StringInput));
	auto it = find(ToString.begin(), ToString.end(), '/');
	string answerStr = "";
	FString answerFStr;
	TArray<int32> answerArr;

	size_t slashIndex = it - ToString.begin();
	size_t startIndex = slashIndex + 1;
	size_t count = 3;
	if (ToString.size() - startIndex < 3)
	{
		count = ToString.size() - startIndex;
	}

	if (it != ToString.end())
	{
		answerStr = ToString.substr(startIndex, count);
		answerFStr = FString(UTF8_TO_TCHAR(answerStr.c_str()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString(TEXT("YOUR ANSWER: ")) + answerFStr);
	}

	if (answerStr != "")
	{
		for (int32 i = 0; i < 3; i++)
		{
			answerArr.Add(answerStr[i] - '0');
		}
	}

	return answerArr;
}

FGameResult UMyTestCPPLib::JudgeNumbers(const TArray<int32>& SecretNumber, const TArray<int32>& PlayerNumber)
{
	FGameResult Result;
	Result.StrikeCount = 0;
	Result.BallCount = 0;
	Result.OutCount = 0;

	if (SecretNumber.IsEmpty() || PlayerNumber.IsEmpty())
	{
		Result.bIsAnswer = false;
		return Result;
	}

	for (int32 i = 0; i < 3; i++)
	{
		if (SecretNumber[i] == PlayerNumber[i])
		{
			Result.StrikeCount++;
		}
		else if (PlayerNumber.Contains(SecretNumber[i]))
		{
			Result.BallCount++;
		}
		else
		{
			Result.OutCount++;
		}
	}

	Result.bIsAnswer = (Result.StrikeCount == 3);
	return Result;
}

FString UMyTestCPPLib::ToFStringFromTArray(const TArray<int32>& PlayerNumber)
{
	string beforeConvert = "";
	for (int32 i = 0; i < PlayerNumber.Num(); i++)
	{
		beforeConvert += PlayerNumber[i] + '0';
	}
	return FString(UTF8_TO_TCHAR(beforeConvert.c_str()));
}