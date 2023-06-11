CC = gcc

CFLAGS = -Wall -Iincludes `pkg-config --cflags glib-2.0` -O2  

LDFLAGS= `pkg-config --libs glib-2.0` -lm -lncursesw


programa-principal : Resultados src/main.o src/iterativo.o src/automatic_tests.o src/drivers.o src/auxiliares.o src/parsing.o src/outputs.o src/users.o src/rides.o src/driversExtensions.o src/usersExtensions.o src/ridesExtensions.o src/catalogo.o
	$(CC) $(CFLAGS) src/main.o src/drivers.o src/iterativo.o src/auxiliares.o src/parsing.o src/outputs.o src/users.o src/rides.o src/driversExtensions.o src/usersExtensions.o src/ridesExtensions.o src/catalogo.o $(LDFLAGS) -o $@
	$(CC) $(CFLAGS) src/automatic_tests.o src/drivers.o src/auxiliares.o src/parsing.o src/outputs.o src/users.o src/rides.o src/driversExtensions.o src/usersExtensions.o src/ridesExtensions.o src/catalogo.o $(LDFLAGS) -o programa-testes


Resultados:
	mkdir Resultados

src/main.o : src/main.c

src/automatic_tests.o : src/automatic_tests.c

src/iterativo.o : src/iterativo.c

src/catalogo.o : src/catalogo.c

src/drivers.o : src/drivers.c

src/auxiliares.o : src/auxiliares.c

src/parsing.o : src/parsing.c

src/outputs.o : src/outputs.c

src/users.o : src/users.c

src/rides.o : src/rides.c

src/driversExtensions.o : src/driversExtensions.c

src/usersExtensions.o : src/usersExtensions.c

src/ridesExtensions.o : src/ridesExtensions.c

clean:
	rm -f main src/*.o Resultados/*
	rmdir Resultados
	rm programa-principal
	rm programa-testes
	rm Errors_File.txt
