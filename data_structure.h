#ifndef DATA_STRUCTURE_H_
#define DATA_STRUCTURE_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define N_TOPIC 2
#define N_SENDER 10
#define N_LISTENER 4
#define N_ITER 3

extern struct broker_t broker;
void init_broker(struct broker_t *b);

#endif