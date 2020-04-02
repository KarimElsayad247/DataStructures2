#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum EColor {BLACK = 0, RED = 1}EColor;
typedef char     DATA;



//Node structure
struct Node {
        EColor color;
        DATA* value;
        struct Node* parent;
        struct Node* left;
        struct Node* right;
};


struct Tree{
        struct Node* root;
        unsigned int size;
};


void inserted(struct Node* node);
void traverse_indepth(struct Node* node);
void nprint(struct Node* node);
void compare_with_file(struct Tree* tree, char* file_name);

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
        if(strcmp(new_node->value,root->value) < 0){
                root->left = normal_insert(root->left, new_node);
                root->left->parent = root;

        }
        else if (strcmp(new_node->value,root->value) > 0){
                root->right = normal_insert(root->right, new_node);
                root->right->parent = root;
        }
        else{
                printf("word already exits!\n");
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
        new_tree->size = 0;
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
        printf("%s\n",node->value, node->color);
        tree_print_all(node->right);
}



int rb_search(struct Node* node, DATA* value){
        /****remember to check for NULL before printing, searching, or doing anything****/
        //recursion FTW
        if(node){
                if(strcmp(value, node->value) == 0)
                        return 1;
                else if(strcmp(value, node->value) < 1)
                        return rb_search(node->left, value);
                else
                        return rb_search(node->right, value);
        }
        else
                return 0;
}


void insert_new_word(struct Tree* my_tree){
        char input[16];
        fgets(input, sizeof input, stdin);
        printf("Word to add: ");
        fgets(input, sizeof input, stdin);
        sscanf(input, " %[^\n]", input);
        if(rb_search(my_tree->root, input)){
                printf("Word already exists\n");
                return;
        }
        rb_insert(my_tree, create_rb_node(input));
        printf("Word added successfully!\n");
        printf("Size: %3d\tHight: %2d\n",my_tree->size,find_tree_hight(my_tree->root));
}


void lookup_word(struct Tree* my_tree){
        char input[16];
        fgets(input, sizeof input, stdin);
        printf("Enter Word to lookup: ");
        fgets(input, sizeof input, stdin);
        sscanf(input, " %[^\n]", input);
        printf("Word exists? ");
        if(rb_search(my_tree->root, input))
                printf("YES\n");
        else
                printf("NO\n");

}


void write_tree(struct Node* node, FILE* file){
        if(node == NULL) return;

        write_tree(node->left, file);
        fprintf(file, "%s\n", node->value);
        write_tree(node->right, file);
}


void save_to_file(struct Tree* tree, char* file_name){
        fgetc(stdin);
        printf("This will overwrite the current file, are you sure?\n");
        printf("Y/N\t");
        char temp[30];
        char letter = 'N';
        fgets(temp, sizeof temp, stdin);
        sscanf(temp, " %[^\n]", temp);
        letter = temp[0];
        if(letter == 'Y' || letter == 'y')
        {
                FILE* file = fopen(file_name, "w");
                if(file == NULL){
                        printf("Error opening file!\nmake sure file name is correct.\ninclude file extension e.g. file.txt\n input was %s\n", file_name);
                        exit(-12);
                }
                write_tree(tree->root, file);
                fclose(file);
                printf("Saved successfully!\n");
        }
        else{
                printf("Canceled!\n");
        }
}

void handle_users(struct Tree* my_tree, char* file_name){
        int input = 0;
        int on = 1;
        char temp[200];
        while(on){
                printf("***********************************\n");
                printf("options:\n");
                printf("1- View all entries\n");
                printf("2- look up a word\n");
                printf("3- Add new entry in dictionary\n");
                printf("4- Print hight of tree\n");
                printf("5- Print size of tree\n");
                printf("6- Save dictionary\n");
                printf("7- Exit\n");
                printf("Choose option: ");
                scanf(" %d", &input);

                switch(input){
                case 1:
                        tree_print_all(my_tree->root);
                        fgetc(stdin);
                        break;
                case 2:
                        lookup_word(my_tree);

                        break;
                case 3:
                        insert_new_word(my_tree);
                        break;
                case 4:
                        printf("Tree hight is: %d\n", find_tree_hight(my_tree->root));
                        fgetc(stdin);
                        break;
                case 5:
                        printf("There are %d words in dictionary\n", my_tree->size);
                        fgetc(stdin);
                        break;
                case 6:
                        save_to_file(my_tree, file_name);
                        break;
                case 7:
                        on ^= on;
                        break;
                default:
                        printf("Please enter a valid choice! (1 -> 6)\n");
                        fgets(temp, sizeof temp, stdin);
                }
                if(on)
                        printf("Press Enter to get back\n");

                fgetc(stdin);
        }
}

void check_args(int argc){
        if(argc < 2 ){
                printf("Enter the name of a file!");
                exit(-1);
        }
        else if(argc > 2){
                printf("Enter only one Name!");
                exit(-2);
        }
}

int main(int argc, char** argv) {
        check_args(argc);
        char* target_file_name = argv[1];
        FILE* target_file = fopen((const char*)target_file_name, "r");
        if(target_file == NULL){
                printf("Error opening file!\nmake sure file name is correct.\ninclude file extension e.g. file.txt\n input was %s\n", target_file_name);
                exit(-12);
        }
        struct Tree* my_tree = build_tree(target_file);
        fclose(target_file);

        if(my_tree)
                printf("File loaded successfully!\n");
        else
                 printf("Error loading file!");

        handle_users(my_tree, target_file_name);

        printf("Exited successfully!\n");
        return 0;
}
/***LEGACY CODE: FOR REFERENCE ONLY(personal attempt)
        who woudda thunk using a void function is worse...***/
//main insert function, takes a root and a value and recursively
//inserts the value
//void rb_insert(struct Node* root, DATA* value){
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
void nprint(struct Node* node){
        printf("Node is %s\n", node->value);
}
void inserted(struct Node* node){
        printf("inserted %s\tcolor = %d\n",node->value, node->color);
}
void traverse_indepth(struct Node* node){
        if(node == NULL) return;
        traverse_indepth(node->left);
        printf("node %3s\tcolor %3d\n",node->value, node->color);
        traverse_indepth(node->right);
}

void compare_with_file(struct Tree* tree, char* file_name){





//TODO: compare existing tree with an external file and check if they have same words




}
/************************************************************************************************/
