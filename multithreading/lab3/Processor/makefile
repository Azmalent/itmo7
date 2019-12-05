TARGET = processor
COMPILER_ARGS = -c -std=c99 -Wall -pedantic
LINKER_ARGS = -pthread

all: $(TARGET) clean

clean:
	rm -f *.o *.gcda *.gcno

coverage: COMPILER_ARGS += -O0 -fprofile-arcs -ftest-coverage
coverage: LINKER_ARGS += -fprofile-arcs -ftest-coverage
coverage: $(TARGET) test report clean

test:
	./tests.bats

report:
	gcovr -r . --html --html-details -o ../report/processor/processor.html

main.o: main.c
	gcc $(COMPILER_ARGS) -o main.o main.c

message.o: message.c
	gcc $(COMPILER_ARGS) -o message.o message.c

tasks.o: tasks.c
	gcc $(COMPILER_ARGS) -o tasks.o tasks.c

queue.o: queue.c
	gcc $(COMPILER_ARGS) -o queue.o queue.c

writer.o: writer.c
	gcc $(COMPILER_ARGS) -o writer.o writer.c

per_task.o: per_task.c
	gcc $(COMPILER_ARGS) -o per_task.o per_task.c

per_type.o: per_type.c
	gcc $(COMPILER_ARGS) -o per_type.o per_type.c

thread_pool.o: thread_pool.c
	gcc $(COMPILER_ARGS) -o thread_pool.o thread_pool.c

$(TARGET): main.o message.o tasks.o queue.o writer.o per_task.o per_type.o thread_pool.o
	gcc $(LINKER_ARGS) -o $(TARGET) $^ 