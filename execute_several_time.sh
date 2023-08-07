#!/bin/bash
for i in {1..1000}
do
    gcc -Wall -o progetto main.c broker.c listener.c sender.c -DN_LISTENER=$i -lpthread && ./progetto
done