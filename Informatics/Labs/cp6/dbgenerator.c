#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "person.h"
#include "allowdata.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int num;
    printf("Enter the number of records: ");
    scanf("%d", &num);
    FILE* fp = fopen("database.txt", "w");


    for (int i = 0; i < num; i++) {
        char record[num / 2];

        char* surname = *((rand() % 2 == 0)
            ? (allowedSurnamesMale + rand() % 10)
            : (allowedSurnamesFemale + rand() % 10));  
        // if (rand() % 2 == 0) {
        //     surname = allowedSurnamesMale[rand() % 10];
        // } else {
        //     surname = allowedSurnamesFemale[rand() % 10];
        // }
        char* initials = allowedInitials[rand() % 20];
        char* sex = allowedSex[rand() % 2];
        int group = allowedGroups[rand()%4];
        int mathematics = allowedMarks[rand()%4];
        int physics = allowedMarks[rand()%4];
        int informatics = allowedMarks[rand()%4];
        int history = allowedMarks[rand()%4];
        sprintf(record, "%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d",          
                surname, initials, sex, group, mathematics, physics, informatics, history);    //записываем в массив record
        fprintf(fp, "%s\n", record);   //записываем в файл построчно
        
    }

    fclose(fp);
}