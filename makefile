.SILENT:

CC = g++ -m64 -DLINUX  -Wall
OBJS = ServerCheckIn.o libUtils.o
PROGRAMS = Application CheckIn Serveur
TMP = "default"

ALL:
	clear
	clear
	echo ALL ...
	make ser
	
ser: ServerCheckIn.cpp libUtils.o
	echo Creation ServerCheckIn.o ...
	$(CC) ServerCheckIn.cpp libUtils.o -o ServerCheckIn -lpthread -lrt		  

libUtils.o: libUtils.cpp libUtils.h
	echo Creation de libUtils.o ...
	$(CC) libUtils.cpp -c -o libUtils.o

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

