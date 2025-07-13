#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include "structs.h"
#include "transactions.h"

void transaction()
{
    FILE *fp_initial, *fp_banking;
    initial acc;
    banking trans;
    int flag = 0;

    long int acc_no;
    printf("enter account number\n");
    scanf("%ld", &acc_no);


    // opens the account

    fp_initial = fopen("INITIAL.dat", "rb");

    if(fp_initial == NULL) {
        printf("Error opening account\n");
        fclose(fp_initial);
        return;
    }

    // reads the accounts and finds the one

    while (fread(&acc, sizeof(acc), 1, fp_initial)) {
        if (acc.acc_no == acc_no) {
            flag = 1;
            break;
        }
    }
    
    if(flag ==  0) {
        printf("account not found. \n");
        fclose(fp_initial);
        return;
    }

    printf("Account holder: %s\n", acc.name);
    printf("Current balance: %f\n", acc.balance);

    printf("Enter transaction type (Deposit / Withdraw): ");
    scanf("%s", trans.trans);

    printf("Enter mode (cash / cheque): ");
    scanf("%s", trans.type);

    printf("Enter transaction amount: ");
    scanf("%f", &trans.amount);

    // Fill the date into transaction
    
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    trans.date.day = local->tm_mday;
    trans.date.month = local->tm_mon + 1;
    trans.date.year = local->tm_year + 1900;

    trans.acc_no = acc.acc_no;
    trans.interest = 0;

    // Process
    if (strcmp(trans.trans, "Withdraw") == 0)  {
        if (trans.amount > acc.balance) {
            printf("Insufficient balance.\n");
            fclose(fp_initial);
            return;
        }
        acc.balance -= trans.amount;
    } else if (strcmp(trans.trans, "Deposit") == 0) {
        acc.balance += trans.amount;
    } else  {
        printf("Invalid transaction type.\n");
        fclose(fp_initial);
        return;
    }    
    trans.balance = acc.balance;
    update_balance(acc); 
    add_to_file_transaction(trans);
    printf("Transaction successful. Updated balance: %.2f\n", acc.balance);   
}


void update_balance(initial acc) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (fp == NULL) {
        printf("Error opening INITIAL.dat\n");
        return;
    }

    initial temp;
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.acc_no == acc.acc_no){
            fseek(fp, -sizeof(temp), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            break;
        }
    }
    fclose(fp);
}


float give_balance(long int acc_no) {
    float balance = 0.0;
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (fp == NULL) {
        printf("Error opening INITIAL.dat\n");
        return 0.0;
    }
    
    initial acc;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            fclose(fp);
            return acc.balance;
        }
    }

    fclose(fp);
    printf("Account not found.\n");
    return 0.0;
}


void add_to_file_transaction(banking trans) {
    FILE *fp = fopen("BANKING.dat", "ab");
    if (fp == NULL) {
        printf("Error opening BANKING.dat\n");
        return;
    }
    fwrite(&trans, sizeof(trans), 1, fp);
    fclose(fp);
}
