all: bin1 test

bin: bin1

bin1:
	gcc -Wl,-subsystem,windows -o url-handler url-handler.c

bin2:
	gcc -mwindows -o url-handler url-handler.c

test:
	t/all

clean:
	@rm -f url-handler.exe
	@rm -f url-handler
	@rm -f url-handler-shell.exe

install:
	mv url-handler.exe /C/Users/Harrij/bin/url-handler.exe

instms:
	mv ms_eg.exe /C/Users/Harrij/bin/url-handler.exe

instbg:
	mv background.exe /C/Users/Harrij/bin/url-handler.exe

