.SILENT:

CC = g++ -m64 -DLINUX  -Wall
OBJS = ServerCheckIn.o libUtils.o socketLib.o
PROGRAMS = CheckIn Serveur
TMP = "default"

ALL:
	clear
	clear
	echo ALL ...
	make ser
	
ser: ServerCheckIn.cpp libUtils.o socketLib.o
	echo Creation ServerCheckIn.o ...
	$(CC) ServerCheckIn.cpp libUtils.o socketLib.o -o ServerCheckIn -lpthread -lrt -lnsl -lsocket	  

libUtils.o: libUtils.cpp libUtils.h
	echo Creation de libUtils.o ...
	$(CC) libUtils.cpp -c -o libUtils.o
	
socketLib.o: socketLib.cpp socketLib.h
	echo Creation de socketLib.o ...
	$(CC) socketLib.cpp -c -o socketLib.o

clean:
	clear
	clear
	echo Clean ...
	rm -f $(OBJS) core

clobber:	clean
	clear
	clear
	echo Clobber ...
	rm -f tags $(PROGRAMS)

git:
	clear
	clear
	git add -A
	git commit -m "$(TMP)"
	git push

