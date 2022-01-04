#ifndef AVL_H
#define AVL_H

// DO NOT ADD OR REMOVE ANYTHING FROM THIS FILE

typedef enum Position {LEFT, RIGHT} Position;

typedef struct AVLNode * AVLNodePtr;

// Use only this struct definition.
typedef struct AVLNode{
    AVLNodePtr child[2]; // child[0] is left, child[1] is right.
    int key, height;
} AVLNode;


// Utilities

// return the maximum of x and y.
int _max( int x, int y );

// return the absolute value of x.
int _abs( int x );

// AVL Operations and Queries

// search for a node with key <x> in the and return the node if exists. Otherwise returns NULL.
AVLNodePtr avl_search( AVLNodePtr root, int x );

// insert a new node with key <x> into the subtree. If the key exists do nothing. Returns a pointer to root of the tree.
AVLNodePtr avl_insert( AVLNodePtr root, int x );

// delete the node with key <x> (if exists). Returns a pointer to the root of the tree.
AVLNodePtr avl_delete( AVLNodePtr root, int x );

// given roots of AVL trees T1 and T2 (root1 and root2 resp.), and a joint node <xnode> 
// such that keys(T1)<key(xnode)<keys(T2), avl_join joins all the three into a single AVL tree. 
// Returns a pointer to the root of the newly formed tree.
AVLNodePtr avl_join( AVLNodePtr root1, AVLNodePtr xnode, AVLNodePtr root2 );

// Split an AVL tree rooted at <root>, according to a key <x> that also exists in the tree.
// The output is two AVL trees T0 and T1 such that keys(T0)<x<keys(T1), and also the node with key x.
// The node of x is returned by the function and the roots of T0 and T1 are placed in 
// trees_out[0] and trees_out[1] respectively.
AVLNodePtr avl_split( AVLNodePtr root, int x, AVLNodePtr trees_out[2] );

// return a new initialized avl node with key <x>. Returns NULL if malloc fails.
AVLNodePtr new_avl_node( int x );

// Free all nodes in the tree.
void delete_avl_tree( AVLNodePtr root );


#endif
