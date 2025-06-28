#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void display_account() {

}

void month_report() {

}

void box_for_display() {

}



void return_name(int accno, char *name_out) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;
    
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == accno) {
            strcpy(name_out, acc.name);
            break;
        }
    }
    fclose(fp);
}



void return_address(int accno, char *address_out) {
    FILE *fp = fopen("INITIAL.dat", "rb");
    initial acc;

    if (fp == NULL) {
        strcpy(address_out, "");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == accno) {
            strcpy(address_out, acc.address);
            fclose(fp);
            return;
        }
    }

    strcpy(address_out, "Account not found");
    fclose(fp);
}
