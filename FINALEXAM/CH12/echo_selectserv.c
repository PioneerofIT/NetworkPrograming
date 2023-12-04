#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    struct timeval timeout;
    fd_set reads, cpy_reads;

    int fd_max, fd_num;
    int str_len;
    char buf[BUF_SIZE];

    if(argc != 2) {
        printf("Usage ~");
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr))==-1) {
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    FD_ZERO(&reads); //reads 초기화 reads는 select() 함수가 감시할 소켓들
    FD_SET(serv_sock, &reads); //serv_sock을 reads 집합에 추가 serv_sock 들어오는 연결 요청 감시 가능
    fd_max = serv_sock; //감시해야 하는 가장 높은 파일 디스크립터 번호 설정

    while (1)
    {
        cpy_reads = reads; //select() 함수는 감시 대상의 변경사항 외에 수정하기 떄문에 원본을 유지하기 위함
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1) { //감시할 파일 디스크립터 범위, 읽기 , 쓰기, 예외, 함수 대기시간
            break; //-1은 오류 반환
        }
        if(fd_num ==0) { // timeout으로 인한 반환
            continue;
        }
        /*감지된 입출력 준비가 완료된 파일 디스크립터 처리하는 루프*/
        for(int i=0; i<fd_max+1; i++) { // for문은 모든 파일 디스크립터 순회
            if(FD_ISSET(i, &cpy_reads)) {// FD_ISSET은 i번째 파일 디스크립터의 준비 유무 true반환 시 준비완료
                if(i==serv_sock) { //변화가 있는 디스크립터가 서버인지->새로운 연결 요청이 들어온 경우
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads); // 새로운 클라이언트 소켓 감시대상 추가
                    if(fd_max < clnt_sock){ //클라이언트 디스크립터가 더 큰 경우 fd_max 교체
                        fd_max = clnt_sock;
                    }
                    printf("connected client : %d \n", clnt_sock);
                }
                else{ //기존 클라이언트로부터 데이터 처리
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d", i);
                    }
                    else {
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
