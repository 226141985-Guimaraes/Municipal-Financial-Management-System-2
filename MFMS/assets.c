/* assets.c - Asset Management module
 * Basic municipal asset register: add, display and search assets. */
#include <stdio.h>
#include <string.h>
#include "assets.h"
#include "validation.h"

static const char *ASSET_TYPES[] = {"Vehicle", "Computer", "Building", "Equipment", "Furniture"};
static const char *CONDITIONS[]  = {"New", "Good", "Fair", "Poor"};
#define NUM_TYPES 5
#define NUM_CONDITIONS 4

static char   assetId[MAX_ASSETS][10];
static char   assetName[MAX_ASSETS][NAME_LEN];
static int    assetType[MAX_ASSETS];        /* index into ASSET_TYPES */
static double assetValue[MAX_ASSETS];
static char   assetDept[MAX_ASSETS][NAME_LEN];
static int    assetCondition[MAX_ASSETS];   /* index into CONDITIONS */
static int    assetYear[MAX_ASSETS];
static int    assetCount = 0;
static int    nextAssetNumber = 1;

static void storeAsset(const char *name, int type, double value, const char *dept,
                       int condition, int year)
{
    sprintf(assetId[assetCount], "AST%03d", nextAssetNumber++);
    strcpy(assetName[assetCount], name);
    assetType[assetCount] = type;
    assetValue[assetCount] = value;
    strcpy(assetDept[assetCount], dept);
    assetCondition[assetCount] = condition;
    assetYear[assetCount] = year;
    assetCount++;
}

static int chooseFromList(const char *title, const char *list[], int size)
{
    int i;
    printf("%s\n", title);
    for (i = 0; i < size; i++) {
        printf("  %d. %s\n", i + 1, list[i]);
    }
    return readInt("Choice: ", 1, size) - 1;
}

static void printAssetHeader(void)
{
    printLine();
    printf("%-7s %-22s %-10s %-12s %-6s %5s %13s\n",
           "ID", "Name", "Type", "Department", "Cond.", "Year", "Value (N$)");
    printLine();
}

static void printAssetRow(int i)
{
    printf("%-7s %-22.22s %-10s %-12.12s %-6s %5d %13.2f\n",
           assetId[i], assetName[i], ASSET_TYPES[assetType[i]], assetDept[i],
           CONDITIONS[assetCondition[i]], assetYear[i], assetValue[i]);
}

static void addAsset(void)
{
    char name[NAME_LEN], dept[NAME_LEN];
    int type, condition, year;
    double value;

    if (assetCount >= MAX_ASSETS) {
        printf("  [ERROR] Asset register is full.\n");
        return;
    }
    printf("\n--- ADD ASSET ---\n");
    readNonEmptyString("Asset name       : ", name, NAME_LEN);
    type = chooseFromList("Asset type:", ASSET_TYPES, NUM_TYPES);
    value = readPositiveDouble("Purchase value N$: ", 0);
    readNonEmptyString("Department       : ", dept, NAME_LEN);
    condition = chooseFromList("Condition:", CONDITIONS, NUM_CONDITIONS);
    year = readInt("Purchase year    : ", 1950, 2026);

    storeAsset(name, type, value, dept, condition, year);
    printf("  [OK] Asset registered with ID %s.\n", assetId[assetCount - 1]);
}

void displayAssets(void)
{
    int i;
    printf("\n--- MUNICIPAL ASSET REGISTER (%d) ---\n", assetCount);
    if (assetCount == 0) {
        printf("No assets registered.\n");
        return;
    }
    printAssetHeader();
    for (i = 0; i < assetCount; i++) {
        printAssetRow(i);
    }
    printLine();
    printf("Total asset value: N$%.2f\n", calculateTotalAssetValue());
}

static void searchAsset(void)
{
    char key[NAME_LEN], upperKey[NAME_LEN];
    int option, i, found = 0, typeChoice = 0, condChoice = 0;

    printf("\n--- SEARCH ASSET ---\n");
    printf("1. By Asset ID\n2. By Name\n3. By Type\n4. By Department\n5. By Condition\n");
    option = readInt("Choice: ", 1, 5);

    if (option == 3) {
        typeChoice = chooseFromList("Type:", ASSET_TYPES, NUM_TYPES);
    } else if (option == 5) {
        condChoice = chooseFromList("Condition:", CONDITIONS, NUM_CONDITIONS);
    } else {
        readNonEmptyString("Search for: ", key, NAME_LEN);
        toUpperCase(key, upperKey, NAME_LEN);
    }

    for (i = 0; i < assetCount; i++) {
        int match = 0;
        switch (option) {
            case 1: match = (strcmp(assetId[i], upperKey) == 0); break;
            case 2: match = containsIgnoreCase(assetName[i], key); break;
            case 3: match = (assetType[i] == typeChoice); break;
            case 4: match = containsIgnoreCase(assetDept[i], key); break;
            case 5: match = (assetCondition[i] == condChoice); break;
        }
        if (match) {
            if (found == 0) printAssetHeader();
            printAssetRow(i);
            found++;
        }
    }
    if (found == 0) {
        printf("No matching assets found.\n");
    } else {
        printLine();
        printf("%d asset(s) found.\n", found);
    }
}

void assetMenu(void)
{
    int choice;
    do {
        printf("\n========== ASSET MANAGEMENT ==========\n");
        printf("1. Add asset\n");
        printf("2. Display asset register\n");
        printf("3. Search asset\n");
        printf("0. Back to main menu\n");
        choice = readInt("Enter your choice: ", 0, 3);

        switch (choice) {
            case 1: addAsset(); break;
            case 2: displayAssets(); break;
            case 3: searchAsset(); break;
            case 0: break;
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);
}

int getAssetCount(void) { return assetCount; }

double calculateTotalAssetValue(void)
{
    double total = 0;
    int i;
    for (i = 0; i < assetCount; i++) {
        total += assetValue[i];
    }
    return total;
}

void loadSampleAssets(void)
{
    storeAsset("Toyota Hilux N12345W", 0, 650000, "Roads",   1, 2022);
    storeAsset("Dell Latitude 5440",   1,  21000, "Finance", 0, 2025);
    storeAsset("Civic Centre Block A", 2, 9500000,"Admin",   2, 2008);
    storeAsset("Water Pump WP-200",    3, 180000, "Water",   3, 2015);
    storeAsset("Boardroom Table",      4,  35000, "HR",      1, 2019);
}
