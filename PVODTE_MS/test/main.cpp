#include "PVODTE_MS_tester.h"

int main(int, char **)
{
    // Arrays of depths and N_attributes to test
    int depths[] = {3, 8, 13, 17, 20};
    int N_attributes[] = {13, 9, 13, 57, 784};
    int msgbit = 10;
    int cyctimes = 5;
    bool debug = false;

    // Loop through each combination
    for (int i = 0; i < 5; i++) {
        int depth = depths[i];
        int N_attribute = N_attributes[i];
        
        cout << "=== Testing depth: " << depth << ", N_attribute: " << N_attribute << " ===\n";
        cout << "Number of non-leaf nodes: " << power2_ZZ(depth) - 1 << endl;

        ODTE_TIME_TEST(depth, N_attribute, msgbit, cyctimes, debug);
    }
    return 0;
}
