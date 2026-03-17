#include "HSS.h"

void HSS_Gen(HSS_PK &pk, HSS_EK &ek0, HSS_EK &ek1, int skLen)
{
    Elgamal_SK s;
    Elgamal_Gen(pk, s, skLen);

    RandomBits(ek0, skLen);
    add(ek1, ek0, s);
}


void HSS_Input(HSS_CT &I, const HSS_PK &pk, const ZZ &x)
{
    Elgamal_Enc(I[0], pk, x);
    Elgamal_skEnc(I[1], pk, x);
}

void HSS_ConvertInput(HSS_MV &Mx, int idx, const HSS_PK &pk, const HSS_EK &ek, const HSS_CT &Ix, int &prf_key)
{
    HSS_MV M1;
    M1[0] = idx;
    M1[1] = ek;
    HSS_Mul(Mx, idx, pk, Ix, M1, prf_key);
}

void HSS_Mul(HSS_MV &Mz, int idx, const HSS_PK &pk, const HSS_CT &Ix, const HSS_MV &My, int &prf_key)
{
    ZZ temp1, temp2;
    PowerMod(temp1, Ix[0][1], My[0], pk.N2);
    PowerMod(temp2, Ix[0][0], -My[1], pk.N2);
    MulMod(Mz[0], temp1, temp2, pk.N2);
    HSS_DDLog(Mz[0], pk, Mz[0]);
    Mz[0] = PRF_ZZ(prf_key++, pk.N) + Mz[0];

    PowerMod(temp1, Ix[1][1], My[0], pk.N2);
    PowerMod(temp2, Ix[1][0], -My[1], pk.N2);
    MulMod(Mz[1], temp1, temp2, pk.N2);
    HSS_DDLog(Mz[1], pk, Mz[1]);
    Mz[1] = PRF_ZZ(prf_key++, pk.N) + Mz[1];
}

void HSS_DDLog(ZZ &z, const HSS_PK &pk, const ZZ &g)
{
    ZZ h1, h, temp1;
    DivRem(h1, h, g, pk.N); // h = g % N; h1 = g / N
    InvMod(temp1, h, pk.N);
    MulMod(z, h1, temp1, pk.N);
}

void HSS_AddMemory(HSS_MV &Mz, const HSS_PK &pk, const HSS_MV &Mx, const HSS_MV &My)
{
    add(Mz[0], Mx[0], My[0]);
    add(Mz[1], Mx[1], My[1]);
}

void HSS_SubMemory(HSS_MV &Mz, const HSS_PK &pk, const HSS_MV &Mx, const HSS_MV &My)
{
    sub(Mz[0], Mx[0], My[0]);
    sub(Mz[1], Mx[1], My[1]);
}

void HSS_AddInput(HSS_CT &I, const HSS_PK &pk, const HSS_CT &Ix, const HSS_CT &Iy)
{
    MulMod(I[0][0], Ix[0][0], Iy[0][0], pk.N2);
    MulMod(I[0][1], Ix[0][1], Iy[0][1], pk.N2);
    MulMod(I[1][0], Ix[1][0], Iy[1][0], pk.N2);
    MulMod(I[1][1], Ix[1][1], Iy[1][1], pk.N2);
}

void HSS_Evaluate(HSS_MV &y_b_res, int b, const vector<HSS_CT> &Ix, const HSS_PK &pk, const HSS_EK &ekb, int &prf_key, vector<vector<int>> F_TEST)
{
    HSS_MV M1, Monomial, tmp;
    M1[0] = b;
    M1[1] = ekb;

    y_b_res[0] = 0;
    y_b_res[1] = 0;

    int i, j, k;
    for (i = 0; i < F_TEST.size(); ++i)
    {
        copy(begin(M1), end(M1), begin(Monomial));
        for (j = 0; j < Ix.size(); ++j)
        {
            for (k = 0; k < F_TEST[i][j]; ++k)
            {
                HSS_Mul(tmp, b, pk, Ix[j], Monomial, prf_key);
                copy(begin(tmp), end(tmp), begin(Monomial));
            }
        }
        HSS_AddMemory(y_b_res, pk, y_b_res, Monomial);
    }
}

void HSS_cMul(HSS_MV &Mz, const HSS_PK &pk, const ZZ &c, const HSS_MV &Mx)
{
    MulMod(Mz[0], Mx[0], c, pk.N);
    MulMod(Mz[1], Mx[1], c, pk.N);
}