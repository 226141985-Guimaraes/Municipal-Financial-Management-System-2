# Municipal Financial Management System (MFMS)

**Course:** PAP521S – Programming in Practice
**Project:** Project A – Foundation System
<img src="docs/nust_logo.png" width="320" alt="NUST logo">

**Institution:** Namibia University of Science and Technology (NUST)
**Group number:** G24

## Group Members

| Name | Student Number | Responsibility |
|------|----------------|----------------|
| Guimany Guimaraes | 226141985 | All modules: Employee, Budget, Supplier, Asset, Reports, integration/validation, testing, documentation and Git |

> Note: this project was developed by one student. No other group members were available, because all groups were already full.

## Project Description

MFMS is a menu-driven console application written in ANSI C (C99). It gives a municipality one place to manage employees and salaries, departmental budgets and expenditure, suppliers, and a municipal asset register. It also produces summary reports. This is the foundation version that Project B will extend.

## System Features

**Employee Management**
- Add an employee. The ID is auto-generated, for example EMP001.
- Display all employees.
- Search by ID, name or department. Name and department searches are partial and case-insensitive.
- Show a payslip for one employee: gross salary, PAYE (Namibian tax brackets), 7% pension, Social Security (0.9%, capped at N$99) and net salary.

**Budget Management**
- Enter or update a departmental budget.
- Record expenditure. You get an alert as soon as a department goes over budget.
- See the remaining budget, the percentage used and the status (WITHIN BUDGET / WITHIN (WARNING) at 90% or more / EXCEEDED).
- List departments that have exceeded their budget.

**Supplier Management**
- Add suppliers, with email and phone validation and no duplicate names.
- Display all suppliers.
- Search by ID, name, town or category.
- Compare two suppliers side by side using `strcmp`.

**Asset Management**
- Register assets of these types: Vehicle, Computer, Building, Equipment, Furniture. Each asset has a condition, a purchase year and a value.
- Display the asset register with its total value.
- Search by ID, name, type, department or condition.

**Reports**
- Employee report: total employees, payroll, average, highest and lowest salary.
- Budget report: total allocated, total expenditure (summed recursively), remaining budget, and departments over budget.
- Supplier report and asset report.
- A full municipal summary that combines all of the above.

**Input Validation**
- Menus accept only valid options. Text and out-of-range numbers are rejected.
- Salaries, budgets and values cannot be negative.
- Names and other required fields cannot be empty or only spaces.
- Email and telephone formats are checked.

## Project Structure

```
MFMS/
├── main.c           Main menu and program entry point
├── employees.c/.h   Employee Management and salary calculations
├── budget.c/.h      Budget Management
├── suppliers.c/.h   Supplier Management
├── assets.c/.h      Asset Management
├── reports.c/.h     Reports
├── validation.c/.h  Input validation and string helpers
├── Makefile
├── README.md
└── docs/            Technical report and contribution record
```

## Compilation

You need GCC.

```bash
gcc -std=c99 -Wall -Wextra main.c employees.c budget.c suppliers.c assets.c reports.c validation.c -o mfms
```

Or, if you have `make`:

```bash
make
```

## How to Run

- Linux/macOS: `./mfms`
- Windows: `mfms.exe`

When the program starts, it loads sample data so that every feature can be demonstrated straight away. Use the number keys to navigate the menus. Enter `0` to go back to the previous menu, and `6` in the main menu to exit.

## Individual Responsibilities

| Area | Developer |
|------|-----------|
| Employee Management | Guimany |
| Budget Management | Guimany |
| Supplier Management | Guimany |
| Asset Management | Guimany |
| Reports | Guimany |
| Functions, integration and validation | Guimany |
| Testing, documentation and Git | Guimany |
