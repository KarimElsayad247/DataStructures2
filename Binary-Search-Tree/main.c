#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#define TYPE char

//contents of nodes
/*typedef struct data{
    int key;
    char val[128];
}data;*/



//tree constructor
typedef struct Node{
    char key[128];
    struct Node*left, *right, *parent;
}Node;


//constructor
Node *newTree()
{
    Node *n = malloc(sizeof(Node));
    strcpy(n->key, "root");
    n->left  = NULL;
    n->right = NULL;
    n->parent = NULL;
    return n;
}


Node *LastAdded;

Node *newNode(char *val){

    Node* n=malloc(sizeof(Node));

    strcpy(n->key, val);
    n->left  = NULL;
    n->right = NULL;

    return n;
}


Node* latestParent;
Node *insert(Node *node, char *key){
    if(node == NULL)
    {
        Node *n = newNode(key);
        //printf("created new node! this node has ----->%s<------\n", key);
        n->parent = latestParent;
        LastAdded = n;
        return n;
    }
    else if(strcmp(key, node->key) < 0)
    {
        //printf("node has %s,        inserting smaller\n", node->key);
        latestParent = node;
        node->left = insert(node->left, key);
    }
    else if(strcmp(key, node->key) > 0)
    {

        //printf("node has %s,        inserting bigger\n",node->key);
        latestParent = node;
        node->right= insert(node->right, key);
    }
    else
    {
//        printf("node has %s,        it already exists\n",node->key);
        return node;
    }
    return node;
}
int globalCount = 0;

Node *readIntoTree(char *fileName)
{
    Node *tree = newTree();
    FILE* sourceFilePtr = fopen(fileName, "r");
    if(sourceFilePtr == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }

    int count = 0;
    while(!feof(sourceFilePtr))
    {
        char word[255];
        fscanf(sourceFilePtr, " %[^\n]", word);
        Node *tempNode = insert(tree, word);
//        printf("inserted new node node %d      %s\n in read into we see %s\nLast added node has %s\n",
//                globalCount++, word, tempNode->key, LastAdded->key);
//        printf("****************************************************************\n");
        globalCount++;
        count++;
    }
    printf("read %d words\n", count);
    printf("Words loaded\n");
    return tree;
}


void inorder(Node *node)
{
    if (node) {
        inorder(node->left);
        printf("in tree: %s\n", node->key);
        inorder(node->right);
    }
}

Node *latestFound;
Node *beforeLatest;
Node *search(Node * node, char* val)
{
    if (node == NULL) {
//        printf("%s\n" ,node->key);
        printf("Did you mean: %s, %s, %s\n", latestFound->key,
                latestFound->left->key, latestFound->left->left->key);
        return NULL;
    }
    else
    {

        if(strcmp(val ,node->key) == 0)
        {
//            printf("%s\n", node->key);
            return node;
        }
        else if (strcmp(val ,node->key) < 0)
        {
//            printf("smaller, found %s\n", node->key);
            beforeLatest = latestFound;
            latestFound = node;
            if(node->left->key == NULL)
                return NULL;
            else
                node = search(node->left, val);

        }
        else if(strcmp(val ,node->key) > 0)
        {
            beforeLatest = latestFound;
            latestFound = node;
            if(node->right->key == NULL)
                return NULL;
            else
                node = search(node->right, val);
        }

    }
    return node;
}

Node *successor;
Node *next(Node *node, Node *d) {

    if (node == NULL)
        return d;

    else if (strcmp(d->key, node->key) > 0)
        return next(node->right, d);

    else if (strcmp(d->key, node->key) == 0)
        return (node->right == NULL) ? d : next(node->right, d);

    else
    {
        Node *tmp = next(node->left, d);
        return strcmp(tmp->key ,d->key) ? node : tmp;
    }

}


int height(Node *node)
{
    if (node == NULL)
        return 0;
    else
    {
        int l = 1+height(node->left);
        int r = 1+height(node->right);
        return l> r ? l :r;
    }
}


Node *findLeftmost(Node *tree)
{

        while(tree->left != NULL)
        {
            printf("%s - looking even more left \n", tree->key);
            tree = tree->left;
        }
    return tree;
}

Node *successorFromChild(Node *root)
{

    if(root->right)
    {
            root = findLeftmost(root->right);
    }
    else
    {
        root = successorFromChild(root->parent);
    }
    return root;
}


//    if(root->right != NULL)
//    {
//        printf("%s - looking right", root->right->key)
//        successorFromParent = find,Leftmost(root->right);
//    }
//    else if(root->parent->right != NULL)
//    {
//        successorFromParent = findLeftmost(root->parent->right);
//    }
//    else
//    {
//        successorFromParent = successorFromChild(root->parent);
//    }

//    return successorFromParent;
//}

void takeWords(Node *tree)
{
    int flag = 1;
    while(flag)
    {
        printf("Enter a word: ");
        char userInput[1024];
        char parsedInput[1024];
        fgets(userInput, 1023, stdin);
        sscanf(userInput, " %[^\n]", parsedInput);

//        const char delim[2] = " ";
        char *token;
        token = strtok(parsedInput, " ");
        while(token != NULL)
        {
            Node *n = search(tree, token);

            if(n == NULL){
//               Node *successor = inorderSuccessor(tree, latestFound);
//                Node *nextInLine = next(tree, latestFound);
                Node *nextInLine = successorFromChild(latestFound);
                printf("%s - Did you mean %s, %s, %s?\n",token ,latestFound->key, beforeLatest->key, nextInLine->key);
            }
            else
                printf("%s - CORRECT!\n", token);

//            printf("**************tokenized\n");
            token = strtok(NULL, " ");
//            printf("tokenized**************\n");
        }



        char newInput[256];
        printf("Done?? (Y\\N)\n");
        fgets(newInput, 255, stdin);
        if(newInput[0] == 'Y' || newInput[0] == 'y')
        {
            flag = 0;
        }

    }
    return;
}

int main(int argc, char**argv)
{

    if(argc!=2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }
    Node *tree = readIntoTree(argv[1]);
    printf("Hight: %d\n", height(tree));
//    inorder(tree);

    takeWords(tree);

    return 0;
}










