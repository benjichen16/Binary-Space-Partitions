
/*
Benjamin Chen
Prog 4
Implementation of Room.c
*/
#include <cairo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Wall.h"
#include "List.h"
#include "BSP.h"
#include <math.h>
#include <limits.h>

int isOnWall(BSPTree* tree, point door, TreeNode* node);
// Two line segments are equal if they start and end at the same point
// Returns 0 if segments are equal, -1 otherwise
int SegmentsEqual(void* segment1, void* segment2){
	segment* seg1 = (segment*)segment1;
	segment* seg2 = (segment*)segment2;
	if((seg1->start == seg2->start && seg1->end == seg2->end) || (seg1->start == seg2->end && seg1->end == seg2->start)){
		return 0;
	}
	return -1;
}

//print out the coordinates of the segment, remember to use %d for integers
void PrintSegment(void* segment1){
	segment* seg1 = (segment*)segment1;
	printf("%d %d %d %d\n", seg1->start->x, seg1->start->y, seg1->end->x, seg1->end->y);
}

// remember to free the start and end points of the segment
void DestroySegment(segment* seg){
	free(seg->start);
	free(seg->end);
	free(seg);
}

/**
* @brief Takes a parent node and generates a list containing all elements that form the left subtree of parent
* Note: The original list should be modified so that it contains only elements that form the right subtree of parent
* Assumes that parent is not in original
*
* @param parent The parent node which elements in the list argument will be compared with to determine left/right relationship
* @param original A pointer to a list containing all children of the parent. Original is modified to contain all right children
*
* @return ListPtr pointing to a list which holds all elements which will form the left subtree of parent. Returns NULL if parent or orignal
* are null
*/
ListPtr GetPartitions(void* parent, ListPtr remaining){
	//GetSubTrees
	ListPtr left = list_create(SegmentsEqual, PrintSegment, DestroySegment);
	int counter = 0;
	TreeNode* node = (TreeNode*)parent;
	segment* curr2 = (segment*)node->data;
	while(list_get_index(remaining, counter) != NULL){
		segment* curr = (segment*)(list_get_index(remaining, counter));
		//PrintSegment(curr);
		//printf("Comparison Node: ");
		//PrintSegment(curr2);
		if(ClassifySegment(*curr2, *curr) == SEGMENT_WALL_SIDE_FRONT){
			segment* seg = (segment*)list_del_index(remaining, counter);

			list_append(left, seg);
		}
		else if(ClassifySegment(*curr2, *curr) == SEGMENT_WALL_SIDE_BEHIND){
			counter++;
		}
		else {
			//printf("Straddle\n");
			segment* seg1 = (segment*)malloc(sizeof(segment));//allocates mem for new segments
			segment* seg2 = (segment*)malloc(sizeof(segment));
			SplitWallSegment(*curr2, *curr, seg1, seg2);
			//PrintSegment(curr);
			list_del_index(remaining, counter);//deletes old segments
			if(ClassifySegment(*curr2, *seg1) == SEGMENT_WALL_SIDE_FRONT){
				list_append(left, seg1);
				list_insert(remaining, counter, seg2);
			}
			else{
				list_append(left, seg2);
				list_insert(remaining, counter, seg1);
			}
			counter++;
		}
	}
	return left;
}

/**
 * @brief used to select the data which will form the root of a new subtree
 *
 * @param remaining A listpointer containing all the remaining data, after execution it should not contain the removed value
 *
 * @return void* A pointer to the extracted data
 */ 
void* SelectPartitionRoot(ListPtr remaining){
	//GetSubtreeRoot
	int index = floor((list_length(remaining)/2.0) - 1);
	if(index < 0){//if index is negative, bring it back to zero
		index = 0;
	}
	return list_del_index(remaining, index);
}

/**
 * BONUS POINT CASE.
 * Determines which wall the Fancy Door is located on using bsp tree
 * The fancy door is a segment with given endpoints
 *
 * @param tree The BSP tree you built with the provided rooms
 * @param door A segment to be found
 * @param Wall It will store the wall contains the door here.
 *
 * @return int Returns -1 if the door outside the room (as long as part of the door is outside the room), 
 *						0 if the door inside the room (including the door touch the walls), 
 *						1 if the door on the walls
*/ 
int FindFancyDoor(BSPTree* tree, segment fancydoor, segment* Wall);

/**
 * Determines which wall the door is located on using bsp tree
 * The door is a point with given coordinates
 *
 * @param tree The BSP tree that you built with the provided rooms
 * @param door A coordinate point to be found
 * @param Wall It will store the wall contains the door here.
 *
 * @return int Returns -1 if the door outside the room, 0 if the door inside the room, 1 if the door on the walls
 */
int FindDoor(BSPTree* tree, point door, segment* Wall){
		ListPtr list = BSP_GetTraversal(tree);
		int counter = 0;
		//BSP_print(tree);
		while(list_get_index(list, counter) != NULL){
			segment* s = (segment*)list_get_index(list, counter);
		if(s -> orientation == VERTICAL){
			if((s->start->x == door.x) && ((s->end->y >= door.y && door.y >= s->start->y) ||(s->end->y <= door.y && s->start->y >= door.y))){
				//printf("veri\n");
				Wall->start ->x = s->start -> x;//sets wall node to curr node to be returned
				Wall->start ->y = s->start->y;
				Wall->end ->x = s->end -> x;
				Wall->end ->y = s->end ->y;
				return 1;
			}
		}
		if(s -> orientation == HORIZONTAL){
			if((s->start->y == door.y) && ((s->end->x >= door.x && s->start->x <= door.x) ||(s->start->x >= door.x && door.x >= s->end->x))){
				//printf("hori\n");
				Wall->start->x = s->start->x;
				Wall->start->y = s->start->y;
				Wall->end->x = s->end->x;
				Wall->end->y = s->end->y;
				return 1;
			}
		}
			counter++;
		}
		int num = 0;
		counter = 0;
		point* x = (point*) malloc(sizeof(point));
		x->x = INT_MAX;
		x->y = door.y;
		segment* seg = segment_create(&door, x);
		while(list_get_index(list, counter) != NULL){
			segment* seg2 = (segment*) list_get_index(list, counter);
			if((seg2->start->x > seg->start->x) && ((seg2->start->y > seg->start->y && seg2->end->y < seg->end->y) || (seg2->end->y > seg->start->y && seg2->start->y <= seg->start->y))){
				num++;
			}
			counter++;
		}
	num++;
	if(num % 2 != 0){
		return 0;
	}
	if(num % 2 == 0){ //if number of intersections mod 2 is 0, that means it wasnt in the polygon.
		return -1;
	}
}

/**
 * @brief Used to draw out our BSP (room)
 *
 * @param tree A pointer to the tree containing the room
 * @param filename The name of the outputfile, DrawBSP concatenates with ".png"
 * @param doors A list of doors (points) that will be drawn to the cairo canvas in red
 * @param fancyDoors A list of fancyDoors (segments) that will be drawn to the cairo canvas in yellow
 *
 */
void DrawBSP(BSPTree* tree, char* filename, ListPtr doors, ListPtr fancyDoors){
	// Get a list of our BSP data sorted by pre-order traversal
	ListPtr traversalList = BSP_GetTraversal(tree);
	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 512, 512);
	cairo_t *cr = cairo_create (surface);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_rectangle(cr, 0, 0, 512, 512);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_set_line_width(cr, 5.0);
	cairo_set_font_size(cr, 32);
	char label[10];
	for(int i = 0; i < list_length(traversalList); i++){
		segment* seg = (segment*)list_get_index(traversalList, i);
		//draw the line
		cairo_move_to(cr, seg->start->x, seg->start->y);
		cairo_line_to(cr, seg->end->x, seg->end->y);
		cairo_stroke(cr);
		int midX = (seg->start->x + seg->end->x)/2;
		int midY = (seg->start->y + seg->end->y)/2;
		cairo_move_to(cr, midX, midY);
		sprintf(label, "%d", i);
		cairo_show_text(cr, label);
	}
	
	if(doors != NULL){
		cairo_close_path(cr);
		cairo_set_source_rgb(cr, 1, 0, 0);
		for(int i = 0; i < list_length(doors); i++){
			point* d = (point*)list_get_index(doors, i);
			cairo_rectangle(cr, (d->x) - 5, (d->y) - 5, 10, 10);
			cairo_stroke(cr);
		}
	}

	if(fancyDoors != NULL){
		cairo_set_source_rgb(cr, 1, 1, 0);
		for(int i = 0; i < list_length(fancyDoors); i++){
			segment* seg = (segment*)list_get_index(fancyDoors, i);
			//draw the line
			cairo_move_to(cr, seg->start->x, seg->start->y);
			cairo_line_to(cr, seg->end->x, seg->end->y);
			cairo_close_path(cr);
			cairo_stroke(cr);
		}
	}

	cairo_destroy(cr);
	char* outputName = strcat(filename, ".png");
	cairo_surface_write_to_png(surface, outputName);
	cairo_surface_destroy(surface);
}
int main (int argc, char **argv){
	FILE *file;
	FILE *file1;

	file = fopen(argv[1], "r");

	char s[1000];
	char first[100];
	bool firstLine = true;
	char *nums;
	char x[100];

	int x1,x2,y1,y2;

	ListPtr walls = list_create(SegmentsEqual, PrintSegment, DestroySegment);
	while(fgets(s, 1000, file)!= NULL){
		if(firstLine){ //saves first line
			strcpy(first, s);
			firstLine = false;
		}
		else{ //gets rest of file
			nums = strtok(s, " ");
			while(nums!= NULL){
				strcpy(x, nums);
				x1 = atoi(x);
				nums = strtok(NULL, " ");
				strcpy(x, nums);
				y1 = atoi(x);
				nums = strtok(NULL, " ");
				strcpy(x, nums);
				x2 = atoi(x);
				nums = strtok(NULL, " ");
				strcpy(x, nums);
				y2 = atoi(x);
				//printf("%i %i %i %i\n", x1, y1, x2, y2);
				point* start = point_create(x1, y1);
				point* end = point_create(x2, y2);
				segment* seg = segment_create(start, end);
				list_append(walls, seg);
				nums = strtok(NULL, " ");
			}
		}
	}
	//list_print(walls);
	BSPTree* tree = BSP_Create(walls, SelectPartitionRoot, GetPartitions, SegmentsEqual, PrintSegment, DestroySegment);
	//DrawBSP(tree, first);
	//BSP_print(tree);
	ListPtr doors = list_create(SegmentsEqual,PrintSegment, DestroySegment);
	char number[10];
	firstLine = true;
	file = fopen(argv[2], "r");
	first[strlen(first)-1] = '\0';
	char title[50];
	strcpy(title, first);
	strcat(first,".out");
	file1 = fopen(first,"w");
	int inRoom;
	point* p1 = point_create(0,0);
	point* p2 = point_create(0,0);
	segment* seg = segment_create(p1,p2);
	while(fgets(s, 100, file) != NULL){ //gets input from second file
		if(firstLine){
			strcpy(number, s);
			firstLine = false; //saves first line
		}
		else{
			nums = strtok(s, " ");
			while(nums!= NULL){
				strcpy(x, nums);
				x1 = atoi(x);
				nums = strtok(NULL, " ");
				strcpy(x, nums);
				x2 = atoi(x);
				nums = strtok(NULL, " ");
				
				point* x = point_create(x1, x2);
				list_append(doors, x);
				inRoom = FindDoor(tree, *x, seg);
				if(inRoom == 1){
					//fprintf(file1,"%d %d %d %d\n",seg->start->x, seg->start->y, seg->end->x, seg->end->y); //writes to text file
					tree->DataPrinter(seg);
				}
				if(inRoom == 0){
					//fprintf(file1,"OUT\n");
					printf("OUT\n");
				}
				if(inRoom == -1){
					//fprintf(file1,"IN\n");
					printf("IN\n");
				}
			}

		}
	}
	ListPtr lp = BSP_GetTraversal(tree);
	//BSP_print(tree);
	DrawBSP(tree, title, doors,  NULL);
	fclose(file);
	fclose(file1);
}