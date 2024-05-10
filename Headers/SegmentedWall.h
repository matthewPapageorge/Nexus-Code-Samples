/**
 * Author: Matthew Papageorge
 * Date: May 10, 2024
 * Description: Represents a wall composed of one or more adjacent segments. This class enables independent modification of each segment's static mesh.
 * Its primary use is to dynamically connect rooms at runtime by transforming specific segments into door meshes.
 * 
 * @warning Each segment must be identified with a numerical index tag, which represents its position within the wall. Counting begins at 0.
 * 
 * @note The decision to use tags for identifying specific wall segments, rather than determining them at runtime, was made for performance and simplicity.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SegmentedWall.generated.h"

UCLASS(ClassGroup = (Dungeon), meta = (BlueprintSpawnableComponent))
class ARPG_API USegmentedWall : public USceneComponent
{
	GENERATED_BODY()

public:
	USegmentedWall();

	using UWallSegment = UStaticMeshComponent;

	/**
	 * Returns true if there is a segment corresponding to the given index.
	 *
	 * @note The index is valid if it's 1 less than the total number of segments.
	 */
	bool IsValidSegmentIndex(int32 SegmentIndex);

	/**
	 * Returns the corresponding static mesh component to the provided index.
	 * 
	 * @warning An assertion is triggered if the SegmentIndex is invalid.
	 */
	UWallSegment* GetSegment(int32 SegmentIndex);

private:
	/** Contains the segment meshes sorted by index. */
	TArray<UStaticMeshComponent*> m_SegmentMeshes { }; 

	/**
         * Ensures that the internal array m_SegmentMeshes is properly initialized.
	 * 
	 * @note You do not need to call this function manually; it is handled by the engine.
	 */ 
	virtual void InitializeComponent() override;

	/** Initializes the array m_SegmentMeshes by populating it with the child components of this instance. */
	void InitializeSegmentMeshes();

	/** 
	 * Returns the index of the specified segment.
	 * This function is called during initialization.
	 */
	int32 GetIndexOfSegment(USceneComponent* Segment);
};
