#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    long int acc_no;
    char name[21];     // 20 characters + 1 for '\0'
    char address[51];  // 50 characters + 1 for '\0'
    double balance;
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
    char remarks[100];
} banking;



#endif
