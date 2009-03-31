# CPP = g++-4.1
CPP = g++
# OPTS = -Wall -g
OPTS = -Wall -O3

all: buscar.so

FONTS = uniformOracle.cc bruteForce.cc mdf.cc checkInNNAlg.cc checkInOracle.cc 
OBJ = uniformOracle.o bruteForce.o mdf.o checkInNNAlg.o checkInOracle.o 

FILTER = 2>&1 | head -10

buscar.so: ${OBJ} 
	${CPP} ${OPTS} ${OBJ} -shared -o buscar.so ${FILTER}
	# "-------------------"

# --- ORACLES ---

uniformOracle.o : uniformOracle.cc uniformOracle.h oracle.h checkInOracle.h
	${CPP} ${OPTS} -c uniformOracle.cc ${FILTER}

# ---- NN Algorithms ---

bruteForce.o: bruteForce.cc bruteForce.h nnAlg.h checkInNNAlg.h
	${CPP} ${OPTS} -c bruteForce.cc ${FILTER}

mdf.o: mdf.cc mdf.h nnAlg.h checkInNNAlg.h
	${CPP} ${OPTS} -c mdf.cc ${FILTER}

# --- Other ---

checkInNNAlg.o: checkInNNAlg.cc checkInNNAlg.h nnAlg.h
	${CPP} ${OPTS} -c checkInNNAlg.cc ${FILTER}

checkInOracle.o: checkInOracle.cc checkInOracle.h oracle.h
	${CPP} ${OPTS} -c checkInOracle.cc ${FILTER}

# --- Utilities ---

install: buscar.so
	cp buscar.so ../lib
	cp *.h ../include
	
clean:
	rm -f *.o buscar
