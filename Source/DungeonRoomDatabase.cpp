#include "DungeonRoomDatabase.h"

#include "DungeonRoom.h"
#include "Dungeon/SizeTypes/NumberOfTiles.h"
#include "Dungeon/Enums/DungeonTheme.h"
#include "Dungeon/Structs/DungeonRoomSpecs.h"
#include "System/AssetSearcher.h"			
#include "System/DungeonRoomAssetAnalyzer.h" 
#include "Algo/ForEach.h"

FDungeonRoomDatabase::FDungeonRoomDatabase(FName PathToAssets)
{
	InitializeDatabase(PathToAssets);
}

void FDungeonRoomDatabase::InitializeDatabase(FName PathToAssets)
{
	TArray<FAssetData> RoomAssets { FAssetSearcher::FindDerivedBlueprintAssetsInPath(PathToAssets, ADungeonRoom::StaticClass()) };
	checkf(RoomAssets.Num() > 0, TEXT("Error: No DungeonRoomAssets were found in the designated path: %s"), *PathToAssets.ToString());

	Algo::ForEach(RoomAssets, [this](const FAssetData& Asset) { 
		this->AddAssetToDatabase(Asset);
	});
}

void FDungeonRoomDatabase::AddAssetToDatabase(const FAssetData& Asset)
{
	UpdateAssetPathsMap(Asset);
	
	UpdateMaxWidthAndLengthMaps(Asset);
}

void FDungeonRoomDatabase::UpdateAssetPathsMap(const FAssetData& Asset)
{
	FSoftObjectPath PathToAsset{ FDungeonRoomAssetAnalyzer::GetSoftObjectPath(Asset) };

	FDungeonRoomSpecs RoomSpecs{ FDungeonRoomAssetAnalyzer::GetRoomSpecs(Asset) };

	TArray<FSoftObjectPath>& AssetPaths{ m_PathsByRoomSpecs.FindOrAdd(RoomSpecs) };
	AssetPaths.Add(PathToAsset);
}

void FDungeonRoomDatabase::UpdateMaxWidthAndLengthMaps(const FAssetData& Asset)
{
	FDungeonRoomSpecs RoomSpecs{ FDungeonRoomAssetAnalyzer::GetRoomSpecs(Asset) };
	
	FNumberOfTiles& MaxWidth{ m_MaxWidthByTheme.FindOrAdd(RoomSpecs.Theme) };
	if (RoomSpecs.Dimensions.Width > MaxWidth)
	{
		MaxWidth = RoomSpecs.Dimensions.Width;
	}

	FNumberOfTiles& MaxLength{ m_MaxLengthByTheme.FindOrAdd(RoomSpecs.Theme) };
	if (RoomSpecs.Dimensions.Length > MaxLength)
	{
		MaxLength = RoomSpecs.Dimensions.Length;
	}
}

const TArray<FSoftObjectPath>& FDungeonRoomDatabase::GetAssetPaths(const FDungeonRoomSpecs& RoomSpecs) const
{
	return m_PathsByRoomSpecs.FindChecked(RoomSpecs);
}

FNumberOfTiles FDungeonRoomDatabase::GetMaxWidth(EDungeonTheme Theme) const
{
	return m_MaxWidthByTheme.FindChecked(Theme);
}

FNumberOfTiles FDungeonRoomDatabase::GetMaxLength(EDungeonTheme Theme) const
{
	return m_MaxLengthByTheme.FindChecked(Theme);
}

bool FDungeonRoomDatabase::DoesAssetExistWithSpecs(const FDungeonRoomSpecs& Specs) const
{
	checkf(Specs.Dimensions.Width > 0 && Specs.Dimensions.Length > 0, 
		TEXT("Error: Checked if a room asset exists with an invalid room size"));

	return m_PathsByRoomSpecs.Contains(Specs);
}



