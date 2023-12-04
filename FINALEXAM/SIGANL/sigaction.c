#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM) {
        puts("Time out !");
    }
    alarm(2);
}

int main (int argc, char *argv[]) {
    int i;
    struct sigaction act; 
    
    act.sa_handler = timeout; //signal 발생 시 처리할 함수 등록
    sigemptyset(&act.sa_mask); //timeout 함수가 실행되는 동안 차단할 추가 시그널 설정 , 초기화 했음으로 없음
    act.sa_flags = 0; 
    sigaction(SIGALRM, &act, 0);

    alarm(2);

    for(i=0; i<3; i++) {
        puts("wait..");
        sleep(100);
    }

    return 0;
}