
CPP = g++
# OPTS = -Wall -g
OPTS = -Wall -O3


all: buscar #exp exp2

OBJ = uniformOracle.o bruteForce.o mdf.o checkInNNAlg.o checkInOracle.o lib_string.o



buscar: buscar.cc ${OBJ}
	${CPP} ${OPTS} buscar.cc ${OBJ} -o buscar


lib_string.o: lib_string.cc lib_string.h
	${CPP} ${OPTS} -c lib_string.cc

checkInNNAlg.o: checkInNNAlg.cc checkInNNAlg.h nnAlg.h
	${CPP} ${OPTS} -c checkInNNAlg.cc

checkInOracle.o: checkInOracle.cc checkInOracle.h oracle.h
	${CPP} ${OPTS} -c checkInOracle.cc

#exp: exp.cc ${OBJ} nnAlg.h checkInNNAlg.h checkInOracle.h
#	${CPP} ${OPTS} exp.cc ${OBJ} -o exp 
# "-------------------"

#exp2: exp2.cc ${OBJ} nnAlg.h checkInNNAlg.h checkInOracle.h
#	${CPP} ${OPTS} exp2.cc ${OBJ} -o exp2
# "-------------------"

clean:
	rm -f *.o buscar


####################################
# --- ORACLES ---

uniformOracle.o : uniformOracle.cc uniformOracle.h oracle.h checkInOracle.h
	${CPP} ${OPTS} -c uniformOracle.cc


#####################################
# --- NN Algorithms ---

bruteForce.o: bruteForce.cc bruteForce.h nnAlg.h checkInNNAlg.h
	${CPP} ${OPTS} -c bruteForce.cc

mdf.o: mdf.cc mdf.h nnAlg.h checkInNNAlg.h
	${CPP} ${OPTS} -c mdf.cc




