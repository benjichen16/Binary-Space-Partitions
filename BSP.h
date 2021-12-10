//BSP.h

#include "Wall.h"
#include "List.h"
#include<stdio.h>

typedef struct TreeNode {
	void* data;
	struct TreeNode* left;
	struct TreeNode* right; 
	struct TreeNode* parent;
}TreeNode;

typedef struct BSPTree{
	struct TreeNode* root;
	void* (*GetSubtreeRoot)(ListPtr data);
	ListPtr (*GetSubtrees)(void* parent, ListPtr remaining);
	int (*DataCompare)(void* data1, void* data2);
	void (*DataPrinter)(void* data);
	void (*FreeData)(void* data);
} BSPTree;
/**
 * @brief Creates a new BSP and using the provided data. This constructor calls BuildSubtree, which is a recursive function 
 *
 * @param data A ListPtr of the data that should be used to build the tree
 * @param GetPartitionRoot A function which delets a node from the list and returns the data
 * @param GetSubtrees Given a data value, modifies the provided list to contain the 'right' sublist and returns a pointer to the 'left' sub list
 * @param DataCompare A function for comparing two values, returns 0 if equal, >0 if obj1 is greater, and <0 if obj2 is greater
 * @param DataPrinter A function for printing out a piece of data in the list
 * @param FreeData A function used to free data from the BSP
 *
 * @return A pointer to the newly created and partitioned BSP
 */

BSPTree* BSP_Create(ListPtr data, void* (*GetSubtreeRoot)(ListPtr data), ListPtr (*GetSubtrees)(void* parent, ListPtr remaining), int (*DataCompare)(void* data1, void* data2), void (*DataPrinter)(void* data), void(*FreeData)(void* data));


/**
 * @brief Helper function to create a new node for the BSP
 *
 * @param data The data to be stored within the created node
 *
 * @return A pointer to the newly created TreeNode
 */ 
TreeNode* TreeNode_Create(void* data);

/**
 * @brief Recursive function that operates on a List to select a subtree root node and build that node's left and right subtrees
 *
 * @param tree A pointer to a BSPTree which contains functions to GetSubtreeRoot, and GetSubtrees
 * @param remaining A list pointer to the remaining elements not already in the BSP
 * @param parent The node which is parent to the returned TreeNode
 *
 * @return TreeNode* A pointer to the root of the generated subtree
 *
 */
TreeNode* BuildSubtree(BSPTree* tree, ListPtr remaining, TreeNode* parent);

/**
 * @brief A function which prints the contents of the BSP in using pre-order traversal
 *
 * @param tree A pointer to the tree to print
 */
void BSP_print(BSPTree* tree);

/**
 * @brief Returns a list containing all elements in the BSP, sorted using pre-order traversal
 *
 * @param tree The tree to build the list from
 *
 * @return ListPtr A pointer to a list which contains all elements sorted using pre-order traversal
 */
ListPtr BSP_GetTraversal(BSPTree* tree);

/**
 * @brief A recursive function which traverses the BSP and adds the data to the provided list
 *
 * @param list A pointer to a list. When the recursion ends, this list will contain all elements from the BSP
 * @param node A TreeNode* from a BSP
 */
void PreOrderTraverse(ListPtr list, TreeNode* node);

/**
 * @brief
 *
 * @param tree A pointer to the tree to delete
 * @param freeData A bool indicating whether the underlying data should be freed
 */ 
void DestroyTree(BSPTree* tree, bool freeData);

