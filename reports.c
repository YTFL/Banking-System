#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "structs.h"
#include "util.h"
#include "account.h"
#include "reports.h"

// Case-insensitive string compare (cross-platform)
int equals_ignore_case(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower(*a) != tolower(*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

void display_account()
{
    long int accno;
    printf("Enter account number: ");
    scanf("%ld", &accno);

    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp)
    {
        printf("\n\tERROR!\nCannot open account file!\n");
        return;
    }

    if (!found_account(fp, accno))
    {
        printf("\n\tERROR!\nAccount not found!\n");
        fclose(fp);
        return;
    }

    char name[50], address[100];
    float initial_balance = 0.0;
    initial acc;
    rewind(fp);
    while (fread(&acc, sizeof(initial), 1, fp))
    {
        if (acc.acc_no == accno)
        {
            strcpy(name, acc.name);
            strcpy(address, acc.address);
            initial_balance = acc.balance;
            break;
        }
    }

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date today = {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};

    printf("\n\n\t\tGLOBAL REPORT OF ACCOUNT\n");
    printf("Account Number: %ld\n", accno);
    printf("%s\n", name);
    printf("Date: %02d/%02d/%d\n\n", today.day, today.month, today.year);

    box_for_display();

    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp)
    {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        fclose(fp);
        return;
    }

    banking t;
    float total_deposit = 0, total_withdraw = 0, running_balance = initial_balance;
    int transaction_count = 0;

    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno)
        {
            transaction_count++;
            printf("%02d-%02d-%04d  %-10s  ",
                   t.date.day, t.date.month, t.date.year, t.type);

            if (equals_ignore_case(t.trans, "Deposit") || equals_ignore_case(t.trans, "Initial"))
            {
                printf("%10.2f  %10s  ", t.amount, "");
                total_deposit += t.amount;
                running_balance += t.amount;
            }
            else
            {
                printf("%10s  %10.2f  ", "", t.amount);
                total_withdraw += t.amount;
                running_balance -= t.amount;
            }

            printf("%10.2f\n", running_balance);
        }
    }

    fclose(bank_fp);
    fclose(fp);

    if (transaction_count == 0)
    {
        printf("\tNo transactions found for this account!\n");
    }

    printf("------------------------------------------------------------\n");
    printf("TOTAL -> %10.2f  %10.2f  %10.2f\n\n",
           total_deposit, total_withdraw, "");
}

void month_report()
{
    long int accno;
    int ch;

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date today = {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};

    printf("Enter account number: ");
    scanf("%ld", &accno);
    clear_input_buffer();

    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp)
    {
        printf("\nERROR!\nCannot open account file!\n");
        return;
    }

    while (!found_account(fp, accno))
    {
        printf("\nAccount not found. Please enter a valid account number: ");
        scanf("%ld", &accno);
        clear_input_buffer();
        rewind(fp);
    }

    date from_date, to_date;
    banking t;

    printf("\nEnter FROM date (dd mm yyyy): ");// gather from date from user
    while (1)
    {
        if (scanf("%d %d %d", &from_date.day, &from_date.month, &from_date.year) != 3)
        {
            printf("Invalid format. Please enter as dd mm yyyy: ");
            clear_input_buffer();
            continue;
        }
        if (!is_valid_date(from_date))
        {
             if (from_date.year > today.year || from_date.day > today.day || from_date.month > today.month) {
                 printf("Error: Date beyond current date is not supported.\nPlease enter a real date : ");
                 continue;
            }
            else if (from_date.year < 1900) {
                printf("Error: Year must be 1900 or later.\nPlease enter a real date : ");
                continue;
            }
            else { printf("Invalid date. Please enter a real date : ");
            continue;
            }
        }
        break;
    }
    clear_input_buffer();

    printf("\nEnter TO date (dd mm yyyy): ");// gather to date from user
    while (1)
    {
        if (scanf("%d %d %d", &to_date.day, &to_date.month, &to_date.year) != 3)
        {
            printf("Invalid format. Please enter as dd mm yyyy: ");
            clear_input_buffer();
            continue;
        }
        if (!is_valid_date(to_date))
        {
             if (to_date.year > today.year || to_date.day > today.day || to_date.month > today.month) {
                 printf("Error: Date beyond current date is not supported.\nPlease enter a real date : ");
                 continue;
            }
            else if (to_date.year < 1900) {
                printf("Error: Year must be 1900 or later.\nPlease enter a real date : ");
                continue;
            }
            else { printf("Invalid date. Please enter a real date : ");
            continue;
            }
        }
        break;
    }
   clear_input_buffer();

    if (isEarlier(to_date, from_date))//swap dates if needed
    {
        date temp = from_date;
        from_date = to_date;
        to_date = temp;
    }

    char name[50];//variables
    float initial_amount = 0;
    initial acc;

    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp)
    {
        printf("\nSYSTEM ERROR!\nNo transaction records found!\n");
        fclose(fp);
        return;
    }

        rewind(fp);  // Ensure you're at the beginning

    while (fread(&acc, sizeof(initial), 1, fp)) {
        if (acc.acc_no == accno) {
            strcpy(name, acc.name);
            break;
        }
    }

    date creation_date = {0};  // To store the first transaction date (account creation)
    int creation_date_found = 0;

   banking first_transaction;


rewind(bank_fp);
while (fread(&t, sizeof(banking), 1, bank_fp)) //to detect first transaction aka creation date and amount aka opening deposit amount
{
    if (t.acc_no == accno) {
        if (!creation_date_found || isEarlier(t.date, first_transaction.date)) {
            first_transaction = t;
            creation_date_found = 1;
        }
    }
}
creation_date = first_transaction.date;

    float total_deposit = 0, total_withdraw = 0;// initialize totals
    float opening_balance = 0;
    float running_balance = opening_balance;
    float closing_balance = 0;
    int transaction_count = 0;
    banking latest_before_from;
    date opening_date;
    int found = 0;

    while (fread(&t, sizeof(banking), 1, bank_fp))//iterate through banking transactions
    {
        if (t.acc_no == accno && isEarlier(t.date, from_date))
        {
            if (!found || isEarlier(latest_before_from.date, t.date))
            {
                latest_before_from.balance = t.balance;
                latest_before_from.date = t.date;

                found = 1;
            }
        }
    }
     opening_balance = found ? latest_before_from.balance : first_transaction.balance;
     running_balance = opening_balance;
     if (creation_date_found && isEarlier(from_date, creation_date)) // if the from_date is before the creation date
    opening_date = creation_date;
     else 
    opening_date = from_date;
    
    printf("\n\n\t\t\t\t%s\n", name);
    printf("+============+==============+=============+============+===============+\n");
    printf("|   Date     |  Particular  |   Deposit   |  Withdraw  |    Balance    |\n");
    printf("+============+==============+=============+============+===============+\n");

    printf("| %02d-%02d-%04d | %-12s | %11.2s | %10.2s | %13.2f |\n",
           opening_date.day, opening_date.month, opening_date.year,
           "Opening", "   ", "   ", opening_balance);
    printf("+------------+--------------+-------------+-------------+--------------+\n");

    rewind(bank_fp);

    while (fread(&t, sizeof(banking), 1, bank_fp))//print records in the specified date range
    {
        if (t.acc_no == accno &&
            !isEarlier(t.date, from_date) && !isEarlier(to_date, t.date))
        { // Skip first transaction if already used as opening balance
        if (!found && t.date.day == first_transaction.date.day &&
            t.date.month == first_transaction.date.month &&
            t.date.year == first_transaction.date.year &&
            t.amount == first_transaction.amount &&
            strcmp(t.trans, first_transaction.trans) == 0 &&
            strcmp(t.type, first_transaction.type) == 0) 
            continue;  // skip duplicate
            
            float dep = 0.0, wdr = 0.0;

            if (equals_ignore_case(t.trans, "Deposit") || equals_ignore_case(t.trans, "Initial"))
            {
                dep = t.amount;
                total_deposit += dep;
                running_balance += dep;
            }
            else
            {
                wdr = t.amount;
                total_withdraw += wdr;
                running_balance -= wdr;
            }

            transaction_count++;

            printf("| %02d-%02d-%04d | %-12s | %11.2f | %10.2f | %13.2f |\n",
                   t.date.day, t.date.month, t.date.year, t.type,
                   dep, wdr, running_balance);
            printf("+------------+--------------+-------------+------------+---------------+\n");
        }
    }

    closing_balance = running_balance;

    if (transaction_count == 0)
    {
        printf("|   No transactions found in this date range.                          |\n");
        printf("+============+==============+=============+============+===============+\n");
    }
    else
    {
        printf("| %-10s | %-12s | %11.2f | %10.2f | %13.2s |\n",
               "Total", "", total_deposit, total_withdraw, "");
        printf("+============+==============+=============+============+===============+\n");
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

    if (!fp)
    {
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

    if (!fp)
    {
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
