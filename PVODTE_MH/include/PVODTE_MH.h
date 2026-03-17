#pragma once
#include "VHSS.h"

typedef struct
{
    Elgamal_PK pk;

    int t; // bit length
    int m; // decision nodes
    int k; // leaf nodes
    int h; // tree depth
    int n; // attributes
} Para;

void KeyGen(Para &param, VHSS_EK &ek0, VHSS_EK &ek1, VHSS_VK &vk);
void KeyFree(Para &param, VHSS_EK &ek0, VHSS_EK &ek1);

void FeatureSelection2(Mat<VHSS_CT> &Ix, const Para &param, const vec_ZZ &x, const Mat<VHSS_CT> &Idelta);
void ProviderEnc(Mat<VHSS_CT> &Idelta, Mat<VHSS_CT> &Iy, Vec<VHSS_CT> &Iv, const Para &param, const vec_ZZ &y, const vec_ZZ &v, const vector<vector<int>> &delta);
void VHSSCMP(ZZ &c_b, int b, const Para &param, const VHSS_EK &ekb, const Vec<VHSS_CT> &Ix, const Vec<VHSS_CT> &Iy, int &prf_key);
void ClassificationGen(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, Vec<ZZ> &vvv_b, int b,
                       const Para &param, const VHSS_EK &ekb,
                       const Vec<ZZ> &cmp_res, const Vec<VHSS_CT> &Iv, int &prf_key);
void DTEvaluation(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, Vec<ZZ> &vvv_b, int b,
                  const Para &param, const VHSS_EK &ekb,
                  const Mat<VHSS_CT> &Ix, const Mat<VHSS_CT> &Iy,
                  const Vec<VHSS_CT> &Iv);
void ClDecryption(ZZ &res, const Para &param, const VHSS_VK &vk, 
const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0, const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1, const Vec<ZZ> &vvv_0, const Vec<ZZ> &vvv_1);