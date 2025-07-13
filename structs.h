#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    long int acc_no;
    char name[20];
    char address[50];
    float balance;
} initial;


typedef struct {
    int day;
    int month;
    int year;
} date;

typedef struct {
    long int acc_no;
    char type[10];
    date date;
    char trans[20];
    float interest;
    float amount;
    float balance;
} banking;

#endif
