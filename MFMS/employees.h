/* employees.h - Employee Management module */
#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#define MAX_EMPLOYEES 100

void employeeMenu(void);
void loadSampleEmployees(void);
int getEmployeeCount(void);
double calculateGrossSalary(int index);
double calculatePAYE(double monthlyGross);
double calculateNetSalary(int index);
const char *getEmployeeName(int index);

#endif
