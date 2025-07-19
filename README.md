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

- [**Venkat Piyush**](https://github.com/YTFL) *(Team Lead)*
- [**Varsha TM**](https://github.com/varsha2006147)
- [**Zainab Siddiqua**](https://github.com/Zainab-Siddiqua)
- [**Vijeta Nadgir**](https://github.com/vanilla2409)

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
 To compile and build the project, use:
 ```bash
 gcc main.c menu.c account.c reports.c transactions.c util.c -o banking_system -lm
```
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

## Testing Documentation and Known Issues
All test cases, validation checks, and known issues (bugs, limitations, edge cases) are documented in the testing report linked below.
[BankingSystem_TestingDocumentation](https://docs.google.com/spreadsheets/d/1qDCoJd-bLsNjO8sg1TkAqTwn_imdBhK9NAa8JJjgcCY/edit?usp=sharing)

---

## Possible Features and Improvements
- Prevent truncation of name/address during storage — allow full-length storage but truncate only for display.
- Add a feature to support account-to-account fund transfers with proper validation.

---

## Usage

This project was developed as part of an academic requirement at **REVA University**, under the guidance of **Dr. Ramaprasad H C**.

It is provided strictly for **academic and educational purposes only**.

You may **view**, **fork**, and **improve** this project **for academic learning or evaluation**.  
However, **you are not permitted** to use, copy, modify, distribute, or publish this project or its code for **commercial, public, or non-academic** use without explicit permission from the authors.

**Pull Requests are Welcome**  
Contributions that enhance or improve the project (especially for academic learning) are appreciated.  
If your pull request is accepted, you will be **credited appropriately** in README.md under a separate tab.


