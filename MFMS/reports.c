/* reports.c - Reports module
 * Produces summary reports using data from the other modules. */
#include <stdio.h>
#include "reports.h"
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "assets.h"
#include "validation.h"

static void employeeReport(void)
{
    int i, n = getEmployeeCount();
    double salary, total = 0, highest, lowest;
    int highIdx = 0, lowIdx = 0;

    printf("\n=============== EMPLOYEE REPORT ===============\n");
    if (n == 0) {
        printf("No employees registered.\n");
        return;
    }
    highest = lowest = calculateGrossSalary(0);
    for (i = 0; i < n; i++) {
        salary = calculateGrossSalary(i);
        total += salary;
        if (salary > highest) { highest = salary; highIdx = i; }
        if (salary < lowest)  { lowest = salary;  lowIdx = i; }
    }
    printf("Total Employees    : %d\n", n);
    printf("Total Monthly Payroll (gross): N$%.2f\n", total);
    printf("Average Salary     : N$%.2f\n", total / n);
    printf("Highest Salary     : N$%.2f (%s)\n", highest, getEmployeeName(highIdx));
    printf("Lowest Salary      : N$%.2f (%s)\n", lowest, getEmployeeName(lowIdx));
}

/* Recursive sum of expenditure for departments 0..index (Week 8: recursion) */
static double sumExpenditureRecursive(int index)
{
    if (index < 0) {
        return 0;
    }
    return getExpenditure(index) + sumExpenditureRecursive(index - 1);
}

static void budgetReport(void)
{
    int i, n = getDepartmentCount(), exceeded = 0;
    double totalAllocated = 0, totalSpent;

    printf("\n================ BUDGET REPORT ================\n");
    if (n == 0) {
        printf("No budgets recorded.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        totalAllocated += getAllocatedBudget(i);
    }
    totalSpent = sumExpenditureRecursive(n - 1);

    printf("Number of Departments : %d\n", n);
    printf("Total Allocated Budget: N$%.2f\n", totalAllocated);
    printf("Total Expenditure     : N$%.2f\n", totalSpent);
    printf("Remaining Budget      : N$%.2f\n", calculateRemainingBudget(totalAllocated, totalSpent));
    printf("Overall Status        : %s\n",
           isWithinBudget(totalAllocated, totalSpent) ? "WITHIN BUDGET" : "OVER BUDGET");
    printf("\nDepartments exceeding budget:\n");
    for (i = 0; i < n; i++) {
        if (!isWithinBudget(getAllocatedBudget(i), getExpenditure(i))) {
            printf("  - %s (over by N$%.2f)\n", getDepartmentName(i),
                   getExpenditure(i) - getAllocatedBudget(i));
            exceeded++;
        }
    }
    if (exceeded == 0) {
        printf("  None\n");
    }
}

static void fullSummary(void)
{
    employeeReport();
    budgetReport();
    printf("\n=============== SUPPLIER REPORT ===============");
    displaySuppliers();
    printf("\n================ ASSET REPORT =================");
    displayAssets();
}

void reportsMenu(void)
{
    int choice;
    do {
        printf("\n================ REPORTS ================\n");
        printf("1. Employee report\n");
        printf("2. Budget report\n");
        printf("3. Supplier report\n");
        printf("4. Asset report\n");
        printf("5. Full municipal summary\n");
        printf("0. Back to main menu\n");
        choice = readInt("Enter your choice: ", 0, 5);

        switch (choice) {
            case 1: employeeReport(); break;
            case 2: budgetReport(); break;
            case 3: printf("\n=============== SUPPLIER REPORT ==============="); displaySuppliers(); break;
            case 4: printf("\n================ ASSET REPORT ================="); displayAssets(); break;
            case 5: fullSummary(); break;
            case 0: break;
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);
}
