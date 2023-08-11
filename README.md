# RTES-QoS_of_MQTT_protocol
The purpose of this project is to ***implement*** in C the two main qualities of service (QoS 1 and 2) of the MQTT messaging protocol
and ***compare*** the difference that there are in duration of interaction with both.

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

The two main QoS that you can use with this protocol are:
* ***At least once*** (QoS 1) = ...
* ***Exactly once*** (QoS 2) = ...

## Implementation

semafori...
scelte progettuali...

## Results
As can be seen from the following plot, the results obtained are in line with the actual behavior of QoS 1 and QoS 2; in fact, remembering
the nature of QoS 2 in which more checks have to be made to avoid processing messages multiple times and thus because of the 4 way handshake,
as the number of listeners increases, sending messages with QoS 2 becomes more onerous from the efficiency point of view than using QoS 1.

![plot_of_results](https://github.com/FrrMhl/RTES-QoS_of_MQTT_protocol/assets/64307404/74cd9e36-0654-4f72-874e-0f50144be65f)

While the x-axis depicts the number of listeners, the y-axis depicts the average time taken by all senders from when a message was sent to 
when they received the end-of-communication confirmation (PUBACK in QoS 1 and PUBCOMP in QoS 2). 


