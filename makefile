main:
	gcc dush.c common/common.c services/parser.c services/executor.c  -o dush
clean:
	rm dush