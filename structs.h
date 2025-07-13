#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    long int acc_no;
    char name[50];
    char address[100];
    float balance;
} initial;

typedef struct {
    long int acc_no;
    char date[20];
    char type[10];   
    float amount;
} transaction;

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
