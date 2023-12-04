#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);
void read_childproc (int sig);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    pid_t pid;
    struct sigaction act;
    
    int str_len, state;
    char buf[BUF_SIZE];

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    while(1) {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);

        if(clnt_sock == -1) {
            continue;
        }

        else {
            puts("new client connected ...");
        }
        pid = fork(); // 새로운 클라이언트 처리를 위한 자식 프로세스 생성
        if(pid == -1) {
            close(clnt_sock);
            continue;
        }
        if(pid == 0) { // 자식 프로세스의 클라이언트 처리부분
            close(serv_sock); //이미 clnt_sock을 할당 받았고 서버소켓은 사용하지 않으므로 닫음
            while((str_len=read(clnt_sock, buf, BUF_SIZE))!= 0){
                write(clnt_sock, buf, str_len);
            } 
            close(clnt_sock);
            puts("client disconnected....");
            return 0;
        }
        else { // 부모 프로세스는 클라이언트를 처리할 필요가 없으니 닫아줌 
            close(clnt_sock);
        }
    }

    close(serv_sock);
    return 0;


}

void read_childproc (int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)) {
        printf("Remove proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status));

    }
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}