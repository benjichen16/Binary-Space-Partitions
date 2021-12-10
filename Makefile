room:	room.o Wall.h Wall.o List.h List.o BSP.h BSP.o
	cc -o prog4 `pkg-config --cflags --libs cairo` -lm room.o BSP.o Wall.o List.o

%.o: %.c
	cc -o $@ -c -g `pkg-config --cflags --libs cairo` $< -std=c99

clean:
	rm prog4 *.o
