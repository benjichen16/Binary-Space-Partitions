#ifndef list_h
#define list_h
#include <stdio.h>
#include <stdlib.h>

enum WallOrientation{
	HORIZONTAL,
	VERTICAL
};

typedef struct point {
	int x;
	int y;
}point;

typedef struct segment
{
	point* start;
	point* end;
	enum WallOrientation orientation;
}segment;


// The location of walls of the partition segment
// e.g. a wall being above the partitioning segment means that Wall->y  <= PartitionSeg->y.
// Since we are using cairo and for cairo y increases downward
enum ESegmentWallSide
{
    SEGMENT_WALL_SIDE_FRONT,        // Wall in front of or in the partition segment, to become part of the left subtree
    SEGMENT_WALL_SIDE_BEHIND,       // Wall behind the the partition segment, to become part of the right subtree
    SEGMENT_WALL_SIDE_STRADDLING,   // Wall intersect with the partition segment
};

/**
 * brief Creates a point and returns a pointer to it
 *
 * @param x The x coordinate for the point
 * @param y The y coordinate for the point
 *
 * @return point* A pointer to the created point
 */
point* point_create(int x, int y);

/**
 *brief Creates a segment and returns a pointer to it Hint: make sure to store orientation
 *
 * @param start The starting point for the line segment
 * @param end The end point for the line segment
 *
 * @return segment* Returns a pointer to a fully initialized line segment
 */
segment* segment_create(point* start, point* end);

enum WallOrientation GetOrientation(segment* wall);

// determine a wall located on which side of the partition segment
enum ESegmentWallSide ClassifySegment(segment PartitionSeg, segment wall);

/**
 * brief Determines the intersection point on the wall divided by the PartitionSeg
 *
 * @param PartitionSeg The partition segment involved in the potential intersection
 * @param wall A wall segment to be divided involved in the potential intersection
 *
 * @return int Returns -1 if the walls do not intersect, 1 otherwise
 */
int IntersectionWalls(segment PartitionSeg, segment wall, point* out);

// Output the two segments of the original wall divided by the partition segment to frontSeg and backSeg
void SplitWallSegment(segment PartitionSeg, segment wall, segment* frontSeg, segment* backSeg);




#endif