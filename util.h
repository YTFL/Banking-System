#ifndef UTIL_H
#define UTIL_H
#include "structs.h"

#define MAX_AMOUNT 999999999LL

void clear();
int no_of_days(date d1, date d2);
int isEarlier(date d1, date d2);
int is_valid_date(int month, int year);
void clear_input_buffer();
void get_input(char *output, int max_len, const char *prompt, const char *field_name);

#endif