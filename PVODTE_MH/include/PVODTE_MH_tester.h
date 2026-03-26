#pragma once

#include "PVODTE_MH.h"

void ODTE_TIME_TEST(int depth, int N_attribute, int msgbit, int cyctimes, bool debug);

void ODTE_SETUP_TEST(Para &param, VHSS_EK &ek0, VHSS_EK &ek1, VHSS_VK &vk, int depth, int N_attribute, int msgbit, int cyctimes, bool debug);

void ODTE_ProviderEnc_TEST(Mat<VHSS_CT> &Iy,
                           Vec<VHSS_CT> &Iv,
                           Mat<VHSS_CT> &Idelta,
                           const vec_ZZ &Y,
                           const vec_ZZ &V,
                           const vector<vector<int>> &delta,
                           const Para &param, int cyctimes, bool debug);

void ODTE_DATA_PREPARATION(Vec<ZZ> &X, Vec<ZZ> &Y, Vec<ZZ> &V, vector<vector<int>> &delta, const Para &param);

void ODTE_VHSSCMP_TEST(const Para &param, int b, const VHSS_EK &ek0, const VHSS_EK &ek1, int cyctimes);

void ODTE_ClassificationGen_TEST(const Para &param, int b, const VHSS_EK &ek0, const VHSS_EK &ek1, int cyctimes);

void ODTE_DTEvaluation_TEST(Vec<ZZ> &pc0, Vec<ZZ> &vv0, Vec<ZZ> &pc1, Vec<ZZ> &vv1, Vec<ZZ> &vvv0, Vec<ZZ> &vvv1,
                            const Para &param, const VHSS_EK &ek0, const VHSS_EK &ek1, const Mat<VHSS_CT> &Ix, const Mat<VHSS_CT> &Iy, const Vec<VHSS_CT> &Iv,
                            int cyctimes);

void ODTE_Decryption_TEST(ZZ &res, const Para &param, const VHSS_VK &vk,
                          const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0,
                          const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1,
                          const Vec<ZZ> &vvv_0, const Vec<ZZ> &vvv_1, int cyctimes);
