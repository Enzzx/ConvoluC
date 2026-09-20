CC = gcc

ifeq ($(OS),Windows_NT)
    RM = del /q
    EXE = .exe
    TARGET_LIB = image_processor.dll
    RUN_CMD = image_processor.exe
else
    RM = rm -f
    EXE =
    TARGET_LIB = image_processor.so
    RUN_CMD = ./image_processor
endif

CLI_CFLAGS = -Wall -Wextra -O3 -fopenmp $(if $(filter Windows_NT,$(OS)),-mthreads,-pthread)
CLI_LDFLAGS = -fopenmp $(if $(filter Windows_NT,$(OS)),-mthreads,-pthread) -lm
CLI_SRCS = src/main.c src/transform.c src/utils.c src/clio.c
CLI_OBJS = $(CLI_SRCS:.c=.o)
TARGET_CLI = image_processor$(EXE)

LIB_CFLAGS = -Wall -Wextra -O3 -fopenmp -fPIC -std=gnu99 -D_CRT_SECURE_NO_WARNINGS -DSTBIW_NOTUSED
LIB_LDFLAGS = -fopenmp
LIB_SRCS = src/transform.c src/utils.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

all: cli lib

cli: $(CLI_OBJS)
	$(CC) $(CLI_CFLAGS) -o $(TARGET_CLI) $(CLI_OBJS) $(CLI_LDFLAGS)

lib: $(LIB_OBJS)
	$(CC) $(LIB_CFLAGS) -shared -o $(TARGET_LIB) $(LIB_OBJS) $(LIB_LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CLI_CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	$(RM) src\*.o $(TARGET_CLI) $(TARGET_LIB)
else
	$(RM) src/*.o $(TARGET_CLI) $(TARGET_LIB)
endif

run: cli
	$(RUN_CMD)

.PHONY: all cli lib clean run