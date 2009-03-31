# CPP = g++-4.1
CPP = g++
# OPTS = -Wall -g
OPTS = -Wall -O3

all: buscar exp exp2

OBJ = uniformOracle.o bruteForce.o mdf.o checkInNNAlg.o checkInOracle.o 

FILTER = 2>&1 | head -10

buscar: buscar.cc ${OBJ} nnAlg.h checkInNNAlg.h checkInOracle.h
	${CPP} ${OPTS} buscar.cc ${OBJ} -o buscar ${FILTER}
	# "-------------------"

exp: exp.cc ${OBJ} nnAlg.h checkInNNAlg.h checkInOracle.h
	${CPP} ${OPTS} exp.cc ${OBJ} -o exp ${FILTER}
	# "-------------------"

exp2: exp2.cc ${OBJ} nnAlg.h checkInNNAlg.h checkInOracle.h
	${CPP} ${OPTS} exp2.cc ${OBJ} -o exp2 ${FILTER}
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

clean:
	rm -f *.o buscar
