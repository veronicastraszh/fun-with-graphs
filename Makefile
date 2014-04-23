
.DUMMY: clean

all: tests

tests: lib
	(cd test; make)

run: lib
	(cd test; make run)

lib:
	(cd src; make)

clean:
	(cd test; make clean)
	(cd src; make clean)

