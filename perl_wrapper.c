#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[] ) {

   char command[50];
   int result;
   char perl[30];
   char handler[30];
   strcpy(perl, "C:\\strawberry\\perl\\bin\\perl.exe");
   strcpy(handler, "P:\\bin\\url_handler.pl");

   // sprintf(command, "%s %s %s", perl, handler, argv[1]);
   sprintf(command, "%s %s", handler, argv[1]);

   result = system(command);

   return(0);

}

