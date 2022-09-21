all: dush 
	
dush: dush.o common.o parser.o executor.o logger.o
	gcc dush.o common.o parser.o executor.o logger.o -o dush

dush.o: dush.c common/common.h services/services.h
	gcc -c dush.c	

common.o: common/common.c
	gcc -c common/common.c

executor.o: services/executor.c services/services.h common/common.h
	gcc -c services/executor.c

parser.o: services/parser.c services/parser.h common/common.h
	gcc -c services/parser.c

logger.o: services/logger.c services/parser.h
	gcc -c services/logger.c

clean:
	rm *.o dush
