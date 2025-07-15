#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "account.h"
#include "transactions.h"
#include "util.h"

void new_account(void)  { 
    initial acc;
    banking trans;
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    printf("\n----- Open New Account -----\n");
    acc.acc_no = last_accno() + 1;

    // Input for Name
    do {
        printf("\nEnter Name: ");
        fgets(acc.name, sizeof(acc.name), stdin);

        if (!strchr(acc.name, '\n')) {
            clear_input_buffer();
        }

        acc.name[strcspn(acc.name, "\n")] = '\0';
    } while(strlen(acc.name) == 0);

        if (strlen(acc.name) == 0) {
            printf("Name cannot be empty.\n");
        }
    } while (strlen(acc.name) == 0);

    do {
        printf("Enter Address: ");
        fgets(acc.address, sizeof(acc.address), stdin);

        if (!strchr(acc.address, '\n')) {
            clear_input_buffer();
        }

        acc.address[strcspn(acc.address, "\n")] = '\0';

        if (strlen(acc.address) == 0) {
            printf("Address cannot be empty.\n");
        }
    } while (strlen(acc.address) == 0);


    char input[100];

    do {
        printf("Enter Initial deposit (>= 500): ");

        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input.\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (sscanf(input, "%lf", &acc.balance) != 1) {
            printf("Invalid input. Please enter a numeric value.\n");
            continue;
        }

        if (acc.balance < 500 || acc.balance > 999999999.99) {
            printf("The Minimum initial deposit is 500 and less than 1 billion.\n");
        }

    } while (acc.balance < 500 || acc.balance > 999999999.99);


    printf("\nPlease review your details:\n");
    printf("Account Number : %ld\n", acc.acc_no);
    printf("Name           : %s\n", acc.name);
    printf("Address        : %s\n", acc.address);
    printf("Balance        : %.2lf\n", acc.balance);

    char confirm;
    do {
        printf("\nConfirm to create this account? (Y/N): ");
        confirm = getchar();
        while (getchar() != '\n');
    } while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

    if (confirm == 'N' || confirm == 'n') {
        printf("Account creation cancelled.\n");
        return;
    }


    add_to_file(acc);

    trans.acc_no = acc.acc_no;
    strcpy(trans.trans, "deposit");
    strcpy(trans.type, "cash");
    trans.amount = acc.balance;
    trans.interest = 0;
    trans.balance = acc.balance;
    trans.date.day = local->tm_mday;
    trans.date.month = local->tm_mon + 1;
    trans.date.year = local->tm_year + 1900;
    strcpy(trans.remarks, "Initial deposit");

    add_to_file_transaction(trans);

    printf("\nAccount created successfully!\n");
    printf("Account Number : %ld\n", acc.acc_no);
    printf("Balance        : %.2f\n", acc.balance);
    printf("An initial transaction has been recorded in BANKING.dat\n");
}


void add_to_file(initial acc) {
   FILE *fp = fopen("INITIAL.dat","ab");
   if(fp == NULL) {
        printf("Could not open file.\n");
        return;
   }
   fwrite(&acc, sizeof(acc), 1, fp);
   fclose(fp); 
}

void display_list() {
    FILE *fp = fopen("INITIAL.dat","rb");
    if(fp == NULL)
    {
        printf("Cannot Open File\n");
        return ;
    }

    initial acc;
    double total_balance = 0 ;

    printf("+====================+========================+======================================================+=====================+\n");
    printf("| %-18s | %-22s | %-52s | %-19s |\n", "Account Number", "Name", "Address", "Balance");
    printf("+====================+========================+======================================================+=====================+\n");

    while (fread(&acc, sizeof(initial), 1, fp) == 1) {
        printf("| %-18ld | %-22s | %-52s | %19.2f |\n", acc.acc_no, acc.name, acc.address, acc.balance);
        printf("+--------------------+------------------------+------------------------------------------------------+---------------------+\n");
        total_balance += acc.balance;
    }

    printf("| %-18s | %-22s | %-52s | %19.2f |\n", "", "", "Total Balance in Bank:", total_balance);
    printf("+--------------------+------------------------+------------------------------------------------------+---------------------+\n");

    fclose(fp);
}

void display() {
    long int acc_no;
    FILE *fp = fopen("INITIAL.dat","rb");
    if(fp == NULL) {
        printf("Cannot open file\n");
        return;
    }
    initial acc;
    while (1) {
        printf("Enter account number : ");
        scanf("%ld", &acc_no);
        while ((getchar()) != '\n'); 
        if (found_account(fp, acc_no)) {
            break;
        } else {
            printf("Account not found. Please try again.\n");
            rewind(fp);  
        }
    }
    rewind(fp);
     
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("\n+------------------------ ACCOUNT DETAILS ------------------------+\n");
            printf("Account Number : %ld\n", acc.acc_no);
            printf("Name           : %s\n", acc.name);
            printf("Address        : %s\n", acc.address);
            printf("Balance        : %.2f\n", acc.balance);
            break;
        }
    }
  
    fclose(fp);  
}

void modify(FILE *fp, long pos, initial *acc) {
    fseek(fp, pos, SEEK_SET);
    fwrite(acc, sizeof(initial), 1, fp);
    printf("Account changes have been saved to the file.\n");
}

int found_account(FILE *fp, int acc_no) {
    if (!fp) return 0;
    initial acc;
    rewind(fp);
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            return 1;
        }
    }
    return 0;
}

void modify_account(int choice) {
    long int acc_no;
    printf("Enter account number to modify: ");
    scanf("%ld", &acc_no);
    while ((getchar()) != '\n');

    FILE *fp = fopen("INITIAL.dat", "rb+");
    if (!fp) {
        printf("Could not open file.\n");
        return;
    }
    if (!found_account(fp, acc_no)) {
        printf("Account not found.\n");
        fclose(fp);
        return;
    }
    rewind(fp);
    initial acc;
    long pos;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            pos = ftell(fp) - sizeof(acc);
            break;
        }
    }
    if (choice == 1) {
        do {
        printf("\nEnter Name: ");
        fgets(acc.name, sizeof(acc.name), stdin);

        if (!strchr(acc.name, '\n')) {
            clear_input_buffer();
        }

        acc.name[strcspn(acc.name, "\n")] = '\0';

        if (strlen(acc.name) == 0) {
            printf("Name cannot be empty.\n");
        }
    } while (strlen(acc.name) == 0);
    } else if (choice == 2) {
        do {
        printf("Enter Address: ");
        fgets(acc.address, sizeof(acc.address), stdin);

        if (!strchr(acc.address, '\n')) {
            clear_input_buffer();
        }

        acc.address[strcspn(acc.address, "\n")] = '\0';

        if (strlen(acc.address) == 0) {
            printf("Address cannot be empty.\n");
        }
    } while (strlen(acc.address) == 0);
    }
    printf("\nReview updated details:\n");
    printf("Account Number : %ld\n", acc.acc_no);
    printf("Name           : %s\n", acc.name);
    printf("Address        : %s\n", acc.address);
    printf("Balance        : %.2lf\n", acc.balance);

    char confirm;
    do {
        printf("\nConfirm to create this account? (Y/N): ");
        confirm = getchar();
        while (getchar() != '\n');
    } while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

    if (confirm == 'N' || confirm == 'n') {
        printf("Account creation cancelled.\n");
        return;
    }
    modify(fp, pos, &acc);
    fclose(fp);
}

void delete_account() {
    long int acc_no;
    FILE *fp = fopen("INITIAL.dat","rb");
    if(fp == NULL) {
        printf("Cannot open file");
        return;
    }
    printf("Enter account number you want to delete : ");
    scanf("%ld",&acc_no);
    while((getchar()) != '\n');

    if (!found_account(fp, acc_no)) {
        printf("Account not found.\n");
        fclose(fp);
        return;
    }
    printf("Are you sure you want to delete account %ld? (Y/N): ", acc_no);
    char confirm = getchar();
    while ((getchar()) != '\n');
    if (confirm != 'Y' && confirm != 'y') {
        printf("Deletion cancelled.\n");
        return;
    }
    rewind(fp);
    FILE *temp = fopen("temp.dat","wb");
    if (temp == NULL) {
        printf("Cannot open file");
        fclose(fp);
        return;
    }
    
    initial acc;
    while(fread(&acc, sizeof(acc), 1, fp)) {
        if(acc.acc_no == acc_no) continue;
        fwrite(&acc, sizeof(acc), 1 , temp);
   }
   fclose(fp);
   fclose(temp);
   remove("INITIAL.dat");
   rename("temp.dat","INITIAL.dat");
   close_account(acc_no);
   printf("Account and associated transactions deleted successfully.\n");
}

void close_account(long int acc_no) {
    FILE *fp = fopen("BANKING.dat","rb");
    if(fp == NULL) {
        printf("Cannot open file");
        return;
    }
    
    FILE *temp = fopen("temp.dat","wb");
    if(temp == NULL) {
        printf("Cannot open file");
        fclose(fp);
        return;
    }
    
    banking ban;
    while(fread(&ban, sizeof(ban) , 1 , fp)) {
        if(ban.acc_no == acc_no) continue;
        fwrite(&ban, sizeof(ban), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    remove("BANKING.dat");
    rename("temp.dat","BANKING.dat");
}

long int last_accno() {
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) return 1000000000;
    initial acc;
    long int last = 1000000000;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no > last) last = acc.acc_no;
    }
    fclose(fp);
    return last;
}

int recordno() {
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) return 0;
    int count = 0;
    initial acc;
    while (fread(&acc, sizeof(acc), 1, fp)) count++;
    fclose(fp);
    return count;
}