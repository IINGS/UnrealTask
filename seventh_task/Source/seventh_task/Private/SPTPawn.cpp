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

	// 메시 로드
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PawnMesh(
		TEXT("SkeletalMesh'/Game/FourEvilDragonsPBR/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'")
	);
	if (PawnMesh.Succeeded())
	{
		MeshComponent->SetSkeletalMesh(PawnMesh.Object);
	}
	MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* --- SpringArm & Camera --- */
	// 스프링암을 MeshComponent 아래에 부착
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 4500.f;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 500.f));
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	/* --- 기본 변수 초기화 --- */
	AccelFactor = 4000.f;		//가속도
	Mass = 500.f;				//질량
	YawPitchSpeed = 100.f;		//회전속도
	CoefResist = 1.f;		//공기저항계수
	CoefLift = 7.f;				//양력계수
	GravityFactor = 980.f;		//중력가속도

	
	bIsFlying = false;

	CurrentRoll = 0.f;
	RollSpeed = 100.f;         // 입력에 따른 Roll 변화율 (필요에 따라 조절)
	RollReturnSpeed = 1.f;     // 입력이 없을 때 Roll이 0으로 복귀하는 속도 (값이 클수록 빨리 복귀)

	CurrentPitch = 0.f;
	PitchSpeed = 100.f;        // 입력에 따른 Pitch 변화율 (필요에 따라 조절)
	PitchReturnSpeed = 1.f;    // 입력이 없을 때 Pitch가 0으로 복귀하는 속도 (값이 클수록 빨리 복귀)
}

void ASPTPawn::BeginPlay()
{
	Super::BeginPlay();
	LastLocation = GetActorLocation();
}

void ASPTPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. 이전 프레임과의 위치 차이로 속도 계산
    FVector CurrentLocation = GetActorLocation();
    FVector Velocity = (CurrentLocation - LastLocation) / DeltaTime;
    CurrentSpeed = Velocity.Size();
    LastLocation = CurrentLocation; // 다음 프레임 업데이트

    UE_LOG(LogTemp, Log, TEXT("CurrentSpeed: %f"), CurrentSpeed);

    // 2. 양력 계산 (메시의 전방 방향 성분만 사용)
    float EffectiveForwardSpeed = FVector::DotProduct(Velocity, MeshComponent->GetRightVector());
    EffectiveForwardSpeed = FMath::Max(0.f, EffectiveForwardSpeed); // 후진 시 음수이면 0으로 처리
    Lift = EffectiveForwardSpeed * EffectiveForwardSpeed * CoefLift;
    FVector DeltaLiftLocation = MeshComponent->GetUpVector() * Lift * DeltaTime * DeltaTime / Mass;
    const float MaxDeltaLift = 50.f; // 필요에 따라 조절
    DeltaLiftLocation = DeltaLiftLocation.GetClampedToMaxSize(MaxDeltaLift);

    // 3. 중력 오프셋 계산 (월드 좌표계 기준 -Z 방향)
    FVector DeltaGravityLocation = FVector(0.f, 0.f, -GravityFactor * DeltaTime * DeltaTime * 100.f);

    // 4. 공기저항(드래그) 계산
    FVector DragForce = FVector::ZeroVector;
    if (CurrentSpeed > 0.f)
    {
        DragForce = -(CurrentSpeed * CurrentSpeed + CurrentSpeed) * CoefResist / Mass * Velocity.GetSafeNormal();
    }
    FVector DeltaDragLocation = DragForce * DeltaTime * DeltaTime;
    const float MaxDeltaDrag = 30.f; // 필요에 따라 조절
    DeltaDragLocation = DeltaDragLocation.GetClampedToMaxSize(MaxDeltaDrag);

    UE_LOG(LogTemp, Log, TEXT("DeltaLiftLocation: (%f, %f, %f)"), DeltaLiftLocation.X, DeltaLiftLocation.Y, DeltaLiftLocation.Z);
    UE_LOG(LogTemp, Log, TEXT("DeltaDragLocation: (%f, %f, %f)"), DeltaDragLocation.X, DeltaDragLocation.Y, DeltaDragLocation.Z);

    // 5. 전체 오프셋 계산 및 적용
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
		// 입력에 따라 변화할 회전량 계산
		float DeltaRoll = InputValue * RollSpeed * dt;
		// 누적 회전값을 제한 범위 내에서 업데이트
		float NewRoll = FMath::Clamp(CurrentRoll + DeltaRoll, -70.f, 70.f);
		float DeltaToApply = NewRoll - CurrentRoll;
		CurrentRoll = NewRoll;
		// 누적된 변화량만 MeshComponent에 더함
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
		// Pitch는 FRotator의 X축에 해당
		MeshComponent->AddLocalRotation(FRotator(DeltaToApply, 0.f, 0.f));
	}
}


void ASPTPawn::Look(const FInputActionValue& Value)
{
	    // 마우스 입력을 FVector2D로 받습니다. (X: 좌우, Y: 상하)
    const FVector2D LookInput = Value.Get<FVector2D>();
    const float dt = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.f;

    // Pawn의 움직임이나 회전에는 영향을 주지 않고, 오직 스프링암의 상대 회전을 업데이트
    FRotator CurrentRotation = SpringArmComponent->GetRelativeRotation();

    // Yaw(좌우 회전): 입력 X에 비례하여 변경 (원하는 속도로 곱해줌)
    CurrentRotation.Yaw += LookInput.X * YawPitchSpeed * dt;

    // Pitch(상하 회전): 입력 Y에 비례하여 변경 (클램프를 통해 상하 각도 제한을 둘 수도 있음)
    CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch - LookInput.Y * YawPitchSpeed * dt, -80.f, 80.f);

    // 스프링암의 상대 회전 업데이트 (이로 인해 카메라만 회전)
    SpringArmComponent->SetRelativeRotation(CurrentRotation);
}

