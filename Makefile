CC = g++
ARG = -g
BUILD_DIR = ./build
OBJ = util.o scheduler.o

all:$(OBJ)
	$(CC) $(ARG) ./scheduler_test.cpp $(BUILD_DIR)/*.o -o ./main -lpthread

util.o:
	$(CC) $(ARG) -c ./util.cpp -o $(BUILD_DIR)/util.o

scheduler.o:
	$(CC) $(ARG) -c ./scheduler.cpp -o $(BUILD_DIR)/scheduler.o

clean:
	rm ./build/*.o
