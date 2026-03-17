#include "PVODTE_MS.h"

void ODTE_TIME_TEST(int depth, int N_attribute, int msgbit, int cyctimes, bool debug);

void ODTE_SETUP_TEST(Para &param, HSS_EK &ek0, HSS_EK &ek1, int depth, int N_attribute, int msgbit, int cyctimes, bool debug);

void ODTE_ProviderEnc_TEST(Mat<HSS_CT> &Iy,
                           Vec<HSS_CT> &Iv,
                           Mat<HSS_CT> &Idelta,
                           const vec_ZZ &Y,
                           const vec_ZZ &V,
                           const vector<vector<int>> &delta,
                           const Para &param, int cyctimes, bool debug);

void ODTE_DATA_PREPARATION(Vec<ZZ> &X, Vec<ZZ> &Y, Vec<ZZ> &V, vector<vector<int>> &delta, const Para &param);

void ODTE_HSSCMP_TEST(const Para &param, int b, const HSS_EK &ek0, const HSS_EK &ek1, int cyctimes);

void ODTE_ClassificationGen_TEST(const Para &param, int b, const HSS_EK &ek0, const HSS_EK &ek1, int cyctimes);

void ODTE_DTEvaluation_TEST(Vec<ZZ> &pc0, Vec<ZZ> &vv0, Vec<ZZ> &pc1, Vec<ZZ> &vv1, Vec<ZZ> &vvv0, Vec<ZZ> &vvv1,
                            const Para &param, const HSS_EK &ek0, const HSS_EK &ek1, const Mat<HSS_CT> &Ix, const Mat<HSS_CT> &Iy, const Vec<HSS_CT> &Iv,
                            int cyctimes);

void ODTE_Decryption_TEST(ZZ &res, const Para &param,
                          const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0,
                          const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1,
                          const Vec<ZZ> &vvv_0, const Vec<ZZ> &vvv_1, int cyctimes);