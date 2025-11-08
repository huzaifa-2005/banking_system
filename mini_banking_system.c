/* mini banking system in C
   - Array of accounts (no files)
   - Modular functions for each feature (create, login, view, deposit, withdraw, transfer, delete)
   - Simple username/password authentication
   - Each account has a serial integer ID (starting at 1) and a unique username
   
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ACCOUNTS 100
#define NAME_LEN 100
#define USER_LEN 50
#define PASS_LEN 50

typedef struct {
    int id;                         // Unique serial ID (1,2,3,...)
    char name[NAME_LEN];            // Full name (can include spaces)
    char username[USER_LEN];        // Unique username (no spaces)
    char password[PASS_LEN];        // Password (no spaces)
    double balance;                 // Current balance
    int active;                     // 1 = active, 0 = deleted/inactive
} Account;

/* Global storage of accounts */
Account accounts[MAX_ACCOUNTS];
int account_count = 0;

/* Utility: flush remaining characters on current stdin line */
void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

/* Utility: read whole line into buffer, strip newline */
void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        size_t ln = strlen(buf);
        if (ln > 0 && buf[ln-1] == '\n') buf[ln-1] = '\0';
    } else {
        buf[0] = '\0';
    }
}

/* Find account index by username; returns -1 if not found or inactive */
int find_by_username(const char *username) {
    for (int i = 0; i < account_count; ++i) {
        if (accounts[i].active && strcmp(accounts[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

/* Find account index by id; returns -1 if not found or inactive */
int find_by_id(int id) {
    for (int i = 0; i < account_count; ++i) {
        if (accounts[i].active && accounts[i].id == id) return i;
    }
    return -1;
}

/* Module: Create new account */
void create_account() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Sorry, maximum number of accounts reached (%d).\n", MAX_ACCOUNTS);
        return;
    }

    Account acc;
    acc.id = account_count + 1; // serial id
    acc.balance = 0.0;
    acc.active = 1;

    printf("\n--- Create New Account ---\n");

    printf("Enter full name: ");
    flush_stdin();              // clear leftover newline from previous scanf if any
    read_line(acc.name, NAME_LEN);

    // username (unique)
    while (1) {
        char temp_user[USER_LEN];
        printf("Choose a username (no spaces): ");
        if (scanf("%49s", temp_user) != 1) { flush_stdin(); continue; }
        if (find_by_username(temp_user) != -1) {
            printf("That username is already taken. Please pick another (try a slight variation).\n");
        } else {
            strncpy(acc.username, temp_user, USER_LEN);
            acc.username[USER_LEN-1] = '\0';
            break;
        }
    }

    // password
    printf("Set a password (no spaces): ");
    scanf("%49s", acc.password);

    // initial deposit optional
    double init;
    while (1) {
        printf("Enter initial deposit amount (>= 0): ");
        if (scanf("%lf", &init) != 1) {
            printf("Invalid input.\n");
            flush_stdin();
            continue;
        }
        if (init < 0) {
            printf("Deposit cannot be negative.\n");
            continue;
        }
        acc.balance = init;
        break;
    }

    // store account
    accounts[account_count] = acc;
    account_count++;

    printf("\nAccount created successfully!\n");
    printf("Assigned User ID: %d\n", acc.id);
    printf("Username: %s\n", acc.username);
    printf("Current Balance: %.2f\n\n", acc.balance);
}

/* Module: View account info (given index) */
void view_account(int idx) {
    Account *a = &accounts[idx];
    printf("\n--- Account Information ---\n");
    printf("User ID   : %d\n", a->id);
    printf("Name      : %s\n", a->name);
    printf("Username  : %s\n", a->username);
    printf("Balance   : %.2f\n", a->balance);
    printf("----------------------------\n\n");
}

/* Module: Deposit into account */
void deposit_money(int idx) {
    double amt;
    while (1) {
        printf("Enter amount to deposit: ");
        if (scanf("%lf", &amt) != 1) {
            printf("Invalid number.\n");
            flush_stdin();
            continue;
        }
        if (amt <= 0) {
            printf("Please enter an amount greater than 0.\n");
            continue;
        }
        accounts[idx].balance += amt;
        printf("Deposit successful. New balance: %.2f\n", accounts[idx].balance);
        break;
    }
}

/* Module: Withdraw from account (with balance check) */
void withdraw_money(int idx) {
    double amt;
    while (1) {
        printf("Enter amount to withdraw: ");
        if (scanf("%lf", &amt) != 1) {
            printf("Invalid number.\n");
            flush_stdin();
            continue;
        }
        if (amt <= 0) {
            printf("Please enter an amount greater than 0.\n");
            continue;
        }
        if (amt > accounts[idx].balance) {
            printf("Insufficient balance. Current balance: %.2f\n", accounts[idx].balance);
            continue;
        }
        accounts[idx].balance -= amt;
        printf("Withdrawal successful. New balance: %.2f\n", accounts[idx].balance);
        break;
    }
}

/* Module: Transfer money from one account to another by username or id */
void transfer_money(int from_idx) {
    printf("\nTransfer by: 1) Username  2) User ID\nChoose option (1-2): ");
    int opt;
    if (scanf("%d", &opt) != 1) { flush_stdin(); printf("Invalid input.\n"); return; }

    int to_idx = -1;
    if (opt == 1) {
        char target_user[USER_LEN];
        printf("Enter recipient's username: ");
        if (scanf("%49s", target_user) != 1) { flush_stdin(); printf("Invalid input.\n"); return; }
        to_idx = find_by_username(target_user);
        if (to_idx == -1) {
            printf("No active account with that username.\n");
            return;
        }
    } else if (opt == 2) {
        int target_id;
        printf("Enter recipient's User ID: ");
        if (scanf("%d", &target_id) != 1) { flush_stdin(); printf("Invalid input.\n"); return; }
        to_idx = find_by_id(target_id);
        if (to_idx == -1) {
            printf("No active account with that ID.\n");
            return;
        }
    } else {
        printf("Invalid option.\n");
        return;
    }

    if (to_idx == from_idx) {
        printf("Cannot transfer to the same account.\n");
        return;
    }

    double amt;
    printf("Enter amount to transfer: ");
    if (scanf("%lf", &amt) != 1) { flush_stdin(); printf("Invalid amount.\n"); return; }
    if (amt <= 0) { printf("Amount must be > 0.\n"); return; }
    if (amt > accounts[from_idx].balance) {
        printf("Insufficient balance. Current balance: %.2f\n", accounts[from_idx].balance);
        return;
    }

    accounts[from_idx].balance -= amt;
    accounts[to_idx].balance += amt;
    printf("Transfer successful. Your new balance: %.2f\n", accounts[from_idx].balance);
}

/* Module: Delete account (must first withdraw all money or transfer it) */
void delete_account(int idx) {
    Account *a = &accounts[idx];
    if (!a->active) {
        printf("Account already inactive.\n");
        return;
    }
    printf("\n--- Delete Account ---\n");
    if (a->balance > 0.0) {
        printf("Your account still has %.2f balance.\n", a->balance);
        printf("You must either:\n");
        printf("  1) Withdraw all money (set balance to 0)\n");
        printf("  2) Transfer all money to another account\n");
        printf("Choose 1 or 2: ");
        int opt;
        if (scanf("%d", &opt) != 1) { flush_stdin(); printf("Invalid input.\n"); return; }
        if (opt == 1) {
            // simulate withdrawing all money as cash
            printf("Withdrawing all money: %.2f\n", a->balance);
            a->balance = 0.0;
            printf("All money withdrawn. Balance is now 0.00\n");
        } else if (opt == 2) {
            // transfer entire balance
            double to_transfer = a->balance;
            printf("Enter recipient username to transfer all money: ");
            char target_user[USER_LEN];
            if (scanf("%49s", target_user) != 1) { flush_stdin(); printf("Invalid input.\n"); return; }
            int to_idx = find_by_username(target_user);
            if (to_idx == -1) {
                printf("No such recipient username found or recipient inactive.\n");
                return;
            }
            if (to_idx == idx) {
                printf("Cannot transfer to the same account.\n");
                return;
            }
            accounts[to_idx].balance += to_transfer;
            a->balance = 0.0;
            printf("Transferred %.2f to %s. Your balance is now 0.00\n", to_transfer, accounts[to_idx].username);
        } else {
            printf("Invalid option.\n");
            return;
        }
    }

    // confirm deletion
    printf("Are you sure you want to permanently delete your account? (y/n): ");
    char ch;
    flush_stdin(); // clear newline
    ch = getchar();
    if (ch == 'y' || ch == 'Y') {
        a->active = 0; // mark inactive
        printf("Account deleted successfully. Goodbye!\n");
    } else {
        printf("Account deletion cancelled.\n");
    }
}

/* Authentication and per-user menu */
void user_session(int idx) {
    while (1) {
        printf("\n--- User Menu (%s) ---\n", accounts[idx].username);
        printf("1) View account information\n");
        printf("2) Deposit money\n");
        printf("3) Withdraw money\n");
        printf("4) Transfer money\n");
        printf("5) Delete account\n");
        printf("6) Logout\n");
        printf("Choose option (1-6): ");
        int choice;
        if (scanf("%d", &choice) != 1) { flush_stdin(); printf("Invalid input.\n"); continue; }
        switch (choice) {
            case 1: view_account(idx); break;
            case 2: deposit_money(idx); break;
            case 3: withdraw_money(idx); break;
            case 4: transfer_money(idx); break;
            case 5: delete_account(idx);
                    if (!accounts[idx].active) return; // end session if account deleted
                    break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice, try again.\n");
        }
    }
}

/* Module: Login (returns index of logged-in account or -1) */
int login() {
    char uname[USER_LEN], pass[PASS_LEN];
    printf("\n--- Login ---\n");
    printf("Username: ");
    if (scanf("%49s", uname) != 1) { flush_stdin(); return -1; }
    printf("Password: ");
    if (scanf("%49s", pass) != 1) { flush_stdin(); return -1; }

    int idx = find_by_username(uname);
    if (idx == -1) {
        printf("No such username or account inactive.\n");
        return -1;
    }
    if (strcmp(accounts[idx].password, pass) != 0) {
        printf("Incorrect password.\n");
        return -1;
    }
    printf("Welcome, %s!\n", accounts[idx].name);
    return idx;
}

/* Main menu loop */
void main_menu() {
    while (1) {
        printf("\n===== Mini Banking System =====\n");
        printf("1) Create account\n");
        printf("2) Login\n");
        printf("3) Exit\n");
        printf("Choose option (1-3): ");
        int opt;
        if (scanf("%d", &opt) != 1) { flush_stdin(); printf("Invalid input.\n"); continue; }
        if (opt == 1) {
            create_account();
        } else if (opt == 2) {
            int idx = login();
            if (idx != -1) user_session(idx);
        } else if (opt == 3) {
            printf("Exiting program. Goodbye!\n");
            break;
        } else {
            printf("Invalid option. Try again.\n");
        }
    }
}

/* Entry point */
int main() {
    // small friendly greeting
    printf("Welcome to the Mini Banking System (no-files version)\n");
    printf("Note: usernames must be unique and act as transfer identifiers.\n");
    main_menu();
    return 0;
}
