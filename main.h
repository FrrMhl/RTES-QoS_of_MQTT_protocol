#ifndef MAIN_H_
#define MAIN_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#define N_TOPIC 3
#define N_SENDER 4
#define N_ITER 3
#define TIMEOUT 1000000

/*
    in order to execute the program with different N_LISTENER 
    from the script bash;
*/
#ifndef N_LISTENER
#define N_LISTENER 20
#endif

///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                    BROKER FUNCTIONS                               //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

/*
    mutex_topic:     binary mutex because broker can handle one topic at once;
    message[255]:     param where store message that broker receives; i manage one sender at time so i 
                      do not need space for all senders;

    message_is_arrived:     private semaphore where broker wait for new sender;
    waiting_end_broker:     private semaphore used by sender to wait the end of the broker
                            before gives the mutex_topic to the next sender;
    semaphore_subscriber[N_TOPIC][N_LISTENER]:     private semaphores where subscribere waiting new messages;
    matrix_of_subscriber[N_TOPIC][N_LISTENER]:     matrix to distinguish every listener; 
    message_dup:     param used by broker to process dup messages;
    message_dup_broker[N_TOPIC][N_LISTENER]:     param used by listeners to process dup messages;
    message_qos:     param used by broker to choose the correct policy to use;
    topic:     param used by broker to send message to the correct subscribers;
    puback:     param used by sender to stop sending the same message;
    puback_broker:     param used by broker to stop sending the same message;
    disconnected:     param used by listener to know when there are no more senders;

    pubrec:     param used by sender to stop sending the same message;
    pubrel:     param used by sender to say to the broker to process his message;
    pubcomp:     param used by broker to say to the sender that his message is delivered;
    pubrec_broker:     param used by broker to stop sending the same message;
    pubrel_broker:     param used by broker to say to the sender to process his message;
    pubcomp_broker:     param used by listener to say to the broker that his message is delivered;

    communication_id:     param used for better output;
    average_time[2]:     param used to calculate the difference in execution time for QoS 1 and 2;
    num_qos1:     param used to save the number of senders that use QoS 1;
    num_qos2:     param used to save the number of senders that use QoS 2;
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
    double average_time[2];
    int num_qos1;
    int num_qos2;
};

void init_broker(struct broker_t *);

void broker_send_with_qos_one(int, struct broker_t *);

void broker_receive_with_qos_one(int, struct broker_t *);

void broker_send_with_qos_two(int, struct broker_t *);

void broker_receive_with_qos_two(int, struct broker_t *);

void *broker_routine(void *);





///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                    LISTENER FUNCTIONS                             //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

void listener_receive_with_qos_one(int, int, struct broker_t *);

void listener_receive_with_qos_two(int, int, struct broker_t *);

void *listener_routine(void *);





///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                    SENDER FUNCTIONS                               //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

void sender_send_with_qos_one(int, int, struct broker_t *, int);

void sender_send_with_qos_two(int, int, struct broker_t *, int);

void *sender_routine(void *);

#endif