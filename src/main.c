//
//  main.c
//  
//
//  Created by Павел Тяпкин on 14.10.2020.
//

#include <stdio.h>

int main()
{
    printf( "Press 'r' if you want to read config\nPress any key if you want to write config\n");
    //scanf("%d", &mode);
    char mode = getchar();
    if (mode == 'r')
        config_read();
    else
        config_write();
}
