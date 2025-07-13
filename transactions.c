#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <ctype.h>
#include "structs.h"
#include "transactions.h"
#include "account.h"

void transaction() {
    FILE *fp_initial;
    initial acc;
    banking trans;

    long int acc_no;

    // keep asking for valid account number
    while (1) {
        printf("Enter account number: ");
        scanf("%ld", &acc_no);
        while (getchar() != '\n');

        fp_initial = fopen("INITIAL.dat", "rb");
        if (fp_initial == NULL) {
            printf("Error opening account file.\n");
            return;
        }

        if (!found_account(fp_initial, acc_no)){
            printf("Account not found. Please try again.\n");
            fclose(fp_initial);
            continue;
        }

        // load the account details
        rewind(fp_initial);
        while (fread(&acc, sizeof(acc), 1, fp_initial)) {
            if (acc.acc_no == acc_no) {
                break;
            }
        }

        fclose(fp_initial);
        break; 
    }

    printf("Account holder: %s\n", acc.name);
    printf("Current balance: %.2f\n", acc.balance);

    // transaction type loop
    while (1) {
        printf("Enter transaction type (Deposit / Withdraw): ");
        scanf("%s", trans.trans);
        while (getchar() != '\n');

        for (int i = 0; trans.trans[i]; i++)
            trans.trans[i] = tolower(trans.trans[i]);

        if (strcmp(trans.trans, "withdraw") == 0 || strcmp(trans.trans, "deposit") == 0)
            break;

        printf("Invalid transaction type. Please enter 'Deposit' or 'Withdraw'.\n");
    }

    // mode loop
    while (1) {
        printf("Enter mode (cash / cheque): ");
        scanf("%s", trans.type);
        while (getchar() != '\n');

        for (int i = 0; trans.type[i]; i++)
            trans.type[i] = tolower(trans.type[i]);

        if (strcmp(trans.type, "cash") == 0 || strcmp(trans.type, "cheque") == 0)
            break;

        printf("Invalid mode. Please enter 'cash' or 'cheque'.\n");
    }

    // amount loop
    while (1) {
        printf("Enter transaction amount: ");
        if (scanf("%f", &trans.amount) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (trans.amount > 0)
            break;

        printf("Transaction amount must be greater than zero.\n");
    }

    // fill date
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    trans.date.day = local->tm_mday;
    trans.date.month = local->tm_mon + 1;
    trans.date.year = local->tm_year + 1900;

    trans.acc_no = acc.acc_no;
    trans.interest = 0;

    // process
    if (strcmp(trans.trans, "withdraw") == 0) {
        if (acc.balance <= 500) {
            printf("Transaction rejected. Account balance is already at or below the minimum required balance of 500.\n");
            return;
        }
        if (trans.amount > acc.balance) {
            printf("Insufficient balance.\n");
            return;
        }
        acc.balance -= trans.amount;
    } else if (strcmp(trans.trans, "deposit") == 0) {
        acc.balance += trans.amount;
    }

    trans.balance = acc.balance;
    update_balance(acc);
    add_to_file_transaction(trans);

    printf("Transaction successful. Updated balance: %.2f\n", acc.balance);
}

void update_balance(initial acc) {
    FILE *fp = fopen("INITIAL.dat", "r+b");
    if (!fp) {
        printf("Error opening INITIAL.dat\n");
        return;
    }
    initial temp;

    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.acc_no == acc.acc_no) {
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