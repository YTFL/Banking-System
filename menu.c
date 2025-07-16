#include <stdio.h>
#include "account.h"
#include "reports.h"
#include "transactions.h"
#include "menu.h"
#include "util.h"

void main_menu(void) {
    int choice;
    do {
        printf("\n==== BANKING SYSTEM MAIN MENU ====\n");
        printf("1. Open New Account\n");
        printf("2. Display Account List\n");
        printf("3. Reports\n");
        printf("4. Edit Menu\n");
        printf("5. Transactions\n");
        printf("6. Clear Screen\n");
        printf("0. Exit\n");
        printf("Select an option: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        switch (choice) {
            case 1:
                new_account();
                break;
            case 2:
                display_list();
                break;
            case 3:
                reports();
                break;
            case 4:
                edit_menu();
                break;
            case 5:
                transaction();
                break;
            case 6:
                clear();
                break;
            case 0:
                printf("Exiting the app....\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);
}

void edit_menu(void) {
    int choice;
    do {
        printf("\n==== EDIT MENU ====\n");
        printf("1. Change Account Holder Name\n");
        printf("2. Change Account Address\n");
        printf("3. Delete Account\n");
        printf("4. Display Account Details\n");
        printf("0. Back to Main Menu\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
            case 2:
                modify_account(choice);
                break;
            case 3:
                delete_account();
                break;
            case 4:
                display();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}


void reports(void) {
    int choice;
    do {
        printf("\n==== REPORTS MENU ====\n");
        printf("1. Display Account\n");
        printf("2. Monthly Report\n");
        printf("0. Back to Main Menu\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                display_account();
                break;
            case 2:
                month_report();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}