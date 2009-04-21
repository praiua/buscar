all:
	make -C src
	make -C buscar

clean:
	make -C src clean
	make -C buscar clean

