all: clean binary test

binary:
	gcc -o ssh-to-putty ssh-to-putty.c

test:
	t/all

clean:
	@rm ssh-to-putty

