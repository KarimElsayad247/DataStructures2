#include <stdio.h>
#include <stdlib.h>



/*
 * Item: Each Item has a customer name and a balance.
 *       and an integer order(it is used to handle the case when two items have same priority)
*/
typedef struct
{
    char cName[70];
    double balance;
    int order;
} Item;

Item extract_maximum(Item*arr,int n);

/*
 * Heapify index i in array of Items with size n
 */
void max_heapify(Item *arr, int n, int i)
{

    //if the member has no parents, return
    if((i/2 < 1))
        return;

    if(arr[i].balance > arr[i/2].balance)
    {
        Item temp;
        temp = arr[i];
        arr[i] = arr[i/2];
        arr[i/2] = temp;
        max_heapify(arr,n,i/2);
    }
    else
        return;

}


/*
 * Construct Priority queue from the given array of Items with size n(Build Max Heap)
 */
void construct_PQ(Item*arr,int n)
{
    for(int i = 1; i < n; i++)
        max_heapify(arr, n, i);
}




/*
 * delete the item with max priority and then return it.
 */
Item extract_maximum(Item*arr,int n)
{
    Item result = arr[1];
    arr[1] = arr[n];
    arr[0].order--;
    construct_PQ(arr, arr[0].order);
    return result;
}


/*
 * read array of items from file in arr and return its size.
 */
int fillArrayFromFile(char*fname,Item*arr)
{
    FILE *file = fopen(fname, "r");
    int count = 1;

    //first element in the array is reserved
    //for the amount of customers in it
    arr[0].order = 0;

    while(!feof(file))
    {
        Item *newItem = malloc(sizeof(Item));
        //scan into new item, put item in array
        fscanf(file, " %s %lf", newItem->cName, &(newItem->balance));
        arr[count] = *newItem;
        arr[count].order = count;
        arr[0].order = count;
        count++;
    }
    for(int i = 1; i<count; i++)
    {
        printf("%s %lf\n", arr[i].cName, arr[i].balance);
    }
    return count;
}


void checkOrder(Item *arr, int i)
{
    if(i <= 1) return;


    if(arr[i].balance == arr[i-1].balance)
    {
        if(arr[i].order < arr[i-1].order)
        {
            Item temp;
            temp = arr[i];
            arr[i] = arr[i-1];
            arr[i-1] = temp;
            checkOrder(arr,i-1);
        }
    }
    else
        return;
}


/*
 * print customer names in decreasing order according their priority.
 */
void printCustomersbyPriority(Item*arr,int n)
{
//    int count = arr[0].order;
    Item *orderedArr = malloc(sizeof(Item)*1000);

    for(int i = 1; i < n; i++)
    {
        Item currentMax = extract_maximum(arr, arr[0].order);
        orderedArr[i] = currentMax;
        checkOrder(orderedArr, i);

    }


    //extract a number if items equal to the size of the array
    printf("****************\nprinting according to priority\n****************\n");
    for(int i = 1; i < n; i++)
        printf("%10s \t %3.2lf \t %3d\n",orderedArr[i].cName, orderedArr[i].balance, orderedArr[i].order);

}



/*
 *
 */
int main(int argc, char**argv)
{

    if(argc!=2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }
    Item *customers=(Item*)malloc(sizeof(Item)*1000);
    int arrLength=fillArrayFromFile(argv[1],customers);
    construct_PQ(customers,arrLength);
    printCustomersbyPriority(customers,arrLength);
    return 0;
}
