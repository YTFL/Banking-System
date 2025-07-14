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

    banking t;  // Changed to banking struct
    float total_deposit = 0, total_withdraw = 0;
    int transaction_count = 0;

    printf("%-10s  %-10s  %10s  %10s  %10s\n",
           "Date", "Particular", "Deposit", "Withdraw", "Balance");
    printf("------------------------------------------------------------\n");

    // Read banking structs
    while (fread(&t, sizeof(banking), 1, bank_fp))  // Changed to banking
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
    fclose(fp);  // Close account file

    if (transaction_count == 0)
    {
        printf("\tNo transactions found for this account!\n");
    }

    printf("------------------------------------------------------------\n");
    printf("TOTAL -> %10.2f  %10.2f  %10.2f\n\n",
           total_deposit, total_withdraw, total_deposit - total_withdraw);
}

void date_range_report()
{
    long int accno;
    int ch;

    printf("Enter account number: ");
    scanf("%ld", &accno);
    while ((ch = getchar()) != '\n' && ch != EOF);  // Clear buffer

    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) {
        printf("\n\tERROR!\nCannot open account file!\n");
        return;
    }

    while (!found_account(fp, accno))
    {
        printf("\n\tAccount not found. Please enter a valid account number: ");
        scanf("%ld", &accno);
        while ((ch = getchar()) != '\n' && ch != EOF);
        rewind(fp);
    }

    date from_date, to_date;

    printf("\nEnter FROM date (dd mm yyyy): ");
    scanf("%d %d %d", &from_date.day, &from_date.month, &from_date.year);
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("Enter TO date (dd mm yyyy): ");
    scanf("%d %d %d", &to_date.day, &to_date.month, &to_date.year);
    while ((ch = getchar()) != '\n' && ch != EOF);

    // Swap if dates are reversed
    if (!isEarlier(from_date, to_date)) {
        date temp = from_date;
        from_date = to_date;
        to_date = temp;
    }

    // Get account name and initial deposit
    char name[50];
    float initial_amount = 0;
    initial acc;
    rewind(fp);
    while (fread(&acc, sizeof(initial), 1, fp))
    {
        if (acc.acc_no == accno)
        {
            strcpy(name, acc.name);
            initial_amount = acc.balance;
            break;
        }
    }

    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp)
    {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        fclose(fp);
        return;
    }

    banking t;
    float total_deposit = 0, total_withdraw = 0;
    float opening_balance = initial_amount, closing_balance = initial_amount;
    int transaction_count = 0;

    // Determine opening balance before from_date
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno && isEarlier(t.date, from_date))
        {
            opening_balance = t.balance;
        }
    }
    rewind(bank_fp);

    // Print the report header
    printf("\n\n\t\t\t\t%s\n", name);
    printf("+===========+==============+=============+============+===============+\n");
    printf("|   Date    |  Particular  |   Deposit   |  Withdraw  |    Balance    |\n");
    printf("+===========+==============+=============+============+===============+\n");

    // Print opening line
    printf("| %02d-%02d-%02d | %-12s | %11.2f | %10.2f | %13.2f |\n",
           from_date.day, from_date.month, from_date.year,
           "Opening", 0.00, 0.00, opening_balance);

    printf("+-----------+--------------+-------------+------------+---------------+\n");

    // Process transactions
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno &&
            (isEarlier(from_date, t.date) || (from_date.day == t.date.day && from_date.month == t.date.month && from_date.year == t.date.year)) &&
            (isEarlier(t.date, to_date) || (to_date.day == t.date.day && to_date.month == t.date.month && to_date.year == t.date.year)))
        {
            transaction_count++;

            float dep = 0.0, wdr = 0.0;
            if (strcmp(t.trans, "Deposit") == 0 || strcmp(t.trans, "Initial") == 0)
            {
                dep = t.amount;
                total_deposit += dep;
            }
            else
            {
                wdr = t.amount;
                total_withdraw += wdr;
            }

            printf("| %02d-%02d-%02d | %-12s | %11.2f | %10.2f | %13.2f |\n",
                   t.date.day, t.date.month, t.date.year, t.type,
                   dep, wdr, t.balance);

            printf("+-----------+--------------+-------------+------------+---------------+\n");

            closing_balance = t.balance;
        }
    }

    if (transaction_count == 0)
    {
        printf("|   No transactions found in this date range.                          |\n");
        printf("+===========+==============+=============+============+===============+\n");
    }
    else
    {
        // Totals row
        printf("| %-10s | %-12s | %11.2f | %10.2f | %13.2f |\n",
               "Total", "", total_deposit, total_withdraw, closing_balance);
        printf("+===========+==============+=============+============+===============+\n");
    }

    fclose(bank_fp);
    fclose(fp);
}



void box_for_display()
{
    printf("------------------------------------------------------------\n");
    printf("%-10s  %-10s  %10s  %10s  %10s\n",
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