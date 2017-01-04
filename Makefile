all: segDents

segDents: segDents.cpp
	g++  -o segDents segDents.cpp -O2 -L/usr/X11R6/lib  -lm  -lpthread -lX11
	
run: segDents
	./segDents img1.img

clean:
	rm -f *.o *~ segDents
