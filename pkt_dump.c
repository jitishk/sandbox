#include <stdio.h>
#include <string.h>
#include <stdint.h>


void jdump (char *msg, size_t msg_len) {
    static unsigned int pkt_count = 0;

    if (pkt_count > 3000) {
        return;
    }
    FILE *fp;
    if (pkt_count == 0){
        fp = fopen("/tmp/nfm_pkt.dump", "w");
    } else {
        fp = fopen("/tmp/nfm_pkt.dump", "a");
    }
    pkt_count++;
    for (int i=0; i<msg_len; i++) {
        fprintf(fp, "%1x", (unsigned)(unsigned char)msg[i]);
        if ((i+1)%2 == 0) {
            fprintf(fp, " ");
        }
        if ((i+1)%16 == 0) {
            fprintf(fp, "\n");
        }
    }
    fprintf(fp, "\n");
    fclose(fp);
}

void nfm_dump_data(char *data, int count)
{
    uint16_t      idx, str_offset = 0;
    uint8_t       str[4096];
#define DBG_BYTE_CNT    256
    uint16_t      pbytes  = DBG_BYTE_CNT ; /* Num bytes to print */

    if( count < pbytes )
        pbytes = count ;
    for(idx = 0; idx < pbytes; idx++) {
        str_offset += sprintf(str + str_offset, "%1x ", (unsigned)(unsigned char)data[idx]);
        if ((idx != 0) && !(idx % 16)) {
            printf(" %s \n", str);
            str_offset = 0;
        }
    }
    if (str_offset)
        printf( " %s \n", str);
}

int main() {
	char pkt_buf[128];
	memset(pkt_buf, 0x5e, sizeof(pkt_buf));
	jdump(pkt_buf, sizeof(pkt_buf));
	nfm_dump_data(pkt_buf, sizeof(pkt_buf));
	return 0;
}
