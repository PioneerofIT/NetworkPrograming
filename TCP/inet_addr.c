/*빅 엔디안 예제*/
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";

    /*inet_addr은 문자열 IP 값을 32비트 바이너리 형태로 변환.*/
    /*INADDR_NONE 변환 에러 값*/
    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE) {
        printf("Error !");
    }
    else {
        printf("Network ordered int addr : %#lx \n", conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE){
        printf (" Error ! \n");
    }
    else {
        printf("Network ordered int addr : %#lx \n", conv_addr);
    }

    return 0;
}
