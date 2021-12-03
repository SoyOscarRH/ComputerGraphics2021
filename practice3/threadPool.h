#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

struct Values{
    int x;
    int y;
};

typedef struct Values Values;

struct Task{
    void (*taskFunction)(void *);
    void *args;
    struct Task *next;
};

typedef struct Task Task;

struct ThreadPool{
    Task *first;
    Task *last;
    pthread_mutex_t mutexQueue;
    pthread_mutex_t mutexImage;
    pthread_mutex_t mutexTWorking;
    pthread_mutex_t mutexTotal;
    pthread_mutex_t mutexDone;
    pthread_cond_t condQueue;
    int totalTasks;
    int doneTasks;
    int threadsWorking;
    int stop;   
};

typedef struct ThreadPool ThreadPool;

void addTask(ThreadPool *pool, void (*function)(void *), void *args);
Task *getTask(ThreadPool *pool);

void *runThread(void *args);
void *masterThread(void *args);

#endif