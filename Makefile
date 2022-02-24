CC=gcc
CFLAGS=-Wall -lrt

build: Starter HarmonicSeries HexagonalSeries Lucas Reader

Starter: Starter.c
	$(CC) -o Starter $(CFLAGS) Starter.c

HarmonicSeries: HarmonicSeries.c
	$(CC) -o harmonicseries $(CFLAGS) HarmonicSeries.c

HexagonalSeries: HexagonalSeries.c
	$(CC) -o hexagonalseries $(CFLAGS) HexagonalSeries.c

Lucas: Lucas.c
	$(CC) -o lucas $(CFLAGS) Lucas.c

Reader: Reader.c
	$(CC) -o reader $(CFLAGS) Reader.c

clean:
	rm -rf *.o Starter HarmonicSeries HexagonalSeries Lucas

test:
	./Starter file_01.in

tar:
	tar -cvzf Nick-Korte-HW3.tar Starter.c Lucas.c HexagonalSeries.c HarmonicSeries.c Makefile README.txt





