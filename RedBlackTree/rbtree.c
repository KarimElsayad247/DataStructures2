#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rbtree.h"

struct Node* create_rb_node(DATA* value){
        struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        if(!temp){
                printf("Error allocating memory for temp!");
                return NULL;
        }
        //an rb node always starts red, and is modified later
        temp->value = malloc(strlen(value)+1);
        if(temp->value == NULL){
                printf("Error allocating memory for string on creation!");
                exit(255);
        }

        strcpy(temp->value, value);
        temp->color = RED;
        temp->left = NULL;
        temp->right = NULL;
        temp->parent = NULL;
        return temp;
}


void rotate_left(struct Tree* tree, struct Node* node){
//        save nodes
        struct Node* parent= node->parent;
        struct Node* right = node->right;

//        first node
        node->right = right->left;
        if(right->left)
                right->left->parent = node;
        right->parent = node->parent;
        if(parent){
//              then right
                if(node == parent->left)
                        parent->left = right;
                else if(node == parent->right)
                        parent->right = right;
        }
        else{
//              if parent is null then it must become root
                tree->root = right;
        }
//        then right and node
        right->left = node;
        node->parent = right;
}


void rotate_right(struct Tree* tree, struct Node* node){
//        save nodes
        struct Node* parent= node->parent;
        struct Node* left = node->left;
        //first the node
        node->left = left->right;
        if(left->right != NULL)
                left->right->parent = node;

        //then the left
        left->parent = node->parent;
        if(parent){
                if(node == parent->left)
                        parent->left = left;
                else if(node == parent->right)
                        parent->right = left;
        }
        else{
//                if parent is nullm then it must become root
                tree->root = left;
        }
//        then left and node
        left->right = node;
        node->parent = left;
}


/*
 *better algorithm, straight outta geeks4geeks!
 *a lot less comparisons to insert
 *a helper function, the build_tree doesn't call it, it inserts normally,
 *but leaves fixing violations to the caller function which invokes fix up and which
 *knows the tree struct
 */
struct Node* normal_insert(struct Node* root, struct Node* new_node){
        //printf("in normal insert I see %s\n", new_node->value);
        //if you find NULL, then insert the new node here
        //i.e. the previous node will now point here
        if(root == NULL){
                return new_node;
        }

        //insert recursively depending on value
        if(strcasecmp(new_node->value,root->value) < 0){
                root->left = normal_insert(root->left, new_node);
                root->left->parent = root;

        }
        else if (strcasecmp(new_node->value,root->value) > 0){
                root->right = normal_insert(root->right, new_node);
                root->right->parent = root;
        }
        else{
                printf("word already exits!\n");
        }
        return root;
}


void rb_fixup(struct Tree* tree, struct Node* node) {
        struct Node* parent = node->parent;
        struct Node* gparent = NULL;
        struct Node* uncle = NULL;
        int black_uncle = 0;      //this is set to 1 when an uncle happens to be nil

        /*base cases:
         *if parent is null, then it's root
         *if parent is black, then tree is good
         */
        if(parent == NULL){
                node->color = BLACK;            //Case 0: Node is root of tree
                return;
        }
        else if (parent->color == BLACK){       //case nothing: tree is actually alright!
                return;
        }
        else{
                gparent = parent->parent;
        }

        //finding uncle node
        if(parent == gparent->left){
                uncle = gparent->right;
        }
        else if(parent == gparent->right){
                uncle = gparent->left;
        }

        if(uncle == NULL || uncle->color == BLACK){
                black_uncle = 1;
        }

        /*case 1: uncle is red*/
        if(!black_uncle){
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                rb_fixup(tree, gparent);
                return;
        }
        /*case 2: uncle is black and node is a right child*/
        if(black_uncle){
                if(parent == gparent->left){
                        if(node == parent->right){
                                rotate_left(tree, parent);
                                struct Node* temp = node;
                                node = parent;
                                parent = temp;
                        }
                        /*after case 2, check also for case 3
                         *case 3: uncle is black and node is a left child
                         */
                        if(node == parent->left ){
                                parent->color = BLACK;
                                gparent->color = RED;
                                rotate_right(tree, gparent);
                        }
                }
                else if(parent == gparent->right){
                        if(node == parent->left){
                                rotate_right(tree, parent);
                                struct Node* temp = node;
                                node = parent;
                                parent = temp;
                        }
                        /*after case 2, check also for case 3
                         *case 3: uncle is black and node is a left child
                         */
                        if(node == parent->right){
                                parent->color = BLACK;
                                gparent->color = RED;
                                rotate_left(tree, gparent);
                        }
                }
        }
        return;
}


void rb_insert(struct Tree* my_tree, struct Node* node){
        my_tree->root = normal_insert(my_tree->root, node);
        my_tree->size++;
        if((my_tree->size%10000) == 0 ) printf("%d\n", my_tree->size);
        rb_fixup(my_tree, node);
}


/*
 *srcArray is source array, take elements and build tree
 *n is array size
 *root is the tree to build
 */
struct Tree* build_tree(FILE* file){
        //build tree
        //first declare a root node then,
        //loop over the file and insert words into the tree
        DATA root_data[16];
        fscanf(file,"%[^\n]", root_data);
        struct Node* root = create_rb_node(root_data);
        root->color = BLACK;
        struct Tree* new_tree = malloc(sizeof(struct Tree));
        new_tree->root = root;
        new_tree->size = 1;
        char input[16];
        while(fscanf(file, " %[^\n]", input) != EOF ){
                rb_insert(new_tree, create_rb_node(input));
        }
        return new_tree;
}


int find_tree_hight(struct Node* node){
        if(node == NULL)
                return 0;
        int h1 = find_tree_hight(node->left);
        int h2 = find_tree_hight(node->right);
        return ((h1 > h2) ? h1 : h2) + 1;
}


void tree_print_all(struct Node* node){
        if(node == NULL) return;
        tree_print_all(node->left);
        printf("%s\n",node->value);
        tree_print_all(node->right);
}



int rb_search(struct Node* node, DATA* value){
        /****remember to check for NULL before printing, searching, or doing anything****/
        //recursion FTW
        if(node){
                if(strcasecmp(value, node->value) == 0)
                        return 1;
                else if(strcasecmp(value, node->value) < 1)
                        return rb_search(node->left, value);
                else
                        return rb_search(node->right, value);
        }
        else
                return 0;
}
