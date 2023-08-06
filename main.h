#ifndef MAIN_H_
#define MAIN_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define N_TOPIC 3
#define N_SENDER 4
#define N_LISTENER 400
#define N_ITER 3
#define TIMEOUT 99999

///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                    BROKER FUNCTION                                //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

/*
    binary mutex because broker can handle one topic at once;

    param where store message that broker receives; i manage one sender at time so i 
    do not need space for all senders;

    private semaphore where broker wait for new sender;

    private semaphore used by sender to wait the end of the broker
    before gives the mutex_topic to the next sender;

    private semaphores where subscribere waiting new messages;

    matrix to distinguish every listener; 

    param used by broker to process dup messages;

    param used by listeners to process dup messages;

    param used by broker to choose the correct policy to use;

    param used by broker to send message to the correct subscribers;

    param used by sender to stop sending the same message;

    param used by broker to stop sending the same message;

    param used by listener to know when there are no more senders;

    param used by sender to stop sending the same message;
    
    param used by sender to say to the broker to process his message;

    param used by broker to say to the sender that his message is delivered;

    param used by broker to stop sending the same message;
    
    param used by broker to say to the sender to process his message;

    param used by listener to say to the broker that his message is delivered;

    param used for better output
*/
struct broker_t{

    sem_t mutex_topic;
    char message[255];

    sem_t message_is_arrived;
    sem_t waiting_end_broker;
    sem_t semaphore_subscriber[N_TOPIC][N_LISTENER];
    int matrix_of_subscriber[N_TOPIC][N_LISTENER];
    int message_dup;
    int message_dup_broker[N_TOPIC][N_LISTENER];
    int message_qos;
    int topic;
    int puback;
    int puback_broker;
    int disconnected;

    int pubrec;
    int pubrel;
    int pubcomp;
    int pubrec_broker;
    int pubrel_broker;
    int pubcomp_broker;

    int communication_id;
};

void init_broker(struct broker_t *);

void broker_send_with_qos_one(int, struct broker_t *);

void broker_receive_with_qos_one(int, struct broker_t *);

void broker_send_with_qos_two(int, struct broker_t *);

void broker_receive_with_qos_two(int, struct broker_t *);

void *broker_routine(void *);





///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                    LISTENER FUNCTION                              //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

void listener_receive_with_qos_one(int, int, struct broker_t *);

void listener_receive_with_qos_two(int, int, struct broker_t *);

void *listener_routine(void *);





///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                    SENDER FUNCTION                                //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

void sender_send_with_qos_one(int, int, struct broker_t *, int);

void sender_send_with_qos_two(int, int, struct broker_t *, int);

void *sender_routine(void *);

#endif