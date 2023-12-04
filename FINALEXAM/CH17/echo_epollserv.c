#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *message);

int main(int argc, char *argv[]) {
    
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_event; //epoll 이벤트를 저장하기 위한 배열, epoll_wait() 함수가 반환할 때 이벤트 발생한 디스크립터 정보 저장
    struct epoll_event event; //epoll_ctl()함수 호출할 때 사용
    int epfd; //epoll 인스턴스의 파일 디스크립터
    int event_cnt; // epoll_wait() 함수가 반환할 떄 이벤트가 발생한 파일 디스크립터 수

    if(argc != 2) {
        printf("Usage :%s <port> \n", argv[0]);
        exit(1);
    }

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

    epfd = epoll_create(EPOLL_SIZE);//epoll 인스턴스 생성 EPOLL_SIZE :최대 파일 디스크립터 개수
    ep_event = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    event.events = EPOLLIN; // 읽기 가능한 상태 이벤트 타입(새로운 연결 요청 감시)

    event.data.fd = serv_sock;

    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); //epfd 인스턴스에 serv_sock을 인스턴스에 추가하며 이벤트 정보를 넘겨줌

    while(1) {
        event_cnt =  epoll_wait(epfd, ep_event, EPOLL_SIZE, -1); //epfd에 등록된 인스턴스를 감시하며 이벤트 발생시 해당 디스크립터 ep_events에 저장                           
                                                               //-1은 무한대기, EPOLL_SIZE는 ep_event배열 크기와 일치해야함
                                                            //event_cnt는 이벤트가 발생한 파일 디스크립터 수 저장
        if(event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }

        for(i=0; i<event_cnt; i++) {
            if(ep_event[i].data.fd == serv_sock) { // 새로운 연결요청이 들어 왔을 때
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
                
                /*새로운 클라이언트가 데이터를 보내는것에 대한 이벤트 감지를 위해 epfd 인스턴스에 등록*/
                event.events = EPOLLIN;
                event.data.fd=clnt_sock;

                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            }
            else { // 기존 클라이언트 처리 부분
                str_len = read(ep_event[i].data.fd, buf, BUF_SIZE);
                if(str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_event[i].data.fd, NULL);
                    close(ep_event[i].data.fd);
                    printf("closed client: %d \n", ep_event[i].data.fd);
                }
                else {
                    write(ep_event[i].data.fd, buf, str_len);
                }
            }
        }
    }

    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
