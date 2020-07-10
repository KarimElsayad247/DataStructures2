/***Question 6****/
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* next;
}Node;
int hash1(int x) {
    return x%13;
}
void chain_add(int x, Node** table){
    int h1 = hash1(x);
    Node* current = table[h1];
    while(1){
        if(current == NULL){
            current = malloc(sizeof(Node));
            current->key = x;
            current->next = NULL;
            break;
        }
        else{
            current = current->next;
        }
    }
}
int main() {
    Node* table[13] = {NULL};
    int i = 1;
    printf("Enter 0 to exit");
    while(i){
        scanf(" %d", &i);
        chain_add(i, table);
    }
    return 0;
}
