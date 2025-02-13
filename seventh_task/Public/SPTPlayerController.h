#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPTPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SEVENTH_TASK_API ASPTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPTPlayerController();

	virtual void BeginPlay() override;

	/* --- Enhanced Input 관련 UPROPERTY --- */

	// 용 Pawn 전용 Input Mapping Context (에디터/블루프린트에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Dragon;

	// 전진 입력 (예:space bar)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Accel;

	// rolling 입력 액션 (예: A, D)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Rolling;

	// pitching 입력 액션 (예: W, S)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Pitching;

	// 카메라 입력 액션 (예: 마우스 X, Y)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Look;

};
