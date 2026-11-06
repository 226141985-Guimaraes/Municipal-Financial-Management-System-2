/* budget.h - Budget Management module */
#ifndef BUDGET_H
#define BUDGET_H

#define MAX_DEPARTMENTS 20

void budgetMenu(void);
void loadSampleBudgets(void);
int getDepartmentCount(void);
const char *getDepartmentName(int index);
double getAllocatedBudget(int index);
double getExpenditure(int index);
double calculateRemainingBudget(double allocated, double expenditure);
int isWithinBudget(double allocated, double expenditure);

#endif
