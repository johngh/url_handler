/*
 * Originally from
 * http://blog.dest-unreach.be/2009/07/11/putty-as-ssh-url-handler 
 *
 * With some modification by johngh
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char* myname) {
  fprintf(stderr, "Usage: %s ssh://[USER@]HOST[:PORT]\n");
}

char* get_putty_cmd(char* userinfo, char* c_params,
                    char* host, char* port) {
	char* cmd = malloc(32768);

	cmd = "\"C:\\Program Files\\PuTTY\\putty.exe\" -ssh ";
	
	if(port) {
		strncat(cmd, "-P ", 3);
		strncat(cmd, port, strlen(port) );
		strncat(cmd, " ", 1);
	}

	if(userinfo) {
		strncat(cmd, userinfo, strlen(userinfo) );
		strncat(cmd, "@", 1);
	}

	if(host) {
		strncat(cmd, host, strlen(host) );
	}

	return cmd;
}

int main(int argc, char* argv[]) {
	// sshURI        =  "ssh:" hier-part
	// hier-part     =  "//" authority path-abempty
	// authority     =  [ [ ssh-info ] "@" ] host [ ":" port ]
	// host          =  <as specified in [RFC3986]>
	// port          =  <as specified in [RFC3986]>
	// path-abempty  =  <as specified in [RFC3986]>
	// ssh-info      =  [ userinfo ] [";" c-param *("," c-param)]
	// userinfo      =  <as specified in [RFC3986]>
	// c-param       =  paramname "=" paramvalue
	// paramname     =  *( ALPHA / DIGIT / "-" )
	// paramvalue    =  *( ALPHA / DIGIT / "-" )
	char* host = NULL;
	char* port = NULL;
	char* userinfo = NULL;
	char* c_params = NULL;
	char *p, *t1, *t2; // temp
	char* cmd;

	if( argc != 2 ) {
		usage(argv[1]);
		return 1;
	}

	p = argv[1];
	if( strncmp(p, "ssh://", 6) != 0 ) {	
		fprintf(stderr, "Url not recognized as ssh:// url\n");
		return 1;
	}
	p += 6;
	t1 = p + strlen(p) - 1;
	if( *t1 == '/' ) {
		*t1 = '\0';
	}

	t1 = strstr(p, "@");
	if( t1 != NULL ) {
		// ssh-info part present
		*t1 = '\0';
		userinfo = p;
		p = t1+1;

		t2 = strstr(userinfo, ";");
		if( t2 != NULL ) {
			*t2 = '\0';
			c_params = t2+1;
		}
	}

	host = p;
	t1 = strstr(p, ":");
	if( t1 != NULL ) {
		// port present
		*t1 = '\0';
		port = t1+1;
	}

	cmd = get_putty_cmd(userinfo, c_params, host, port);

	//fprintf(stderr, "Running: %s\n", cmd);
	system(cmd);
	//sleep(5000);

	free(cmd);

	return 0;
}
