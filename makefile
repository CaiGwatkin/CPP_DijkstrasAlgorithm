dijkstrasAlgorithm.exe	:	dijkstrasAlgorithm.o
	g++ dijkstrasAlgorithm.o -std=c++11 -o dijkstrasAlgorithm.exe

dijkstrasAlgorithm.o	:	dijkstrasAlgorithm.cpp dijkstrasAlgorithm.h
	g++ -std=c++11 -c dijkstrasAlgorithm.cpp
	