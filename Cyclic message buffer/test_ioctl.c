#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
    struct timespec ts;
    char message[50];
} buf_message;

typedef struct {
    buf_message message;
    int number;
} message_request;


#define GET_BUFFER _IOR('a','a', int *)
#define GET_MESSAGE _IOWR('a','b', message_request *)

int main(){
    message_request req;
    printf("mrq size %ld\n", sizeof(message_request));
    while (1){
        int number;
        int fp = open("/dev/simpmod", O_RDWR);
        if(fp < 0) {
            printf("Cannot open device file...\n");
            return 0;
        }
        ioctl(fp, GET_BUFFER,(int *)&number);
        printf("buffer have %d \nwrite number of message to get[0...%d)\n", number, number);
        scanf("%i", &req.number);
        printf("==== requesting message %i\n", req.number);
        ioctl(fp, GET_MESSAGE,(message_request *)&req);
        printf("message %ld.%li: %s\n====\n", req.message.ts.tv_sec, req.message.ts.tv_nsec, req.message.message);
        close(fp);
    }
}
