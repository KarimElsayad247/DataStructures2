#ifndef RBTREE_H_INCLUDED
#define RBTREE_H_INCLUDED


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




/*Creates the tree node
 *takes the required value as an argument.
 *parent, left and right children are initialized to NULL
 *each new node starts RED
 */
struct Node* create_rb_node(DATA* value);


/*
 *rotates right/left on a given node
 *tree is required to handle changes in the root node
 */
void rotate_left(struct Tree* tree, struct Node* node);
void rotate_right(struct Tree* tree, struct Node* node);



/*
 *better algorithm, straight outta geeks4geeks!
 *a lot less comparisons to insert
 *a helper function, the build_tree doesn't call it.
 *it inserts normally,
 *but leaves fixing violations to the caller function which calls rb_fixup()
 */
struct Node* normal_insert(struct Node* root, struct Node* new_node);



/*
 *the fixup function fixes violations in the RB tree.
 *since each new node is RED, after each input, there might be a violation
 *to one of the RB tree properties, specifically the no adjacent red nodes property.
 *the function handles all cases and restores the properties.
 */
void rb_fixup(struct Tree* tree, struct Node* node);




/*
 *this function is called by build_tree,
 *it calls normal_insert and rb_fix up
 *it inserts the node and fixes violations in the tree properties
 *it also handles increasing the size of the tree after each node
 *by incrementing the size member of the tree struct
 */
void rb_insert(struct Tree* my_tree, struct Node* node);




/*
 *the build_tree function takes a file, reads it line by line,
 *creates a node containing said line, and inserts it in the tree.
 *it calls tb_insert which handles inserting the node and fixing violations
 *build_tree initializes the root node and assigns it to the tree struct
 */
struct Tree* build_tree(FILE* file);




/*
 *handles finding the hight of the tree
 *recursively calls it self, base condition returns 1
 *each subsequent call returns +1 to the highest hight
 */
int find_tree_hight(struct Node* node);


/*
 *in order traversal of the tree, prints all contents of the tree
 */
void tree_print_all(struct Node* node);


/*
 *returns 1 if a value/string exists in the tree
 *case-insensitive: no difference between uppercase and lowercase litters
 *returns 0 if a word doesn't exists.
 */
int rb_search(struct Node* node, DATA* value);

#endif // RBTREE_H_INCLUDED
