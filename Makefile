all: binary test

bin: binary

binary:
	gcc -o url-handler url-handler.c

test:
	t/all

clean:
	@rm url-handler.exe
	@rm url-handler

