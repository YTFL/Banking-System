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

void display_account() {
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
    
    initial acc;
    while (1) {
        FILE *fp_initial = fopen("INITIAL.dat", "rb");
        if (fp_initial == NULL) {
            printf("Error opening account file.\n");
            return;
        }
        
        while (1) {
            printf("Enter account number or enter 0 to go back: ");
            if (scanf("%ld", &acc_no) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
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
    
    char name[20], address[50];
    return_name(acc_no, name);
    return_address(acc_no, address);
    printf("\nAccount Number: %ld\n", acc_no);
    printf("Account Holder: %s\n", name);
    printf("Address: %s\n", address);
    printf("Month: %02d, Year: %d\n", month, year);
    printf("\n\t\t\t\t    MONTHLY STATEMENT\n");
    box_for_display();

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.acc_no == acc_no && t.date.month == month && t.date.year == year) {
            found = 1;

            float deposit_amt = 0.0, withdraw_amt = 0.0;
            if (strcasecmp(t.trans, "Deposit") == 0)
                deposit_amt = t.amount;
            else if (strcasecmp(t.trans, "Withdraw") == 0)
                withdraw_amt = t.amount;

            total_deposit += deposit_amt;
            total_withdraw += withdraw_amt;

            printf("| %02d-%02d-%04d | %-11s | %19.2lf | %19.2lf | %19.2lf |\n",
                   t.date.day, t.date.month, t.date.year,
                   t.type, deposit_amt, withdraw_amt, t.balance);
            printf("+------------+-------------+---------------------+---------------------+---------------------+\n");
        }
    }

    if (!found) {
        printf("| No transactions found for this account.                                                    |\n");
    }

    if (found) {
        printf("| %-24s | %19.2lf | %19.2lf | %19s |\n",
               "Total", total_deposit, total_withdraw, "");
    }
    printf("+------------+-------------+---------------------+---------------------+---------------------+\n");

    fclose(fp);
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
    
    initial acc;
    while (1) {
        FILE *fp_initial = fopen("INITIAL.dat", "rb");
        if (fp_initial == NULL) {
            printf("Error opening account file.\n");
            return;
        }
        
        while (1) {
            printf("Enter account number or enter 0 to go back: ");
            if (scanf("%ld", &acc_no) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
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
        if (scanf("%d", &month) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        if (month == 0) {
            printf("Reports cancelled.\n");
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
            printf("Reports cancelled.\n");
            fclose(fp);
            return;
        }
        if (year < 1900) {
            printf("Invalid year. Please enter a year >= 1900.\n");
            clear_input_buffer();
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
    printf("\n\t\t\t\t    MONTHLY STATEMENT\n");
    box_for_display();

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.acc_no == acc_no && t.date.month == month && t.date.year == year) {
            found = 1;

            float deposit_amt = 0.0, withdraw_amt = 0.0;
            if (strcasecmp(t.trans, "Deposit") == 0)
                deposit_amt = t.amount;
            else if (strcasecmp(t.trans, "Withdraw") == 0)
                withdraw_amt = t.amount;

            total_deposit += deposit_amt;
            total_withdraw += withdraw_amt;

            printf("| %02d-%02d-%04d | %-11s | %19.2lf | %19.2lf | %19.2lf |\n",
                   t.date.day, t.date.month, t.date.year,
                   t.type, deposit_amt, withdraw_amt, t.balance);
            printf("+------------+-------------+---------------------+---------------------+---------------------+\n");
        }
    }

    if (!found) {
        printf("| No transactions found for this account in %02d/%d.                                         |\n", month, year);
    }

    if (found) {
        printf("| %-24s | %19.2lf | %19.2lf | %19s |\n",
               "Total", total_deposit, total_withdraw, "");
    }
    printf("+------------+-------------+---------------------+---------------------+---------------------+\n");

    fclose(fp);
}

void box_for_display()
{
    printf("+============+=============+=====================+=====================+=====================+\n");
    printf("| Date       | Particular  |      Deposit        |      Withdraw       |        Balance      |\n");
    printf("+============+=============+=====================+=====================+=====================+\n");
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