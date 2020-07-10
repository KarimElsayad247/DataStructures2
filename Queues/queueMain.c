#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 * Item: An item that is being enqueued to or dequeued from the queue
 *       It is a pointer to the process
 */
 //#define TYPE int




typedef struct {
	char name[10];
	int arrival_time;
	int remaining_time;
} Process, *Item;
/*
 *
 */
typedef struct Node
{
	Item data;
	struct Node* next;
} Node;
/*
 *
 */
typedef struct {
	Node* head;
	Node* tail;
	int numberOfItems;
} Queue;
/*
 *
 */
void init(Queue *q) {
	q->numberOfItems = 0;
	q->head = NULL;
	q->tail = NULL;
}
/*
 *
 */
int isEmpty(Queue *q) {
	if (q->numberOfItems == 0)
        return 1;
    else
        return 0;
}
/*
 for dequeue, take pointer, copy head pointer, move head pointer ,free previous head
 */
Item dequeue(Queue *q) {
    /*Item value = q->head->data;
    Node * temp = malloc(sizeof(Node));
    temp = q->head;
    q->head = q->head-> next ;
    free(temp);
    return value ;*/
    if(isEmpty(q))
    {
        return NULL;
    }
    Node* desposedNode = q->head;
	q->head = q->head->next;

	Item frontValue;
	frontValue = desposedNode->data;

    free(desposedNode);
    q->numberOfItems--;
    return frontValue;
}
/*
 *
 */
void enqueue(Queue *q, Item val) {
	Node* newNode = malloc(sizeof(Node));
	newNode->data = val;

	if (isEmpty(q))
    {
        q->head = newNode;
        q->tail = newNode;
        newNode->next = NULL;
        q->numberOfItems++;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
        newNode->next = NULL;
        q->numberOfItems++;
    }
}
/*
 *
 */
void destroy(Queue *q) {
	while(!isEmpty(q))
    {
        Item temp = dequeue(q);
    }

    if(isEmpty(q))
    {
        return;
    }
}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{

    FILE* sourceFilePtr = fopen(filename, "r");
    if(sourceFilePtr == NULL)
    {
        printf("Couldn't open file\n");
        exit(1);
    }
    //int waitingCount = 0;

    Queue arrivalQueue;
    Queue readyQueue;

    init(&readyQueue);
    init(&arrivalQueue);

    //Process arrivalArray[1024];

    while(!feof(sourceFilePtr))
    {
        char name[10];
        int arrival_time;
        int requiredTime;
        fscanf(sourceFilePtr, "%s %d %d", name, &arrival_time, &requiredTime);
        printf("%s %d %d\n", name, arrival_time, requiredTime);

        //Process newItem;
        Process *newItem2 = malloc(sizeof(Process));


        strcpy(newItem2->name, name);
        newItem2->arrival_time = arrival_time;
        newItem2->remaining_time = requiredTime;

        enqueue(&arrivalQueue, newItem2);

        //strcpy(newItem.name ,name);
        //newItem.arrival_time=arrival_time;
        //newItem.remaining_time=requiredTime;


        //arrivalArray[waitingCount]= newItem;           //save all incoming items in the file in an array


        //printf("\nEnqueued: \n %s\t%d\t%d\n", newItem.name, newItem.arrival_time, newItem.remaining_time);
        //waitingCount++;
    }

    int currentCycleCount = 0;
    //int handledProcess = 0;




    while(1)
    {


        if(!isEmpty(&arrivalQueue))
        {
            if(arrivalQueue.head->data->arrival_time <= currentCycleCount)
            {
                Process *newProcess = malloc(sizeof(Process));
                newProcess = dequeue(&arrivalQueue);
                printf("%s Enters\n", newProcess->name);
                newProcess->remaining_time--;
                printf("%s\t(%d --> %d)\n",newProcess->name ,currentCycleCount, currentCycleCount+1);
                enqueue(&readyQueue, newProcess);
                currentCycleCount++;
                continue;
            }

        }


        if(isEmpty(&readyQueue) && isEmpty(&arrivalQueue))
        {
            break;
        }
        else if (isEmpty(&readyQueue) && !isEmpty(&arrivalQueue))
        {
            printf("idle\t(%d --> %d)\n", currentCycleCount, currentCycleCount+1);
            currentCycleCount++;
        }

        if(!isEmpty(&readyQueue))
        {
            Process *currentProcess = malloc(sizeof(Process));
            currentProcess = dequeue(&readyQueue);
            printf("%s\t(%d --> %d)\n",currentProcess->name ,currentCycleCount, currentCycleCount+1);
            currentProcess->remaining_time--;
            if(currentProcess->remaining_time > 0)
            {
                enqueue(&readyQueue, currentProcess);
            }
            else
            {
                printf("%s Halts\n", currentProcess->name);
            }
            currentCycleCount++;
        }

    }

/*

    while(!isEmpty(&arrivalQueue))
    {
        Process *newProcess = malloc(sizeof(Process));
        newProcess = dequeue(&arrivalQueue);
        printf("\n\ndequeued:\n%s\t%d\t%d\n\n", newProcess->name, newProcess->arrival_time, newProcess->remaining_time);
    }
*/



//    while(waitingCount)
//    {
//        Process newItem = arrivalArray[waitingCount-1];
//        printf("\n\ndequeued: \n %s\t%d\t%d\n\n", newItem.name, newItem.arrival_time, newItem.remaining_time);
//        waitingCount--;
//    }
}





/*
 *
 */
int main(int argc, char**argv) {


if(argc!=2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	RoundRobin(argv[1]);

    printf("******\n");
    printf("Done!\n");
    printf("******\n");

	return 0;
}
