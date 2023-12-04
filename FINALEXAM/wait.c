#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//부모 프로세서에서 자식 프로세서의 종료 상태 처리 예제
int main(int argc, char *argv[]){
    int status;
    pid_t pid = fork();

    if(pid == 0) {
        return 3; //자식 프로세서 반환 값 3
    }

    else {
        printf("Child PID: %d \n", pid);
        pid = fork();
        if(pid == 0) {
            exit(7);
        }
        else {
            printf("Child PID: %d \n", pid);
            wait(&status); // 자식 프로세스의 종료 상태 확인
            if(WIFEXITED(status)) { //자식프로세서의 정상적인 종료
                printf("Child send one: %d[%d] \n", WEXITSTATUS(status), status);  // 3 출력
            }
            wait(&status); // 자식 프로세스의 종료 상태 확인
            if(WIFEXITED(status)) {
                printf("Child sned two : %d[%d] \n", WEXITSTATUS(status), status); // 7 출력
            }
            sleep(30);
        }

    }
    return 0;
}