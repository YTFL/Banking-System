#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "structs.h"
#include "transactions.h"
#include "account.h"
#include "util.h"

void transaction() {
    FILE *fp_initial;
    initial acc;
    banking trans;

    long int acc_no;
    fp_initial = fopen("INITIAL.dat", "rb");
        if (fp_initial == NULL) {
            printf("Error opening account file.\n");
            return;
        }

    // keep asking for valid account number
    while (1) {
        while (1) {
            printf("Enter account number or enter 0 to go back: ");
            scanf("%ld", &acc_no);
            clear_input_buffer(); 
            if (found_account(fp_initial, acc_no)) {
                break;
            } else if (acc_no == 0) {
                fclose(fp_initial);
                return;
            } else {
                printf("Account not found. Please try again.\n");
                rewind(fp_initial);  
            }
        }

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
    printf("Current balance: %.2f\n", acc.balance/100.0);

    char confirm[3];
    while (1) {
        printf("\nContinue with this account? (Y/N): ");
        if (!fgets(confirm, sizeof(confirm), stdin)) {
            printf("Input error.\n");
            continue;
        }

        if (strchr(confirm, '\n') == NULL) {
            clear_input_buffer();
        } else {
            confirm[strcspn(confirm, "\n")] = '\0';
        }

        if (strcasecmp(confirm, "Y") == 0) {
            break;
        } else if (strcasecmp(confirm, "N") == 0) {
            printf("Transaction cancelled.\n");
            return;
        } else {
            printf("Invalid input. Please enter Y or N.\n");
        }
    }

    while (1) {
        printf("Enter transaction type (Deposit / Withdraw) or 'cancel' to exit: ");
        scanf("%s", trans.trans);
        clear_input_buffer();

        if (strcasecmp(trans.trans, "cancel") == 0) {
            printf("Transaction cancelled.\n");
            return;
        }

        if (strcasecmp(trans.trans, "withdraw") == 0 || strcasecmp(trans.trans, "deposit") == 0)
            break;

        printf("Invalid transaction type. Please enter 'Deposit' or 'Withdraw'.\n");
    }

    // mode loop
    while (1) {
        printf("Enter mode (cash / cheque) or 'cancel' to exit: ");
        scanf("%s", trans.type);
        clear_input_buffer();

        if (strcasecmp(trans.type, "cancel") == 0) {
            printf("Transaction cancelled.\n");
            return;
        }

        if (strcasecmp(trans.type, "cash") == 0 || strcasecmp(trans.type, "cheque") == 0)
            break;

        printf("Invalid mode. Please enter 'cash' or 'cheque'.\n");
    }

    char input[100];
    double amount;
    long long paise;

    do {
        printf("Enter transaction amount or 0 to cancel: ");

        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input.\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "0") == 0 || strcmp(input, "0.00") == 0) {
            printf("Transaction cancelled.\n");
            return;
        }

        int valid = 1, dot_seen = 0, decimals = 0;
        for (int i = 0; input[i]; ++i) {
            if (isdigit(input[i])) {
                if (dot_seen) decimals++;
            } else if (input[i] == '.') {
                if (dot_seen) {
                    valid = 0;
                    break;
                }
                dot_seen = 1;
            } else {
                valid = 0;
                break;
            }
        }

        if (!valid || decimals > 2) {
            printf("Invalid input. Please enter a valid number with at most two decimal places.\n");
            continue;
        }

        amount = strtod(input, NULL);
        paise = (long long)(amount * 100);

        if (paise <= 0 || paise > MAX_AMOUNT) {
            printf("Transaction amount must be greater than zero and less than 1 Crore.\n");
            continue;
        }

        trans.amount = paise;
        break;

    } while (1);



    while (1) {
        printf("\nContinue with this transaction? (Y/N): ");
        if (!fgets(confirm, sizeof(confirm), stdin)) {
            printf("Input error.\n");
            continue;
        }

        if (strchr(confirm, '\n') == NULL) {
            clear_input_buffer();
        } else {
            confirm[strcspn(confirm, "\n")] = '\0';
        }

        if (strcasecmp(confirm, "Y") == 0) {
            break;
        } else if (strcasecmp(confirm, "N") == 0) {
            printf("Transaction cancelled.\n");
            return;
        } else {
            printf("Invalid input. Please enter Y or N.\n");
        }
    }

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    trans.date.day = local->tm_mday;
    trans.date.month = local->tm_mon + 1;
    trans.date.year = local->tm_year + 1900;

    trans.acc_no = acc.acc_no;
    trans.interest = 0;

    // process
    if (strcmp(trans.trans, "withdraw") == 0) {
    if (acc.balance <= 50000) { 
        printf("Transaction rejected. Account balance is already at or below the minimum required balance of 500.\n");
        return;
    } else if (trans.amount > acc.balance) {
        printf("Insufficient balance.\n");
        return;
    }
    acc.balance -= trans.amount;

    } else if (strcmp(trans.trans, "deposit") == 0) {
        if ((acc.balance + trans.amount) > MAX_AMOUNT) {
            printf("Transaction rejected. Deposit amount exceeds the maximum allowed balance of 1 Crore.\n");
            return;
        }
    acc.balance += trans.amount;
    }

    trans.balance = acc.balance;


    update_balance(acc);
    add_to_file_transaction(trans);

    printf("Transaction successful. Updated balance: %.2f\n", acc.balance/100.0);
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
    long long balance = 0;
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