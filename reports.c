#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "util.h"
#include "account.h"
#include "reports.h"

// Helper function to compare dates for sorting
int compare_dates(const void *a, const void *b) {
    banking *t1 = (banking *)a;
    banking *t2 = (banking *)b;
    
    if (t1->date.year != t2->date.year) 
        return t1->date.year - t2->date.year;
    if (t1->date.month != t2->date.month) 
        return t1->date.month - t2->date.month;
    return t1->date.day - t2->date.day;
}

// Updated table formatting to match your example
void box_for_display() {
    printf("\n| Date       | Particular   | Deposit     | Withdraw    | Balance    |\n");
    printf("|------------|--------------|-------------|-------------|------------|\n");
}

void return_name(long int accno, char *name_out) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) return;
    
    initial acc;
    while (fread(&acc, sizeof(initial), 1, fp)) {
        if (acc.acc_no == accno) {
            strcpy(name_out, acc.name);
            break;
        }
    }
    fclose(fp);
}

void custom_date_report() {
    long int accno;
    FILE *fp = NULL;
    int valid_account = 0;
    
    // Get valid account number with retry
    while (!valid_account) {
        printf("Enter account number: ");
        if (scanf("%ld", &accno) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');  // Clear buffer
        
        fp = fopen("INITIAL.dat", "rb");
        if (!fp) {
            printf("\nERROR! Cannot open account file!\n");
            return;
        }
        
        if (found_account(fp, accno)) {
            valid_account = 1;
        } else {
            printf("\nAccount not found! Please try again.\n");
        }
        fclose(fp);
    }

    // Get date range
    date start_date, end_date;
    printf("Enter start date (DD MM YYYY): ");
    if (scanf("%d %d %d", &start_date.day, &start_date.month, &start_date.year) != 3) {
        printf("Invalid date format.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');  // Clear buffer
    
    printf("Enter end date (DD MM YYYY): ");
    if (scanf("%d %d %d", &end_date.day, &end_date.month, &end_date.year) != 3) {
        printf("Invalid date format.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');  // Clear buffer

    // Validate dates using util.h functions
    if (start_date.month < 1 || start_date.month > 12 ||
        end_date.month < 1 || end_date.month > 12 ||
        start_date.day < 1 || start_date.day > 31 ||
        end_date.day < 1 || end_date.day > 31) {
        printf("\nINVALID DATE!\n");
        return;
    }

    char name[50];
    return_name(accno, name);

    // Get current date for report
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date today = {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};

    // Print report header
    printf("\n| CUSTOM DATE RANGE REPORT                              |\n");
    printf("|--------------------------------------------------------|\n");
    printf("| Account: %-42ld |\n", accno);
    printf("| Name: %-45s |\n", name);
    printf("| Period: %02d/%02d/%d to %02d/%02d/%d                     |\n", 
           start_date.day, start_date.month, start_date.year,
           end_date.day, end_date.month, end_date.year);
    printf("| Report Date: %02d/%02d/%d                              |\n", 
           today.day, today.month, today.year);
    printf("|--------------------------------------------------------|\n");

    // Read and process transactions
    FILE *bank_fp = fopen("BANKING.dat", "rb");
    if (!bank_fp) {
        printf("\nSYSTEM ERROR! No transaction records found!\n");
        return;
    }

    // Collect relevant transactions
    banking t;
    banking *transactions = NULL;
    int count = 0, capacity = 10;
    float opening_balance = 0.0;
    int found_initial = 0;
    float total_deposit = 0.0, total_withdraw = 0.0;
    
    // Get initial balance
    FILE *initial_fp = fopen("INITIAL.dat", "rb");
    if (initial_fp) {
        initial acc;
        while (fread(&acc, sizeof(initial), 1, initial_fp)) {
            if (acc.acc_no == accno) {
                opening_balance = acc.balance;
                found_initial = 1;
                break;
            }
        }
        fclose(initial_fp);
    }
    
    if (!found_initial) {
        printf("Account initial balance not found!\n");
        fclose(bank_fp);
        return;
    }

    // Allocate initial array
    transactions = malloc(capacity * sizeof(banking));
    if (!transactions) {
        printf("Memory allocation error!\n");
        fclose(bank_fp);
        return;
    }

    // Find initial balance and collect transactions
    while (fread(&t, sizeof(banking), 1, bank_fp)) {
        if (t.acc_no == accno) {
            // Check if within date range
            if ((t.date.year > start_date.year || 
                (t.date.year == start_date.year && t.date.month > start_date.month) ||
                (t.date.year == start_date.year && t.date.month == start_date.month && t.date.day >= start_date.day)) &&
                (t.date.year < end_date.year || 
                (t.date.year == end_date.year && t.date.month < end_date.month) ||
                (t.date.year == end_date.year && t.date.month == end_date.month && t.date.day <= end_date.day))) {
                
                if (count >= capacity) {
                    capacity *= 2;
                    banking *temp = realloc(transactions, capacity * sizeof(banking));
                    if (!temp) {
                        free(transactions);
                        fclose(bank_fp);
                        printf("Memory reallocation error!\n");
                        return;
                    }
                    transactions = temp;
                }
                transactions[count++] = t;
            }
        }
    }
    fclose(bank_fp);

    // Sort transactions by date
    qsort(transactions, count, sizeof(banking), compare_dates);

    // Print table header
    box_for_display();

    // Process transactions in sorted order
    float running_balance = opening_balance;
    
    // Print opening balance
    printf("| %02d-%02d-%04d | %-12s | %10.2f | %10.2f | %10.2f |\n",
           start_date.day, start_date.month, start_date.year,
           "Opening", 0.00, 0.00, opening_balance);
    
    for (int i = 0; i < count; i++) {
        float deposit_amt = 0.0, withdraw_amt = 0.0;
        
        if (strcmp(transactions[i].trans, "Deposit") == 0 || 
            strcmp(transactions[i].trans, "Initial") == 0) {
            deposit_amt = transactions[i].amount;
            running_balance += transactions[i].amount;
            total_deposit += transactions[i].amount;
        } else {
            withdraw_amt = transactions[i].amount;
            running_balance -= transactions[i].amount;
            total_withdraw += transactions[i].amount;
        }
        
        printf("| %02d-%02d-%04d | %-12s | %10.2f | %10.2f | %10.2f |\n",
               transactions[i].date.day, transactions[i].date.month, transactions[i].date.year, 
               transactions[i].type,
               deposit_amt, withdraw_amt, running_balance);
    }

    // Print summary
    printf("|--------------------------------------------------------|\n");
    printf("| OPENING BALANCE: %38.2f |\n", opening_balance);
    printf("| TOTAL DEPOSITS:  %38.2f |\n", total_deposit);
    printf("| TOTAL WITHDRAW:  %38.2f |\n", total_withdraw);
    printf("| CLOSING BALANCE: %38.2f |\n\n", running_balance);
    
    free(transactions);
}

void month_report() {
    custom_date_report();
}