/**
 *
 * Author: Matthew Papageorge
 * Date: 05/10/24
 * Description: This class represents a Dungeon in the game scene, containing interconnected rooms.
 *
 */

#pragma once
#include "Dungeon/Rooms/DungeonRoom.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dungeon.generated.h"

UCLASS()
class ARPG_API ADungeon : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_RootComponent;      // Top level component used for scene organization; All rooms are children of the root

	TArray<ADungeonRoom*> m_RoomsArray{ }; // Contains every room in the dungeon

public:	
	ADungeon();
	/**
    	 * Attaches the provided room to the dungeon.
     	 */
	void AddRoom(ADungeonRoom* Room);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
