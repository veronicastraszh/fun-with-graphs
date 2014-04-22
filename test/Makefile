include ../make.inc

VPATH=../include


.DUMMY: all

all: test
	./test

test: test.o | ../$(LIB)
	$(CPP) $(CPPOPTS) -o $@ $< -L .. -lfungraphs

%.o: %.cpp edge.h graph.h graph_algo.h heaps.h random_graph.h
	$(CPP) -c $(CPPOPTS) -I ../include -o $@ $<

clean:
	rm -f *.o
