#pragma once
#include "helper.h"
#include "elgamal.h"

typedef Elgamal_PK VHSS_PK;
typedef array<ZZ, 3> VHSS_EK;
typedef ZZ VHSS_VK;
typedef array<Elgamal_CT, 2> VHSS_CT;
typedef array<ZZ, 4> VHSS_MV;

void VHSS_Gen(VHSS_PK &pk, VHSS_EK &ek0, VHSS_EK &ek1, VHSS_VK &vk, int skLen);
void VHSS_Input(VHSS_CT &I, const VHSS_PK &pk, const ZZ &x);
void VHSS_ConvertInput(VHSS_MV &Mx, int idx, const VHSS_PK &pk, const VHSS_EK &ek, const VHSS_CT &Ix, int &prf_key);
void VHSS_cMul(VHSS_MV &Mz, const VHSS_PK &pk, const ZZ &c, const VHSS_MV &Mx);
void VHSS_Mul(VHSS_MV &Mz, int idx, const VHSS_PK &pk, const VHSS_CT &Ix, const VHSS_MV &My, int &prf_key);
void VHSS_DDLog(ZZ &z, const VHSS_PK &pk, const ZZ &g);
void VHSS_AddMemory(VHSS_MV &Mz, const VHSS_PK &pk, const VHSS_MV &Mx, const VHSS_MV &My);
void VHSS_SubMemory(VHSS_MV &Mz, const VHSS_PK &pk, const VHSS_MV &Mx, const VHSS_MV &My);
void VHSS_AddInput(VHSS_CT &I, const VHSS_PK &pk, const VHSS_CT &Ix, const VHSS_CT &Iy);
void VHSS_Evaluate(VHSS_MV &y_b_res, int b, const vector<VHSS_CT> &Ix, const VHSS_PK &pk, const VHSS_EK &ekb, int &prf_key, vector<vector<int>> F_TEST);