#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class CH3INTERFACE_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,					//�ڱ��ڽ�
		AActor* OtherActor,										//�ε��� ��
		UPrimitiveComponent* OtherComp,							//�ε��� ���� �⵿ ������Ʈ
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;						//������ ���� �� ������ �Լ�
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;								//������ ����� �� ������ �Լ�
	UFUNCTION()
	virtual void ActivateItem(AActor* Activator) = 0;			//������ Ȱ��ȭ
	UFUNCTION()
	virtual FName GetItemType() const = 0;						//������ Ÿ���� ��ȯ. FString�� �ᵵ �Ǵµ�, FName�� �ӵ��� ������ �޸𸮵� �� ����


	//����, ����, ����
	//����, ���� - ��� �ߵ��� - ������
	//����- ���� ���� ���� ������ - 5�� �� ���� - 5�ʵ��� ������ - ������
};
