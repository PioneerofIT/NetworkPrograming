#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[]) {

    /*소켓 및 주소정보 변수 클라이언트는 서버에게 요청만 하면 됨 */
    /*클라이언트는 OS에서 IP와 PORT를 자동으로 할당 */
    
    int sock;
    struct sockaddr_in serv_addr;

    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;

    if(argc !=3) {
        printf("Usage : %s <IP> <PORT>", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error " );
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        printf("connect() error");
    }    
    else printf("Connect ..............");


    fputs("피연산자 갯수 :", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char) opnd_cnt;

    for(i=0; i<opnd_cnt; i++) {
        printf("Operand %d : ", i+1);
        scanf("%d", (int *)&opmsg[i*OPSZ+1]); /* char형에 int형 변수를 대입 시 포인터로 형변환 opmsg 메모리구조는 4바이트를 연속되게 할당하여 int형 변수를 저장*/
        // +1 의 의미는 opmsg[0]에는 갯수값을 할당했기 때문
    }

    fgetc(stdin); // 버퍼 비우기
    fputs("Operator : ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); // i=3일 때 4바이트 할당 후 그 다음 번지의 opmsg[4*4+1]
    write(sock, opmsg, opnd_cnt*OPSZ+2); //오버플로우 고려한 바이트까지 데이터 전송
    read(sock, &result, RLT_SIZE); //4바이트를 받아 result에 저장
    
    printf("result : %d ", result);
    close(sock);
    return 0;


}


void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}