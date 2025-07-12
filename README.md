# Banking System

A file-based, menu-driven banking system project developed in C as part of an academic assignment at **REVA University**. It simulates core banking operations such as account creation, deposit/withdrawal transactions, transaction reports, and monthly statements — all stored persistently using binary file handling.

---

## Features

- Open new accounts with validation (minimum balance ₹500)
- Perform deposits and withdrawals via **Cash** or **Cheque**
- Display full list of account holders with current balances
- View detailed transaction history per account
- Modify or close existing accounts
- Generate monthly statements
- Data is saved in binary files (`INITIAL.dat`, `BANKING.dat`)
- Modular code using multiple header and source files

---

## Team Members

- **Venkat Piyush D** *(Team Lead)*
- **Varsha TM**
- **Zainab Siddiqua**
- **Vijeta Nadgir**

> **Project Guide:**  
> *Dr. Ramaprasad H C*  
> Associate Professor, School of CSE, REVA University

---

## File Structure

| File                   | Purpose                                        |
|------------------------|------------------------------------------------|
| `INITIAL.dat`          | Stores master account information              |
| `BANKING.dat`          | Stores transaction records                     |
| `account.c / account.h`       | Account creation, deletion, modification     |
| `transaction.c / transaction.h` | Handling daily transactions (D/W)         |
| `report.c / report.h`         | Global and monthly reports generation       |
| `menu.c / menu.h`             | Main and edit menu navigation logic         |
| `main.c`                      | Entry point; calls `main_menu()`            |

---

## Technology Used

- Language: **C**
- Compiler: **GCC**
- Platform: **Windows (Console-based)**
- File Handling: Binary (`fread`, `fwrite`, `fseek`, etc.)
- Console clearing: `system("cls")` (Windows only)

---

## Project Notes
- Account numbers are auto-generated using last_accno().
- Minimum opening balance is ₹500; enforced during account creation.
- Withdrawals that reduce balance below ₹500 are rejected.
- Transaction type (Deposit/Withdraw) and mode (Cash/Cheque) are saved.
- All modifications, deletions, and transactions are reflected in binary files.
- Modular functions help organize code cleanly and logically.
- Uses system("cls") for screen clearing (Windows only).

---

 ## License
This project is licensed under the [Creative Commons Attribution-NonCommercial 4.0 International License](https://creativecommons.org/licenses/by-nc/4.0/).
It was developed as part of an academic requirement at **REVA University**, under the guidance of **Dr. Ramaprasad H C**, and is intended strictly for educational use only.
