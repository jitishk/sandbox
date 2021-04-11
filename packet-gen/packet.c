#include <stdio.h>



typedef struct _packet_t; {
    /* data */
} packet_t;


int main(int argc, const char *argv[])
{
    char buffer
        int sd;

    // No data/payload just datagram
    //
    char buffer[PCKT_LEN];
    struct ipheader *ip = (struct ipheader *) buffer;
    
    struct udpheader *udp = (struct udpheader *) (buffer + sizeof(struct ipheader));
    return 0;
}
