/*
Benjamin Chen
BSP Tree
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "BSP.h"
#include "Wall.h"
#include "List.h"

BSPTree* BSP_Create(ListPtr data, void* (*GetSubtreeRoot)(ListPtr data), ListPtr (*GetSubtrees)(void* parent, ListPtr remaining), int (*DataCompare)(void* data1, void* data2), void (*DataPrinter)(void* data), void (*FreeData)(void *data)){
	BSPTree* tree = (BSPTree*)malloc(sizeof(BSPTree));//creates BSPTree from parameteres
    tree ->GetSubtreeRoot = GetSubtreeRoot;
    tree ->GetSubtrees = GetSubtrees;
    tree ->DataCompare = DataCompare;
    tree ->DataPrinter = DataPrinter;
    tree ->FreeData = FreeData;
    tree->root = BuildSubtree(tree, data, tree->root); //calls BuildSubtree and BuildSubtree returns the root
    return tree;
}

TreeNode* TreeNode_Create(void* data){
	TreeNode* tree = (TreeNode*)malloc(sizeof(TreeNode));
    tree->data = data;//stores data in data compartment
    tree->left = NULL;//instantiates all the nodes
    tree->right = NULL;
    tree->parent = NULL;
    return tree;
}

TreeNode* BuildSubtree(BSPTree* tree, ListPtr remaining, TreeNode* parent){
    if(list_length(remaining) == 0){
        return NULL;
    }
    else{
        TreeNode* root = TreeNode_Create(tree->GetSubtreeRoot(remaining));
        root->parent = parent;
        ListPtr left = tree->GetSubtrees(root,remaining);
        //list_print(remaining);
        root ->left = BuildSubtree(tree, left, root); //recursively calls the left and right tree of the node
        root ->right = BuildSubtree(tree,remaining,root); 
        return root;
    }
}

void BSP_print(BSPTree* tree){
	ListPtr l = list_create(tree->DataCompare, tree->DataPrinter, tree->FreeData); //creates list from methods
    PreOrderTraverse(l, tree->root); //adds elements from tree into list
    list_print(l); //prints the elements in the list
}	

ListPtr BSP_GetTraversal(BSPTree* tree){
	ListPtr l = list_create(tree->DataCompare, tree->DataPrinter, tree->FreeData); //creates list from methods
    PreOrderTraverse(l, tree->root); //adds elements into list from tree
    return l;
}

void PreOrderTraverse(ListPtr list, TreeNode* node){
	if(node == NULL){
        return;
    }
    list_append(list, node->data);
    PreOrderTraverse(list, node ->left);//goes down tree from the left side
    PreOrderTraverse(list, node ->right);//goes down tree from the right side
}