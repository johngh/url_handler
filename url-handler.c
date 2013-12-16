/*
 * url-handler
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
 * Compile with:
 * gcc -Wl,-subsystem,windows -o url-handler url-handler.c
 * or:
 * gcc -mwindows -o url-handler url-handler.c
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int show_usage(char *my_name) {
    char usage[2048];
    sprintf(usage, "Usage: %s ssh://[LOGIN@]HOST[:PORT]", my_name);
    MessageBox(NULL, TEXT(usage), TEXT("Error"), MB_ICONERROR | MB_OK);
    return 0;
}

/*
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
*/


int main(int argc, char* argv[]) {

    char *host = NULL;
    char *port = NULL;
    char *login_p = NULL;
    char *c_params = NULL;
    char *url_p, *t1, *t2; // temp
    char my_name[256];
    DWORD dword_size = 1025;

    strcpy(my_name, argv[0]);

    FILE * log_fh;

    log_fh = fopen ("url-handler.log","w");

    fprintf (log_fh, "my_name: '%s'\n", my_name);

    //
    // char *rdp_cmd = "\"mstsc.exe\"";
    // char *rdp_cmd = "%SystemRoot%\\system32\\mstsc.exe";
    // char *rdp_cmd = "\"C:\\Windows\\system32\\mstsc.exe\"";
    //
    // char *putty_cmd = "\"putty.exe\"";
    // char *putty_cmd = "C:\\Program Files\\PuTTY\\putty.exe";
    // char *putty_cmd = "\"C:\\Program Files\\PuTTY\\putty.exe\"";
    // strncpy(putty_cmd, "C:\\Program Files\\PuTTY\\putty.exe", 32);
    //

    char putty_cmd[256];
    char ent_name[256];
    strcpy(ent_name, "HKEY_CLASSES_ROOT\\ssh\\PuTTY Path");

    if(
        RegGetValue(HKEY_CLASSES_ROOT, "ssh", "PuTTY Path", RRF_RT_ANY, NULL, (PVOID)&putty_cmd, &dword_size)
	== ERROR_SUCCESS
    ) {

        if ( ! strlen(putty_cmd) ) {

            char error[2048];
            sprintf (error, "%s: ERROR: PuTTY command read from [%s] is empty.", my_name, ent_name);
            MessageBox(NULL, TEXT(error), TEXT("Error"), MB_ICONERROR | MB_OK);
            return 1;

        } else {

            // fprintf(log_fh, "Read putty_cmd: '%s'\n", putty_cmd); 

        }

    } else {

        char error[2048];
        sprintf(error, "%s: Can't read [%s] from registry.", my_name, ent_name);
        MessageBox(NULL, TEXT(error), TEXT("Error"), MB_ICONERROR | MB_OK);
        return 1;

    }

    // char *rdp_cmd = "%SystemRoot%\\system32\\mstsc.exe";
    char *rdp_cmd = "C:\\Windows\\system32\\mstsc.exe";

    char cmd[2048];

    if( argc != 2 ) {

        show_usage(my_name);
        return 2;

    }

    url_p = argv[1];

    int not_ssh = strncmp(url_p, "ssh://", 6);
    int not_rdp = strncmp(url_p, "rdp://", 6);

    if ( not_ssh != 0 && not_rdp != 0 ) {

        char error[2048];
        sprintf(error, "%s: '%s' is not a valid ssh:// or rdp:// URL",
            my_name, url_p);
        MessageBox(NULL, TEXT(error), TEXT("Error"),
            MB_ICONERROR | MB_OK);
        return 1;

    }

    if ( strlen(url_p) < 7 ) {
        // show_usage(my_name);
        char error[2048];
        sprintf(error, "Hostname required. I only saw '%s'", url_p);
        MessageBox(NULL, TEXT(error), TEXT("Error"), MB_ICONERROR | MB_OK);
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

    if ( not_ssh == 0 ) {

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

    } else if ( not_rdp == 0 ) {

        strcpy(cmd, rdp_cmd);
        strncat(cmd, " /v ", 4);

    } else {

        char huh[2048];
        sprintf(huh, "How did I get here with no protocol?");
        MessageBox(NULL, TEXT(huh), TEXT("Error"), MB_ICONERROR | MB_OK);

    }

    if(host) {
        strncat(cmd, host, strlen(host) );
    }

    /*
    if ( show_debug == NULL ) {

        char running[2048];
        sprintf(running, "%s", cmd);
        int msgboxID = MessageBox(NULL, TEXT(running), TEXT("Running"),
            MB_ICONINFORMATION | MB_OKCANCEL | MB_DEFBUTTON2);

        if ( msgboxID == IDCANCEL ) {
            return 0;
        }

    }
    */

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process.
    if( !CreateProcess( NULL,   // No module name (use command line)
        cmd,            // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {

        int exit_code = GetLastError();

        char failed[2048];
        sprintf( failed, "Can't run '%s' (%s)", cmd, strerror(exit_code));

        MessageBox(NULL, TEXT(failed), TEXT("Error"), MB_ICONERROR | MB_OK);

        return exit_code;

    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    fclose (log_fh);

}

