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

	// Tick���� �ʿ��� ó���� ���� override
	virtual void Tick(float DeltaTime) override;

	// Pawn�� �Է� ����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� �������� ��ġ�� �����ϱ� ���� ����
	FVector LastLocation;
	// Pawn�� ���� �ӵ��� �����ϱ� ���� ���� (cm/s)
	float CurrentSpeed;

protected:
	// BeginPlay���� �ʱ�ȭ ó��
	virtual void BeginPlay() override;

	/* --- Components --- */

	// ���� ���̷�Ż �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	// ī�޶� �ε巴�� ����ٴϰ� �� ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	// ���� ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	// Pawn �̵��� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UFloatingPawnMovement* MovementComponent;

	/* --- Movement Variables --- */

	// ���� ���ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AccelFactor;

	// ȸ�� �ӵ� (wasd)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float YawPitchSpeed;

	// ��� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CoefLift;

	// ���� ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CoefResist;

	// ���� ����
	float Resist;

	// ���
	float Lift;

	//�߷°��ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GravityFactor;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Mass;

	// Roll ó���� ���� ��� ���� (�ʱⰪ 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight")
	float CurrentRoll;

	// Roll ����(����) �ӵ� (���ϴ� ������ ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float RollReturnSpeed;

	// Roll ȸ�� �ӵ� (�Է¿� ���� ȸ�� ����ġ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float RollSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight")
	float CurrentPitch;

	// Pitch �Է¿� ���� ��ȭ �ӵ� (�ʿ信 ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float PitchSpeed;

	// �Է��� ���� �� Pitch�� 0���� �����ϴ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float PitchReturnSpeed;

	

	/* --- ���� ���� --- */

	// ���� Pawn�� ���߿� �ִ��� ���� (false�̸� ����)
	bool bIsFlying;

	/* --- Input Functions --- */

	// ����/���� (space bar)
	UFUNCTION()
	void Acceleration(const FInputActionValue& Value);

	// ���߿��� rolling
	UFUNCTION()
	void Rolling(const FInputActionValue& Value);

	// ���߿��� pitching
	UFUNCTION()
	void Pitching(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);


};