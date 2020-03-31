#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum EColor {BLACK = 0, RED = 1}EColor;
typedef int     DATA;



//Node structure
struct Node {
        EColor color;
        DATA value;
        struct Node* parent;
        struct Node* left;
        struct Node* right;
};


struct Tree{
        struct Node* root;
        unsigned int size;
};


void parent_print(struct Node* root);
void inserted(struct Node* node);
void traverse_indepth(struct Node* node);
void print_tree_stuff(struct Tree* my_tree);

struct Node* create_rb_node(DATA value){
        struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        if(!temp){
                printf("Error allocating memory for temp!");
                return NULL;
        }
        //an rb node always starts red, and is modified later
        temp->value = value;
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
        //if you find NULL, then insert the new node here
        //i.e. the previous node will now point here
        if(root == NULL)
                return new_node;

        //insert recursively depending on value
        if(new_node->value < root->value){
                root->left = normal_insert(root->left, new_node);
                root->left->parent = root;
//                parent_print(root->left);

        }
        else if (new_node->value > root->value){
                root->right = normal_insert(root->right, new_node);
                root->right->parent = root;
//                parent_print(root->right);
        }
        else{
                printf("Node already exits!\n");
        }
        return root;
}


/*personal implementation of fix up*/
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
                if(parent->right == node)
                        rotate_left(tree, parent);

                /*after case 2, check also for case 3
                 *case 3: uncle is black and node is a left child
                 */
                if(parent->left == node){
                        parent->color = BLACK;
                        gparent->color = RED;
                        rotate_right(tree, gparent);
                }
        }
        return;
}


void rb_insert(struct Tree* my_tree, struct Node* node){
        my_tree->root = normal_insert(my_tree->root, node);
        rb_fixup(my_tree, node);
}


/*
 *srcArray is source array, take elements and build tree
 *n is array size
 *root is the tree to build
 */
struct Tree* build_tree(DATA* srcArray, unsigned int n){
        //if pointer is null, do nothing
        if(srcArray == NULL){
                printf("no array!");
                return NULL;
        }
        //if array is empty, also do nothing
        if(n < 1){
                printf("array empty!");
                return NULL;
        }
        //otherwise, build tree
        //first declare a root node then,
        //loop over the array and insert values into the tree
        struct Node* root = create_rb_node(srcArray[0]);
        root->color = BLACK;
        struct Tree* new_tree = malloc(sizeof(struct Tree));
        new_tree->root = root;
        new_tree->size = n;
        unsigned int i = 0;
        for(i = 1; i < n; i++) {
                rb_insert(new_tree, create_rb_node(srcArray[i]));
//                new_tree->root = normal_insert(new_tree->root, create_rb_node(srcArray[i]));
        }

        if(new_tree->size != i)
                printf("something wrong with tree size!\n");
        return new_tree;
}



/*in depth traverse and check for value
 *if smaller, recursively search left
 *if larger, recursively search right
 */
int rb_search(struct Node* node, DATA value){
        /****remember to check for NULL before printing, searching, or doing anything****/

        //recursion FTW
        if(node != NULL){
                if(node->value == value)
                        return 1;
                else if(value < node->value)
                        return rb_search(node->left, value);
                else
                        return rb_search(node->right, value);
        }
        else
                return 0;
}





int main() {

        DATA array[8] = {10,9,8,7 ,6,5,4,3};//,6,5,4,3,2,1};//,15,90,11,23};
        struct Tree* my_tree = build_tree(array, 8);
        if(my_tree == NULL) printf("Error creating tree in main");


        /*tests*/
//        printf("traversing\n");
        printf("\n in main\nroot is %5d\tcolor %d\n",my_tree->root->value,my_tree->root->color);
        print_tree_stuff(my_tree);
//        parent_print(my_tree->root);
        traverse_indepth(my_tree->root);
//        for(int i = 0; i < 3; i++){
//                rotate_right(my_tree, my_tree->root);
//                printf("\n\n\ntraversing\n");
//                printf("after rotate %d\n", i);
//                traverse_indepth(my_tree->root);
////                printf("root is%5d\t\t\t\t\n",my_tree->root->value);
//                print_tree_stuff(my_tree);
//        }
//        for(int i = 0; i < 3; i++){
//                rotate_left(my_tree, my_tree->root);
//                printf("\n\n\ntraversing\n");
//                printf("after rotate %d\n", i);
//                traverse_indepth(my_tree->root);
////                printf("root is%5d\t\t\t\t\n",my_tree->root->value);
//                print_tree_stuff(my_tree);
//        }
//        rotate_left(my_tree, my_tree->root->right);
//        printf("rotating\n");
//        print_tree_stuff(my_tree);
//        parent_print(my_tree->root);
//        traverse_indepth(my_tree->root);
//        int test_num = 90;
//        if(rb_search(my_tree->root, test_num))
//                printf("YES %d\n", test_num);
//        else
//                printf("NO\n");
        return 0;
}
/***LEGACY CODE: FOR REFERENCE ONLY(personal attempt)
        who woudda thunk using a void function is worse...***/
//main insert function, takes a root and a value and recursively
//inserts the value
//void rb_insert(struct Node* root, DATA value){
//        //check where to insert the new value
//        //if the value is smaller, insert right
//        //if larger, insert left
//        //if they are equal, return because value already exists
//        if(value < root->value){
//                if(root->left){
//                        //if the left node is not NULL, there exists a node.
//                        //insert recursively
//                        rb_insert(root->left, value);
//                }
//                else{
//                        //if it's null, it's an empty place
//                        //create node and insert
//                        struct Node* node = create_rb_node(value);
//                        root->left = node;
//                }
//        }
//        else if(value > root->value){
//                if(root->right){
//                        rb_insert(root->right, value);
//                }
//                else{
//                        struct Node* node = create_rb_node(value);
//                        root->right = node;
//                }
//        }
//        else {
//                printf("Value already exists!\n");
//        }
//        return;
//}

/************************DEBUGGING STUFF***************************************************************************/
void print_tree_stuff(struct Tree* my_tree){
        printf("root %d\t", my_tree->root->value);
        if(my_tree->root->left)
                printf("left %d\t",my_tree->root->left->value);
        if(my_tree->root->right) {
                printf("right %d\t",my_tree->root->right->value);
                if(my_tree->root->right->right)
                        printf("right right%d\t",my_tree->root->right->right->value);
        }
        printf("\n");
}
void parent_print(struct Node* node){
        if(node->parent) printf("node %3d\tcolor %3d\tparent %3d\tcolor %3d\n", node->value, node->color, node->parent->value,node->parent->color);
        else printf("root is %3d\tcolor is%3d\n");
}

void inserted(struct Node* node){
        printf("inserted %d\tcolor = %d\n",node->value, node->color);
}
void traverse_indepth(struct Node* node){
        if(node == NULL) return;
        traverse_indepth(node->left);
        printf("node %3d\tcolor %3d\n",node->value, node->color);
//        parent_print(node);
        traverse_indepth(node->right);
}
/************************************************************************************************/
