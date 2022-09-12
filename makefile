main:
	gcc dush.c common/common.c services/parser.c services/executor.c services/logger.c  -o dush
clean:
	rm dush