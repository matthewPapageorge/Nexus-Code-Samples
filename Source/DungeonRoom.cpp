#include "DungeonRoom.h"

#include "System/BaseBlueprintAssetAnalyzer.h"
#include "Dungeon/SegmentedWall.h"
#include "Components/StaticMeshComponent.h"
#include "Dungeon/Enums/Direction.h"

ADungeonRoom::ADungeonRoom()
{
	m_Root        = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_Root;

	m_NorthWall   = CreateWall("NorthWall");
	m_SouthWall   = CreateWall("SouthWall");
	m_WestWall    = CreateWall("WestWall");
	m_EastWall    = CreateWall("EastWall");
}

USegmentedWall* ADungeonRoom::GetWall(const EDirection Direction) const
{
	switch (Direction)
	{
		case EDirection::North: return m_NorthWall;
		case EDirection::South: return m_SouthWall;
		case EDirection::East:	return m_EastWall;
		case EDirection::West:  return m_WestWall;
		
		default:
			checkf(false, TEXT("Error: Invalid argument given to GetWall()"));
			return nullptr;
	}
}

void ADungeonRoom::SetStaticMesh(const FWallLocation& Location, UStaticMesh* NewMesh)
{
	USegmentedWall* WallBeingUpdated{ GetWall(Location.WallDirection) };
	WallBeingUpdated->GetSegment(Location.SegmentIndex)->SetStaticMesh(NewMesh);
}

bool ADungeonRoom::IsValidWallLocation(const FWallLocation& Location) const
{
	USegmentedWall* Wall{ GetWall(Location.WallDirection) };
	return Wall->IsValidSegmentIndex(Location.SegmentIndex);
}

UStaticMesh* ADungeonRoom::GetRandomMesh(TArray<UStaticMesh*> Meshes) const
{
	return Meshes[FMath::RandRange(0, Meshes.Num() - 1)];
}

ADungeonRoom* ADungeonRoom::Spawn(const FSpawnInfo& SpawnInfo, UWorld* World)
{
	UClass* SpawnableClass { FBaseBlueprintAssetAnalyzer::GetSpawnableClass(SpawnInfo.LoadedAsset) };

	ADungeonRoom* SpawnedRoom{ World->SpawnActor<ADungeonRoom>(SpawnableClass, SpawnInfo.RoomLocation, FRotator())};
	
	for (auto Location : SpawnInfo.DoorLocations)
	{
		SpawnedRoom->AddDoor(Location);
	}

	return SpawnedRoom;
}

void ADungeonRoom::AddDoor(const FWallLocation& Location)
{
	checkf(m_DoorMeshes.Num() > 0, TEXT("Error: Blueprint missing door meshes: %s"), *GetPathName());

	checkf(IsValidWallLocation(Location), TEXT("Error: Attempted to add a door to an invalid location"));

	checkf(!HasDoorAtLocation(Location), TEXT("Error: Attempted to add a door to a location that already has a door"));

	SetStaticMesh(Location, GetRandomMesh(m_DoorMeshes));
}

void ADungeonRoom::RemoveDoor(const FWallLocation& Location)
{
	checkf(m_WallMeshes.Num() > 0, TEXT("Error: Blueprint missing wall meshes: %s"), *GetPathName());

	checkf(IsValidWallLocation(Location), TEXT("Error: Attempted to remove a door from an invalid location."));
	
	checkf(HasDoorAtLocation(Location), TEXT("Error: Attempted to remove a door from a location that did not have a door."))
	
	SetStaticMesh(Location, GetRandomMesh(m_WallMeshes));
}

bool ADungeonRoom::HasDoorAtLocation(const FWallLocation& Location) const
{
	checkf(IsValidWallLocation(Location), TEXT("Error: Attempted to check if there was a door at an invalid location"));

	UStaticMesh* MeshAtLocation{ GetWall(Location.WallDirection)->GetSegment(Location.SegmentIndex)->GetStaticMesh() };

	return m_DoorMeshes.Contains(MeshAtLocation);
}


USegmentedWall* ADungeonRoom::CreateWall(const FName Name)
{
	USegmentedWall* Wall = CreateDefaultSubobject<USegmentedWall>(Name);
	Wall->SetupAttachment(m_Root);
	return Wall;
}

void ADungeonRoom::PostActorCreated()
{
	checkf(m_DoorMeshes.Num() > 0, TEXT("Error: Blueprint missing door meshes: %s"), *GetPathName());

	checkf(m_WallMeshes.Num() > 0, TEXT("Error: Blueprint missing wall meshes: %s"), *GetPathName());
}




