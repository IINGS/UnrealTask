#include "SpawnVolume.h"
#include "SpartaGameState.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
	LoopValue = 20;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	UDataTable* CurrentDataTable = nullptr;
	if (ASpartaGameState* GS = GetWorld()->GetGameState<ASpartaGameState>())
	{
		const int32 DataTableIndex = GS->CurrentLevelIndex * 3 + GS->CurrentWaveIndex;
		if (GS->LevelWaveDataTables.IsValidIndex(DataTableIndex))
		{
			CurrentDataTable = GS->LevelWaveDataTables[DataTableIndex];
		}
	}

	// DataTable�� ��ȿ�ϴٸ� ItemDataTable�� ����
	if (CurrentDataTable)
	{
		ItemDataTable = CurrentDataTable;
	}

	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.f, TotalChance);
	float AccumulateChance = 0.f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;//����
		}
	}

	return nullptr;//������
}

FVector ASpawnVolume::GetRandomPointVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();	//spawningbox�� �߽ɺ��� ������������ ������ �������� �Լ�
	FVector BoxOrigin = SpawningBox->GetComponentLocation();//spawningbox�� �߽� ��ǥ�� �������� �Լ�

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
			ItemClass,
			GetRandomPointVolume(),
			FRotator::ZeroRotator
	);

	return SpawnedActor;
}