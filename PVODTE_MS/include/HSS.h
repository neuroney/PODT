#pragma once
#include "helper.h"
#include "elgamal.h"

typedef Elgamal_PK HSS_PK;
typedef ZZ HSS_EK;
typedef array<Elgamal_CT, 2> HSS_CT;
typedef array<ZZ, 2> HSS_MV;

void HSS_Gen(HSS_PK &pk, HSS_EK &ek0, HSS_EK &ek1, int skLen);
void HSS_Input(HSS_CT &I, const HSS_PK &pk, const ZZ &x);
void HSS_ConvertInput(HSS_MV &Mx, int idx, const HSS_PK &pk, const HSS_EK &ek, const HSS_CT &Ix, int &prf_key);
void HSS_cMul(HSS_MV &Mz, const HSS_PK &pk, const ZZ &c, const HSS_MV &Mx);
void HSS_Mul(HSS_MV &Mz, int idx, const HSS_PK &pk, const HSS_CT &Ix, const HSS_MV &My, int &prf_key);
void HSS_DDLog(ZZ &z, const HSS_PK &pk, const ZZ &g);
void HSS_AddMemory(HSS_MV &Mz, const HSS_PK &pk, const HSS_MV &Mx, const HSS_MV &My);
void HSS_SubMemory(HSS_MV &Mz, const HSS_PK &pk, const HSS_MV &Mx, const HSS_MV &My);
void HSS_AddInput(HSS_CT &I, const HSS_PK &pk, const HSS_CT &Ix, const HSS_CT &Iy);
void HSS_Evaluate(HSS_MV &y_b_res, int b, const vector<HSS_CT> &Ix, const HSS_PK &pk, const HSS_EK &ekb, int &prf_key, vector<vector<int>> F_TEST);