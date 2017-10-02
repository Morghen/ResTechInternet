.SILENT:

CC = g++ -m64 -DLINUX  -Wall
OBJS = libUtils.o socketLib.o SocketException.o
PROGRAMS = ApplicationCheckIn ServerChekIn
TMP = "default"


ALL:
	clear
	clear
	echo ALL ...
	make ser
	make cli
	
ser: ServerCheckIn.cpp $(OBJS)
	echo Creation ServerCheckIn.o ...
	$(CC) ServerCheckIn.cpp $(OBJS) -o ServerCheckIn -lpthread -lrt -lnsl
	
cli: ApplicationCheckIn.cpp $(OBJS)
	echo Creation app client
	$(CC) ApplicationCheckIn.cpp $(OBJS) -o ApplicationCheckIn -lpthread -lrt -lnsl
	
Billet.o: Billet.cpp	Billet.h
	echo Creation de Billet.o
	$(CC) Billet.cpp -c -o Billet.o

libUtils.o: libUtils.cpp libUtils.h
	echo Creation de libUtils.o ...
	$(CC) libUtils.cpp -c -o libUtils.o
	
socketLib.o: socketLib.cpp socketLib.h
	echo Creation de socketLib.o ...
	$(CC) socketLib.cpp -c -o socketLib.o
	
SocketException.o: SocketException.cpp SocketException.h
	echo Creation de SocketException.o ...
	$(CC) SocketException.cpp -c -o SocketException.o

clean:
	clear
	clear
	echo Clean ...
	rm -f *.o ApplicationCheckIn ServerCheckIn

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

