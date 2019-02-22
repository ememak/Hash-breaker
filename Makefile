CUDA_HOME=/usr/local/cuda/

INC	:= -I$(CUDA_HOME)/include -I.
LIB	:= -L$(CUDA_HOME)/lib64 -lcudart

NVCCFLAGS	:= -O3 -lineinfo -arch=sm_35 --ptxas-options=-v --use_fast_math

GPU := -DGPU_CALC -UCPU_CALC
CPU := -DCPU_CALC -UGPU_CALC
CFLAGS := -O3 -fopenmp
DFLAGS :=

CFILES := Init.c HC_main.c cpu.c
CUFILES := Init.c HC_main.c gpu.cu
HFILES := Init.h gpu.h cpu.h

all: md5 gpu cpu

md5:
		gcc -o md5.x -O3 -lm md5_main.c

gpu:
		nvcc gpu.cu -c gpu.o $(INC) $(NVCCFLAGS) $(LIB)
		g++ Init.cpp HC_main.cpp gpu.o -o HC_gpu.x $(INC) $(GPU) $(LIB)
		rm gpu.o

cpu:
		g++ Init.cpp cpu.cpp HC_main.cpp -o HC_cpu.x $(CFLAGS) $(CPU)

clean:
		rm -f *.x *.o
