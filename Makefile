CC = gcc
CFLAGS = -Wall -Wextra -I./src -O3

LIB_SRCS = src/dict.c src/json.c src/variable.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

APP_SRCS = examples/main.c
APP_OBJS = $(APP_SRCS:.c=.o)

APP_TARGET = cdict_app
LIB_TARGET = libcdict.a

all: $(APP_TARGET) run

lib: $(LIB_TARGET)


$(APP_TARGET): $(APP_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(LIB_TARGET): $(LIB_OBJS)
	ar rcs $@ $^
	@echo "Library created: $(LIB_TARGET)"

run:
	@echo "\n--- RUNNING APPLICATION ---"
	@./$(APP_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o examples/*.o $(APP_TARGET) $(LIB_TARGET)