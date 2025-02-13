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
		UPrimitiveComponent* OverlappedComp,					//자기자신
		AActor* OtherActor,										//부딪힌 놈
		UPrimitiveComponent* OtherComp,							//부딪힌 놈의 출동 컴포넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;						//오버랩 됐을 때 실행할 함수
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;								//오버랩 벗어났을 때 실행할 함수
	UFUNCTION()
	virtual void ActivateItem(AActor* Activator) = 0;			//아이템 활성화
	UFUNCTION()
	virtual FName GetItemType() const = 0;						//아이템 타입을 반환. FString을 써도 되는데, FName이 속도도 빠르고 메모리도 덜 먹음


	//지뢰, 힐링, 코인
	//힐링, 코인 - 즉시 발동형 - 오버랩
	//지뢰- 폭발 범위 내에 오버랩 - 5초 뒤 폭발 - 5초동안 오버랩 - 데미지
};
