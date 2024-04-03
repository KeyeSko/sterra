#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <getopt.h>
#include "node.h"

#define NTHREADS 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct arg_struct {
    int flag;
    LinkedList *list;
    int bits;
    int count;
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
    int flag = args->flag;

    int bits = 0, count = 0;
    
    Node *next = NULL;
    Node *current = NULL;

    for(;;){

        pthread_mutex_lock(&mutex);

        switch(flag){
        case 0:
            current = list->head;
            next = current ? current->next : NULL;
            if(next) next->prev = NULL;

			/*
			 * if data->head_tail->prev == data->head_tail->next,
			 * then this element is the last one to be processed,
			 * so we must NULL both pointers, otherwise,
			 * second thread will start processing already
			 * freed element by first thread
			 */
            if(list->tail == list->head){
                list->head = NULL;
                list->tail = NULL;
            }else{
                list->head = next;
            }

            break;
        case 1:
             /*
			 * if data->head_tail->prev == data->head_tail->next,
			 * then this element is the last one to be processed,
			 * let the first thread to process it, because
			 * the first thread is more privileged
			 */
            if(list->tail == list->head){
                pthread_mutex_unlock(&mutex);
                return NULL;
            }

            current = list->tail;
            next = current ? current->prev : NULL;
            if(next) next->next = NULL;
            list->tail = next;

            break;
        }

        pthread_mutex_unlock(&mutex);

        if(!current)
            return NULL;

        args->bits += flag ? num_1_bits(current->value) : num_0_bits(current->value);
        args->count++;

        free(current);
    }
    return NULL;
}



int main(int argc, char *argv[]){
    pthread_t threads[NTHREADS];
    struct arg_struct thread_args[NTHREADS];
    int size = -1, rc;

    static struct option options[] = {{"size", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0; 
    for(;;)
    {
        int c = getopt_long(argc, argv, "f", options, &option_index);
        if (c == -1) break;
        switch(c)
        {
            case 0:
            switch(option_index)
            {
                case 0:
                    size = atoi(optarg);
				    if(size < 1)
					    size = -1;
				    break;                
                }
                break;
                case ':':
			        printf("no argument for option\n");
			        return 0;
		        case '?':
			        printf("wrong option\n");
			        return 0;
                default:
                    printf("Index %d is out of options\n", option_index);
                }
    }

    if(size == -1){
        printf("A positive integer is accepted as input\n");
        printf("Using: --size 10");
        exit(1);
    }

    LinkedList *list = createLinkedList();
    if(!list){
        perror("main error: memory can't be allocate");
        exit(1);
    }

    srandom(time(NULL));
    for(int i = 0; i < size; i++){
        int r = random();
        pushFront(list, r);
    }

    thread_args[0].flag = 0;
    thread_args[1].flag = 1;
    thread_args[0].list = list;
    thread_args[1].list = list;


    for (int i = 0; i < NTHREADS; i++)
    {    
        rc = pthread_create(&threads[i], NULL, fun, (void *) &thread_args[i]);
        if (rc != 0) {
            perror("main error: thread can't be create\n");
            exit(1);
            }
    }

    for (int i = 0; i < NTHREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            perror("main error: thread can't be join\n");
            exit(1);
            }
    }

    for(int i = 0; i < NTHREADS; i++){
        printf("results on the %d thread: bits = %d, count = %d\n", i, thread_args[i].bits, thread_args[i].count);
    }

    free(list);
    list = NULL;
    return 0;
}
