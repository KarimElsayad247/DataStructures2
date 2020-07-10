#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "farray.h"
#define INT_MAX +2147483647
void insertionSort(int array[],int size );

void merge_with_no_breakout(int array[], size_t start, size_t mid, size_t end)
{
    size_t first_array_size = mid - start +1;   //size of sorted left array
    size_t second_array_size = end - mid;       //size of right sorted array

    int temp_array_1[first_array_size+1];         //array to hold left elements
    int temp_array_2[second_array_size+1];        //array to hold right elements


    for (size_t i = 0; i < first_array_size; i++)   //copy left to first array
    {
        temp_array_1[i] = array[start + i];         //middle is taken here
    }
    temp_array_1[first_array_size] = INT_MAX;
    for (size_t j = 0; j < second_array_size; j++)              //copy right to first array
    {
        temp_array_2[j] = array[mid + j + 1];      //we start after element mid
    }
    temp_array_2[second_array_size] = INT_MAX;

    size_t i = 0;
    size_t j = 0;
    //now sort
    for (size_t k = start; k<=end; k++)
    {
        if(temp_array_1[i] < temp_array_2[j])
        {
            array[k] = temp_array_1[i++];
        }
        else
        {
            array[k] = temp_array_2[j++];
        }
    }
}
void merge_sort(int array[], size_t start, size_t end)
{
    size_t mid = (start + end)/2;
    if(start < end)
    {
        merge_sort(array, start, mid); //sort left half
        merge_sort(array, mid+1, end); //sort right half
        merge_with_no_breakout(array, start, mid, end); //merge both halves
 //       printf("first element: %d\n", array[0]);
    }
}
void print_array(int* array_to_print, size_t array_size)
{
    //arbitrary number: it takes too long to print an array
    if(array_size>10000)
    {
        printf("Array too big to print: time consuming\n");
        return;
    }
    for(size_t i = 0; i < array_size; i++)
    {
        printf("%6d", array_to_print[i]);
    }
}
void insertionSort(int array[],int size )
{
    int counter=0,stop=0,temp;
    for (int i = 1 ; i<size ; i++)
    {
        stop=0;
        counter=0;
        for (int j = i-1 ; j>=0 ; j--)
        {
            if ((array[i]>array[j]) & (stop==0))
            {
                stop=1;
                j=-1;
            }
            else
            {
                counter++;
            }
        }
        if(counter !=0)
        {
            temp = array[i];
            for (int k = 0 ; k<=counter ; k++)
            {
                array[i-k] = array[i-(k+1)];
            }
            array[i-counter] = temp;
        }
    }
}

int check_identical(int* array1, size_t size1 ,int* array2, size_t size2 )
{
    //if the files aren't of identical size, they aren't the same
    if(size1!=size2)
    {
        return 0;
    }


    for(size_t i = 0; i<size1; i++)
    {
        //if any 2 elements of the same index aren't equal
        //they aren't the same file
        if(array1[i] != array2[i])
        {
            return 0;
        }
    }

    return 1;

}

int main(int argc, char** argv)
{
    //make sure a name is entered
    if(argc < 2)
    {
        printf("Please enter the name of the files!\nPress enter to exit");
        getchar();
        exit(-1);
    }
    //and no more than 2 arguments
    else if(argc > 3)
    {
        printf("Please enter only two names!\nPress enter to exit");
        getchar();
        exit(-1);
    }

    //open target file
    FILE* target_file = fopen((const char*)argv[1],"r");
    if(target_file == NULL)
    {
        printf("Error opening file! make sure the name is correct!\n");
        exit(-2);
    }
    FILE* target_file2 = fopen((const char*)argv[2],"r");
    if(target_file2 == NULL)
    {
        printf("Error opening file! make sure the name is correct!\n");
        exit(-2);
    }

    size_t array_size = file_elemnts_number(target_file);
    int* array_to_sort = array_from_file(target_file, array_size);


    size_t array_size2 = file_elemnts_number(target_file2);
    int* array_to_sort2 = array_from_file(target_file2, array_size2);

    clock_t start, end,start2,end2;
    double cpu_time_used,cpu_time_used2;

    start = clock();
    insertionSort(array_to_sort,array_size);
    end = clock();

    cpu_time_used = ((double) (end - start))*1000.0 / CLOCKS_PER_SEC;
    printf("Running time for O(n^2) is %.0f ms \n", cpu_time_used);


    start2 = clock();
    merge_sort(array_to_sort2,0,array_size2-1);
//    printf("first element: %d\n", array_to_sort2[0]);
//    printf("first to last element: %d\n", array_to_sort2[array_size2-1]);
//    printf("last element: %d\n", array_to_sort2[array_size2]);
    end2 = clock();

    cpu_time_used2 = ((double) (end2 - start2))*1000.0 / CLOCKS_PER_SEC;
    printf("Running time for O(n log n) is %.0f ms \n", cpu_time_used2);

    if(check_identical(array_to_sort, array_size, array_to_sort2, array_size2))
    {
        printf("Files are identical\n");
    }
    else
    {
        printf("Files aren't identical\n");
    }

    //print_array(array_to_sort, array_size);
}
