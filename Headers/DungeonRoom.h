/**
 * 
 * Author: Matthew Papageorge
 * Date: May 10, 2024
 * Description: This class represents the fundamental structural primitive within dungeon environments.
 * Within the context of the game, dungeons are constructed by interconnecting individual rooms.
 * 
 * DungeonRoom blueprint assets consist of four walls initially devoid of doors. 
 * Each wall is divided into segments, and each segment can be targeted from code during runtime.
 * This flexibility allows for dynamic door placement, enabling the connection of rooms as needed.
 * 
 * Door meshes must be specified within the blueprint.
 * Each wall segment must be tagged with a numerical index representing its position within the wall.
 * The counting convention for these indices starts from 0.
 * 
 * Sample blueprint assets can be located in the "Game/Dungeon/Rooms" folder in the content directory.
 */

#pragma once
#include "CoreMinimal.h"

#include "Dungeon/SizeTypes/NumberOfTiles.h"
#include "Dungeon/Enums/DungeonTheme.h"
#include "Dungeon/Enums/Direction.h"
#include "Dungeon/SegmentedWall.h"

#include "GameFramework/Actor.h"
#include "DungeonRoom.generated.h"

UCLASS()
class ARPG_API ADungeonRoom : public AActor
{
	GENERATED_BODY()

public:
	/** Structure defining the location of individual wall segments */
	struct FWallLocation
	{
		/** Specifies a wall (North, South, East, or West). */
		EDirection WallDirection;	
		
		/** Indicates the index within the specified wall direction (starting from 0). */
		FNumberOfTiles SegmentIndex;
	};

	/** Structure defining the information required to spawn a room */
	struct FSpawnInfo
	{
		using FDoorLocations = TArray<ADungeonRoom::FWallLocation>;

		UObject* 	LoadedAsset;		// the asset to spawn		
		
		FVector 	RoomLocation;		// defines where to spawn the room
		
		FDoorLocations  DoorLocations;		// defines the locations of the doors
	};

	/** Spawns a room according to the provided spawn info  */
	static ADungeonRoom* Spawn(const FSpawnInfo& SpawnInfo, UWorld* World);

public:
	/**
	 * Adds a door to the room at the specified location. The door static mesh used is randomly selected
         * from the set of door meshes defined in the blueprint.
         */
	void AddDoor(const FWallLocation& Location);

	/**
	 * Removes the door from the room at the specified location. The wall static mesh used to replace
	 * the door is randomly selected from the set of wall meshes defined in the blueprint.
	 */
	void RemoveDoor(const FWallLocation& Location);

	/**
	 * Checks if the given wall location exists within the context of this room.
	 *
	 * @param Location - The location to be checked.
	 * @return True if the location's index falls within the valid range:
	 *          - [0, Width) for North and South walls
	 *	    - [0, Length) for West and East walls,
	 *	    where Width and Length are the dimensions of the room
	 */
	bool IsValidWallLocation(const FWallLocation& Location) const;
	/** 
	 * Returns true if the room has a door at the provided location.
	 * 
	 * @warning An assertion is triggered if the location is not valid.
	 */
	bool HasDoorAtLocation(const FWallLocation& Location) const;

	/** Initializes the root and wall scene components. */
	ADungeonRoom();

private:
	/**
	 * Array of static meshes defining the doors that can be added to the room.
	 * When a door is added, a random mesh from this array is selected for use.
	 *
	 * @note Ensure that this array is populated with valid door meshes in the blueprint.
	 */
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> m_DoorMeshes;

	/**
	 * Array of static meshes defining the wall segments used to replace doors.
	 * When a door is removed, a random mesh from this array is selected as its replacement.
	 *
	 * @note Make sure this array includes suitable wall meshes for replacing doors in the blueprint.
	 */
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> m_WallMeshes;

	/**
	 * The root scene component of the room
	 * Used for organization within the blueprint to structure and group other components
	 */
	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_Root;		  

	/**
	 * Walls defining the boundaries of the room.
	 * Facilitates connections to other rooms.
	 */
	UPROPERTY(EditAnywhere)
	USegmentedWall* m_NorthWall;

	UPROPERTY(EditAnywhere)
	USegmentedWall* m_WestWall;

	UPROPERTY(EditAnywhere)
	USegmentedWall* m_EastWall;

	UPROPERTY(EditAnywhere)
	USegmentedWall* m_SouthWall;

	/** 
	 * Creates and returns a SegmentedWall with the given name
	 * Used to initialize the blueprint asset
	 */
	USegmentedWall* CreateWall(const FName Name);

	/**
	 * The theme of the dungeon, describing its visual and gameplay characteristics.
	 * Used for categorization and searchability in the Asset Registry.
	 */
	UPROPERTY(EditAnywhere, AssetRegistrySearchable)
	EDungeonTheme m_Theme; 

	/**
	 * The width of the room in terms of the number of tiles it occupies.
         * This field, along with length, defines the overall size of the room.
         * Used for categorization and searchability in the Asset Registry.
         */
	UPROPERTY(EditAnywhere, AssetRegistrySearchable)
	int32 m_Width;

	/**
	 * The length of the room in terms of the number of tiles it occupies.
	 * This field, along with width, defines the overall size of the room.
         * Used for categorization and searchability in the Asset Registry.
         */
	UPROPERTY(EditAnywhere, AssetRegistrySearchable)
	int32 m_Length; 

	/** Ensures that the blueprint has door and wall meshes set. */
	virtual void PostActorCreated() override;

	/* Returns the wall corresponding to the given direction. */
	USegmentedWall* GetWall(const EDirection Direction) const;

	/** 
	 * Sets the static mesh of the specified wall segment. Used by AddDoor and RemoveDoor.
	 */
	void SetStaticMesh(const FWallLocation& WallSegmentToUpdate, UStaticMesh* NewMesh);

	/** Returns a random static mesh from the provided array */
	UStaticMesh* GetRandomMesh(TArray<UStaticMesh*> Meshes) const;

	/* Requires access to the FNames of the AssetRegistrySearchable fields to enable searching for their values */
	friend class FDungeonRoomAssetAnalyzer;
};
