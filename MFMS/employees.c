/* employees.c - Employee Management module
 * Stores employees in parallel arrays and calculates salary information
 * (gross salary, pension, Social Security, PAYE tax and net salary). */
#include <stdio.h>
#include <string.h>
#include "employees.h"
#include "validation.h"

/* Parallel arrays: index i in each array belongs to the same employee */
static char   empId[MAX_EMPLOYEES][10];
static char   empName[MAX_EMPLOYEES][NAME_LEN];
static char   empDept[MAX_EMPLOYEES][NAME_LEN];
static char   empPosition[MAX_EMPLOYEES][NAME_LEN];
static double empBasic[MAX_EMPLOYEES];
static double empHousing[MAX_EMPLOYEES];
static double empTransport[MAX_EMPLOYEES];
static int    employeeCount = 0;
static int    nextEmployeeNumber = 1;

#define PENSION_RATE 0.07      /* 7% of basic salary */
#define SSC_RATE     0.009     /* 0.9% Social Security */
#define SSC_MAX      99.00     /* monthly SSC cap (N$) */

/* ---------- internal helpers ---------- */

static void generateEmployeeId(char *dest)
{
    sprintf(dest, "EMP%03d", nextEmployeeNumber);
    nextEmployeeNumber++;
}

static void storeEmployee(const char *name, const char *dept, const char *position,
                          double basic, double housing, double transport)
{
    generateEmployeeId(empId[employeeCount]);
    strcpy(empName[employeeCount], name);
    strcpy(empDept[employeeCount], dept);
    strcpy(empPosition[employeeCount], position);
    empBasic[employeeCount] = basic;
    empHousing[employeeCount] = housing;
    empTransport[employeeCount] = transport;
    employeeCount++;
}

static void printEmployeeHeader(void)
{
    printLine();
    printf("%-7s %-22s %-14s %-16s %12s\n", "ID", "Name", "Department", "Position", "Gross (N$)");
    printLine();
}

static void printEmployeeRow(int i)
{
    printf("%-7s %-22.22s %-14.14s %-16.16s %12.2f\n",
           empId[i], empName[i], empDept[i], empPosition[i], calculateGrossSalary(i));
}

/* ---------- salary calculations ---------- */

/* Gross = basic + housing + transport */
double calculateGrossSalary(int index)
{
    return empBasic[index] + empHousing[index] + empTransport[index];
}

/* PAYE using Namibian annual tax brackets, returned as a monthly amount */
double calculatePAYE(double monthlyGross)
{
    double annual = monthlyGross * 12;
    double tax;

    if (annual <= 100000) {
        tax = 0;
    } else if (annual <= 150000) {
        tax = (annual - 100000) * 0.18;
    } else if (annual <= 350000) {
        tax = 9000 + (annual - 150000) * 0.25;
    } else if (annual <= 550000) {
        tax = 59000 + (annual - 350000) * 0.28;
    } else if (annual <= 850000) {
        tax = 115000 + (annual - 550000) * 0.30;
    } else if (annual <= 1550000) {
        tax = 205000 + (annual - 850000) * 0.32;
    } else {
        tax = 429000 + (annual - 1550000) * 0.37;
    }
    return tax / 12;
}

static double calculatePension(int index)
{
    return empBasic[index] * PENSION_RATE;
}

static double calculateSSC(int index)
{
    double ssc = empBasic[index] * SSC_RATE;
    return (ssc > SSC_MAX) ? SSC_MAX : ssc;
}

double calculateNetSalary(int index)
{
    double gross = calculateGrossSalary(index);
    return gross - calculatePAYE(gross) - calculatePension(index) - calculateSSC(index);
}

/* ---------- menu operations ---------- */

static void addEmployee(void)
{
    char name[NAME_LEN], dept[NAME_LEN], position[NAME_LEN];
    double basic, housing, transport;

    if (employeeCount >= MAX_EMPLOYEES) {
        printf("  [ERROR] Employee list is full (%d).\n", MAX_EMPLOYEES);
        return;
    }
    printf("\n--- ADD EMPLOYEE ---\n");
    readNonEmptyString("Full name           : ", name, NAME_LEN);
    readNonEmptyString("Department          : ", dept, NAME_LEN);
    readNonEmptyString("Position            : ", position, NAME_LEN);
    basic     = readPositiveDouble("Basic salary (N$)   : ", 0);
    housing   = readPositiveDouble("Housing allow. (N$) : ", 1);
    transport = readPositiveDouble("Transport allow.(N$): ", 1);

    storeEmployee(name, dept, position, basic, housing, transport);
    printf("\n  [OK] Employee added with ID %s.\n", empId[employeeCount - 1]);
}

static void displayEmployees(void)
{
    int i;
    printf("\n--- ALL EMPLOYEES (%d) ---\n", employeeCount);
    if (employeeCount == 0) {
        printf("No employees registered.\n");
        return;
    }
    printEmployeeHeader();
    for (i = 0; i < employeeCount; i++) {
        printEmployeeRow(i);
    }
    printLine();
}

/* Linear search: by ID uses strcmp, by name/department uses partial match */
static void searchEmployee(void)
{
    char key[NAME_LEN], upperKey[NAME_LEN];
    int option, i, found = 0;

    printf("\n--- SEARCH EMPLOYEE ---\n");
    printf("1. By Employee ID\n2. By Name\n3. By Department\n");
    option = readInt("Choice: ", 1, 3);
    readNonEmptyString("Search for: ", key, NAME_LEN);
    toUpperCase(key, upperKey, NAME_LEN);

    for (i = 0; i < employeeCount; i++) {
        int match = 0;
        if (option == 1) {
            match = (strcmp(empId[i], upperKey) == 0);
        } else if (option == 2) {
            match = containsIgnoreCase(empName[i], key);
        } else {
            match = containsIgnoreCase(empDept[i], key);
        }
        if (match) {
            if (found == 0) {
                printEmployeeHeader();
            }
            printEmployeeRow(i);
            found++;
        }
    }
    if (found == 0) {
        printf("No employee matches \"%s\".\n", key);
    } else {
        printLine();
        printf("%d employee(s) found.\n", found);
    }
}

static int findEmployeeById(const char *id)
{
    char upperId[10];
    int i;
    toUpperCase(id, upperId, sizeof(upperId));
    for (i = 0; i < employeeCount; i++) {
        if (strcmp(empId[i], upperId) == 0) {
            return i;
        }
    }
    return -1;
}

/* Prints a full payslip for one employee */
static void calculateSalary(void)
{
    char id[10];
    int i;
    double gross, paye, pension, ssc, net;

    printf("\n--- SALARY CALCULATION (PAYSLIP) ---\n");
    if (employeeCount == 0) {
        printf("No employees registered.\n");
        return;
    }
    readNonEmptyString("Enter Employee ID (e.g. EMP001): ", id, sizeof(id));
    i = findEmployeeById(id);
    if (i == -1) {
        printf("  [ERROR] Employee %s not found.\n", id);
        return;
    }

    gross   = calculateGrossSalary(i);
    paye    = calculatePAYE(gross);
    pension = calculatePension(i);
    ssc     = calculateSSC(i);
    net     = gross - paye - pension - ssc;

    printLine();
    printf("Employee   : %s - %s\n", empId[i], empName[i]);
    printf("Department : %s  |  Position: %s\n", empDept[i], empPosition[i]);
    printLine();
    printf("Basic salary              N$ %12.2f\n", empBasic[i]);
    printf("Housing allowance         N$ %12.2f\n", empHousing[i]);
    printf("Transport allowance       N$ %12.2f\n", empTransport[i]);
    printf("GROSS SALARY              N$ %12.2f\n", gross);
    printf("  Less PAYE tax           N$ %12.2f\n", paye);
    printf("  Less pension (7%%)       N$ %12.2f\n", pension);
    printf("  Less Social Security    N$ %12.2f\n", ssc);
    printf("NET SALARY                N$ %12.2f\n", net);
    printf("Annual gross              N$ %12.2f\n", gross * 12);
    printLine();
}

void employeeMenu(void)
{
    int choice;
    do {
        printf("\n========== EMPLOYEE MANAGEMENT ==========\n");
        printf("1. Add employee\n");
        printf("2. Display employees\n");
        printf("3. Search employee\n");
        printf("4. Calculate salary (payslip)\n");
        printf("0. Back to main menu\n");
        choice = readInt("Enter your choice: ", 0, 4);

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: calculateSalary(); break;
            case 0: break;
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- data for reports ---------- */

int getEmployeeCount(void) { return employeeCount; }

const char *getEmployeeName(int index) { return empName[index]; }

void loadSampleEmployees(void)
{
    storeEmployee("Maria Shikongo",   "Finance",   "Accountant",      18500, 3000, 1500);
    storeEmployee("Johannes Nghidinwa","Water",    "Technician",      12000, 2000, 1200);
    storeEmployee("Anna Kandjii",     "HR",        "HR Officer",      15000, 2500, 1000);
    storeEmployee("Peter Amutenya",   "Finance",   "Finance Manager", 42000, 6000, 3000);
    storeEmployee("Selma Haufiku",    "Roads",     "Clerk",            8500, 1000,  800);
}
