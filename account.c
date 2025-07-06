#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void new_account() {

}

void add_to_file() {

}

void display_list() {
    
}

void display() {

}

void modify() {

}

void modify_account(int choice) {

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