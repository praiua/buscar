# CXX = g++-3.4
# CXX = g++-3.3
# CXX = g++-3.0
# CXX = g++-2.95
# CXX = g++-4.0

# OPTS = -O3 -Wall -IInclude -march=athlon -mcpu=athlon
OPTS = -O3 -Wall -IInclude
# OPTS = -g -Wall -IInclude 

LIB = -lm
# LIB = -lm -lefence

# a probar: -Weffc++ -Wold-style-cast -Woverloaded-virtual
# all : buscar
# all : pesos

FONTS = $(wildcard Lib/*.C)
OBJ = $(subst Lib,Obj,$(FONTS:.C=.o))

# all: buscar bayes cmeans pesos prugauss pruuni prumat miEdit
all: buscar

install:
	mv buscar ${HOME}/Bin

Obj/libclasif.so: ${OBJ}
	${CXX} ${OPTS} -shared -o Obj/libclasif.so ${OBJ}

Obj/%.o :: Lib/%.C
	${CXX} ${OPTS} Lib/$*.C -o Obj/$*.o -c

Obj/%.o :: %.C
	${CXX} ${OPTS} $*.C -o Obj/$*.o -c

%.o : %.C
	${CXX} ${OPTS} $*.C -c

#----------- buscar 

buscar : buscar.o Obj/libclasif.so
	${CXX} $(OPTS) -o $@ buscar.o ./Obj/libclasif.so $(LIB)

#----------- OTROS

clean :
	rm -f buscar *.o core *.bak *~
	rm -f Include/*.bak Include/*~
	rm -f Lib/*.bak Lib/*~
	for i in *.C; do rm -f $${i%.C}.o; done
	rm -f Obj/*.o Obj/*.so

depend:
#	makedepend -pObj/ -I/usr/include/g++-2 -IInclude *.C
#	makedepend -I/usr/include/g++-2 -IInclude *.C
	${CXX} -MM -IInclude *.C > dependencias
	${CXX} -MM -IInclude Lib/*.C | \
	  awk '/.*:/ {print "Obj/"$$0;next;}{print $0}' >> dependencias

include dependencias
