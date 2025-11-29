merge.exe: merge.o
	g++ -o merge.exe merge.o

merge.o: merge.cpp
	g++ -c merge.cpp

debug:
	g++ -g -o mergeDebug.out merge.cpp 

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups