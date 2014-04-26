
.DUMMY: all, tests, clean

all: tests

tests:
	(cd test; make run)

clean:
	(cd test; make clean)

