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
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

void display_account()
{
    long int accno;
    initial acc;
    int found = 0;

    printf("Enter account number: ");
    scanf("%ld", &accno);
    clear_input_buffer(); // Assuming clear_input_buffer() handles the newline

    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp)
    {
        printf("\n\tERROR!\nCannot open account file!\n");
        return;
    }

    // Find the account and store its details in one pass
    while (fread(&acc, sizeof(initial), 1, fp))
    {
        if (acc.acc_no == accno)
        {
            found = 1;
            break;
        }
    }
    fclose(fp); // Close the file as soon as we're done with it

    if (!found)
    {
        printf("\n\tERROR!\nAccount not found!\n");
        return;
    }

    // Now display the retrieved account details
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date today = {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};

    printf("\n\n\t\tGLOBAL REPORT OF ACCOUNT\n");
    printf("Account Number: %ld\n", acc.acc_no);
    printf("%s\n", acc.name);
    printf("Date: %02d/%02d/%d\n\n", today.day, today.month, today.year);

    box_for_display();

    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp)
    {
        printf("\n\tSYSTEM ERROR!\nNo transaction records found!\n");
        return;
    }

    banking t;
    float total_deposit = 0, total_withdraw = 0;
    // The running balance starts with the initial balance from the account file
    float running_balance = acc.balance;
    int transaction_count = 0;

    // We add the initial deposit to the report display
    printf("%-10s  %-10s  %10.2f  %10s  %10.2f\n",
           "Initial", "Deposit", acc.balance, "", acc.balance);
    total_deposit += acc.balance;

    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno)
        {
            transaction_count++;
            printf("%02d-%02d-%-4d  %-10s  ",
                   t.date.day, t.date.month, t.date.year, t.type);

            if (equals_ignore_case(t.trans, "Deposit"))
            {
                printf("%10.2f  %10s  ", t.amount, "");
                total_deposit += t.amount;
                running_balance += t.amount;
            }
            else // Withdraw
            {
                printf("%10s  %10.2f  ", "", t.amount);
                total_withdraw += t.amount;
                running_balance -= t.amount;
            }

            printf("%10.2f\n", running_balance);
        }
    }
    fclose(bank_fp);

    if (transaction_count == 0)
    {
        printf("\tNo subsequent transactions found for this account!\n");
    }

    printf("------------------------------------------------------------\n");
    printf("TOTAL -> %22.2f  %10.2f\n\n",
           total_deposit, total_withdraw);
}

void month_report()
{
    long int accno;

    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) {
        printf("Could not open file.\n");
        return;
    }

    while (1) {
        printf("Enter account number or enter 0 to go back: ");
        scanf("%ld", &accno);
        clear_input_buffer(); 
        if (found_account(fp, accno)) {
            break;
        } else if (accno == 0) {
            fclose(fp);
            return;
        } else {
            printf("Account not found. Please try again.\n");
            rewind(fp);  
        }
    }

    initial acc;
    date from_date, to_date;
    printf("\nEnter FROM date (dd mm yyyy): ");
    while (scanf("%d %d %d", &from_date.day, &from_date.month, &from_date.year) != 3 || !is_valid_date(from_date))
    {
        printf("Invalid date. Please enter a real date (dd mm yyyy): ");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("\nEnter TO date (dd mm yyyy): ");
    while (scanf("%d %d %d", &to_date.day, &to_date.month, &to_date.year) != 3 || !is_valid_date(to_date))
    {
        printf("Invalid date. Please enter a real date (dd mm yyyy): ");
        clear_input_buffer();
    }
    clear_input_buffer();

    if (isEarlier(to_date, from_date))
    {
        date temp = from_date;
        from_date = to_date;
        to_date = temp;
    }

    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp)
    {
        printf("\nSYSTEM ERROR!\nNo transaction records found!\n");
        return;
    }

    banking t;
    // 1. Calculate the opening balance
    float opening_balance = acc.balance; // Start with the initial balance
    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        if (t.acc_no == accno && isEarlier(t.date, from_date))
        {
            if (equals_ignore_case(t.trans, "Deposit"))
            {
                opening_balance += t.amount;
            }
            else // Withdraw
            {
                opening_balance -= t.amount;
            }
        }
    }

    // 2. Print the report
    float total_deposit = 0, total_withdraw = 0;
    float running_balance = opening_balance;
    int transaction_count = 0;

    printf("\n\n\t\t\t\t%s\n", acc.name);
    printf("+============+==============+=============+============+===============+\n");
    printf("|   Date     |  Particular  |   Deposit   |  Withdraw  |    Balance    |\n");
    printf("+============+==============+=============+============+===============+\n");
    printf("| %02d-%02d-%04d | %-12s | %11s | %10s | %13.2f |\n",
           from_date.day, from_date.month, from_date.year,
           "Opening Bal", "", "", opening_balance);
    printf("+------------+--------------+-------------+------------+---------------+\n");

    rewind(bank_fp); // VERY IMPORTANT: Reset file pointer to the beginning

    while (fread(&t, sizeof(banking), 1, bank_fp))
    {
        // Check if the transaction is within the specified date range (inclusive)
        if (t.acc_no == accno && !isEarlier(t.date, from_date) && !isEarlier(to_date, t.date))
        {
            transaction_count++;
            float dep = 0.0, wdr = 0.0;

            if (equals_ignore_case(t.trans, "Deposit"))
            {
                dep = t.amount;
                total_deposit += dep;
                running_balance += dep;
            }
            else // Withdraw
            {
                wdr = t.amount;
                total_withdraw += wdr;
                running_balance -= wdr;
            }

            printf("| %02d-%02d-%04d | %-12s | %11.2f | %10.2f | %13.2f |\n",
                   t.date.day, t.date.month, t.date.year, t.type, dep, wdr, running_balance);
        }
    }
    fclose(bank_fp);

    printf("+------------+--------------+-------------+------------+---------------+\n");
    if (transaction_count == 0)
    {
        printf("|           No transactions found in this date range.               |\n");
    }
    else
    {
        printf("| Total      |              | %11.2f | %10.2f |               |\n",
               total_deposit, total_withdraw);
    }
    printf("+============+==============+=============+============+===============+\n");
}


void box_for_display()
{
    printf("------------------------------------------------------------\n");
    printf("%-10s  %-10s  %10s  %10s  %10s\n",
           "Date", "Particular", "Deposit", "Withdraw", "Balance");
    printf("------------------------------------------------------------\n");
}

void return_name(long int accno, char *name_out)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;
    strcpy(name_out, "Not Found"); // Default value

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

void return_address(long int accno, char *address_out)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;
    strcpy(address_out, "Not Found"); // Default value

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