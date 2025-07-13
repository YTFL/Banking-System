#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "account.h"

void new_account(void)
{
     initial acc;
     printf("\n-----Open New Account-----\n");
     acc.acc_no = last_accno() + 1;
    do {
        printf("\nEnter Name: \n");
        fgets(acc.name,sizeof(acc.name),stdin);
        int len = strlen(acc.name);
        if (len > 0 && acc.name[len - 1] == '\n') {
            acc.name[len - 1] = '\0';
        }
    } while(strlen(acc.name) == 0);
   
    do {
        printf("Enter Address : \n");
        fgets(acc.address,sizeof(acc.address),stdin);
        int len = strlen(acc.address);
        if(len > 0  && acc.address[len-1] == '\n') {
            acc.address[len - 1] = '\0';
       }
    } while(strlen(acc.address)==0);
  
    do {
        printf("Enter Initial deposit (initial deposit must be >=500) : \n");
        scanf("%f",&acc.balance);
        if(acc.balance < 500) { 
           printf("Initial deposit must be >=500\n");
        }
    } while(acc.balance< 500);
      while ((getchar()) != '\n');
    
    add_to_file(acc);

    printf("Account created successfully!\n");
    printf("Account Number : %ld \n",acc.acc_no);
    printf("Balance : %.2f\n",acc.balance);
}

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
    if(fp == NULL)
    {
        printf("Cannot Open File\n");
        return ;
    }


    initial acc;
    double total_balance = 0 ;

    printf("%-20s %-20s %-40s %-20s\n" , "Account Number" , "Name" , "Address" , "Balance");

    while (fread(&acc, sizeof(initial), 1, fp) == 1)
   {
       printf("%-20ld %-20s %-40s %20.2f\n", acc.acc_no, acc.name, acc.address, acc.balance);

        total_balance += acc.balance;
    }

    printf("%40s %50.2f\n", "Total Balance in Bank:", total_balance);

    fclose(fp);
}



void display() {

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
        printf("Enter new name: ");
        getchar();
        fgets(acc.name, sizeof(acc.name), stdin);
        acc.name[strcspn(acc.name, "\n")] = 0;
        printf("Name updated successfully.\n");
    } else if (choice == 2) {
        printf("Enter new address: ");
        getchar();
        fgets(acc.address, sizeof(acc.address), stdin);
        acc.address[strcspn(acc.address, "\n")] = 0;
        printf("Address updated successfully.\n");
    }
    modify(fp, pos, &acc);
    fclose(fp);
}

void delete_account() {

}

void close_account() {

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
