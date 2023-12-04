#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int status; //자식 프로세스 상태 변수
    pid_t pid = fork(); //자식 프로세스 생성

    //자식 프로세스가 15초 sleep 상태
    if(pid == 0) {
        sleep(15);
        return 24;
    }

    else {

        //자식 프로세스 sleep이 끝날 때 까지
        while(!waitpid(-1, &status, WNOHANG))  // WNOHANG: 종료된 자식이 없는 경우 0을 리턴
        {
            sleep(3);
            puts("sleep 3sec");        
        }

        if(WIFEXITED(status)) {
            printf("Child send %d \n", WEXITSTATUS(status));
        }
    }

    return 0;

}