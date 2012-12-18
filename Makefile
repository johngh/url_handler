all: clean binary test

binary:
	gcc -o url-handler url-handler.c

test:
	t/all

clean:
	@rm url-handler

