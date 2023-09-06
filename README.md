# RTES-QoS_of_MQTT_protocol
The purpose of this project is to ***implements*** in C the two main qualities of service (QoS 1 and 2) of the MQTT messaging protocol
and ***compares*** the difference that there are in duration of interaction with both.

## Overview
Specifically, ***MQTT*** (Message Queuing Telemetry Transport) is a messaging protocol used primarily by Internet of Things devices (IoT)
that typically need to transmit and receive data over a network with limited resources and limited bandwidth.

There are 3 basic components and we have to keep in mind that the key point is that sender and receiver do not communicate directly
but rely on an intermediary called broker to exchange messages. 
<br>
So, in the communication partecipate:
* ***Senders*** = these are the devices that, based on their exigencies and quality of their network, decide to use a particular QoS; after this, they
  publish their message on a specific topic on the broker. 
* ***Broker*** = is the intermediary between senders and listeners; specifically, he manages all the various messages and, based on the topic on which
  a sender has posted the message, he forwards it to the listeners who have subscribed to that particular topic.
* ***Listeners*** = these are certain devices that, based on their interests, decide to subscribe to certain topics on the broker so that when messages
  arrive to the broker referring to that topic, they automatically receive them.

The two main QoS that you can use with this protocol (and that have been implemented) are:
* ***At least once*** (QoS 1) = with this quality of service, there is ***certainty*** that listeners have received the message, but it could also be the case     that messages are ***delivered more than once***.
<br> In more detail, the operation consists of the sender sending a message and if within a certain TIMEOUT it has not received a reception response (PUBACK) it resends it. The broker takes the newly received message and sends it to all the listeners subscribed to that particular topic and waits for all of them to send their acknowledgement (PUBACK). Eventually, once the broker has received all the PUBACKs, he also sends it to the sender who will be assured that all the listeners have received his message.
<br> Due to connection problems, it might happen that too much time passes without a response from when the sender sends the message, so he sends the message again, which will be processed by the broker and sent to the listeners for a second time.
<br> Below you can see a representation of a connection with QoS 1;

  ![Diagramma_qos1](https://github.com/FrrMhl/RTES-QoS_of_MQTT_protocol/assets/64307404/2e3dfb5e-a510-413a-94fe-aedbc9594ac6)

* ***Exactly once*** (QoS 2) = this represents the most reliable quality of service but also the slowest because of the ***4 way handshake*** that allows the MQTT protocol to deliver the message exactly once without having duplicates and to have the certainty that listeners have received the message.
<br> Regarding specifically how it works, the sender initially sends a message to the broker and if it does not respond within a certain TIMEOUT it will be sent back; the broker, once received, sends confirmation of receipt to the sender (PUBREC). Then the sender sends the PUBREL message to the broker, which is intended to let him know that from that point on he will not receive any more messages and can therefore process the message having the assurance that he will not receive duplicates. Similarly, the broker performs this 4 way handshake with the listeners and then they too will be assured that they will not process the same message multiple times. Eventually all the listeners send the end-of-processing message (PUBCOMP) to the broker who will also send the same to the sender and with this, a connection with QoS 2 is concluded.
<br> Below you can see a representation of a connection with QoS 2;

  ![Diagramma_qos2](https://github.com/FrrMhl/RTES-QoS_of_MQTT_protocol/assets/64307404/f1faa06e-1841-4d5d-9015-98123136f421)



## Implementation
In terms of design choices, it was decided to make the broker a shared resource among the various senders and then manage access to it through ***semaphores***.
Specifically, the semaphores implemented are:
* ***mutex_topic*** = is a binary semaphore that allows one sender at a time to access the broker resource.
* ***message_is_arrived*** = is a private semaphore on which the broker waits for the next message from a sender.
* ***waiting_end_broker*** = is a private semaphore on which the current sender waits for the end of the broker in handling his message; in this way, once he wakes up, via the mutex_topic semaphore he can wakes up the next sender.
* ***semaphore_subscriber*** = is a matrix of private semaphores (one for each listeners and topic) where subscribers waiting new messages from the broker.

In addition, it was decided to have available:
* 4 senders.
* 3 interation, and so 3 messages, for each sender. 
* 3 topic (randomly assigned among the various listeners and senders);
* 1 second of timeout before re-sendig the message in case of too much time without any response.
* 20 listeners for the generic test and from 1 to 1000 listeners to compare the performance between sending message wit QoS 1 and QoS 2.

Of course, these parameters could be changed by going into the ***main.h*** file.

## Results
As can be seen from the following plot, the results obtained are in line with the actual behavior of QoS 1 and QoS 2; in fact, remembering
the nature of QoS 2 in which more checks have to be made to avoid processing messages multiple times and thus because of the 4 way handshake,
as the number of listeners increases, sending messages with QoS 2 becomes more onerous from the efficiency point of view than using QoS 1.

![plot_of_results](https://github.com/FrrMhl/RTES-QoS_of_MQTT_protocol/assets/64307404/74cd9e36-0654-4f72-874e-0f50144be65f)

While the x-axis depicts the number of listeners, the y-axis depicts the average time taken by all senders from when a message was sent to 
when they received the end-of-communication confirmation (PUBACK in QoS 1 and PUBCOMP in QoS 2). 


