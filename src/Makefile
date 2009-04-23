CPP = g++
# OPTS = -Wall -g 
# OPTS = -Wall -O3 -std=c++0x
OPTS = -Wall -O3 

FONTS = $(wildcard *.cc)

OBJ = $(patsubst %.cc,%.o,$(FONTS))


all: fnns.so

fnns.so: $(OBJ)
	$(CPP) $(OPTS) $(OBJ) -shared -o fnns.so 

%.o: %.cc
	$(CPP) $(OPTS) $< -c -o $@
	
clean:
	rm -f *~ *.o fnns.so

#----------------- Dependences managenent

depend:
	${CXX} -MM *.cc > dependences

include dependences

