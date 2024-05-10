/**
 *
 * Author: Matthew Papageorge
 * Date: 05/10/24
 * Description: Automated test suite for the DungeonRoom class. 
 * Tests coverage includes:
 * 	- Spawning
 *      - Detecting doors at specific locations
 * 	- Adding doors to the room
 * 	- Removing doors from the room
 * 
 * @note Test cases are executed within the Unreal development automation test framework.
 *
 */

#include "Misc/AutomationTest.h"
#include "Dungeon/Rooms/DungeonRoom.h"

#include "Engine/StreamableManager.h"
#include "Tests/ApplicationTestUtilities.h"

#if WITH_DEV_AUTOMATION_TESTS
namespace
{
	
	const FString PathToAssets{ TEXT("/Game/Test/Dungeon/Rooms/DungeonRoom/") }; // the room assets to be used for conducting the tests

	/** 
	 * Validates that rooms can accurately detect doors at specified locations.
	 * 
	 * @return True if the door is detected, false otherwise
	 */
	bool TestDetectingDoor(FAutomationTestBase& This)
	{
		// The asset has a door on the north wall at index 1
		const FString CookedAssetName{ TEXT("TestDetectingDoorAsset.TestDetectingDoorAsset_C") }; 
		const FString RoomAssetPath{ PathToAssets + CookedAssetName };
		const ADungeonRoom::FWallLocation LocationWithADoor{ EDirection::North, 1 };

		using ApplicationTestUtilities::SpawnBlueprintAsset;
		ADungeonRoom* SpawnedRoom{ Cast<ADungeonRoom>(SpawnBlueprintAsset(RoomAssetPath)) };
		if (SpawnedRoom)
		{
			return This.TestTrue(TEXT("Must detect doors in locations where they exist"), SpawnedRoom->HasDoorAtLocation(LocationWithADoor));
		}
		else
		{
			const FString FunctionName{ StringCast<TCHAR>(__FUNCTION__).Get() };
			const FString ErrorMessage{ FString::Printf(TEXT("%s failed to spawn room."), *FunctionName) };

			This.AddError(ErrorMessage);
			return false;
		}
	}

	/**
	 * Validates that doors are not incorrectly detected in locations where they do not exist.
	 * 
	 * @return True if the room did not detect a door, false otherwise.
	 */
	bool TestNotIncorrectlyDetectingDoor(FAutomationTestBase& This)
	{
		// The asset does not have a door on the North wall at index 1
		const FString CookedAssetName{ TEXT("TestNotIncorrectlyDetectingDoorAsset.TestNotIncorrectlyDetectingDoorAsset_C") };
		const FString RoomAssetPath{ PathToAssets + CookedAssetName };
		const ADungeonRoom::FWallLocation LocationWithoutADoor{ EDirection::North, 1 };

		using ApplicationTestUtilities::SpawnBlueprintAsset;
		ADungeonRoom* SpawnedRoom{ Cast<ADungeonRoom>(SpawnBlueprintAsset(RoomAssetPath)) };
		if (SpawnedRoom)
		{
			return This.TestFalse(TEXT("Must not incorrectly detect a door at a location where one does not exist"),
				SpawnedRoom->HasDoorAtLocation(LocationWithoutADoor));
		}
		else
		{
			const FString FunctionName{ StringCast<TCHAR>(__FUNCTION__).Get() };
			const FString ErrorMessage{ FString::Printf(TEXT("%s failed to spawn room"), *FunctionName) };

			This.AddError(ErrorMessage);
			return false;
		}
	}

	/**
	 * Validates the ability to add doors to a specific wall in a room. 
         * This function is used as a helper by TestAddingDoorsToRoom.
	 * 
	 * @param WallToCheck - The wall being tested (North, South, East, or West).
	 * @return True if the doors were successfully added, false otherwise.
	 */
	bool CanAddDoorsToWallOfRoom(const EDirection WallToCheck)
	{
		// The asset has 2 wall segments on the North, South, East, and West walls
		static const FString CookedAssetName{ TEXT("CanAddDoorsToWallOfRoomAsset.CanAddDoorsToWallOfRoomAsset_C") };
		static const FString RoomAssetPath{ PathToAssets + CookedAssetName };
		static const ADungeonRoom::FWallLocation LocationToAddDoor0{ WallToCheck, 0 };
		static const ADungeonRoom::FWallLocation LocationToAddDoor1{ WallToCheck, 1 };	

		using ApplicationTestUtilities::SpawnBlueprintAsset;
		ADungeonRoom* SpawnedRoom{ Cast<ADungeonRoom>(SpawnBlueprintAsset(RoomAssetPath)) };
		if (SpawnedRoom)
		{
			SpawnedRoom->AddDoor(LocationToAddDoor0);
			SpawnedRoom->AddDoor(LocationToAddDoor1);

			return SpawnedRoom->HasDoorAtLocation(LocationToAddDoor0) &&
			       SpawnedRoom->HasDoorAtLocation(LocationToAddDoor1);
		}
		else
		{
			const FString FunctionName{ StringCast<TCHAR>(__FUNCTION__).Get() };
			const FString ErrorMessage{ FString::Printf(TEXT("%s failed to spawn room"), *FunctionName) };

			UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
			return false;
		}
	};

	/**
	 * Tests the ability to add doors to the North, South, East, and West walls.
	 *
	 * @note Each wall is tested at multiple locations to ensure proper functionality.
	 * @note This test relies on the proper functioning of ADungeonRoom::HasDoorAtLocation.
	 */
	void TestAddingDoorsToRoom(FAutomationTestBase& This)
	{
		This.TestTrue(TEXT("Must be able to add doors to the North wall of a room."),
			CanAddDoorsToWallOfRoom(EDirection::North));

		This.TestTrue(TEXT("Must be able to add doors to the South wall of a room."),
			CanAddDoorsToWallOfRoom(EDirection::South));

		This.TestTrue(TEXT("Must be able to add doors to the East wall of room."),
			CanAddDoorsToWallOfRoom(EDirection::East));
		
		This.TestTrue(TEXT("Must be able to add doors to the West wall of room."),
			CanAddDoorsToWallOfRoom(EDirection::West));

		UE_LOG(LogTemp, Log, TEXT("%s completed"), StringCast<TCHAR>(__FUNCTION__).Get());
	}

	/**
 	 * Validates the ability to remove doors from a specific wall in a room.
 	 * This function serves as a helper for the TestRemovingDoorsFromRoom function.
	 *
	 * @param WallToCheck - The wall being tested (North, South, East, or West).
	 * @return True if the doors were successfully removed, false otherwise.
	 */
	bool CanRemoveDoorsFromWallOfRoom(const EDirection WallToCheck)
	{
		// The asset has doors at indices 0 and 1 on the North, South, East, and West walls
		static const FString CookedAssetName{ TEXT("CanRemoveDoorsFromWallOfRoomAsset.CanRemoveDoorsFromWallOfRoomAsset_C") };
		static const FString RoomAssetPath{ PathToAssets + CookedAssetName };
		const ADungeonRoom::FWallLocation DoorLocation0{ WallToCheck, 0 }; 
		const ADungeonRoom::FWallLocation DoorLocation1{ WallToCheck, 1 };

		using ApplicationTestUtilities::SpawnBlueprintAsset;
		ADungeonRoom* SpawnedRoom{ Cast<ADungeonRoom>(SpawnBlueprintAsset(RoomAssetPath)) };

		SpawnedRoom->RemoveDoor(DoorLocation0);
		SpawnedRoom->RemoveDoor(DoorLocation1);

		return !SpawnedRoom->HasDoorAtLocation(DoorLocation0) &&
		       !SpawnedRoom->HasDoorAtLocation(DoorLocation1);		
	}

	/**
	 * Tests the ability to remove doors from rooms.
	 *
	 * @note Each wall is tested at multiple locations to ensure proper functionality.
	 * @note This test relies on the proper functioning of ADungeonRoom::HasDoorAtLocation.
	 */
	void TestRemovingDoorsFromRoom(FAutomationTestBase& This)
	{
		This.TestTrue(TEXT("Must be able to remove doors from the North wall of a room."),
			CanRemoveDoorsFromWallOfRoom(EDirection::North));

		This.TestTrue(TEXT("Must be able to remove doors from the South wall of a room."),
			CanRemoveDoorsFromWallOfRoom(EDirection::South));

		This.TestTrue(TEXT("Must be able to remove doors from the East wall of a room."),
			CanRemoveDoorsFromWallOfRoom(EDirection::East));

		This.TestTrue(TEXT("Must be able to remove doors from the West wall of a room."),
			CanRemoveDoorsFromWallOfRoom(EDirection::West));

		UE_LOG(LogTemp, Log, TEXT("%s completed"), StringCast<TCHAR>(__FUNCTION__).Get());
	}

	/** 
	 * Validates that rooms created via ADungeonRoom::Spawn have doors at the specified locations.
	 * 
	 * @note The test includes door locations for each cardinal direction.
	 * @note This test relies on the proper functioning of ADungeonRoom::HasDoorAtLocation.
	 */
	void TestIfSpawnMethodAddsDoors(
		FAutomationTestBase& This,
		UObject* AssetToSpawn,
		UWorld* WorldToSpawnIn
	)
	{
		const ADungeonRoom::FWallLocation LocationToAddDoor0{ EDirection::North, 0 };	
		const ADungeonRoom::FWallLocation LocationToAddDoor1{ EDirection::South, 1 };
		const ADungeonRoom::FWallLocation LocationToAddDoor2{ EDirection::East, 2 };
		const ADungeonRoom::FWallLocation LocationToAddDoor3{ EDirection::West, 3 };

		ADungeonRoom::FSpawnInfo SpawnInfo;
		SpawnInfo.LoadedAsset = AssetToSpawn;
		SpawnInfo.RoomLocation = FVector{ };
		SpawnInfo.DoorLocations.Add(LocationToAddDoor0);
		SpawnInfo.DoorLocations.Add(LocationToAddDoor1);
		SpawnInfo.DoorLocations.Add(LocationToAddDoor2);
		SpawnInfo.DoorLocations.Add(LocationToAddDoor3);

		ADungeonRoom* SpawnedRoom{ ADungeonRoom::Spawn(SpawnInfo, WorldToSpawnIn) };
		if (SpawnedRoom)
		{
			bool bAreDoorsAdded{
				SpawnedRoom->HasDoorAtLocation(LocationToAddDoor0) &&
				SpawnedRoom->HasDoorAtLocation(LocationToAddDoor1) &&
				SpawnedRoom->HasDoorAtLocation(LocationToAddDoor2) &&
				SpawnedRoom->HasDoorAtLocation(LocationToAddDoor3)
			};

			This.TestTrue(TEXT("The spawned room must have doors at the specified locations."), bAreDoorsAdded);
		}
		else
		{
			This.AddError(TEXT("TestIfSpawnMethodAddsDoors() failed to spawn room."));
		}
	}

	/** 
	 * Verifies that rooms created via ADungeonRoom::Spawn are positioned at the specified location.
	 */
	void TestIfSpawnMethodSpawnsRoomAtExpectedLocation(
		FAutomationTestBase& This,
		UObject* AssetToSpawn,
		UWorld* WorldToSpawnIn
	)
	{
		const FVector DesiredSpawnLocation{ 1000, 1000, 1000 };
		
		ADungeonRoom::FSpawnInfo SpawnInfo;
		SpawnInfo.LoadedAsset = AssetToSpawn;
		SpawnInfo.RoomLocation = DesiredSpawnLocation;
		
		ADungeonRoom* SpawnedRoom{ ADungeonRoom::Spawn(SpawnInfo, WorldToSpawnIn) };
		if (SpawnedRoom)
		{
			This.TestEqual(TEXT("Room must be spawned at the specified location."), 
				SpawnedRoom->GetActorLocation(), DesiredSpawnLocation);
		}
		else
		{
			const FString FunctionName{ StringCast<TCHAR>(__FUNCTION__).Get() };
			const FString ErrorMessage{ FString::Printf(TEXT("%s failed to spawn room"), *FunctionName) };

			This.AddError(ErrorMessage);
		}
	}
						     
	/**
	 * Executes a suite of tests for ADungeonRoom::Spawn, ensuring the spawned room
         * has doors at the correct locations and is positioned accurately.
         */
	void TestSpawnMethodSuite(FAutomationTestBase& This)
	{
		// the asset is a 4x4 room with no doors to support the test 'TestIfSpawnMethodAddsDoors'
		const FString CookedAssetName{ TEXT("TestSpawnMethodSuiteAsset.TestSpawnMethodSuiteAsset_C") };
		const FString RoomAssetPath{ PathToAssets + CookedAssetName };
		
		FStreamableManager StreamableManager;
		UObject* LoadedAsset{ StreamableManager.LoadSynchronous(RoomAssetPath) };
		if (!LoadedAsset)
		{
			const FString FunctionName{ StringCast<TCHAR>(__FUNCTION__).Get() };
			const FString ErrorMessage{ FString::Printf(TEXT("%s failed to load %s."), *FunctionName, *RoomAssetPath) };

			This.AddError(ErrorMessage);
			return;
		}

		UWorld* WorldToSpawnIn{ ApplicationTestUtilities::GetUWorld() };
		if (!WorldToSpawnIn)
		{
			const FString FunctionName{ StringCast<TCHAR>(__FUNCTION__).Get() };
			const FString ErrorMessage{ FString::Printf(TEXT("%s failed to get UWorld."), *FunctionName) };

			This.AddError(ErrorMessage);
			return;
		}

		TestIfSpawnMethodAddsDoors(This, LoadedAsset, WorldToSpawnIn);

		TestIfSpawnMethodSpawnsRoomAtExpectedLocation(This, LoadedAsset, WorldToSpawnIn);

		UE_LOG(LogTemp, Log, TEXT("%s completed"), StringCast<TCHAR>(__FUNCTION__).Get());
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDungeonRoomTest, "ARPG.Dungeon.Rooms.DungeonRoomTest",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FDungeonRoomTest::RunTest(const FString& Parameters)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), StringCast<TCHAR>(__FUNCTION__).Get());
	
	if (TestDetectingDoor(*this) && TestNotIncorrectlyDetectingDoor(*this))
	{
		UE_LOG(LogTemp, Log, TEXT("Door detection is working as intended. Proceeding with tests."));

		TestAddingDoorsToRoom(*this);

		TestRemovingDoorsFromRoom(*this);

		TestSpawnMethodSuite(*this);
	}
	else
	{
		this->AddError(TEXT("Aborting Test: Door detection is broken, rendering future tests meaningless."));
	}

	return true;
}


#endif
