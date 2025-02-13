#include "SPTPawn.h"
#include "SPTPlayerController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"

ASPTPawn::ASPTPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	/* --- Mesh Component --- */
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	// �޽� �ε�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PawnMesh(
		TEXT("SkeletalMesh'/Game/FourEvilDragonsPBR/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'")
	);
	if (PawnMesh.Succeeded())
	{
		MeshComponent->SetSkeletalMesh(PawnMesh.Object);
	}
	MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* --- SpringArm & Camera --- */
	// ���������� MeshComponent �Ʒ��� ����
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 4500.f;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 500.f));
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	/* --- �⺻ ���� �ʱ�ȭ --- */
	AccelFactor = 4000.f;		//���ӵ�
	Mass = 500.f;				//����
	YawPitchSpeed = 100.f;		//ȸ���ӵ�
	CoefResist = 1.f;		//�������װ��
	CoefLift = 7.f;				//��°��
	GravityFactor = 980.f;		//�߷°��ӵ�

	
	bIsFlying = false;

	CurrentRoll = 0.f;
	RollSpeed = 100.f;         // �Է¿� ���� Roll ��ȭ�� (�ʿ信 ���� ����)
	RollReturnSpeed = 1.f;     // �Է��� ���� �� Roll�� 0���� �����ϴ� �ӵ� (���� Ŭ���� ���� ����)

	CurrentPitch = 0.f;
	PitchSpeed = 100.f;        // �Է¿� ���� Pitch ��ȭ�� (�ʿ信 ���� ����)
	PitchReturnSpeed = 1.f;    // �Է��� ���� �� Pitch�� 0���� �����ϴ� �ӵ� (���� Ŭ���� ���� ����)
}

void ASPTPawn::BeginPlay()
{
	Super::BeginPlay();
	LastLocation = GetActorLocation();
}

void ASPTPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. ���� �����Ӱ��� ��ġ ���̷� �ӵ� ���
    FVector CurrentLocation = GetActorLocation();
    FVector Velocity = (CurrentLocation - LastLocation) / DeltaTime;
    CurrentSpeed = Velocity.Size();
    LastLocation = CurrentLocation; // ���� ������ ������Ʈ

    UE_LOG(LogTemp, Log, TEXT("CurrentSpeed: %f"), CurrentSpeed);

    // 2. ��� ��� (�޽��� ���� ���� ���и� ���)
    float EffectiveForwardSpeed = FVector::DotProduct(Velocity, MeshComponent->GetRightVector());
    EffectiveForwardSpeed = FMath::Max(0.f, EffectiveForwardSpeed); // ���� �� �����̸� 0���� ó��
    Lift = EffectiveForwardSpeed * EffectiveForwardSpeed * CoefLift;
    FVector DeltaLiftLocation = MeshComponent->GetUpVector() * Lift * DeltaTime * DeltaTime / Mass;
    const float MaxDeltaLift = 50.f; // �ʿ信 ���� ����
    DeltaLiftLocation = DeltaLiftLocation.GetClampedToMaxSize(MaxDeltaLift);

    // 3. �߷� ������ ��� (���� ��ǥ�� ���� -Z ����)
    FVector DeltaGravityLocation = FVector(0.f, 0.f, -GravityFactor * DeltaTime * DeltaTime * 100.f);

    // 4. ��������(�巡��) ���
    FVector DragForce = FVector::ZeroVector;
    if (CurrentSpeed > 0.f)
    {
        DragForce = -(CurrentSpeed * CurrentSpeed + CurrentSpeed) * CoefResist / Mass * Velocity.GetSafeNormal();
    }
    FVector DeltaDragLocation = DragForce * DeltaTime * DeltaTime;
    const float MaxDeltaDrag = 30.f; // �ʿ信 ���� ����
    DeltaDragLocation = DeltaDragLocation.GetClampedToMaxSize(MaxDeltaDrag);

    UE_LOG(LogTemp, Log, TEXT("DeltaLiftLocation: (%f, %f, %f)"), DeltaLiftLocation.X, DeltaLiftLocation.Y, DeltaLiftLocation.Z);
    UE_LOG(LogTemp, Log, TEXT("DeltaDragLocation: (%f, %f, %f)"), DeltaDragLocation.X, DeltaDragLocation.Y, DeltaDragLocation.Z);

    // 5. ��ü ������ ��� �� ����
    FVector TotalOffset = DeltaLiftLocation + DeltaGravityLocation + DeltaDragLocation;
    FVector NewLocation = GetActorLocation() + TotalOffset;

    if (NewLocation.Z < 0.f)
    {
        NewLocation.Z = 0.f;
    }
    SetActorLocation(NewLocation);
}



void ASPTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASPTPlayerController* PlayerController = Cast<ASPTPlayerController>(GetController()))
		{
			if (PlayerController->IA_Accel)
				EnhancedInput->BindAction(PlayerController->IA_Accel, ETriggerEvent::Triggered, this, &ASPTPawn::Acceleration);
			if (PlayerController->IA_Rolling)
				EnhancedInput->BindAction(PlayerController->IA_Rolling, ETriggerEvent::Triggered, this, &ASPTPawn::Rolling);
			if (PlayerController->IA_Pitching)
				EnhancedInput->BindAction(PlayerController->IA_Pitching, ETriggerEvent::Triggered, this, &ASPTPawn::Pitching);
			if (PlayerController->IA_Look)
				EnhancedInput->BindAction(PlayerController->IA_Look, ETriggerEvent::Triggered, this, &ASPTPawn::Look);

		}
	}
}

void ASPTPawn::Acceleration(const FInputActionValue& Value)
{
	if (const float AxisValue = Value.Get<bool>())
	{
		float dt = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.f;
		FVector DeltaLocation = MeshComponent->GetRightVector() * 50.f * AccelFactor * dt * dt;
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void ASPTPawn::Rolling(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	float dt = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.f;

	if (!FMath::IsNearlyZero(InputValue))
	{
		// �Է¿� ���� ��ȭ�� ȸ���� ���
		float DeltaRoll = InputValue * RollSpeed * dt;
		// ���� ȸ������ ���� ���� ������ ������Ʈ
		float NewRoll = FMath::Clamp(CurrentRoll + DeltaRoll, -70.f, 70.f);
		float DeltaToApply = NewRoll - CurrentRoll;
		CurrentRoll = NewRoll;
		// ������ ��ȭ���� MeshComponent�� ����
		MeshComponent->AddLocalRotation(FRotator(0.f, 0.f, DeltaToApply));
	}
}


void ASPTPawn::Pitching(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	float dt = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.f;

	if (!FMath::IsNearlyZero(InputValue))
	{
		float DeltaPitch = InputValue * PitchSpeed * dt;
		float NewPitch = FMath::Clamp(CurrentPitch + DeltaPitch, -70.f, 70.f);
		float DeltaToApply = NewPitch - CurrentPitch;
		CurrentPitch = NewPitch;
		// Pitch�� FRotator�� X�࿡ �ش�
		MeshComponent->AddLocalRotation(FRotator(DeltaToApply, 0.f, 0.f));
	}
}


void ASPTPawn::Look(const FInputActionValue& Value)
{
	    // ���콺 �Է��� FVector2D�� �޽��ϴ�. (X: �¿�, Y: ����)
    const FVector2D LookInput = Value.Get<FVector2D>();
    const float dt = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.f;

    // Pawn�� �������̳� ȸ������ ������ ���� �ʰ�, ���� ���������� ��� ȸ���� ������Ʈ
    FRotator CurrentRotation = SpringArmComponent->GetRelativeRotation();

    // Yaw(�¿� ȸ��): �Է� X�� ����Ͽ� ���� (���ϴ� �ӵ��� ������)
    CurrentRotation.Yaw += LookInput.X * YawPitchSpeed * dt;

    // Pitch(���� ȸ��): �Է� Y�� ����Ͽ� ���� (Ŭ������ ���� ���� ���� ������ �� ���� ����)
    CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch - LookInput.Y * YawPitchSpeed * dt, -80.f, 80.f);

    // ���������� ��� ȸ�� ������Ʈ (�̷� ���� ī�޶� ȸ��)
    SpringArmComponent->SetRelativeRotation(CurrentRotation);
}

