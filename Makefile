
.DUMMY: clean

all: tests

tests: lib
	(cd test; make run)

build: lib
	(cd test; make)

lib:
	(cd src; make)

clean:
	(cd test; make clean)
	(cd src; make clean)

