#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int size;
int *arr;

typedef struct {
    int left;
    int right;
} MergeSortArgs;

void read_file(char*);
void merge(int, int, int);
void* mergeSort(void*);
void printArray();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }


    read_file(argv[1]);

    MergeSortArgs args = {0, size - 1};

    pthread_t tid;
    pthread_create(&tid, NULL, mergeSort, &args);
    pthread_join(tid, NULL);

    printf("Sorted array: ");
    printArray();
    free(arr);

    return 0;
}
void read_file(char*filename){
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return;
    }
    fscanf(input_file, "%d", &size);
    arr = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        fscanf(input_file, "%d", &arr[i]);
    }
}
void merge(int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void* mergeSort(void* arg) {
    MergeSortArgs* args = (MergeSortArgs*)arg;
    int left = args->left;
    int right = args->right;

    if (left < right) {
        int mid = left + (right - left) / 2;

        pthread_t tid1, tid2;

        MergeSortArgs args1 = {left, mid};
        MergeSortArgs args2 = {mid + 1, right};

        pthread_create(&tid1, NULL, mergeSort, &args1);
        pthread_create(&tid2, NULL, mergeSort, &args2);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        merge(left, mid, right);
    }

    pthread_exit(NULL);
}

void printArray() {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
