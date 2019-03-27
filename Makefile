cc = gcc
CFLAGS = -W -Wall -Werror

all :
	make places
	make concert
	make achat

places: places.o
	$(cc) places.o -o places

places.o: places.c gestionnaire_concert.h
	$(cc) -c places.c -o places.o

concert: concert.o
	$(cc) concert.o -o concert

concert.o: concert.c gestionnaire_concert.h
	$(cc) -c concert.c -o concert.o

achat: achat.o
	$(cc) achat.o -o achat

achat.o: achat.c gestionnaire_concert.h
	$(cc) -c $(CFLAGS) achat.c -o achat.o

clear:
	rm -f *.o places concert achat
