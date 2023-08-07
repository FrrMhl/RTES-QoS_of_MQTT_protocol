#include "main.h"

struct broker_t broker;

int main(){

    pthread_attr_t a;
    pthread_t thread_listener[N_LISTENER];
    pthread_t thread_sender[N_SENDER];
    pthread_t thread_broker;

    srand(555);
    pthread_attr_init(&a);

    init_broker(&broker);

    /*
        creation broker, listeners and senders;
    */
    pthread_create(&thread_broker, &a, broker_routine, (void *)NULL);

    for(int i=0; i<N_LISTENER; i++)
        pthread_create(&thread_listener[i], &a, listener_routine, (void *)(intptr_t)(i));

    for(int i=0; i<N_SENDER; i++)
        pthread_create(&thread_sender[i], &a, sender_routine, (void *)(intptr_t)(i));

    pthread_attr_destroy(&a);

    /*
        waitig the end of listeners, senders and broker;
    */
    for (int j=0; j<N_LISTENER; j++)
        pthread_join(thread_listener[j], NULL);
    
    for (int j=0; j<N_SENDER; j++)
        pthread_join(thread_sender[j], NULL);
    
    pthread_join(thread_broker, NULL);

    /*
        printing same statistics
    */
    printf("\n          [ All senders and listeners are disconnected. ]\n\n");

    printf("\n          [Statistics ]\n");
    printf("          [Number of Senders = %d ]\n", N_SENDER);
    printf("          [Number of Listeners = %d ]\n", N_LISTENER);
    printf("          [Number of messages send by each Sender = %d ]\n", N_ITER);
    printf("          [Number of Topic to subscribe = %d ]\n", N_TOPIC);
    printf("          [Seconds to wait before resending one message = %f seconds ]\n", ((double)TIMEOUT)/CLOCKS_PER_SEC);

    double qos_one = broker.average_time[0] / broker.num_qos1;
    double qos_two = broker.average_time[1] / broker.num_qos2;
    printf("\n          [Average communication times ]\n");
    printf("          [QoS 1 = %f seconds ]\n", qos_one);
    printf("          [QoS 2 = %f seconds ]\n\n", qos_two);

    /*
        save result on files in order to plot next the results
    */
    FILE *fd_qos_one = fopen("results_QoS_1.txt", "a");
    FILE *fd_qos_two = fopen("results_QoS_2.txt", "a");

    char result_qos_one[50];
    char result_qos_two[50];
    sprintf(result_qos_one, "%d %f\n", N_LISTENER, qos_one);
    sprintf(result_qos_two, "%d %f\n", N_LISTENER, qos_two);

    fputs(result_qos_one, fd_qos_one);
    fputs(result_qos_two, fd_qos_two);

    fclose(fd_qos_one);
    fclose(fd_qos_two);

    return 0;
}