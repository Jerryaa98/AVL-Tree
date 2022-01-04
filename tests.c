#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {FAILED,PASSED} TestResult;

void print_result( TestResult result );

// insert all integers in [x,y] to the tree.
AVLNodePtr insert_range( AVLNodePtr root, int x, int y );

// search for all integers in [x,y]. Return FAILED if at least one search has failed.
TestResult search_range( AVLNodePtr root, int x, int y );

// delete all integers in [x,y] from teh tree.
AVLNodePtr delete_range( AVLNodePtr root, int x, int y);

// check if the tree has the AVL property in every node. If not, *result is assigned FAILED.
int avl_property_test( AVLNodePtr root, TestResult * result );

TestResult search_insert_test();

TestResult delete_test();

TestResult join_test();

TestResult split_test();

int avl_property_test( AVLNodePtr root, TestResult * result ){
    int h_left,h_right;
    if( !root )
        return -1;
    h_left = avl_property_test( root->child[LEFT], result );
    h_right = avl_property_test( root->child[RIGHT], result );
    *result =  (_abs(h_left-h_right) > 1 ) ? FAILED:*result;
    return 1+_max(h_left,h_right);
}

AVLNodePtr insert_range( AVLNodePtr root, int x, int y ){
    for( int i=x; i<=y; i++ )
        root = avl_insert( root, i );
    return root;
}

TestResult search_range( AVLNodePtr root, int x, int y ){
    AVLNodePtr node=NULL;
    for( int i=x; i<=y; ++i ){
        node = avl_search( root, i );
        if( !(node && node->key==i) ){
            return FAILED;
        }
    }
    return PASSED;
}

AVLNodePtr delete_range( AVLNodePtr root, int x, int y){
    for( int i=x; i<=y; i++ )
        root = avl_delete( root, i );
    return root;
}

void print_result( TestResult result ){
    if( result==PASSED )
        printf("PASSED.\n");
    else
        printf("FAILED.\n");
}

TestResult search_insert_test(){
    AVLNodePtr root = NULL;
    TestResult result = PASSED;
    root = insert_range( root, 1000000, 2000000 );
    result = search_range( root, 1000000, 2000000 );
    avl_property_test( root, &result );
    printf("INSERT + SEARCH TEST ");
    print_result( result );
    delete_avl_tree( root );
    return result;
}

TestResult delete_test(){
    int i;
    AVLNodePtr root = NULL;
    TestResult result = PASSED;
    root = insert_range( root, 1000000, 2000000 );
    root = delete_range( root, 1100000, 1500000 );
    result = search_range( root, 1000000, 1099999 );
    result = search_range( root, 1500001, 2000000 )?result:FAILED;
    for( i=1100000; i<=1500000; i++){
        if( avl_search(root,i) )
            result = FAILED;
    }
    avl_property_test( root, &result );
    printf("DELETE TEST ");
    print_result( result );
    delete_avl_tree( root );
    return result;
}

TestResult join_test(){
    AVLNodePtr t1=NULL,t2=NULL;
    AVLNodePtr root=NULL;
    TestResult result=PASSED;
    t1 = insert_range( t1, 2, 1000000 );
    t2 = insert_range( t2, 1100000, 1200000 );
    root = avl_join( t1, new_avl_node(1050000), t2 );
    root = avl_join( root, new_avl_node(2000000), NULL );
    root = avl_join( NULL, new_avl_node(1), root );
    result = search_range( root, 1, 1000000 );
    result = search_range( root, 1100000, 1200000 )?result:FAILED;
    avl_property_test(root,&result);
    printf("JOIN TEST ");
    print_result( result );
    delete_avl_tree( root );
    return result;
}

TestResult split_test(){
    int i=1;
    TestResult result=PASSED;
    AVLNodePtr trees_out[2] = {NULL,NULL};
    AVLNodePtr root=NULL,node1=NULL,node2=NULL;
    root = insert_range( root, 1, 1000000 );
    node1 = avl_split( root, 1, trees_out );
    root = trees_out[1];
    trees_out[0] = trees_out[1] = NULL;
    node2 = avl_split( root, 300000, trees_out );
    if( !(node2 && node2->key==300000) || !(node1 && node1->key==1) )
        result = FAILED;
    if( node1 )
        free(node1);
    if( node2 )
        free(node2);
    result = search_range( trees_out[0], 2, 299999 )?result:FAILED;
    result = search_range( trees_out[1], 300001, 1000000 )?result:FAILED;
    for( i=300000; i<=1000000; i++ ){
        if( avl_search(trees_out[0],i) )
            result = FAILED;
    }
    for( i=1; i<=300000; i++ ){
        if( avl_search(trees_out[1],i) )
            result = FAILED;
    }
    avl_property_test( trees_out[0], &result );
    avl_property_test( trees_out[1], &result );
    printf("SPLIT TEST ");
    print_result( result );
    delete_avl_tree( trees_out[0] );
    delete_avl_tree( trees_out[1] );
    return result;
}

int main(){
    search_insert_test();
    delete_test();
    join_test();
    split_test();
    return 0;
}
