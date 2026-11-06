/* assets.h - Asset Management module */
#ifndef ASSETS_H
#define ASSETS_H

#define MAX_ASSETS 100

void assetMenu(void);
void loadSampleAssets(void);
void displayAssets(void);
int getAssetCount(void);
double calculateTotalAssetValue(void);

#endif
