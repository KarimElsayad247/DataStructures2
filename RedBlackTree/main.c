#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbtree.h"

/***strcasecmp is not standard therefore the program might not be portable***/

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
