#include "structs.h"
#include <stdio.h>
#include "menu.h"

int main() {
    main_menu();
    return 0;
}

//gcc main.c menu.c account.c reports.c transactions.c util.c -o banking_system -lm