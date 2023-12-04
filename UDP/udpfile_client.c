#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 60
void error_handling(char *message);


int main(int argc, char *argv[]) {

    int sock; //서버와 연결할 소켓
    struct sockaddr_in serv_addr,from_addr;
    socklen_t from_addr_sz;

    char buf[BUF_SIZE];
    FILE *fp;
    int recv_cnt;

    fp = fopen("receive.dat", "wb");

    if(argc !=3 ) {
        printf("Usage : %s <IP> <PORT>", argv[0]);
    }

    
    sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr)); //연결할 서버주소 초기화
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    sendto(sock,"Start", 5, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); //전송 시작을 알림

    if(!fp) {
        perror("File Open error");
        close(sock);
        return 1;
    }

    from_addr_sz = sizeof(from_addr);
    while (1) {
        memset(buf, 0, BUF_SIZE); // 버퍼 초기화
        recv_cnt = recvfrom(sock, buf, BUF_SIZE, 0, NULL, NULL); //서버로 부터 전송을 받음
        
        if(recv_cnt < 0) {
            perror("recvfrom() error");
            break;
        }
        
        if(strcmp(buf, "END_OF_FILE") == 0) { //파일 전송 완료 문자를 받았다면 잘 받았다는 ACK를 서버에게 전송
            sendto(sock, "THANK_YOU_SERVER", strlen("THANK_YOU_SERVER"),0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            fputs("received end \n", stdout);
            break;
        }

        fwrite(buf, 1, recv_cnt, fp); //buf에 저장되어 있는 문자를 파일에씀
    }
    

    fclose(fp);
    close(sock);
    return 0;


}



void error_handling(char *message) {
    fputs(message, stderr);
    perror(message);
    exit(1);
}