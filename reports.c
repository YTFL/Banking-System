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

void month_report() {
    FILE *fp;
    banking t;
    long int acc_no;
    int month, year;
    int found = 0;
    float total_deposit = 0.0, total_withdraw = 0.0;

    fp = fopen("BANKING.dat", "rb");
    if (fp == NULL) {
        printf("Error: Cannot open BANKING.dat\n");
        return;
    }
    
    long int accno;
    initial acc;
    while (1) {
        FILE *fp_initial = fopen("INITIAL.dat", "rb");
        if (fp_initial == NULL) {
            printf("Error opening account file.\n");
            return;
        }
        
        while (1) {
            printf("Enter account number or enter 0 to go back: ");
            scanf("%ld", &acc_no);
            clear_input_buffer(); 
            if (found_account(fp_initial, acc_no)) {
                break;
            } else if (accno == 0) {
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
    printf("Current balance: %.2f\n", acc.balance);

    char confirm;
    do {
        printf("\nProceed with this account? (Y/N): ");
        confirm = getchar();
        while (getchar() != '\n');
        if (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n') {
            printf("Invalid input. Please enter Y or N.\n");
        }
    } while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

    if (confirm == 'N' || confirm == 'n') {
        printf("Reports cancelled.\n");
        return;
    }
    
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_month = current_time->tm_mon + 1;
    int current_year = current_time->tm_year + 1900; 
    while (1) {
        printf("Enter month (MM) or enter 0 to cancel: ");
        scanf("%d", &month);
        if (month == 0) {
            fclose(fp);
            return;
        }
        if (month < 1 || month > 12) {
            printf("Invalid month. Please enter a value between 1 and 12.\n");
            continue;
        }

        printf("Enter year (YYYY) or enter 0 to cancel: ");
        scanf("%d", &year);
        if (year == 0) {
            fclose(fp);
            return;
        }
        if (year < 1900) {
            printf("Invalid year. Please enter a year >= 1900.\n");
            continue;
        } 
        if (year > current_year || (year == current_year && month > current_month)) {
            printf("Entered date is in the future. Please try again.\n");
            continue;
        }

        break;
    }
    
    char name[20], address[50];
    return_name(acc_no, name);
    return_address(acc_no, address);
    printf("\nAccount Number: %ld\n", acc_no);
    printf("Account Holder: %s\n", name);
    printf("Address: %s\n", address);
    printf("Month: %02d, Year: %d\n", month, year);
    printf("\n\t\tMONTHLY STATEMENT\n");
    printf("+============+=============+==========+==========+==========+\n");
    printf("| Date       | Particular  | Deposit  | Withdraw | Balance  |\n");
    printf("+============+=============+==========+==========+==========+\n");

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.acc_no == acc_no && t.date.month == month && t.date.year == year) {
            found = 1;

            float deposit_amt = 0.0, withdraw_amt = 0.0;
            if ((strcmp(t.trans, "Deposit") == 0)|| (strcmp(t.trans, "deposit") == 0))
                deposit_amt = t.amount;
            else if ((strcmp(t.trans, "Withdraw") == 0) || (strcmp(t.trans, "withdraw") == 0))
                withdraw_amt = t.amount;

            total_deposit += deposit_amt;
            total_withdraw += withdraw_amt;

            printf("| %02d-%02d-%04d | %-11s | %8.2f | %8.2f | %8.2f |\n",
                   t.date.day, t.date.month, t.date.year,
                   t.type, deposit_amt, withdraw_amt, t.balance);
        }
    }

    if (!found) {
        printf("| No transactions found for this account in %02d/%d.        |\n", month, year);
    }

    printf("+------------+-------------+----------+----------+----------+\n");

    if (found) {
        printf("| %-24s | %8.2f | %8.2f |          |\n",
               "Total", total_deposit, total_withdraw);
        printf("+------------+-------------+----------+----------+----------+\n");
    }

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