#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);
#define BUF_SIZE 1024

int main (int argc, char *argv[]) {

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz ;

    int recv_cnt;
    char message[BUF_SIZE];

    if(argc !=2) {
        printf("Usage : %s <PORT>", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(serv_sock == -1 ) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error !");
    }

    if(listen(serv_sock, 5) ==-1) {
        error_handling("listen() error !");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    
    if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz)) == -1) {
        error_handling("accept () error");
    }

    while (1)
    {
        recv_cnt = read(clnt_sock, message, BUF_SIZE); //클라이언트로 부터 데이터 수신
        if (recv_cnt == -1)
        {
            error_handling("데이터 수신 에러");
        }
        
        if(!strcmp(message, "END") ) {
            break;
        }
        message[recv_cnt+1] =0;
        fputs(message, stdout); // 출력
        write(clnt_sock, "SUCESS RECEIVE !", strlen("SUCESS RECEIVE !"));
        memset(message,0, BUF_SIZE);    
    }
    

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    perror(message);
    fputs(message, stderr);
}