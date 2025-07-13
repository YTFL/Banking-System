#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdio.h>
#include "structs.h"

void new_account(void);
void add_to_file(initial acc);
void display_list(void);
void display(void);
void modify(FILE *fp, long pos, initial *acc);
void modify_account(int choice);
void delete_account(void);
void close_account(long int acc_no);
int found_account(FILE *fp, int acc_no);
long int last_accno(void);
int recordno(void);

#endif