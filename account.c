#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void new_account() 
{
     initial acc;
     printf("-----Open New Account-----");
     acc.acc_no = last_accno() + 1;
    do {
        printf("Enter Name: ");
        fgets(acc.name,sizeof(acc.name),stdin);
        int len = strlen(acc.name);
        if (len > 0 && acc.name[len - 1] == '\n') {
            acc.name[len - 1] = '\0';
        }
    } while(strlen(acc.name) == 0);
   
    do {
        printf("Enter Address : ");
        fgets(acc.address,sizeof(acc.address),stdin);
        int len = strlen(acc.address);
        if(len > 0  && acc.address[len – 1] == '\n') {
            acc.address[len – 1] = '\0';
       }
    } while(strlen(acc.address)==0);
  
    do {
        printf("Enter Initial deposit (initial deposit must be >=500) : ");
        scanf("%f",&acc.balance);
        if(acc.balance < 500) { 
           printf("Initial deposit must be >=500");
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
    
}

void display() {

}

void modify(FILE *fp, long pos, initial *acc) {
    fseek(fp, pos, SEEK_SET);
    fwrite(acc, sizeof(initial), 1, fp);
    printf("Account changes have been saved to the file.\n");
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
    initial acc;
    int found = 0;
    long pos;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            found = 1;
            pos = ftell(fp) - sizeof(acc);
            break;
        }
    }
    if (!found) {
        printf("Account not found.\n");
        fclose(fp);
        return;
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

int found_account(int acc_no) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (!fp) return 0;
    initial acc;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
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
