#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "account.h"


void new_account(void) 
{
    initial acc;
    printf("\n----- Open New Account -----\n");
    acc.acc_no = last_accno() + 1;

    // Take name
    do {
        printf("\nEnter Name (max 20 characters): ");
        fgets(acc.name, sizeof(acc.name), stdin);
        int len = strlen(acc.name);
        if (len > 0 && acc.name[len - 1] == '\n') {
            acc.name[len - 1] = '\0';
        } else {
            while ((getchar()) != '\n');
        }

        if (strlen(acc.name) == 0) {
            printf("Name cannot be empty.\n");
            continue;
        }

        if (strlen(acc.name) > 20) {
            printf("WARNING: Name longer than 20. Continue with truncation? (Y/N): ");
            char ch = getchar();
            while ((getchar()) != '\n');
            if (ch != 'Y' && ch != 'y') continue;
            acc.name[20] = '\0';
        }
        break;
    } while(1);

    // Take address
    do {
        printf("\nEnter Address (max 50 characters): ");
        fgets(acc.address, sizeof(acc.address), stdin);
        int len = strlen(acc.address);
        if (len > 0 && acc.address[len - 1] == '\n') {
            acc.address[len - 1] = '\0';
        } else {
            while ((getchar()) != '\n');
        }

        if (strlen(acc.address) == 0) {
            printf("Address cannot be empty.\n");
            continue;
        }

        if (strlen(acc.address) > 50) {
            printf("WARNING: Address longer than 50. Continue with truncation? (Y/N): ");
            char ch = getchar();
            while ((getchar()) != '\n');
            if (ch != 'Y' && ch != 'y') continue;
            acc.address[50] = '\0';
        }
        break;
    } while(1);

    // Take initial deposit
    do {
        printf("\nEnter Initial deposit (>=500): ");
        if (scanf("%f", &acc.balance) != 1) {
            printf("Invalid input.\n");
            while ((getchar()) != '\n');
            continue;
        }
        while ((getchar()) != '\n');
        if (acc.balance < 500) {
            printf("Deposit must be >=500.\n");
        }
    } while (acc.balance < 500);

    add_to_file(acc);

    printf("\nAccount created successfully!\n");
    printf("Account Number : %ld\n", acc.acc_no);
    printf("Balance        : %.2f\n", acc.balance);
}

// -------------------------
// MODIFY ACCOUNT
// -------------------------
void modify_account(int choice) 
{
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
    long pos = -1;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            pos = ftell(fp) - sizeof(acc);
            break;
        }
    }

    if (choice == 1) {
        // Modify name
        do {
            printf("Enter new name (max 20 characters): ");
            fgets(acc.name, sizeof(acc.name), stdin);
            int len = strlen(acc.name);
            if (len > 0 && acc.name[len - 1] == '\n') {
                acc.name[len - 1] = '\0';
            } else {
                while ((getchar()) != '\n');
            }

            if (strlen(acc.name) == 0) {
                printf("Name cannot be empty.\n");
                continue;
            }

            if (strlen(acc.name) > 20) {
                printf("WARNING: Name longer than 20. Continue with truncation? (Y/N): ");
                char ch = getchar();
                while ((getchar()) != '\n');
                if (ch != 'Y' && ch != 'y') continue;
                acc.name[20] = '\0';
            }
            break;
        } while(1);
        printf("Name updated.\n");

    } else if (choice == 2) {
        // Modify address
        do {
            printf("Enter new address (max 50 characters): ");
            fgets(acc.address, sizeof(acc.address), stdin);
            int len = strlen(acc.address);
            if (len > 0 && acc.address[len - 1] == '\n') {
                acc.address[len - 1] = '\0';
            } else {
                while ((getchar()) != '\n');
            }

            if (strlen(acc.address) == 0) {
                printf("Address cannot be empty.\n");
                continue;
            }

            if (strlen(acc.address) > 50) {
                printf("WARNING: Address longer than 50. Continue with truncation? (Y/N): ");
                char ch = getchar();
                while ((getchar()) != '\n');
                if (ch != 'Y' && ch != 'y') continue;
                acc.address[50] = '\0';
            }
            break;
        } while(1);
        printf("Address updated.\n");
    }

    modify(fp, pos, &acc);
    fclose(fp);
}

// -------------------------
// YOUR EXISTING FUNCTIONS
// -------------------------
void add_to_file(initial acc) {
   FILE *fp = fopen("INITIAL.dat","ab");
   if(fp == NULL){
        printf("Could not open file.\n");
        return;
   }
   fwrite(&acc, sizeof(acc), 1, fp);
   fclose(fp); 
}

void display_list() {
    FILE *fp = fopen("INITIAL.dat","rb");
    if(fp == NULL) {
        printf("Cannot Open File\n");
        return;
    }

    initial acc;
    double total_balance = 0;

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
        printf("Cannot open file");
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
            printf(" Account not found please try again.\n");
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

void delete_account() {
   long int acc_no;
   FILE *fp = fopen("INITIAL.dat","rb");
   if(fp == NULL) {
       printf("Cannot open file");
       return;
   }
  while (1) {
    printf("Enter account number you want to delete: ");
    if (scanf("%ld", &acc_no) != 1) {
        printf("Invalid input. Please re-enter account number.\n");
        while ((getchar()) != '\n');
        continue;
    }
    while ((getchar()) != '\n'); 
    if (!found_account(fp, acc_no)) {
        printf(" Account not found.\n");
        rewind(fp);
    } else {
        break;
    }
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
       if(acc.acc_no == acc_no) {
           continue;
       }
        fwrite(&acc, sizeof(acc), 1 , temp);
    }
    fclose(fp);
    fclose(temp);
    remove("INITIAL.dat");
    rename("temp.dat","INITIAL.dat");
    printf("Account deleted successfully .\n\n");
    close_account(acc_no);
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
        if(ban.acc_no == acc_no) {
            continue;
        }
        fwrite(&ban, sizeof(ban), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    remove("BANKING.dat");
    rename("temp.dat","BANKING.dat");
    printf("Transactions deleted successfully\n");
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
    while (fread(&acc, sizeof(acc), 1, fp)) {
        count++;
    }
    fclose(fp);
    return count;
}
