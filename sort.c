#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	// free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int* pData, int l, int r)
{
	// Base Case
	if(l >= r) return;

	int* mData;
	int* mData2;
	int sz = (r - l + 1);
	mData = Alloc(sz/2);
	for(int i = 0; i < sz / 2; i++) {
		mData[i] = pData[i];
	}
	if(sz % 2 == 0) {
		mData2 = Alloc(sz/2);
		for(int i = 0; i < sz/2; i++)
			mData2[i] = pData[sz/2 + i];
	}
	else {
		mData2 = Alloc(sz/2 + 1);
		for(int i = 0; i < (sz/2) + 1; i++)
			mData2[i] = pData[sz/2 + i];
	}

	if(sz % 2 == 1) {
		mergeSort(mData, l, (r/2) - 1);
		mergeSort(mData2, (r + l)/2, r);
	}
	else {
		mergeSort(mData, l, r/2);
		mergeSort(mData2, ((r + l)/2) + 1, r);
	}

	// Begin merge
	int count1 = 0;
	int count2 = 0;
	int sz2 = sz;
	if(sz % 2 == 1) {
		sz2++;
	}
	for(int i = 0; i < sz; i++) {
		if(count1 < sz/2 && count2 < sz2/2) {
			if(mData[count1] <= mData2[count2]) {
				pData[i] = mData[count1];
				count1++;
			}
			else {
				pData[i] = mData2[count2];
				count2++;
			}
		}
		else if(count1 > sz/2) {
			pData[i] = mData2[count2];
			count2++;
		}
		else {
			pData[i] = mData[count1];
			count1++;
		}
	}

	DeAlloc(mData);
	DeAlloc(mData2);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}