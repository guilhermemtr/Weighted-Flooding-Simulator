#!/bin/bash
CC=g++
RM=rm -f

SRC_PATH=src/
UTILS_SRC_PATH=utils/
INC_PATH=include/ utils/
OBJ_PATH=obj/

INCS=$(foreach d, $(INC_PATH), -I$d)

# -fcilkplus -lcilkrts
# -fopenmp

CFLAGS=-Wall $(MODE) $(INCS)
LDFLAGS=-lpthread -lm

FILE_PREFIX=.cc
OBJ=.o

SOURCES=$(SRC_PATH)protocol_simulator.cc $(SRC_PATH)corruptions_stake_protocol_simulator.cc $(SRC_PATH)abstract_send_protocol_simulator.cc $(SRC_PATH)abstract_receive_protocol_simulator.cc $(SRC_PATH)send_protocol_simulator.cc $(SRC_PATH)generate_stake_distribution.cc $(SRC_PATH)receiver_protocol_simulator.cc $(SRC_PATH)hybrid_protocol_simulator.cc $(SRC_PATH)k_out_protocol_simulator.cc $(SRC_PATH)hybrid_k_out_protocol_simulator.cc $(SRC_PATH)protocol_simulator_run_data_extractor.cc $(SRC_PATH)corruptions_stake_protocol_simulator_run_data_extractor.cc $(SRC_PATH)random_sampler.cc $(SRC_PATH)random_utils.cc $(UTILS_SRC_PATH)io_stake_distribution.cc $(UTILS_SRC_PATH)read_args.cc $(UTILS_SRC_PATH)yal_log.cc $(UTILS_SRC_PATH)main_utils.cc $(UTILS_SRC_PATH)scoped_lock.cc


KOUT_SOURCES=main.cc $(SOURCES)
KOUT_BENCH_SOURCES=benchmark.cc $(SOURCES)
HYBRID_SOURCES=hybrid_main.cc $(SOURCES)


#OBJECTS=$(foreach f, $(SOURCES:$(FILE_PREFIX)=$(OBJ)), $(OBJ_PATH)$f)
#OBJECTS=$(SOURCES:$(FILE_PREFIX)=$(OBJ))

KOUT_OBJECTS=$(KOUT_SOURCES:$(FILE_PREFIX)=$(OBJ))
KOUT_OBJECTS=$(KOUT_BENCH_SOURCES:$(FILE_PREFIX)=$(OBJ))
HYBRID_OBJECTS=$(HYBRID_SOURCES:$(FILE_PREFIX)=$(OBJ))


KOUT_BENCH_EXECUTABLE=koutbenchbin
KOUT_EXECUTABLE=koutbin
HYBRID_EXECUTABLE=hybin


#MODE=$(DEBUG)
#MODE=$(SLOW_RELEASE)
#MODE=$(RELEASE)
MODE=$(PRELEASE)


PROFILING=-pg
PRELEASE=-O5 -std=gnu++20 -fopenmp
RELEASE=-O5 -std=gnu++20
SLOW_RELEASE=-std=gnu++20 -D OMP_NUM_THREADS=1
DEBUG=-g -D __YAL_ON__ -std=gnu++20

kout: $(KOUT_SOURCES) $(KOUT_EXECUTABLE)
	$(CC) $(CFLAGS) $(KOUT_OBJECTS) -o $(KOUT_EXECUTABLE) $(LDFLAGS)

kout-bench: $(KOUT_BENCH_SOURCES) $(KOUT_EXECUTABLE)
	$(CC) $(CFLAGS) $(KOUT_OBJECTS) -o $(KOUT_BENCH_EXECUTABLE) $(LDFLAGS)

hybrid: $(HYBRID_SOURCES) $(HYBRID_EXECUTABLE)
	$(CC) $(CFLAGS) $(HYBRID_OBJECTS) -o $(HYBRID_EXECUTABLE) $(LDFLAGS)


$(KOUT_EXECUTABLE):$(KOUT_OBJECTS)

$(HYBRID_EXECUTABLE):$(HYBRID_OBJECTS)

%$(OBJ): %$(FILE_PREFIX)
	$(CC) $(CFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	$(RM) $(KOUT_OBJECTS) $(HYBRID_OBJECTS) $(KOUT_EXECUTABLE) $(HYBRID_EXECUTABLE)
