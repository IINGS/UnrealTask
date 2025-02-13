#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SPTPawn.generated.h"

struct FInputActionValue;

UCLASS()
class SEVENTH_TASK_API ASPTPawn : public APawn
{
	GENERATED_BODY()

public:
	ASPTPawn();

	// Tick에서 필요한 처리를 위해 override
	virtual void Tick(float DeltaTime) override;

	// Pawn의 입력 설정
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 이전 프레임의 위치를 저장하기 위한 변수
	FVector LastLocation;
	// Pawn의 현재 속도를 저장하기 위한 변수 (cm/s)
	float CurrentSpeed;

protected:
	// BeginPlay에서 초기화 처리
	virtual void BeginPlay() override;

	/* --- Components --- */

	// 용의 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	// 카메라를 부드럽게 따라다니게 할 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	// 실제 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	// Pawn 이동을 위한 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UFloatingPawnMovement* MovementComponent;

	/* --- Movement Variables --- */

	// 전진 가속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AccelFactor;

	// 회전 속도 (wasd)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float YawPitchSpeed;

	// 양력 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CoefLift;

	// 공기 저항 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CoefResist;

	// 공기 저항
	float Resist;

	// 양력
	float Lift;

	//중력가속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GravityFactor;

	//질량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Mass;

	// Roll 처리를 위한 멤버 변수 (초기값 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight")
	float CurrentRoll;

	// Roll 감속(복귀) 속도 (원하는 값으로 조절)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float RollReturnSpeed;

	// Roll 회전 속도 (입력에 따른 회전 가중치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float RollSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight")
	float CurrentPitch;

	// Pitch 입력에 따른 변화 속도 (필요에 따라 조절)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float PitchSpeed;

	// 입력이 없을 때 Pitch가 0으로 복귀하는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float PitchReturnSpeed;

	

	/* --- 상태 변수 --- */

	// 현재 Pawn이 공중에 있는지 여부 (false이면 지상)
	bool bIsFlying;

	/* --- Input Functions --- */

	// 전진/후진 (space bar)
	UFUNCTION()
	void Acceleration(const FInputActionValue& Value);

	// 공중에서 rolling
	UFUNCTION()
	void Rolling(const FInputActionValue& Value);

	// 공중에서 pitching
	UFUNCTION()
	void Pitching(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);


};