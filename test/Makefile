include ../make.inc

VPATH=../include


.DUMMY: all run_all

all: shortest_path walks

run: all
	./shortest_path
	./walks

shortest_path: shortest_path.o
	$(CPP) $(CPPOPTS) -o $@ $< -L .. -lfungraphs

walks: walks.o
	$(CPP) $(CPPOPTS) -o $@ $< -L .. -lfungraphs

%.o: %.cpp edge.h graph.h graph_algo.h heaps.h random_graph.h graph_utils.h
	$(CPP) -c $(CPPOPTS) -I ../include -o $@ $<

clean:
	rm -f *.o
