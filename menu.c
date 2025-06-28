#include <stdio.h>

void main_menu () {
    int choice;
    do {
        printf("Select an option:\n");
        printf("Edit Menu\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Option 1 selected.\n");
                break;
            case 2:
                printf("Option 2 selected.\n");
                break;
            case 3:
                printf("Option 3 selected.\n");
                break;
            case 0:
                printf("Exiting menu.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void edit_menu() {
    int choice;
    do {
        printf("Edit Menu:\n");
        printf("1. Open New Account\n");
        printf("2. Edit account details\n");
        printf("3. Delete an account\n");
        printf("0. Back to Main Menu\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("New account opened\n");
                break;
            case 2:
                printf("Edit your account details\n");
                break;
            case 3:
                printf("Enter account number to delete\n");
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}