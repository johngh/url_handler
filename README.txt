Launch PuTTY with a ssh:// URL from a web page by registering this program as the URL handler.

You can do this with:

ftype ssh="C:\bin\ssh-to-putty.exe" "%%1"

(where C:\bin\... is the path to the exe file)

This can be compiled with tcc from http://bellard.org/tcc/

