#pragma once
#include "HSS.h"

typedef struct
{
    Elgamal_PK pk;

    ZZ A;
    HSS_CT IA;

    int t; // bit length
    int m; // decision nodes
    int k; // leaf nodes
    int h; // tree depth
    int n; // attributes
} Para;

void KeyGen(Para &param, HSS_EK &ek0, HSS_EK &ek1);
void KeyFree(Para &param, HSS_EK &ek0, HSS_EK &ek1);

void FeatureSelection2(Mat<HSS_CT> &Ix, const Para &param, const vec_ZZ &x, const Mat<HSS_CT> &Idelta);
void ProviderEnc(Mat<HSS_CT> &Idelta, Mat<HSS_CT> &Iy, Vec<HSS_CT> &Iv, const Para &param, const vec_ZZ &y, const vec_ZZ &v, const vector<vector<int>> &delta);
void HSSCMP(ZZ &c_b, int b, const Para &param, const HSS_EK &ekb, const Vec<HSS_CT> &Ix, const Vec<HSS_CT> &Iy, int &prf_key);
void ClassificationGen(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, Vec<ZZ> &vvv_b, int b,
                       const Para &param, const HSS_EK &ekb,
                       const Vec<ZZ> &cmp_res, const Vec<HSS_CT> &Iv, int &prf_key);
void DTEvaluation(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, Vec<ZZ> &vvv_b, int b,
                  const Para &param, const HSS_EK &ekb,
                  const Mat<HSS_CT> &Ix, const Mat<HSS_CT> &Iy,
                  const Vec<HSS_CT> &Iv);
void ClDecryption(ZZ &res, const Para &param, const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0, const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1, const Vec<ZZ> &vvv_0, const Vec<ZZ> &vvv_1);