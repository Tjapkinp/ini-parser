//
//  ini_parser.c
//  
//
//  Created by Павел Тяпкин on 14.10.2020.
//

#include <stdio.h>
#include <string.h>
#include <stdint.h> // uint32_t
#include <stdlib.h> // atoi
#include <time.h> // strftime
#include "ini_parser.h"

FILE * iniF;
const char *filename = "CONFIG.INI";

char lineString[255] = "";

struct config_t config;
struct tm tm;

int config_read(const char* ini_data){
    
    char *token_chr = "";
    int i = 1;
    char SSID[128] = "";
    char Password[128] = "";
    
    iniF = fopen(filename, "r");
    if (iniF == NULL){
        printf("Cannot open file.\n");
        return 1;
    }
    
    while(!feof(iniF)) {
        if (fgets(lineString, 254, iniF) != NULL){
            token_chr = strtok(lineString," =");
            
            while(token_chr != NULL){
                if (strcmp(token_chr, "SSID") == 0){ // SSID
                    token_chr = strtok(NULL," =");
                    sprintf(SSID, "%s", token_chr);
                }
                
                if (strcmp(token_chr, "PASSWORD") == 0){ // PASSWORD
                    token_chr = strtok(NULL," =");
                    sprintf(Password, "%s", token_chr);
                }
                
                char pwm_channel[4];
                sprintf(pwm_channel, "CH%i",i);
                if (strcmp(token_chr, pwm_channel) == 0){ // {PWM CH1..5}
                    int k=i;
                    i++;
                    token_chr = strtok(NULL," =");
                    switch (k){
                        case 1:
                            config.pwm.ch1 = atoi(token_chr);
                            break;
                        case 2:
                            config.pwm.ch2 = atoi(token_chr);
                            break;
                        case 3:
                            config.pwm.ch3 = atoi(token_chr);
                            break;
                        case 4:
                            config.pwm.ch4 = atoi(token_chr);
                            break;
                        case 5:
                            config.pwm.ch5 = atoi(token_chr);
                            break;
                        default:
                            break;
                    }
                    
                }
                
                if (strcmp(token_chr, "Sunrise") == 0){ // Sunrise
                    token_chr = strtok(NULL," =");
                    
                    char timechr[30];
                    sprintf(timechr, "01-01-1970 %s",token_chr);
                    
                    if (strptime(timechr, "%d-%m-%Y %H:%M", &tm) != NULL)
                        config.day.sunrise = mktime(&tm);
                }
                
                if (strcmp(token_chr, "Sunset") == 0){ // Sunset
                    
                    token_chr = strtok(NULL," =");
                    char timechr[30];
                    sprintf(timechr, "01-01-1970 %s",token_chr); // added date to this token to allow strptime
                    
                    if (strptime(timechr, "%d-%m-%Y %H:%M", &tm) != NULL)
                        config.day.sunset = mktime(&tm);
                    
                }
                
                token_chr = strtok (NULL, " =");
            }
        }
        
    }
    
    printf("\n\nResults of INI file:");
    
    config.wifi.ssid = SSID;
    config.wifi.pass = Password;
    
    printf("\nSSID: %s", config.wifi.ssid);
    printf("Password: %s", config.wifi.pass);
    
    printf("CH1: %d\n", config.pwm.ch1);
    printf("CH2: %d\n", config.pwm.ch2);
    printf("CH3: %d\n", config.pwm.ch3);
    printf("CH4: %d\n", config.pwm.ch4);
    printf("CH5: %d\n", config.pwm.ch5);
    
    char buff[50];
    
    strftime(buff, 20, "%H:%M", localtime(&config.day.sunrise));
    printf("\nSunrise: %s",buff);
    
    strftime(buff, 20, "%H:%M", localtime(&config.day.sunset));
    printf("\nSunset: %s",buff);
    
    fclose(iniF);
    return 0;
}


int config_write(const char* ini_data, struct config_t* test){
    
    iniF = fopen(filename, "w");
    
    // Configuration parameters (maybe this data could be imported from the keyboard, but it takes a long time)
    config.wifi.ssid = "UAV_Test2";
    config.wifi.pass = "Alpine";
    
    config.pwm.ch1 = 16; // (duty cycle?)
    config.pwm.ch2 = 48;
    config.pwm.ch3 = 64;
    config.pwm.ch4 = 96;
    config.pwm.ch5 = 32;
    
    config.day.sunrise = 1602743295; // UNIX time ~ 09:28 (15.10.20)
    config.day.sunset = 1602786015; // UNIX time ~ 21:20 (15.10.20)
    
    for (int i=0; i<=12; i++){
        switch (i){
            case 1:
                sprintf(lineString,"[WIFI]\n");
                break;
            case 2:
                sprintf(lineString,"SSID = %s\n", config.wifi.ssid);
                break;
                
            case 3:
                sprintf(lineString,"PASSWORD = %s\n\n", config.wifi.pass);
                break;
                
            case 4:
                sprintf(lineString,"[PWM]\n");
                break;
            case 5:
                sprintf(lineString,"CH1 = %i\n",config.pwm.ch1);
                break;
            case 6:
                sprintf(lineString,"CH2 = %i\n",config.pwm.ch2);
                break;
            case 7:
                sprintf(lineString,"CH3 = %i\n",config.pwm.ch3);
                break;
            case 8:
                sprintf(lineString,"CH4 = %i\n",config.pwm.ch4);
                break;
            case 9:
                sprintf(lineString,"CH5 = %i\n\n",config.pwm.ch5);
                break;
            case 10:
                sprintf(lineString,"[DAY PARAMS]\n");
                break;
            case 11:
                strftime(lineString, 20, "Sunrise = %H:%M\n", localtime(&config.day.sunrise));
                break;
            case 12:
                strftime(lineString, 20, "Sunset = %H:%M", localtime(&config.day.sunset));
                break;
        }
        
        fputs(lineString, iniF);
    }
    fclose(iniF);
    
    printf("Done!");
    
    return 0;
}

