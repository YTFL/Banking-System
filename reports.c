#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "util.h"
#include "account.h"
#include "reports.h"

void display_account()
{
    long int accno;
    printf("Enter account number: ");
    scanf("%ld", &accno);

    // Open account file for found_account()
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) {
        printf("\n\tERROR!\nCannot open account file!\n");
        return;
    }

    // Pass FILE pointer to found_account()
    if (!found_account(fp, accno))
    {
        printf("\n\tERROR!\nAccount not found!\n");
        fclose(fp);
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
    if (!bank_fp)
    {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        fclose(fp);
        return;
    }

    banking t; 
    float total_deposit = 0, total_withdraw = 0;
    float opening_balance = 0;
    int transaction_count = 0;
    int found_initial = 0;

    // Find initial balance
    rewind(bank_fp);
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno && strcmp(t.trans, "Initial") == 0) {
            opening_balance = t.amount;
            found_initial = 1;
            break;
        }
    }
    rewind(bank_fp);

    // Print opening balance
    printf("%02d-%02d-%04d  %-10s  %10.2f  %10s  %10.2f\n", 
           1, 1, 1900, "Opening", opening_balance, "", opening_balance);

    // Process transactions
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno)
        {
            transaction_count++;
            printf("%02d-%02d-%04d  %-10s  ",
                   t.date.day, t.date.month, t.date.year, t.type);

            if (strcmp(t.trans, "Deposit") == 0 || strcmp(t.trans, "Initial") == 0)
            {
                printf("%10.2f  %10s  ", t.amount, "");
                total_deposit += t.amount;
            }
            else
            {
                printf("%10s  %10.2f  ", "", t.amount);
                total_withdraw += t.amount;
            }

            printf("%10.2f\n", t.balance);
        }
    }
    fclose(bank_fp);
    fclose(fp);

    if (transaction_count == 0)
    {
        printf("\tNo transactions found for this account!\n");
    }

    float closing_balance = opening_balance + total_deposit - total_withdraw;
    
    printf("------------------------------------------------------------\n");
    printf("TOTAL -> %10.2f  %10.2f  %10.2f\n\n",
           total_deposit, total_withdraw, closing_balance);
}

void month_report()
{
    clear();
    long int accno;
    printf("Enter account number: ");
    scanf("%ld", &accno);

    // Open account file for found_account()
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) {
        printf("\n\tERROR!\nCannot open account file!\n");
        return;
    }

    // Pass FILE pointer to found_account()
    if (!found_account(fp, accno))
    {
        printf("\n\tERROR!\nAccount not found!\n");
        fclose(fp);
        return;
    }

    int month, year;
    printf("Enter statement month (1-12): ");
    scanf("%d", &month);
    printf("Enter statement year: ");
    scanf("%d", &year);

    if (month < 1 || month > 12)
    {
        printf("\n\tINVALID MONTH!\n");
        fclose(fp);
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

    // Only call this once - removed duplicate header
    box_for_display();

    // Read banking transactions
    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp)
    {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        fclose(fp);
        return;
    }

    banking t;
    float total_deposit = 0, total_withdraw = 0;
    float opening_balance = 0, closing_balance = 0;
    int found_initial = 0;

    // Find initial balance
    rewind(bank_fp);
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno && strcmp(t.trans, "Initial") == 0) {
            opening_balance = t.amount;
            found_initial = 1;
            break;
        }
    }
    rewind(bank_fp);

    // Print opening balance
    printf("%02d-%02d-%04d  %-10s  %10.2f  %10s  %10.2f\n",
           1, month, year, "Opening", opening_balance, "", opening_balance);

    // Process monthly transactions
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno && t.date.month == month && t.date.year == year)
        {
            printf("%02d-%02d-%04d  %-10s  ",
                   t.date.day, t.date.month, t.date.year, t.type);

            if (strcmp(t.trans, "Deposit") == 0 || strcmp(t.trans, "Initial") == 0)
            {
                printf("%10.2f  %10s  ", t.amount, "");
                total_deposit += t.amount;
            }
            else
            {
                printf("%10s  %10.2f  ", "", t.amount);
                total_withdraw += t.amount;
            }

            printf("%10.2f\n", t.balance);
            closing_balance = t.balance;
        }
    }
    fclose(bank_fp);
    fclose(fp);

    // Calculate closing balance if not set
    if (closing_balance == 0) {
        closing_balance = opening_balance + total_deposit - total_withdraw;
    }

    printf("------------------------------------------------------------\n");
    printf("TOTAL FOR MONTH: %10.2f  %10.2f\n", total_deposit, total_withdraw);
    printf("CLOSING BALANCE: %10.2f\n\n", closing_balance);
}

void box_for_display()
{
    printf("------------------------------------------------------------\n");
    printf("%-12s  %-10s  %10s  %10s  %10s\n",
           "Date", "Particular", "Deposit", "Withdraw", "Balance");
    printf("------------------------------------------------------------\n");
}

void return_name(int accno, char *name_out)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;
    
    if (!fp) {
        strcpy(name_out, "Error");
        return;
    }

    while (fread(&acc, sizeof(initial), 1, fp))
    {
        if (acc.acc_no == accno)
        {
            strcpy(name_out, acc.name);
            break;
        }
    }
    fclose(fp);
}

void return_address(int accno, char *address_out)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;

    if (!fp) {
        strcpy(address_out, "Error");
        return;
    }

    while (fread(&acc, sizeof(initial), 1, fp))
    {
        if (acc.acc_no == accno)
        {
            strcpy(address_out, acc.address);
            break;
        }
    }
    fclose(fp);
}