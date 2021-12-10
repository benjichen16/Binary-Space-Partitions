#include <stdio.h>
#include <math.h>
#include "Wall.h"
#include<assert.h>

point* point_create(int x, int y){
	point* p = (point*)malloc(sizeof(point));
	p->x = (double)x;
	p->y = (double)y;
	return p;
}

segment* segment_create(point* start, point* end){
	segment* wall = (segment*)malloc(sizeof(segment));
	wall->start = start;
	wall->end = end;
	wall->orientation = GetOrientation(wall);
	return wall;
}

enum WallOrientation GetOrientation(segment* wall){
	if (wall->start->x == wall->end->x){
		return VERTICAL;
	} else {
		return HORIZONTAL;
	}
}

// determine a wall located on which side of the partition segment
enum ESegmentWallSide ClassifySegment(segment PartitionSeg, segment wall)
{
   if(PartitionSeg.orientation == VERTICAL){ //classifies the Partitions according to different their different points
			if(PartitionSeg.start->x > wall.end->x && PartitionSeg.start->x > wall.start->x){
				return SEGMENT_WALL_SIDE_FRONT;
			}
			else if(PartitionSeg.start -> x == wall.start->x){
				if(PartitionSeg.start->x >= wall.end->x){
					return SEGMENT_WALL_SIDE_FRONT;
				}
				else if(PartitionSeg.start->x < wall.end->x){
					return SEGMENT_WALL_SIDE_BEHIND;
				}
			}
			else if(PartitionSeg.start->x == wall.end->x){
				if(PartitionSeg.start ->x >= wall.start->x){
					return SEGMENT_WALL_SIDE_FRONT;
				}
				else if(PartitionSeg.start->x < wall.start->x){
					return SEGMENT_WALL_SIDE_BEHIND;
				}
			}
			else if(PartitionSeg.start->x < wall.start->x && PartitionSeg.start->x < wall.end ->x){
				return SEGMENT_WALL_SIDE_BEHIND;
			}
			else{
				return SEGMENT_WALL_SIDE_STRADDLING;
			}
   }
   else{
			if(PartitionSeg.start->y > wall.end->y && PartitionSeg.start->y > wall.start->y){
				return SEGMENT_WALL_SIDE_FRONT;
			}
			else if(PartitionSeg.start ->y == wall.start->y){
				if(PartitionSeg.start ->y >= wall.end->y){
					return SEGMENT_WALL_SIDE_FRONT;
				}
				else if(PartitionSeg.start ->y < wall.end->y){
					return SEGMENT_WALL_SIDE_BEHIND;
				}
			}
			else if(PartitionSeg.start->y == wall.end->y){
				if(PartitionSeg.start ->y >= wall.start->y){
					return SEGMENT_WALL_SIDE_FRONT;
				}
				else if(PartitionSeg.start->y < wall.start->y){
					return SEGMENT_WALL_SIDE_BEHIND;
				}
			}
			else if(PartitionSeg.start->y < wall.start->y && PartitionSeg.start->y < wall.end ->y){
				return SEGMENT_WALL_SIDE_BEHIND;
			}
			else{
				return SEGMENT_WALL_SIDE_STRADDLING;
			}
   }
   return SEGMENT_WALL_SIDE_STRADDLING;	   
}

// compute the intersection point on the wall divided by the PartitionSeg
int IntersectionWalls(segment PartitionSeg, segment wall, point* out){
		if(PartitionSeg.orientation == VERTICAL){
			out->x = PartitionSeg.start->x;
			out->y = wall.start->y;
		}
		else{
			out->x = wall.start->x;
			out->y = PartitionSeg.start->y;
		}

	if(ClassifySegment(PartitionSeg, wall) == SEGMENT_WALL_SIDE_STRADDLING)
		return 1;
	return -1;
}


// Output the two segments of the original wall divided by the partition segment to frontSeg and backSeg
void SplitWallSegment(segment PartitionSeg, segment wall, segment *frontSeg, segment *backSeg)
{
	point* x = point_create(0,0);
	IntersectionWalls(PartitionSeg, wall, x); //calls intersection walls and has data put into x
	//printf("%d, %d\n", x->x, x->y);
	frontSeg -> start = wall.start;
	frontSeg ->end = x;
	backSeg -> start = x;
	backSeg -> end = wall.end;
	frontSeg->orientation = GetOrientation(frontSeg);
	backSeg->orientation = GetOrientation(backSeg);
}

