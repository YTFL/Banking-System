#include<stdio.h>
#include<string.h>
#include "structs.h"

void transaction()
{
    FILE *fp_initial, *fp_banking;
    initial acc;
    transaction trans;
    int flag = 0;

    int acc_no;
    printf("enter you account number \n");
    scanf("%d",&acc_no);

    // opens the account

    fp_initial = fopen("INITIAL.dat", "rb+");

    if(fp_initial = NULL)
    {
        printf("Error opening account\n");
    }

    // reads the accounts and finds the one

    while (fread(&acc, sizeof(acc), 1, fp_initial)) 
    {
        if (acc.acc_no == acc_no)
        {
            flag = 1;
            break;
        }
    }
    if(flag = 0)
    {
        printf("account not found. \n");
        return;
    }

    printf("Account holder: %s\n", acc.name);
    printf("Current balance: %f\n", acc.balance);

    printf("Enter transaction type (Deposit / Withdraw): ");
    scanf("%s", trans.trans);

    printf("Enter mode (cash / cheque): ");
    scanf("%s", trans.type);

    printf("Enter transaction amount: ");
    scanf("%f", &trans.amount);

    trans.acc_no = acc.acc_no;
    trans.interest = 0;

    // Process
    if (strcmp(trans.trans, "Withdraw") == 0) 
    {
       if (trans.amount > acc.balance)
       {
        printf("Insufficient balance.\n");
        fclose(f_initial);
        return;
       }
       acc.balance -= trans.amount;
    }
    else if (strcmp(trans.trans, "Deposit") == 0)
    {
    acc.balance += trans.amount;
    }
    else 
    {
    printf("Invalid transaction type.\n");
    fclose(fp_initial);
    return;
    }    
    trans.balance = acc.balance;
    add_to_file_transaction(trans);
    printf("Transaction successful. Updated balance: %.2f\n", acc.balance);   
}

void update_balance(initial acc)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (fp == NULL)
    {
        printf("Error opening INITIAL.dat\n");
        return;
    }

    initial temp;
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.acc_no == acc.acc_no)
        {
            fseek(fp, -sizeof(temp), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            break;
        }
    }
    fclose(fp);
}

float give_balance(int acc_no) {
    float balance = 0.0;
    FILE *fp = fopen("INITIAL.dat", "rb");
    if (fp == NULL) {
        printf("Error opening INITIAL.dat\n");
        return 0.0;
    }
    initial acc;
    while (fread(&acc, sizeof(acc), 1, fp))
   {
        if (acc.acc_no == acc_no) 
        {
            fclose(fp);
            return acc.balance;
        }
    }

    fclose(fp);
    printf("Account not found.\n");
    return 0.0;
}

void add_to_file_transaction()
{
 FILE *fp = fopen("BANKING.dat", "ab");
    if (fp == NULL) 
    {
        printf("Error opening BANKING.dat\n");
        return;
    }
    fwrite(&trans, sizeof(trans), 1, fp);
    fclose(fp);
}


