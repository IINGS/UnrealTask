#include "SPTPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

ASPTPlayerController::ASPTPlayerController()
{
	// �⺻�� �ʱ�ȭ (������ �Ǵ� �������Ʈ���� �Ҵ�)
	IMC_Dragon = nullptr;
	IA_Accel = nullptr;
	IA_Rolling = nullptr;
	IA_Pitching = nullptr;
	IA_Look = nullptr;
}

void ASPTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// LocalPlayer�� �����ϴ� ��� Enhanced Input Subsystem�� ���� ���ؽ�Ʈ �߰�
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Dragon)
			{
				// Mapping Context�� �켱���� 0���� �߰� (�ʿ信 ���� �켱������ ����)
				InputSubsystem->AddMappingContext(IMC_Dragon, 0);
			}
		}
	}
}
