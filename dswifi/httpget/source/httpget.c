/*---------------------------------------------------------------------------------

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>

void getHttp(char* url);
	
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("\n\n\tSimple Wifi Connection Demo\n\n");
	iprintf("Connecting via WFC data ...\n");

	if(!Wifi_InitDefault(WFC_CONNECT)) {
		iprintf("Failed to connect!");
	} else {

		iprintf("Connected\n\n");

		getHttp("www.akkit.org");	}
	
	
	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}

//---------------------------------------------------------------------------------
void getHttp(char* url) {
//---------------------------------------------------------------------------------
    // Let's send a simple HTTP request to a server and print the results!

    // store the HTTP request for later
    const char * request_text = 
        "GET /dswifi/example1.php HTTP/1.1\r\n"
        "Host: www.akkit.org\r\n"
        "User-Agent: Nintendo DS\r\n\r\n";

    // Find the IP address of the server, with gethostbyname
    struct hostent * myhost = gethostbyname( url );
    iprintf("Found IP Address!\n");
 
    // Create a TCP socket
    int my_socket;
    my_socket = socket( AF_INET, SOCK_STREAM, 0 );
    iprintf("Created Socket!\n");

    // Tell the socket to connect to the IP address we found, on port 80 (HTTP)
    struct sockaddr_in sain;
    sain.sin_family = AF_INET;
    sain.sin_port = htons(80);
    sain.sin_addr.s_addr= *( (unsigned long *)(myhost->h_addr_list[0]) );
    connect( my_socket,(struct sockaddr *)&sain, sizeof(sain) );
    iprintf("Connected to server!\n");

    // send our request
    send( my_socket, request_text, strlen(request_text), 0 );
    iprintf("Sent our request!\n");

    // Print incoming data
    iprintf("Printing incoming data:\n");

    int recvd_len;
    char incoming_buffer[256];

    while( ( recvd_len = recv( my_socket, incoming_buffer, 255, 0 ) ) != 0 ) { // if recv returns 0, the socket has been closed.
        if(recvd_len>0) { // data was received!
            incoming_buffer[recvd_len] = 0; // null-terminate
            iprintf(incoming_buffer);
		}
	}

	iprintf("Other side closed connection!");

	shutdown(my_socket,0); // good practice to shutdown the socket.

	closesocket(my_socket); // remove the socket.
}

