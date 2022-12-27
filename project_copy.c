/*
read data
divide it into chunks
	structure 
	threads
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>

#define MAX_TREE_HT 50
#define  BASE_SIZE 10

int *array;
int numthreads;
//int c=32;
int count[256] ={0};
pthread_mutex_t * locks;


/////////////////////////////////Conditional Variables (Locks)


typedef struct _threadarg{
	int element;
	int start;
	int end;
	int thread_index;
} threadarg;

typedef struct _threadret{
	int* indexes;
	int size;
} threadret;


//void find_frequency(char [], int []);
//char* readfromfile(char*);

typedef struct mydat{
	int c;
	char* data;
	int thread_index;
} mydat;



void find_frequency(char s[], int count[]) 
{
   int c = 32;
   while (s[c] != '\0') 
   {
      if (s[c] >= ' ' && s[c] <= '~' )
         count[s[c]]++;
      c++;
   }
}



void * worker (void * farg){

	mydat* arg = (mydat*)farg;	
	// later part of the worker is being serialized
	pthread_mutex_lock(&locks[arg->thread_index]);

	printf("I got stucked in lock %d please help me", arg->thread_index);
	printf("%s\n\n",arg->data);
	find_frequency(arg->data, count);

	//pthread_mutex_unlock(&locks[arg->thread_index]);
	if (arg->thread_index<numthreads-1){
		pthread_mutex_unlock(&locks[arg->thread_index+1]);
	}

	return arg;
}





char* readfromfile(char* stri)
{
	FILE* file;
	file = fopen(stri, "r");
	if (file == NULL)
	{
		printf("Error opening a file\n");
		return NULL;
	}
	size_t total =0;
	while(!feof(file)&&!ferror(file))
	{
		fgetc(file);
		total++;
	}
	if (ferror(file))
	{
		printf("error reading from a file\n");
		return NULL;
	}
	
	char* string = malloc(total);
	rewind(file);
	size_t index =0;
	
	while(!feof(file)&&!ferror(file))
	{
		string[index] = fgetc(file);
		index++;
	}
	
	if (ferror(file))
	{
		printf("error reading from a file\n");
		return NULL;
	}
	
	string[index-1] = '\0';
	fclose(file);
	
	return string;	
}

// Huffman Coding in C

struct MinHNode {
  char item;
  unsigned freq;
  struct MinHNode *left, *right;
};

struct MinHeap {
  unsigned size;
  unsigned capacity;
  struct MinHNode **array;
};

// Create nodes
struct MinHNode *newNode(char item, unsigned freq) {
  struct MinHNode *temp = (struct MinHNode *)malloc(sizeof(struct MinHNode));

  temp->left = temp->right = NULL;
  temp->item = item;
  temp->freq = freq;

  return temp;
}

// Create min heap
struct MinHeap *createMinH(unsigned capacity) {
  struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

  minHeap->size = 0;

  minHeap->capacity = capacity;

  minHeap->array = (struct MinHNode **)malloc(minHeap->capacity * sizeof(struct MinHNode *));
  return minHeap;
}

// Function to swap
void swapMinHNode(struct MinHNode **a, struct MinHNode **b) {
  struct MinHNode *t = *a;
  *a = *b;
  *b = t;
}

// Heapify
void minHeapify(struct MinHeap *minHeap, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
    smallest = left;

  if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
    smallest = right;

  if (smallest != idx) {
    swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

// Check if size if 1
int checkSizeOne(struct MinHeap *minHeap) {
  return (minHeap->size == 1);
}

// Extract min
struct MinHNode *extractMin(struct MinHeap *minHeap) {
  struct MinHNode *temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

// Insertion function
void insertMinHeap(struct MinHeap *minHeap, struct MinHNode *minHeapNode) {
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
    minHeap->array[i] = minHeap->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
  int n = minHeap->size - 1;
  int i;

  for (i = (n - 1) / 2; i >= 0; --i)
    minHeapify(minHeap, i);
}

int isLeaf(struct MinHNode *root) {
  return !(root->left) && !(root->right);
}

struct MinHeap *createAndBuildMinHeap(char item[], int freq[], int size) {
  struct MinHeap *minHeap = createMinH(size);

  for (int i = 0; i < size; ++i)
    minHeap->array[i] = newNode(item[i], freq[i]);

  minHeap->size = size;
  buildMinHeap(minHeap);

  return minHeap;
}

struct MinHNode *buildHuffmanTree(char item[], int freq[], int size) {
  struct MinHNode *left, *right, *top;
  struct MinHeap *minHeap = createAndBuildMinHeap(item, freq, size);

  while (!checkSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);

    top = newNode('$', left->freq + right->freq);

    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top);
  }
  return extractMin(minHeap);
}



void printHCodes(struct MinHNode *root, int arr[], int top) {
  if (root->left) {
    arr[top] = 0;
    printHCodes(root->left, arr, top + 1);
  }
  if (root->right) {
    arr[top] = 1;
    printHCodes(root->right, arr, top + 1);
  }
  if (isLeaf(root)) {
    printf("  %c   | ", root->item);
    printArray(arr, top);
  }
}

// Wrapper function
void HuffmanCodes(char item[], int freq[], int size) {
  struct MinHNode *root = buildHuffmanTree(item, freq, size);

  int arr[MAX_TREE_HT], top = 0;

  printHCodes(root, arr, top);
}

// Print the array
void printArray(int arr[], int n) {
  int i;
  for (i = 0; i < n; ++i)
    printf("%d", arr[i]);

  printf("\n");
}






int main(int argc, char* argv[])
{
	///	checking if the number of input parameters matches with the requirement  
	//	0: program name,	1: textfile name, and	2: # of threads to be used

	if(argc !=3)
	{
		printf("Invalid number of Arguments!\n");
		return 1;
	}
	
	numthreads = atoi(argv[2]);	
	char* stringdata = readfromfile(argv[1]);
	int array_size = strlen(stringdata);
	int chunk = ceil((float)array_size/numthreads);
	
	printf("Total # of characters in this file:	%d\n", array_size);
	printf("You want this task to be done with %d threads:\n", numthreads);
	printf("Chunk size is:	%d\n", chunk);
	
	mydat* myv = (mydat*) malloc(sizeof(mydat) * numthreads);
	
	for (int i=0; i<numthreads; i++)
	{
		myv[i].data= (char*) malloc(sizeof(char) * chunk);
	}
	
	for(int i =0;i < numthreads;i++)
	{
		for(int j = 0; j<chunk; j++)
		{
			myv[i].data[j]=stringdata[j+(i*chunk)];
		}
		myv[i].thread_index=i;
		myv[i].c=32;
	}


	pthread_t* threads =(pthread_t*) malloc(sizeof(pthread_t) * numthreads);
	// work on the worker function and thread creation, find the frequency and Huffman encode and write a text file and .bin file
	locks = malloc(sizeof(pthread_mutex_t)*numthreads);
	int rc;

	//initializing all locks and locking trhem
	for(int i =0; i< numthreads; i++){
		rc = pthread_mutex_init(&locks[i], NULL);
		assert(rc==0);
		pthread_mutex_lock(&locks[i]);
	}

	pthread_mutex_unlock(&locks[0]); //unlocking the first lock
	for (int i =0; i< numthreads; i++){
		pthread_create(&threads[i], NULL, worker, &myv[i]);
	}

	for (int i =0; i< numthreads; i++){
		pthread_join(threads[i], (void**)&myv[i]);
	}


	printf("Character frequency is:	\n");
	for (int c = 32 ; c < 127 ; c++)
	{
		printf("%c%d\n", c, count[c]);
	}


	free(stringdata);
	for (int i=0; i<numthreads; i++)
	{
		free(myv[i].data);
	}
	free(myv);
	free(threads);
	free(locks);

	char arr[] = {'A', 'B', 'C', 'D'};
	int freq[] = {5, 1, 6, 3};

	int size = sizeof(arr) / sizeof(arr[0]);

	printf(" \n\nChar | Huffman code ");
	printf("\n--------------------\n");

	HuffmanCodes(arr, freq, size);

	return 0;	
}
