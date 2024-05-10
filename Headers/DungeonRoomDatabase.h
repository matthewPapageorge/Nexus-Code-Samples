/**
 * 
 * Author: Matthew Papageorge
 * Date: May 10, 2024
 * Description: This class supports procedurally generating dungeons by providing information about the specifications of the available DungeonRoom assets.
 * The database supports querying for assets based on given specifications and can provide information about rooms of a specified theme, enabling the creation of thematic dungeons.
 * 
 */

#pragma once
#include "CoreMinimal.h"

#include "Dungeon/Enums/DungeonTheme.h"
#include "Dungeon/Structs/DungeonRoomSpecs.h"
#include "Dungeon/SizeTypes/NumberOfTiles.h"

class ARPG_API FDungeonRoomDatabase
{
public:
	/** Creates a database using the room assets inside of the given path. */
	FDungeonRoomDatabase(FName PathToAssets);

	/** Returns true if there is at least one asset in the database with the given specs. */
	bool DoesAssetExistWithSpecs(const FDungeonRoomSpecs& Specs) const;

	/**
	 * Provides paths to assets that have the given specs.
	 *
	 * @warning An assertion is triggered if there are no assets with the given specs; Can verfiy using DoesAssetExistWithSpecs().
	 * @note Uncooked assets resolve to UBlueprint; Cooked assets resolve to UBlueprintGeneratedClass.
	 */
	const TArray<FSoftObjectPath>& GetAssetPaths(const FDungeonRoomSpecs& RoomSpecs) const;

	/** Returns the maximum width of rooms with the given theme. */
	FNumberOfTiles GetMaxWidth(EDungeonTheme Theme) const;
	
	/** Returns the maximum length of rooms with the given theme. */
	FNumberOfTiles GetMaxLength(EDungeonTheme Theme) const;
	
	
private:
	TMap<FDungeonRoomSpecs, TArray<FSoftObjectPath>> m_PathsByRoomSpecs;		     // Maps room specs to paths to assets that have those specs

	TMap<EDungeonTheme, FNumberOfTiles>	         m_MaxWidthByTheme;		     // Maps theme to the maximum width of rooms with that theme
	 
	TMap<EDungeonTheme, FNumberOfTiles>	         m_MaxLengthByTheme;	             // Maps theme to the maximum length of rooms with that theme


	/** Adds every asset in the given path to the database */
	void InitializeDatabase(FName Path);

	/** Updates the mappings to include the provided asset */
	void AddAssetToDatabase(const FAssetData& Asset);

	/** Updates the max width and length maps to include the asset */
	void UpdateMaxWidthAndLengthMaps(const FAssetData& Asset);

	/** Updates the paths map to include the asset */
	void UpdateAssetPathsMap(const FAssetData& Asset);

};
