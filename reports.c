#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <time.h>
#include "util.h"
#include "account.h" 
#include "reports.h"  

void display_account() 
{
    long int accno;
    printf("Enter account number: ");
    scanf("%ld", &accno);
    
    if (!found_account(accno)) {
        printf("\n\tERROR!\nAccount not found!\n");
        return;
    }

    char name[50], address[100];
    return_name(accno, name);
    return_address(accno, address);
    
    // Get current date for report
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date today = {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};

    printf("\n\n\t\tGLOBAL REPORT OF ACCOUNT\n");
    printf("Account Number: %ld\n", accno);
    printf("%s\n", name);
    printf("Date: %02d/%02d/%d\n\n", today.day, today.month, today.year);
    
    box_for_display();
    
    // Read banking transactions
    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp) {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        return;
    }
    
    transaction t;
    float total_deposit = 0, total_withdraw = 0;
    int transaction_count = 0;
    
    printf("%-10s  %-10s  %10s  %10s  %10s\n", 
           "Date", "Particular", "Deposit", "Withdraw", "Balance");
    printf("------------------------------------------------------------\n");
    
    while (fread(&t, sizeof(transaction), 1, bank_fp)) {
        if (t.acc_no == accno) {
            transaction_count++;
            printf("%02d-%02d-%04d  %-10s  ", 
                   t.date.day, t.date.month, t.date.year, t.type);
            
            if (strcmp(t.trans, "Deposit") == 0 || strcmp(t.trans, "Initial") == 0) {
                printf("%10.2f  %10s  ", t.amount, "");
                total_deposit += t.amount;
            } else {
                printf("%10s  %10.2f  ", "", t.amount);
                total_withdraw += t.amount;
            }
            
            printf("%10.2f\n", t.balance);
        }
    }
    fclose(bank_fp);
    
    if (transaction_count == 0) {
        printf("\tNo transactions found for this account!\n");
    }
    
    printf("------------------------------------------------------------\n");
    printf("TOTAL -> %10.2f  %10.2f  %10.2f\n\n", 
           total_deposit, total_withdraw, total_deposit - total_withdraw);

}

void month_report() 
{
    clear();
    long int accno;
    printf("Enter account number: ");
    scanf("%ld", &accno);
    
    if (!found_account(accno)) {
        printf("\n\tERROR!\nAccount not found!\n");
        return;
    }

    int month, year;
    printf("Enter statement month (1-12): ");
    scanf("%d", &month);
    printf("Enter statement year: ");
    scanf("%d", &year);
    
    if (month < 1 || month > 12) {
        printf("\n\tINVALID MONTH!\n");
        return;
    }

    char name[50];
    return_name(accno, name);
    
    // Get current date for report
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date today = {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};

    printf("\n\n\t\tMONTHLY STATEMENT\n");
    printf("Account Number: %ld\n", accno);
    printf("Statement Month: %02d/%04d\n", month, year);
    printf("%s\n", name);
    printf("Date: %02d/%02d/%d\n\n", today.day, today.month, today.year);
    
    box_for_display();
    
    // Read banking transactions
    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp) {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        return;
    }
    
    transaction t;
    float total_deposit = 0, total_withdraw = 0;
    float opening_balance = 0, closing_balance = 0;
    int first_trans = 1;
    
    printf("%-10s  %-10s  %10s  %10s  %10s\n", 
           "Date", "Particular", "Deposit", "Withdraw", "Balance");
    printf("------------------------------------------------------------\n");
    
    // Find opening balance (last balance from previous month)
    while (fread(&t, sizeof(transaction), 1, bank_fp)) {
        if (t.acc_no == accno) {
            if (t.date.year < year || (t.date.year == year && t.date.month < month)) {
                opening_balance = t.balance;
            }
        }
    }
    rewind(bank_fp);
    
    // Print opening balance
    printf("%02d-%02d-%04d  %-10s  %10s  %10s  %10.2f\n", 
           1, month, year, "Opening", "", "", opening_balance);
    
    // Process monthly transactions
    while (fread(&t, sizeof(transaction), 1, bank_fp)) {
        if (t.acc_no == accno && t.date.month == month && t.date.year == year) {
            printf("%02d-%02d-%04d  %-10s  ", 
                   t.date.day, t.date.month, t.date.year, t.type);
            
            if (strcmp(t.trans, "Deposit") == 0 || strcmp(t.trans, "Initial") == 0) {
                printf("%10.2f  %10s  ", t.amount, "");
                total_deposit += t.amount;
            } else {
                printf("%10s  %10.2f  ", "", t.amount);
                total_withdraw += t.amount;
            }
            
            printf("%10.2f\n", t.balance);
            closing_balance = t.balance;
        }
    }
    fclose(bank_fp);
    
    printf("------------------------------------------------------------\n");
    printf("TOTAL FOR MONTH: %10.2f  %10.2f\n", total_deposit, total_withdraw);
    printf("CLOSING BALANCE: %10.2f\n\n", closing_balance);

}

void box_for_display() 
{
    printf("------------------------------------------------------------\n");
    printf("%-10s  %-10s  %10s  %10s  %10s\n", 
           "Date", "Particular", "Deposit", "Withdraw", "Balance");
    printf("------------------------------------------------------------\n");

}



void return_name(int accno, char *name_out) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;
    
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == accno) {
            strcpy(name_out, acc.name);
            break;
        }
    }
    fclose(fp);
}



void return_address(int accno, char *address_out) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;

    if (fp == NULL) {
        strcpy(address_out, "");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == accno) {
            strcpy(address_out, acc.address);
            fclose(fp);
            return;
        }
    }

    strcpy(address_out, "Account not found");
    fclose(fp);
}
