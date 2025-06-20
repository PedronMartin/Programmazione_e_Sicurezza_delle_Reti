#include "network.h"

int main(){
    char* receive;
    FILE *fd;
    
    fd = createTCPConnectionFD("localhost", 35000);
    
    fputc('A', fd);
    fflush(fd);

    receive=fgets(receive, 100, fd);
    fflush(fd);
    printf("[client] %s\n", receive);

    fclose(fd);
}

