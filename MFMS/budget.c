/* budget.c - Budget Management module
 * Stores departmental budgets and expenditure, calculates remaining budget
 * and identifies departments that have exceeded their allocation. */
#include <stdio.h>
#include <string.h>
#include "budget.h"
#include "validation.h"

static char   deptName[MAX_DEPARTMENTS][NAME_LEN];
static double deptAllocated[MAX_DEPARTMENTS];
static double deptExpenditure[MAX_DEPARTMENTS];
static int    departmentCount = 0;

#define WARNING_LEVEL 90.0   /* percent used that triggers a warning */

/* ---------- calculations ---------- */

double calculateRemainingBudget(double allocated, double expenditure)
{
    return allocated - expenditure;
}

/* Returns 1 if expenditure is within the allocated budget, else 0 */
int isWithinBudget(double allocated, double expenditure)
{
    return expenditure <= allocated;
}

static double calculatePercentUsed(double allocated, double expenditure)
{
    if (allocated == 0) {
        return 0;
    }
    return (expenditure / allocated) * 100.0;
}

static const char *getBudgetStatus(double allocated, double expenditure)
{
    double percent = calculatePercentUsed(allocated, expenditure);
    if (!isWithinBudget(allocated, expenditure)) {
        return "EXCEEDED";
    } else if (percent >= WARNING_LEVEL) {
        return "WITHIN (WARNING)";
    } else {
        return "WITHIN BUDGET";
    }
}

/* Case-insensitive department lookup; returns index or -1 */
static int findDepartment(const char *name)
{
    char upperName[NAME_LEN], upperStored[NAME_LEN];
    int i;
    toUpperCase(name, upperName, NAME_LEN);
    for (i = 0; i < departmentCount; i++) {
        toUpperCase(deptName[i], upperStored, NAME_LEN);
        if (strcmp(upperStored, upperName) == 0) {
            return i;
        }
    }
    return -1;
}

static void printDepartmentDetail(int i)
{
    double remaining = calculateRemainingBudget(deptAllocated[i], deptExpenditure[i]);
    printf("Department       : %s\n", deptName[i]);
    printf("Allocated Budget : N$%.2f\n", deptAllocated[i]);
    printf("Expenditure      : N$%.2f\n", deptExpenditure[i]);
    printf("Remaining Budget : N$%.2f\n", remaining);
    printf("Percentage Used  : %.1f%%\n", calculatePercentUsed(deptAllocated[i], deptExpenditure[i]));
    printf("Status           : %s\n", getBudgetStatus(deptAllocated[i], deptExpenditure[i]));
}

/* ---------- menu operations ---------- */

static void enterDepartmentBudget(void)
{
    char name[NAME_LEN];
    int index;

    printf("\n--- ENTER DEPARTMENTAL BUDGET ---\n");
    readNonEmptyString("Department name     : ", name, NAME_LEN);
    index = findDepartment(name);

    if (index != -1) {
        printf("Department exists. Current budget: N$%.2f\n", deptAllocated[index]);
        deptAllocated[index] = readPositiveDouble("New allocated budget (N$): ", 0);
        printf("  [OK] Budget updated.\n");
        return;
    }
    if (departmentCount >= MAX_DEPARTMENTS) {
        printf("  [ERROR] Maximum of %d departments reached.\n", MAX_DEPARTMENTS);
        return;
    }
    strcpy(deptName[departmentCount], name);
    deptAllocated[departmentCount] = readPositiveDouble("Allocated budget (N$): ", 0);
    deptExpenditure[departmentCount] = 0;
    departmentCount++;
    printf("  [OK] Department budget recorded.\n");
}

static void enterExpenditure(void)
{
    char name[NAME_LEN];
    char description[TEXT_LEN];
    double amount;
    int index;

    printf("\n--- ENTER EXPENDITURE ---\n");
    if (departmentCount == 0) {
        printf("No departments yet. Enter a budget first.\n");
        return;
    }
    readNonEmptyString("Department name : ", name, NAME_LEN);
    index = findDepartment(name);
    if (index == -1) {
        printf("  [ERROR] Department \"%s\" not found.\n", name);
        return;
    }
    readNonEmptyString("Description     : ", description, TEXT_LEN);
    amount = readPositiveDouble("Amount (N$)     : ", 0);

    deptExpenditure[index] += amount;
    printf("  [OK] N$%.2f recorded for %s (%s).\n", amount, deptName[index], description);

    if (!isWithinBudget(deptAllocated[index], deptExpenditure[index])) {
        printf("  [ALERT] %s has now EXCEEDED its budget by N$%.2f!\n",
               deptName[index],
               -calculateRemainingBudget(deptAllocated[index], deptExpenditure[index]));
    }
}

static void checkDepartmentBudget(void)
{
    char name[NAME_LEN];
    int index;

    printf("\n--- CHECK DEPARTMENT BUDGET ---\n");
    readNonEmptyString("Department name: ", name, NAME_LEN);
    index = findDepartment(name);
    if (index == -1) {
        printf("  [ERROR] Department \"%s\" not found.\n", name);
        return;
    }
    printLine();
    printDepartmentDetail(index);
    printLine();
}

static void displayBudgets(void)
{
    int i;
    printf("\n--- ALL DEPARTMENT BUDGETS ---\n");
    if (departmentCount == 0) {
        printf("No budgets recorded.\n");
        return;
    }
    printLine();
    printf("%-14s %14s %14s %14s %-16s\n", "Department", "Allocated", "Spent", "Remaining", "Status");
    printLine();
    for (i = 0; i < departmentCount; i++) {
        printf("%-14.14s %14.2f %14.2f %14.2f %-16s\n",
               deptName[i], deptAllocated[i], deptExpenditure[i],
               calculateRemainingBudget(deptAllocated[i], deptExpenditure[i]),
               getBudgetStatus(deptAllocated[i], deptExpenditure[i]));
    }
    printLine();
}

static void displayExceededDepartments(void)
{
    int i, count = 0;
    printf("\n--- DEPARTMENTS THAT EXCEEDED BUDGET ---\n");
    for (i = 0; i < departmentCount; i++) {
        if (!isWithinBudget(deptAllocated[i], deptExpenditure[i])) {
            printf("  %-14s over by N$%.2f\n", deptName[i],
                   -calculateRemainingBudget(deptAllocated[i], deptExpenditure[i]));
            count++;
        }
    }
    if (count == 0) {
        printf("All departments are within budget.\n");
    }
}

void budgetMenu(void)
{
    int choice;
    do {
        printf("\n========== BUDGET MANAGEMENT ==========\n");
        printf("1. Enter / update departmental budget\n");
        printf("2. Enter expenditure\n");
        printf("3. Check a department's budget\n");
        printf("4. Display all budgets\n");
        printf("5. Show departments over budget\n");
        printf("0. Back to main menu\n");
        choice = readInt("Enter your choice: ", 0, 5);

        switch (choice) {
            case 1: enterDepartmentBudget(); break;
            case 2: enterExpenditure(); break;
            case 3: checkDepartmentBudget(); break;
            case 4: displayBudgets(); break;
            case 5: displayExceededDepartments(); break;
            case 0: break;
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- data for reports ---------- */

int getDepartmentCount(void) { return departmentCount; }
const char *getDepartmentName(int index) { return deptName[index]; }
double getAllocatedBudget(int index) { return deptAllocated[index]; }
double getExpenditure(int index) { return deptExpenditure[index]; }

static void storeBudget(const char *name, double allocated, double spent)
{
    strcpy(deptName[departmentCount], name);
    deptAllocated[departmentCount] = allocated;
    deptExpenditure[departmentCount] = spent;
    departmentCount++;
}

void loadSampleBudgets(void)
{
    storeBudget("Finance", 500000, 420000);
    storeBudget("Water",   800000, 865000);
    storeBudget("HR",      250000, 231000);
    storeBudget("Roads",  1200000, 640000);
}
