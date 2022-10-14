ifeq (${shell uname},Darwin)
	CC=g++ -g -Wall -std=c++17 -D_XOPEN_SOURCE
	LIBCPU=libcpu_macos.o
else
	CC=g++ -g -Wall -std=c++17
	LIBCPU=libcpu.o
endif

# List of source files for your thread library
THREAD_SOURCES=cpu.cpp cpu_impl.cpp cv.cpp cv_impl.cpp mutex.cpp mutex_impl.cpp thread.cpp thread_impl.cpp

# Generate the names of the thread library's object files
THREAD_OBJS=${THREAD_SOURCES:.cpp=.o}

all: libthread.o app

# Compile the thread library and tag this compilation
libthread.o: ${THREAD_OBJS}
	./autotag.sh
	ld -r -o $@ ${THREAD_OBJS}

# Compile an application program
app: test6.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -rf ${THREAD_OBJS} libthread.o app* core*