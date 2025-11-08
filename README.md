# Banking System Project

This project is a simple banking system that demonstrates core programming concepts, data handling, and user management while incorporating real-world considerations for safety and usability.  

---

## Features / Innovations

### 1. Dual Identifier Transfers
The program supports transferring money either by **username** or **user ID**, giving two practical ways to locate recipients.  
- **Username:** The main public identifier.  
- **User ID:** Stable, internal identifier.  

This design is **user-friendly** and mirrors real-world banking systems where multiple identifiers can be used for transactions.

### 2. Pre-delete Safety Checks
Before deleting an account, the program ensures that any remaining funds are **transferred or withdrawn**, modeling real-world safety practices.  
- Prevents accidental data loss.  
- Ensures correctness and integrity of account balances.

### 3. Simple Inactive-Slot Handling
Instead of removing entries from the array and shifting elements, accounts are marked as `active = 0` when deleted.  
- Keeps **User IDs stable**.  
- Demonstrates a **simple, robust approach** to managing deletions in array-based storage.  

---

## Possible Improvements / Extensions

### 1. Persistent Storage
- Save accounts to disk using **CSV, binary files, or SQLite**.  
- Ensures data persists between program runs.

### 2. Stronger Authentication & Security
- **Hash passwords** instead of storing plaintext.  
- Add **password strength checks** and **session timeouts**.  

### 3. Concurrency & Locking
- Handle multiple users or threads safely.  
- Implement **locks** to prevent race conditions during transfers.

### 4. Input Validation & Better UI
- Improve input validation, especially for text fields.  
- Build a **menu-driven interface** or simple **GUI / web front-end** for better user experience.

---


