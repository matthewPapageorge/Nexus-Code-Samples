#include "SegmentedWall.h"

USegmentedWall::USegmentedWall()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void USegmentedWall::InitializeComponent()
{
	Super::InitializeComponent();
	const UWorld* World{ GetWorld() };
	if (World && World->IsGameWorld())
	{
		InitializeSegmentMeshes();
	}
}

void USegmentedWall::InitializeSegmentMeshes()
{
	auto WallSegments{ GetAttachChildren() };
	m_SegmentMeshes.SetNum(WallSegments.Num()); // ensures every assignment is in bounds

	for (auto WallSegment : WallSegments)
	{
		UStaticMeshComponent* MeshComponent{ Cast<UStaticMeshComponent>(WallSegment) };
		checkf(MeshComponent, TEXT("Error: SegmentedWall has a child that is not a static mesh: %s"), *GetPathName());

		int32 SegmentIndex{ GetIndexOfSegment(WallSegment) };

		m_SegmentMeshes[SegmentIndex] = MeshComponent;
	}
}

USegmentedWall::UWallSegment* USegmentedWall::GetSegment(int32 SegmentIndex)
{
	checkf(IsValidSegmentIndex(SegmentIndex), TEXT("Error: Attempted to get a wall segment that does not exist."));

	return m_SegmentMeshes[SegmentIndex];
}


int32 USegmentedWall::GetIndexOfSegment(USceneComponent* Segment)
{
	checkf(Segment->ComponentTags.Num() > 0, TEXT("Error: A wall segment is missing its index tag: %s"), *GetPathName());

	FString TagValue{ Segment->ComponentTags[0].ToString() };

	checkf(TagValue.IsNumeric(), TEXT("Error: A wall segment has an invalid index tag: %s"), *GetPathName());
	
	return FCString::Atoi(*TagValue);
}

bool USegmentedWall::IsValidSegmentIndex(int32 SegmentIndex)
{
	return m_SegmentMeshes.IsValidIndex(SegmentIndex);
}



