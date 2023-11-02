all: GigaSoft

GigaSoft: ./src/main.cpp
	g++ ./src/*.cpp ./src/*.hpp -lX11 -lboost_system `pkg-config --cflags --libs gtk+-3.0` -o GigaSoft

clean:
	rm -f GigaSoft


