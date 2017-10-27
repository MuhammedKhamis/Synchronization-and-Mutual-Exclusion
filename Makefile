CC=gcc
CFLAGS=-Wall -Wno-unused-value

caltrain: caltrain.o caltrain-runner.o -lpthread
	$(CC) -o $@ $^ $(CFLAGS)
