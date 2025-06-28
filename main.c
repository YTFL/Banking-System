#include <stdio.h>

typedef struct {
    long int acc_no;
    char name[50];
    char address[100];
    int balance;
}initial;

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
} transaction;

int main( ) {
    
    return 0;
}

