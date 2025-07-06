#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdio.h>
#include "structs.h"

void new_account();
void add_to_file();
void display_list();
void display();
void modify(FILE *fp, long pos, initial *acc);
void modify_account(int choice);
void delete_account();
void close_account();
int found_account(int acc_no);
long int last_accno();
int recordno();

#endif