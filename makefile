table-detect:
	g++ -o table-detect table-detect.cpp `pkg-config --cflags --libs opencv`
clean:
	rm -fr table-detect *~
