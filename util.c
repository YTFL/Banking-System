#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <ctype.h>
#include <string.h>
#include "util.h"
#include "structs.h"

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
    if (d.year < 1900 || d.month < 1 || d.month > 12)
        return 0;

    int max_days = getMonthDays(d.month, d.year);
    if (d.day < 1 || d.day > max_days)
        return 0;
       time_t now = time(NULL);
    struct tm *local = localtime(&now);
    int current_year = local->tm_year + 1900;

    if (d.year < 1900 || d.year > current_year)
        return 0;

    return 1;
}

void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void get_input(char *output, int max_len, const char *prompt, const char *field_name) {
    char buffer[200];
    int valid = 0;

    while (!valid) {
        int i = 0, j = 0, in_word = 0;
        int truncated = 0;

        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);

        if (!strchr(buffer, '\n')) {
            clear_input_buffer();
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        while (isspace(buffer[i])) i++;

        while (buffer[i] != '\0') {
            if (isspace(buffer[i])) {
                if (in_word) {
                    output[j++] = ' ';
                    in_word = 0;
                }
            } else {
                output[j++] = buffer[i];
                in_word = 1;
            }

            if (j == max_len) {
                truncated = 1;
                break;
            }

            i++;
        }

        if (j > 0 && output[j - 1] == ' ')
            j--;

        output[j] = '\0';

        if (strlen(output) == 0) {
            printf("%s cannot be empty.\n", field_name);
        } else {
            valid = 1;
            if (truncated) {
                printf("Warning: %s has been truncated to %d characters: \"%s\"\n", field_name, max_len, output);
            }
        }
    }
}
