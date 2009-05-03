CPP = g++
OPTS = -Wall -O3
# OPTS = -Wall -g
INCLUDE = -I ../src

buscar: buscar.cc ../src/fnns.so
	$(CPP) $(OPTS) $(INCLUDE) buscar.cc ../src/fnns.so -o buscar

../src/fnns.so:
	make -C ../src fnns.so

clean:
	rm -f *~ *.o buscar
