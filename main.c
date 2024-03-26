#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include "node.h"

#define NTHREADS 2

struct arg_struct {
    bool flag;
    LinkedList *list;
};

int num_0_bits(unsigned int value) {
    int count = 0;
    while (value) {
        if ((value & 1) == 0) {
            count++;
        }
        value = value >> 1;
    }
    return count;
}

int num_1_bits(int value){
    int count = 0;
    while (value) {
        count += value & 1;
        value = value >> 1;
    }
    return count;
}

void* fun(void* arguments){
    struct arg_struct *args = arguments;
    LinkedList *list = args->list;
    bool flag = args->flag;
    //printf("flag: %d\n", args->flag);

    int bits = 0;
    int count = list->size / 2;
    if(list->size % 2 == 1){
        count += flag ? 0 : 1;
    }
    
    Node *tmp = flag ? list->tail : list->head;
    Node *next = NULL;
    for(int i = 0; i < count; i++){
        next = flag ? tmp->prev : tmp->next;
        //printf("value(%d): %d\n", flag, tmp->value);
        bits += flag ? num_1_bits(tmp->value) : num_0_bits(tmp->value);
        free(tmp);
        tmp = next;
    }

    printf("count(%d): %d\n", flag, count);
    printf("bits(%d): %d\n", flag, bits);
}

int main(){
    pthread_t threads[NTHREADS];
    struct arg_struct thread_args[NTHREADS];
    int rc;
    int size;

    LinkedList *list = createLinkedList();

    printf("Input size of list:\n");

    scanf("%d", &size);

    srandom(time(NULL));
    for(int i = 0; i < size; i++){
        int r = random();
        //printf("%d ", r);
        pushFront(list, r);
    }

    thread_args[0].flag = false;
    thread_args[1].flag = true;
    thread_args[0].list = list;
    thread_args[1].list = list;


    for (int i = 0; i < NTHREADS; i++)
    {    
      rc = pthread_create(&threads[i], NULL, fun, (void *) &thread_args[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        rc = pthread_join(threads[i], NULL);
    }

    free(list);
    return 1;
}
