include ../make.inc

VPATH=../include


.DUMMY: run, all, clean

all: shortest_path walks

run: all
	./shortest_path
	./walks

shortest_path: shortest_path.cpp edge.h graph.h shortest_paths.h heaps.h graph_utils.h
	$(CPP) $(CPPOPTS) -I ../include -o $@ $<

walks: walks.cpp edge.h graph.h walks.h graph_utils.h
	$(CPP) $(CPPOPTS) -I ../include -o $@ $<

#%.o: %.cpp edge.h graph.h graph_algo.h heaps.h graph_utils.h
#	$(CPP) -c $(CPPOPTS) -I ../include -o $@ $<

clean:
	rm walks
	rm shortest_path
	rm -f *.o
	rm -fr *.dSYM
