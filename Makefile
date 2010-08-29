dltest: dltest.c
	$(CC) -W -Wall -o dltest dltest.c -ldl

clean:
	rm -f dltest