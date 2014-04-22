include ../make.inc

VPATH=../include

OBJ=edge.o random_graph.o

.DUMMY: all

all: ../$(LIB)

../$(LIB): $(OBJ)
	$(ARCH) -static -o ../$(LIB) $(ARCHOPTS) $(OBJ)

%.o: %.cpp edge.h graph.h graph_algo.h heaps.h random_graph.h
	$(CPP) -c $(CPPOPTS) -I ../include -o $@ $<

clean:
	rm -f *.o
