#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>


int main(int argc, const char *argv[])
{
    if (argc == 2) {
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        const char *filename = argv[1];
        FILE *fp = fopen(filename, "r");
        int linenumber = 0;
        int pktcount = 0;
        char pkt[10000];
        int i = 0;
        if (fp == NULL) {
            return 0;
        }

        while ((read = getline(&line, &len, fp)) != -1) {
            printf("%s", line);
            if (strstr(line, "ethertype") != NULL) {
                pktcount++;
                memset(pkt, 0, sizeof(pkt));
                i = 0;
                continue;
            }
            char *token;

            // strip ascii values and skip the first word
            line[50] = '\0';
            token = strtok(line, " ");

            token = strtok(NULL, " ");
            while(token != NULL) {
                pkt[i] = strtol(token, NULL, 16);
                i++;
                token = strtok(NULL, " ");
            }

            linenumber++;
            if (linenumber == 1) {
                break;
            }
        }

        fclose(fp);
        return 0;
    }
#if 0
#endif
}
