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

	/* --- Enhanced Input ���� UPROPERTY --- */

	// �� Pawn ���� Input Mapping Context (������/�������Ʈ���� �Ҵ�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Dragon;

	// ���� �Է� (��:space bar)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Accel;

	// rolling �Է� �׼� (��: A, D)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Rolling;

	// pitching �Է� �׼� (��: W, S)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Pitching;

	// ī�޶� �Է� �׼� (��: ���콺 X, Y)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Look;

};
