#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

// AVL Tree Implementation Created On 1/6/2020 By:-
/****
  Student1 name: Yaseen Abd-Elhaleem
  Student2 name: Giries Abo-Ayoub
****/

/*The algorithms implemented here for the insertion, removal, rotations, split and join
  are similar to that which we learned in the course*/

//Helper Functions
int _hight(AVLNodePtr node);
AVLNodePtr avl_deleteHelpFun(AVLNodePtr root);
AVLNodePtr avl_rotation(AVLNodePtr node);
AVLNodePtr avl_singleRotation(AVLNodePtr node, int sideHeavy);
AVLNodePtr avl_doubleRotation(AVLNodePtr node, int sideHeavy);
AVLNodePtr findmin(AVLNodePtr node);


// Do not change this. You can use but do not touch.
int _max(int x, int y)
{
    return (x < y) ? y : x;
}

// Do not change this. You can use but do not touch.
int _abs(int x)
{
    return (x < 0) ? -x : x;
}

// Operations implementation

//gets height of the node O(1) time
int _hight(AVLNodePtr node)
{
    if (node) return node->height;
    return -1;
}

//search for a specific node in the tree O(log n) time complexity
AVLNodePtr avl_search(AVLNodePtr root, int x)
{
    if (root == NULL) return NULL;

    //use the property of a BST
    if (x > root->key)
        return avl_search(root->child[RIGHT], x);
    else if (x < root->key)
        return avl_search(root->child[LEFT], x);

    if (x == root->key) return root;

    return NULL;

}

// insert a node to the tree O(log n) time complexity
AVLNodePtr avl_insert(AVLNodePtr root, int x)
{
    //tree is empty / leaf
    if (root == NULL) return new_avl_node(x); 

    //search the right place to add the node
    if (x < root->key)
        root->child[LEFT] = avl_insert(root->child[LEFT], x);

    else if (x > root->key)
        root->child[RIGHT] = avl_insert(root->child[RIGHT], x);

    root = avl_rotation(root);
    root->height = _max(_hight(root->child[LEFT]), _hight(root->child[RIGHT])) + 1; //update height

    return root; //duplicate nodes are not allowed

}

//rotates the node if needed O(1) time
AVLNodePtr avl_rotation(AVLNodePtr node)
{
    int i;

    //if there is no node or it's a leaf
    if ((node == NULL) || (node->child[RIGHT] == NULL && node->child[LEFT] == NULL)) return node;

    /*check if the tree suffices the AVL property 
    (if there is a side heavier than the other and how many rotations needed) and fix it if needed */
    for (i = 0; i < 2; i++)
    {
        if (_hight(node->child[i]) - _hight(node->child[!i]) == 2)
        {
            if ((node->child[i] != NULL) && (_hight(node->child[i]->child[i]) < _hight(node->child[i]->child[!i])))
                node = avl_doubleRotation(node, i);
            else node = avl_singleRotation(node, i);
        }
    }

    return node;

}

//preform a single rotation O(1)
AVLNodePtr avl_singleRotation(AVLNodePtr node, int sideHeavy)
{
    if (!(node->child[sideHeavy]))return NULL;
    AVLNodePtr y = node->child[sideHeavy];
    node->child[sideHeavy] = y->child[!sideHeavy];
    y->child[!sideHeavy] = node;

    node->height = _max(_hight(node->child[LEFT]), _hight(node->child[RIGHT])) + 1;
    y->height = _max(_hight(node->child[LEFT]), _hight(node->child[RIGHT])) + 1;

    return y;

}

//preform 2 single rotations O(1) time
AVLNodePtr avl_doubleRotation(AVLNodePtr node, int sideHeavy)
{
    if (!(node->child[sideHeavy]))return NULL;
    node->child[sideHeavy] = avl_singleRotation(node->child[sideHeavy], !sideHeavy);
    return  avl_singleRotation(node, sideHeavy);


}

//deletes a given node from the tree in O(log n) time
AVLNodePtr avl_delete(AVLNodePtr root, int x)
{

    if (root == NULL) return NULL;

    //search the node
    if (x < root->key)
    {
        root->child[LEFT] = avl_delete(root->child[LEFT], x);
    }

    if (x > root->key)
    {
        root->child[RIGHT] = avl_delete(root->child[RIGHT], x);
    }

    if (x == root->key)
    {
        //node found, therfore switch it with the minimum node of the right subtree (successor) then delete it
        if ((root->child[LEFT]) && (root->child[RIGHT]))
        {
            AVLNodePtr temp = findmin(root->child[RIGHT]);
            root->key = temp->key;
            temp->key = x;
            root->child[RIGHT] = avl_delete(root->child[RIGHT], x);
        }
        else  return avl_deleteHelpFun(root);
    }

    //check if rotations are needed
    root = avl_rotation(root);
    root->height = _max(_hight(root->child[LEFT]), _hight(root->child[RIGHT])) + 1;
    return root;

}

//return the minimum of the tree O(log n) time
AVLNodePtr findmin(AVLNodePtr node)
{
    if (node == NULL) return NULL;
    if (node->child[LEFT] == NULL) return node;
    return findmin((node->child[LEFT]));
}

/* Handles the case when the node has one child or no children at all,
frees the memory of the node and returns the child if existed */ 
AVLNodePtr avl_deleteHelpFun(AVLNodePtr root)
{
    AVLNodePtr temp=NULL;

    //no children
    if (root->child[LEFT] == NULL && root->child[RIGHT] == NULL)
    {
        free(root);
        return NULL;
    }

    //one child
    if (root->child[LEFT] == NULL)
    {
        temp = root->child[RIGHT];
    }
    else if (root->child[RIGHT] == NULL)
    {
        temp = root->child[LEFT];
    }

    free(root);
    return temp;
}

//joins 2 trees with a given node O(log n) time
AVLNodePtr avl_join(AVLNodePtr root1, AVLNodePtr xnode, AVLNodePtr root2)

{   /*Check which tree is higher, if they're not the same height,
    go all the way down until you have their heights equal,
    then rotations need to be done in order to keep the AVL property*/
    if (_hight(root1) < _hight(root2))
    {
        root2->child[LEFT] = avl_join(root1, xnode, root2->child[LEFT]);
        root2 = avl_rotation(root2);
        root2->height = _max(_hight(root2->child[LEFT]), _hight(root2->child[RIGHT])) + 1;

        return root2;

    }

    else if (_hight(root1) > _hight(root2))
    {
        root1->child[RIGHT] = avl_join(root1->child[RIGHT], xnode, root2);
        root1 = avl_rotation(root1);
        root1->height = _max(_hight(root1->child[LEFT]), _hight(root1->child[RIGHT])) + 1;

        return root1;

    }

    /* The base case - the two trees have the same height,
    let the first be x's left child and the second x's right child */
    xnode->child[RIGHT] = root2;
    xnode->child[LEFT] = root1;
    xnode = avl_rotation(xnode);
    xnode->height = _hight(root1) + 1;
    return xnode;
}

/* Splits the tree into two trees, one with all keys 
greater than x and another with all keys smaller than x */
AVLNodePtr avl_split(AVLNodePtr root, int x, AVLNodePtr trees_out[2])
{

    if (root == NULL)
    {
        trees_out[LEFT] = trees_out[RIGHT] = NULL;
        return NULL;
    }

    /* If the key is greater than x then split the left subtree 
    and join the right one with all those who are greater than x */
    else  if (x < root->key)
    {
        AVLNodePtr temp = avl_split(root->child[LEFT], x, trees_out);
        trees_out[RIGHT] = avl_join(trees_out[RIGHT], root, root->child[RIGHT]);
        return temp;
    }

     /* If the key is smaller than x then split the right subtree
     and join the left one with all those who are smaller than x */
    else if (x > root->key)
    {
        AVLNodePtr temp = avl_split(root->child[RIGHT], x, trees_out);
        trees_out[LEFT] = avl_join(root->child[LEFT], root, trees_out[LEFT]);
        return temp;
    }

    /* The base case - we are in the x node,
    then return it's right child as the greater tree,
    his left child as the smaller tree and return x itself */
    else
    {

        trees_out[LEFT] = root->child[LEFT];
        trees_out[RIGHT] = root->child[RIGHT];
        root->child[RIGHT] = root->child[LEFT] = NULL;
        return root;
    }

}

//allocates memory for a node
AVLNodePtr new_avl_node(int x)
{

    AVLNodePtr node = (AVLNodePtr)malloc(sizeof(AVLNode));
    node->key = x;
    node->height = 0;
    node->child[LEFT] = NULL;
    node->child[RIGHT] = NULL;
    return node;

}

//traverses the tree PostOrder and frees the allocated memory of each node O(n) time
void delete_avl_tree(AVLNodePtr root)
{

    if (root == NULL) return;
    delete_avl_tree(root->child[LEFT]);
    delete_avl_tree(root->child[RIGHT]);
    free(root);
    root = NULL;
}
