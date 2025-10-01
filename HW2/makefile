useHeap.exe: useHeap.o heap.o hash.o
	g++ -o useHeap.exe useHeap.o heap.o hash.o

useHeap.o: useHeap.cpp
	g++ -c useHeap.cpp

heap.o: heap.cpp heap.h
	g++ -c heap.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

debug:
	g++ -g -o useHeapDebug.exe useHeap.cpp heap.cpp hash.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
