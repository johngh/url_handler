/*
 * ssh-to-putty
 *
 * Originally from
 * http://blog.dest-unreach.be/2009/07/11/putty-as-ssh-url-handler
 *
 * With some modification by johngh
 *
 * sshURI        =  "ssh:" hier-part
 * hier-part     =  "//" authority path-abempty
 * authority     =  [ [ ssh-info ] "@" ] host [ ":" port ]
 * host          =  <as specified in [RFC3986]>
 * port          =  <as specified in [RFC3986]>
 * path-abempty  =  <as specified in [RFC3986]>
 * ssh-info      =  [ userinfo ] [";" c-param *("," c-param)]
 * userinfo      =  <as specified in [RFC3986]>
 * c-param       =  paramname "=" paramvalue
 * paramname     =  *( ALPHA / DIGIT / "-" )
 * paramvalue    =  *( ALPHA / DIGIT / "-" )
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage (char *my_name) {
    fprintf(stderr, "Usage: %s ssh://[LOGIN@]HOST[:PORT]\n", my_name);
}

int main(int argc, char* argv[]) {

    char *host = NULL;
    char *port = NULL;
    char *login_p = NULL;
    char *c_params = NULL;
    char *url_p, *t1, *t2; // temp
    char my_name[256];

    strcpy(my_name, argv[0]);

    // char *putty_cmd = "\"C:\\Program Files\\PuTTY\\putty.exe\"";
    char *putty_cmd = "\"putty.exe\"";

    char cmd[2048];

    if( argc != 2 ) {
        usage(my_name);

        return 2;
    }

    url_p = argv[1];

    if( strncmp(url_p, "ssh://", 6) ) {
        fprintf(stderr, "%s: '%s' is not a valid ssh:// URL\n", my_name, url_p);
        return 2;
    } else if ( strlen(url_p) < 7 ) {
        usage(my_name);
        fprintf(stderr, "  (You must give a hostname. I only saw '%s')\n", url_p);
        return 2;
    }

    // Jump over ssh://
    url_p += 6;

    // Trim slash & anything after it off end of URL
    t1 = strstr(url_p, "/");
    if( t1 ) {
        *t1 = '\0';
    }

    // Get login from login@host (if specified)
    t1 = strstr(url_p, "@");
    if( t1 ) {
        // End login
        *t1 = '\0';
        // Save login from before @ sign
        login_p = url_p;
        url_p = t1+1;

        // I don't know what we use ';' in a URL for...
        // TO-DO: Read RFC3986
        t2 = strstr(login_p, ";");
        if( t2 ) {
            *t2 = '\0';
            c_params = t2+1;
        }
    }

    host = url_p;

    // Get port if specified
    t1 = strstr(url_p, ":");
    if( t1 ) {
        // End host
        *t1 = '\0';
        port = t1+1;
    }

    strcpy(cmd, putty_cmd);
    strncat(cmd, " -ssh ", 6);

    if(port) {
        strncat(cmd, "-P ", 3);
        strncat(cmd, port, strlen(port) );
        strncat(cmd, " ", 1);
    }

    if(login_p) {
        strncat(cmd, login_p, strlen(login_p) );
        strncat(cmd, "@", 1);
    }

    if(host) {
        strncat(cmd, host, strlen(host) );
    }

    fprintf(stderr, "Running: %s\n", cmd);
    int exit_code = system(cmd);

    return exit_code;

}

