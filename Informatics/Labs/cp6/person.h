#ifndef __person_h__
#define __person_h__

typedef struct{
    int grade;
    char name[25];
} Exam;

typedef struct {
    char surname[25];
    char initials[2];
    int sx;
    int group;
    Exam ex[10];
} passenger;


#endif