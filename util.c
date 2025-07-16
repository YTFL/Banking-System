#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include<time.h>

void clear() {
    system("cls");
}



int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


int getMonthDays(int month, int year) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year))
        return 29;
    return daysInMonth[month - 1];
}

int isEarlier(date d1, date d2) {
    if (d1.year < d2.year) return 1;
    if (d1.year == d2.year && d1.month < d2.month) return 1;
    if (d1.year == d2.year && d1.month == d2.month && d1.day < d2.day) return 1;
    return 0;
}

int no_of_days(date d1, date d2) {
    int days = 0;

    if (!isEarlier(d1, d2)) {
        date temp = d1;
        d1 = d2;
        d2 = temp;
    }

    if (d1.year == d2.year && d1.month == d2.month) {
        return d2.day - d1.day;
    }

    while (d1.year < d2.year || (d1.year == d2.year && d1.month < d2.month)) {
        if (d1.day == 1)
            days += getMonthDays(d1.month, d1.year);
        else {
            days += getMonthDays(d1.month, d1.year) - d1.day;
            d1.day = 1;
        }

        d1.month++;
        if (d1.month > 12) {
            d1.month = 1;
            d1.year++;
        }
    }


    days += d2.day;

    return days;
}
int is_valid_date(date d) {
      time_t now = time(NULL);
    struct tm *local = localtime(&now);
    int current_year = local->tm_year + 1900;
    int current_day = local->tm_mday;
    int current_month = local->tm_mon + 1;

    int max_days = getMonthDays(d.month, d.year);
      // Assuming curremt date is the upper limit
    if((d.year > current_year) || (d.year < 1900) || (d.month < 1) || (d.month > 12) || 
       (d.day < 1) || (d.day > getMonthDays(d.month, d.year)) || (d.month > current_month)
        || (d.day > max_days))
        return 0;

    return 1;
}

void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}