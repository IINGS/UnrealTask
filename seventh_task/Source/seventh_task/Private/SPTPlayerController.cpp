#include "SPTPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

ASPTPlayerController::ASPTPlayerController()
{
	// 기본값 초기화 (에디터 또는 블루프린트에서 할당)
	IMC_Dragon = nullptr;
	IA_Accel = nullptr;
	IA_Rolling = nullptr;
	IA_Pitching = nullptr;
	IA_Look = nullptr;
}

void ASPTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// LocalPlayer가 존재하는 경우 Enhanced Input Subsystem에 매핑 컨텍스트 추가
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Dragon)
			{
				// Mapping Context를 우선순위 0으로 추가 (필요에 따라 우선순위를 조정)
				InputSubsystem->AddMappingContext(IMC_Dragon, 0);
			}
		}
	}
}
