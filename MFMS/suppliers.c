/* suppliers.c - Supplier Management module
 * Maintains supplier information and allows searching and comparing. */
#include <stdio.h>
#include <string.h>
#include "suppliers.h"
#include "validation.h"

static char supId[MAX_SUPPLIERS][10];
static char supName[MAX_SUPPLIERS][NAME_LEN];
static char supEmail[MAX_SUPPLIERS][NAME_LEN];
static char supPhone[MAX_SUPPLIERS][20];
static char supTown[MAX_SUPPLIERS][NAME_LEN];
static char supCategory[MAX_SUPPLIERS][NAME_LEN];
static int  supplierCount = 0;
static int  nextSupplierNumber = 1;

/* Returns index of supplier with same name (case-insensitive) or -1 */
static int findSupplierByName(const char *name)
{
    char a[NAME_LEN], b[NAME_LEN];
    int i;
    toUpperCase(name, a, NAME_LEN);
    for (i = 0; i < supplierCount; i++) {
        toUpperCase(supName[i], b, NAME_LEN);
        if (strcmp(a, b) == 0) {
            return i;
        }
    }
    return -1;
}

static int findSupplierById(const char *id)
{
    char upperId[10];
    int i;
    toUpperCase(id, upperId, sizeof(upperId));
    for (i = 0; i < supplierCount; i++) {
        if (strcmp(supId[i], upperId) == 0) {
            return i;
        }
    }
    return -1;
}

static void storeSupplier(const char *name, const char *email, const char *phone,
                          const char *town, const char *category)
{
    sprintf(supId[supplierCount], "SUP%03d", nextSupplierNumber++);
    strcpy(supName[supplierCount], name);
    strcpy(supEmail[supplierCount], email);
    strcpy(supPhone[supplierCount], phone);
    strcpy(supTown[supplierCount], town);
    strcpy(supCategory[supplierCount], category);
    supplierCount++;
}

static void printSupplierHeader(void)
{
    printLine();
    printf("%-7s %-20s %-24s %-15s %-10s\n", "ID", "Name", "Email", "Telephone", "Town");
    printLine();
}

static void printSupplierRow(int i)
{
    printf("%-7s %-20.20s %-24.24s %-15.15s %-10.10s\n",
           supId[i], supName[i], supEmail[i], supPhone[i], supTown[i]);
}

static void printSupplierCard(int i)
{
    char label[NAME_LEN + 20];
    /* strcat used to build a combined label "ID - Name" */
    strcpy(label, supId[i]);
    strcat(label, " - ");
    strcat(label, supName[i]);
    printf("Supplier  : %s\n", label);
    printf("Category  : %s\n", supCategory[i]);
    printf("Email     : %s\n", supEmail[i]);
    printf("Telephone : %s\n", supPhone[i]);
    printf("Town      : %s\n", supTown[i]);
}

static void addSupplier(void)
{
    char name[NAME_LEN], email[NAME_LEN], phone[20], town[NAME_LEN], category[NAME_LEN];

    if (supplierCount >= MAX_SUPPLIERS) {
        printf("  [ERROR] Supplier list is full.\n");
        return;
    }
    printf("\n--- ADD SUPPLIER ---\n");
    readNonEmptyString("Supplier name : ", name, NAME_LEN);
    if (findSupplierByName(name) != -1) {
        printf("  [ERROR] A supplier named \"%s\" already exists.\n", name);
        return;
    }
    readNonEmptyString("Category (e.g. Stationery, Fuel, IT): ", category, NAME_LEN);
    readEmail("Email         : ", email, NAME_LEN);
    readPhone("Telephone     : ", phone, sizeof(phone));
    readNonEmptyString("Town/Location : ", town, NAME_LEN);

    storeSupplier(name, email, phone, town, category);
    printf("  [OK] Supplier added with ID %s.\n", supId[supplierCount - 1]);
}

void displaySuppliers(void)
{
    int i;
    printf("\n--- REGISTERED SUPPLIERS (%d) ---\n", supplierCount);
    if (supplierCount == 0) {
        printf("No suppliers registered.\n");
        return;
    }
    printSupplierHeader();
    for (i = 0; i < supplierCount; i++) {
        printSupplierRow(i);
    }
    printLine();
}

static void searchSupplier(void)
{
    char key[NAME_LEN];
    int option, i, found = 0;

    printf("\n--- SEARCH SUPPLIER ---\n");
    printf("1. By Supplier ID\n2. By Name\n3. By Town/Location\n4. By Category\n");
    option = readInt("Choice: ", 1, 4);
    readNonEmptyString("Search for: ", key, NAME_LEN);

    if (option == 1) {
        i = findSupplierById(key);
        if (i != -1) {
            printLine();
            printSupplierCard(i);
            printLine();
            return;
        }
    } else {
        for (i = 0; i < supplierCount; i++) {
            int match;
            if (option == 2)      match = containsIgnoreCase(supName[i], key);
            else if (option == 3) match = containsIgnoreCase(supTown[i], key);
            else                  match = containsIgnoreCase(supCategory[i], key);
            if (match) {
                if (found == 0) printSupplierHeader();
                printSupplierRow(i);
                found++;
            }
        }
        if (found > 0) {
            printLine();
            printf("%d supplier(s) found.\n", found);
            return;
        }
    }
    printf("No supplier matches \"%s\".\n", key);
}

/* Compare two suppliers field by field using strcmp */
static void compareSuppliers(void)
{
    char id1[10], id2[10];
    int a, b;

    printf("\n--- COMPARE TWO SUPPLIERS ---\n");
    if (supplierCount < 2) {
        printf("At least two suppliers are needed.\n");
        return;
    }
    readNonEmptyString("First supplier ID  : ", id1, sizeof(id1));
    readNonEmptyString("Second supplier ID : ", id2, sizeof(id2));
    a = findSupplierById(id1);
    b = findSupplierById(id2);
    if (a == -1 || b == -1) {
        printf("  [ERROR] One or both supplier IDs were not found.\n");
        return;
    }
    if (a == b) {
        printf("  [ERROR] You entered the same supplier twice.\n");
        return;
    }
    printLine();
    printf("%-10s | %-25.25s | %-25.25s\n", "Field", supName[a], supName[b]);
    printLine();
    printf("%-10s | %-25.25s | %-25.25s | %s\n", "Category", supCategory[a], supCategory[b],
           strcmp(supCategory[a], supCategory[b]) == 0 ? "SAME" : "DIFFERENT");
    printf("%-10s | %-25.25s | %-25.25s | %s\n", "Town", supTown[a], supTown[b],
           strcmp(supTown[a], supTown[b]) == 0 ? "SAME" : "DIFFERENT");
    printf("%-10s | %-25.25s | %-25.25s\n", "Email", supEmail[a], supEmail[b]);
    printf("%-10s | %-25.25s | %-25.25s\n", "Telephone", supPhone[a], supPhone[b]);
    printLine();
    if (strcmp(supName[a], supName[b]) < 0) {
        printf("Alphabetical order: %s comes before %s.\n", supName[a], supName[b]);
    } else {
        printf("Alphabetical order: %s comes before %s.\n", supName[b], supName[a]);
    }
}

void supplierMenu(void)
{
    int choice;
    do {
        printf("\n========== SUPPLIER MANAGEMENT ==========\n");
        printf("1. Add supplier\n");
        printf("2. Display suppliers\n");
        printf("3. Search supplier\n");
        printf("4. Compare two suppliers\n");
        printf("0. Back to main menu\n");
        choice = readInt("Enter your choice: ", 0, 4);

        switch (choice) {
            case 1: addSupplier(); break;
            case 2: displaySuppliers(); break;
            case 3: searchSupplier(); break;
            case 4: compareSuppliers(); break;
            case 0: break;
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);
}

int getSupplierCount(void) { return supplierCount; }

void loadSampleSuppliers(void)
{
    storeSupplier("Office World",   "sales@officeworld.na", "+264 61 234 567", "Windhoek", "Stationery");
    storeSupplier("NamTech IT",     "info@namtech.na",      "+264 61 998 100", "Windhoek", "IT");
    storeSupplier("Coastal Fuels",  "orders@coastal.na",    "+264 64 205 330", "Walvis Bay", "Fuel");
}
