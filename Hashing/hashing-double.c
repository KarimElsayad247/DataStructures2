#include <stdio.h>
#include <stdlib.h>

int hash1(int x) {
    return x%13;
}

int hash2(int x) {
    return 7 - x % 13;
}

void double_add(int x, int* table, int size) {
    int h1 = hash1(x);
    int h2 = hash2(x);
    for(int i = 0; i < size; i++) {
        if(table[h1 + i * h2] == -1){
            table[h1 + i * h2] = x;
            break;
        }
    }
}
int search(int x, int* table, int size){
    int h1 = hash1(x);
    int h2 = hash2(x);
    for(int i = 0; i < size; i++) {
        if(table[h1 + i * h2] == x){
            return 1
    }
    return 0;
}
int main() {
    unsigned int size = 13;
    int table[size];
    for (size_t i = 0; i < size; i++)
        table[i] = -1; 
    int i = 0;
    while (i++ < 5) {
        int input;
        scanf(" %d", &input);
        double_add(input, table, size);
    }
    return 0; 
}