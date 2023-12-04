#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM) {
        puts("Time out !");
    }
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT)
    {
        puts("CTRL+C pressed");
    }
    
}

int main (int argc, char *argv[]) {
    int i;

    signal(SIGALRM, timeout); //시그널이 발생 됬을 떄 실행될 함수 설정
    signal(SIGINT, keycontrol); //SIGINT : CTRL+C 강제종료 시그널 매크로
    alarm(2);

    for(i=0; i<3; i++) {
        puts("wait ...");
        sleep(100);
    }
    return 0;
}