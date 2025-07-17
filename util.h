#ifndef UTIL_H
#define UTIL_H
#include "structs.h"

#define MAX_AMOUNT 99999999999LL

void clear();
int no_of_days(date d1, date d2);
int isEarlier(date d1, date d2);
int is_valid_date(date d);
void clear_input_buffer();

#endif
