#include "VHSS.h"

void VHSS_Gen(VHSS_PK &pk, VHSS_EK &ek0, VHSS_EK &ek1, VHSS_VK &vk, int skLen)
{
    Elgamal_SK s;
    Elgamal_Gen(pk, s, skLen);

    RandomBits(ek0[0], skLen);
    add(ek1[0], ek0[0], s);

    RandomBits(ek0[1], skLen);
    RandomBits(vk, skLen);
    add(ek1[1], ek0[1], vk);

    RandomBits(ek0[2], skLen);
    add(ek1[2], ek0[2], vk * s);
}

void VHSS_Input(VHSS_CT &I, const VHSS_PK &pk, const ZZ &x)
{
    Elgamal_Enc(I[0], pk, x);
    Elgamal_skEnc(I[1], pk, x);
}

void VHSS_ConvertInput(VHSS_MV &Mx, int idx, const VHSS_PK &pk, const VHSS_EK &ek, const VHSS_CT &Ix, int &prf_key)
{
    VHSS_MV M1;
    M1[0] = idx;
    M1[1] = ek[0];
    M1[2] = ek[1];
    M1[3] = ek[2];
    VHSS_Mul(Mx, idx, pk, Ix, M1, prf_key);
}

void VHSS_Mul(VHSS_MV &Mz, int idx, const VHSS_PK &pk, const VHSS_CT &Ix, const VHSS_MV &My, int &prf_key)
{
    ZZ temp1, temp2;
    PowerMod(temp1, Ix[0][1], My[0], pk.N2);
    PowerMod(temp2, Ix[0][0], -My[1], pk.N2);
    MulMod(Mz[0], temp1, temp2, pk.N2);
    VHSS_DDLog(Mz[0], pk, Mz[0]);
    Mz[0] = PRF_ZZ(prf_key++, pk.N) + Mz[0];

    PowerMod(temp1, Ix[1][1], My[0], pk.N2);
    PowerMod(temp2, Ix[1][0], -My[1], pk.N2);
    MulMod(Mz[1], temp1, temp2, pk.N2);
    VHSS_DDLog(Mz[1], pk, Mz[1]);
    Mz[1] = PRF_ZZ(prf_key++, pk.N) + Mz[1];

    PowerMod(temp1, Ix[0][1], My[2], pk.N2);
    PowerMod(temp2, Ix[0][0], -My[3], pk.N2);
    MulMod(Mz[2], temp1, temp2, pk.N2);
    VHSS_DDLog(Mz[2], pk, Mz[2]);
    Mz[2] = PRF_ZZ(prf_key++, pk.N) + Mz[2];

    PowerMod(temp1, Ix[1][1], My[2], pk.N2);
    PowerMod(temp2, Ix[1][0], -My[3], pk.N2);
    MulMod(Mz[3], temp1, temp2, pk.N2);
    VHSS_DDLog(Mz[3], pk, Mz[3]);
    Mz[3] = PRF_ZZ(prf_key++, pk.N) + Mz[3];
}

void VHSS_DDLog(ZZ &z, const VHSS_PK &pk, const ZZ &g)
{
    ZZ h1, h, temp1;
    DivRem(h1, h, g, pk.N); // h = g % N; h1 = g / N
    InvMod(temp1, h, pk.N);
    MulMod(z, h1, temp1, pk.N);
}

void VHSS_AddMemory(VHSS_MV &Mz, const VHSS_PK &pk, const VHSS_MV &Mx, const VHSS_MV &My)
{
    add(Mz[0], Mx[0], My[0]);
    add(Mz[1], Mx[1], My[1]);
    add(Mz[2], Mx[2], My[2]);
    add(Mz[3], Mx[3], My[3]);
}

void VHSS_SubMemory(VHSS_MV &Mz, const VHSS_PK &pk, const VHSS_MV &Mx, const VHSS_MV &My)
{
    sub(Mz[0], Mx[0], My[0]);
    sub(Mz[1], Mx[1], My[1]);
    sub(Mz[2], Mx[2], My[2]);
    sub(Mz[3], Mx[3], My[3]);
}

void VHSS_AddInput(VHSS_CT &I, const VHSS_PK &pk, const VHSS_CT &Ix, const VHSS_CT &Iy)
{
    MulMod(I[0][0], Ix[0][0], Iy[0][0], pk.N2);
    MulMod(I[0][1], Ix[0][1], Iy[0][1], pk.N2);
    MulMod(I[1][0], Ix[1][0], Iy[1][0], pk.N2);
    MulMod(I[1][1], Ix[1][1], Iy[1][1], pk.N2);
}

void VHSS_Evaluate(VHSS_MV &y_b_res, int b, const vector<VHSS_CT> &Ix, const VHSS_PK &pk, const VHSS_EK &ekb, int &prf_key, vector<vector<int>> F_TEST)
{
    VHSS_MV M1, Monomial, tmp;
    M1[0] = b;
    M1[1] = ekb[0];
    M1[2] = ekb[1];
    M1[3] = ekb[2];

    y_b_res[0] = 0;
    y_b_res[1] = 0;
    y_b_res[2] = 0;
    y_b_res[3] = 0;

    int i, j, k;
    for (i = 0; i < F_TEST.size(); ++i)
    {
        copy(begin(M1), end(M1), begin(Monomial));
        for (j = 0; j < Ix.size(); ++j)
        {
            for (k = 0; k < F_TEST[i][j]; ++k)
            {
                VHSS_Mul(tmp, b, pk, Ix[j], Monomial, prf_key);
                copy(begin(tmp), end(tmp), begin(Monomial));
            }
        }
        VHSS_AddMemory(y_b_res, pk, y_b_res, Monomial);
    }
}

void VHSS_cMul(VHSS_MV &Mz, const VHSS_PK &pk, const ZZ &c, const VHSS_MV &Mx)
{
    MulMod(Mz[0], Mx[0], c, pk.N);
    MulMod(Mz[1], Mx[1], c, pk.N);
    MulMod(Mz[2], Mx[2], c, pk.N);
    MulMod(Mz[3], Mx[3], c, pk.N);
}