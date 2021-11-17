
all : release

debug :
	g++ main.cpp curve.cpp data_helper.cpp -O0 -g -std=c++2a -Wall -Wpedantic -lpthread

release :
	g++ main.cpp curve.cpp data_helper.cpp -O3 -std=c++2a -Wall -Wpedantic -lpthread
	
