dltest: dltest.c
	$(CC) -o dltest dltest.c -ldl

clean:
	rm -f dltest