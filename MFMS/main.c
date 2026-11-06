/* main.c - Municipal Financial Management System (MFMS)
 * PAP521S Programming in Practice - Project A (Foundation System)
 * Entry point: loads sample data and runs the main menu loop. */
#include <stdio.h>
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "assets.h"
#include "reports.h"
#include "validation.h"

static void displayMenu(void)
{
    printf("\n========================================\n");
    printf(" MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("1. Employee Management\n");
    printf("2. Budget Management\n");
    printf("3. Supplier Management\n");
    printf("4. Asset Management\n");
    printf("5. Reports\n");
    printf("6. Exit\n");
}

static void loadSampleData(void)
{
    loadSampleEmployees();
    loadSampleBudgets();
    loadSampleSuppliers();
    loadSampleAssets();
}

int main(void)
{
    int choice;

    loadSampleData();
    printf("Sample data loaded (employees, budgets, suppliers, assets).\n");

    do {
        displayMenu();
        choice = readInt("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1: employeeMenu(); break;
            case 2: budgetMenu();   break;
            case 3: supplierMenu(); break;
            case 4: assetMenu();    break;
            case 5: reportsMenu();  break;
            case 6: printf("\nThank you for using MFMS. Goodbye!\n"); break;
        }
    } while (choice != 6);

    return 0;
}
