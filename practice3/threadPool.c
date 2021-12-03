#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadPool.h"

void *runThread(void *args){

    printf("Hilo creado...\n");

    ThreadPool *pool = (ThreadPool*)args;
    Task *nextTask;

    
    while(!pool->stop && pool->last != NULL){

        nextTask = NULL;

        pthread_mutex_lock(&pool->mutexQueue);
        while(pool->last == NULL && !pool->stop){
            //printf("No hay tareas );\n");
            pthread_cond_wait(&pool->condQueue, &pool->mutexQueue); 
        }
        
        if(pool->last != NULL){
            nextTask = pool->last;
            pool->last = pool->last->next;
        }
        pthread_mutex_unlock(&pool->mutexQueue);

        if(nextTask != NULL){
            //printf("Warking\n");
            nextTask->taskFunction(nextTask->args);

            /*        
            pthread_mutex_lock(&pool->mutexDone);
            pool->doneTasks++;
            if(pool->doneTasks % 1000 == 0)
                printf("Done: %d\n", pool->doneTasks);
            pthread_mutex_unlock(&pool->mutexDone);            
            */

            //free(nextTask->args);
        }

    }
    
    return NULL;
}

void *masterThread(void *args){
    
    
    //int stopCondition = 0;
    //int lastCheckQueue = 0;
    ThreadPool *pool = (ThreadPool*)args;

    /*
    while(!stopCondition){

        pthread_mutex_lock(&pool->mutexQueue);
        if(pool->doneTasks >= pool->totalTasks){
            lastCheckQueue++;
        }else{
            lastCheckQueue = 0;
        }
        pthread_mutex_unlock(&pool->mutexQueue);

        if(lastCheckQueue == 2){
            stopCondition = 1;
        }
        sleep(1);
    }
    */

    //int var;
    //scanf("%d", &var);
    sleep(1);

    printf("Maestro terminado...\n");
    pthread_cond_broadcast(&pool->condQueue);
    pool->stop = 1;
    return NULL;

}

void addTask(ThreadPool *pool, void (*function)(void *), void *args){
    
    Task *newTask = (Task*)malloc(sizeof(Task));
    newTask->taskFunction = function;
    newTask->args = args;
    newTask->next = NULL;    
    
    pthread_mutex_lock(&pool->mutexQueue);
    if(pool->first == NULL){
        //printf("Nuevo pool...\n");
        pool->first = newTask;
        pool->last = pool->first;
    }else{
        //printf("Poool existente...\n");
        pool->first->next = newTask;
        pool->first = pool->first->next;
    }
    pthread_mutex_unlock(&pool->mutexQueue);
    
    /*
    pthread_mutex_lock(&pool->mutexTotal);
    pool->totalTasks++;
    pthread_mutex_unlock(&pool->mutexTotal);
    */

    pthread_cond_signal(&pool->condQueue);
}
