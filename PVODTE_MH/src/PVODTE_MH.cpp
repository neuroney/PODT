#include "PVODTE_MH.h"

void KeyGen(Para &param, VHSS_EK &ek0, VHSS_EK &ek1, VHSS_VK &vk)
{
    VHSS_Gen(param.pk, ek0, ek1, vk, param.t);
}

void KeyFree(Para &param, VHSS_EK &ek0, VHSS_EK &ek1)
{
    // Nothing to free for VHSS_EK type (ZZ)
}

void FeatureSelection2(Mat<VHSS_CT> &Ix, const Para &param, const vec_ZZ &x, const Mat<VHSS_CT> &Idelta)
{
    bool first;
    VHSS_CT temp;
    Ix.SetDims(param.m, param.t);
    for (int i = 0; i < param.m; i++)
    {
        for (int j = 0; j < param.t; j++)
        {
            first = true;
            for (int k = 0; k < param.n; ++k)
            {
                if (bit(x[k], j) == 1)
                {
                    if (first)
                    {
                        temp = Idelta[i][k];
                        first = false;
                    }
                    else
                    {
                        VHSS_AddInput(temp, param.pk, temp, Idelta[i][k]);
                    }
                }
            }
            Ix[i][j] = temp;
        }
    }
}

void ProviderEnc(Mat<VHSS_CT> &Idelta, Mat<VHSS_CT> &Iy, Vec<VHSS_CT> &Iv, const Para &param, const vec_ZZ &y, const vec_ZZ &v, const vector<vector<int>> &delta)
{

    Idelta.SetDims(param.m, param.n);
    Iy.SetDims(param.m, param.t);
    Iv.SetLength(param.k);

    // Encode delta matrix
    for (int i = 0; i < param.m; i++)
    {
        for (int j = 0; j < param.n; j++)
        {
            VHSS_Input(Idelta[i][j], param.pk, conv<ZZ>(delta[i][j]));
        }
    }

    // Encode thresholds
    for (int i = 0; i < param.m; ++i)
    {
        for (int j = 0; j < param.t; ++j)
        {
            VHSS_Input(Iy[i][j], param.pk, ZZ(bit(y[i], j)));
        }
    }

    // Encode leaf values
    for (int i = 0; i < param.k; i++)
    {
        VHSS_Input(Iv[i], param.pk, v[i]);
    }
}

void VHSSCMP(ZZ &c_b, int b, const Para &param, const VHSS_EK &ekb, const Vec<VHSS_CT> &Ix, const Vec<VHSS_CT> &Iy, int &prf_key)
{
    VHSS_MV M1b, Mxy, Mcb, Mx, Mcxy, M2c, M2c1;
    VHSS_CT Cxy;

    // Initialize M1b
    M1b[0] = b;
    M1b[1] = ekb[0];
    M1b[2] = ekb[1];
    M1b[3] = ekb[2];

    VHSS_Mul(Mx, b, param.pk, Ix[0], M1b, prf_key);
    VHSS_Mul(Mxy, b, param.pk, Iy[0], Mx, prf_key);
    VHSS_SubMemory(Mcb, param.pk, Mx, Mxy);

    for (int i = 1; i < param.t; ++i)
    {
        VHSS_Mul(Mx, b, param.pk, Ix[i], M1b, prf_key);
        VHSS_AddInput(Cxy, param.pk, Ix[i], Iy[i]);
        VHSS_Mul(Mcxy, b, param.pk, Cxy, Mcb, prf_key);
        VHSS_AddMemory(M2c, param.pk, Mcb, Mcb);
        VHSS_SubMemory(M2c, param.pk, M2c, M1b);
        VHSS_Mul(M2c1, b, param.pk, Ix[i], M2c, prf_key);
        VHSS_Mul(M2c, b, param.pk, Iy[i], M2c1, prf_key);
        VHSS_SubMemory(Mcb, param.pk, Mcb, Mcxy);
        VHSS_AddMemory(Mcb, param.pk, Mcb, M2c);
        VHSS_AddMemory(Mcb, param.pk, Mcb, Mx);

        // VHSS_Mul(Mcxb, b, param.pk, Ix[i], Mcb, prf_key);
        // VHSS_Mul(Mcyb, b, param.pk, Iy[i], Mcb, prf_key);
        // VHSS_Mul(Mcxyb, b, param.pk, Ix[i], Mcyb, prf_key);
        // VHSS_Mul(Mxb, b, param.pk, Ix[i], M1b, prf_key);
        // VHSS_Mul(Mxyb, b, param.pk, Iy[i], Mxb, prf_key);
        // VHSS_SubMemory(Mcb, param.pk, Mcb, Mcxb);
        // VHSS_SubMemory(Mcb, param.pk, Mcb, Mcyb);
        // VHSS_AddMemory(Mcb, param.pk, Mcb, Mcxyb);
        // VHSS_AddMemory(Mcb, param.pk, Mcb, Mcxyb);
        // VHSS_AddMemory(Mcb, param.pk, Mcb, Mxb);
        // VHSS_SubMemory(Mcb, param.pk, Mcb, Mxyb);
    }
    c_b = Mcb[0];
}

void ClassificationGen(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, Vec<ZZ> &vvv_b, int b,
                       const Para &param, const VHSS_EK &ekb,
                       const Vec<ZZ> &cmp_res, const Vec<VHSS_CT> &Iv, int &prf_key)
{
    ZZ pc, v, vv, r_0, r_1;
    int idx;

    VHSS_MV M1b, vb;
    M1b[0] = b;
    M1b[1] = ekb[0];
    M1b[2] = ekb[1];
    M1b[3] = ekb[2];

    for (int i = 0; i < param.k; ++i)
    {
        VHSS_Mul(vb, b, param.pk, Iv[i], M1b, prf_key);

        // compute pc_k
        pc = 0;
        idx = param.m + i;

        while (idx)
        {
            bool lr = (idx - 1) & 1; // & 1
            idx = (idx - 1) / 2;
            if (lr)
            {
                AddMod(pc, pc, cmp_res[idx], param.pk.N); // right
            }
            else
            {
                pc = (pc + b - cmp_res[idx]) % param.pk.N; // left
            }
        }
        // random
        r_0 = PRF_ZZ(prf_key++, param.pk.N);
        r_1 = PRF_ZZ(prf_key++, param.pk.N);

        MulMod(r_1, r_1, pc, param.pk.N);
        AddMod(v, r_1, vb[0], param.pk.N);
        AddMod(vv, r_1, vb[2], param.pk.N);

        MulMod(pc, r_0, pc, param.pk.N);

        pc_b.append(pc);
        vv_b.append(v);
        vvv_b.append(vv);
    }
}

void DTEvaluation(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, Vec<ZZ> &vvv_b, int b,
                  const Para &param, const VHSS_EK &ekb,
                  const Mat<VHSS_CT> &Ix, const Mat<VHSS_CT> &Iy,
                  const Vec<VHSS_CT> &Iv)
{
    int prf_key = 0;

    // Compare at each node
    Vec<ZZ> cmp_res;
    cmp_res.SetLength(param.m);
    for (int i = 0; i < param.m; i++)
    {
        VHSSCMP(cmp_res[i], b, param, ekb, Ix[i], Iy[i], prf_key);
    }

    // Generate classification
    ClassificationGen(pc_b, vv_b, vvv_b, b, param, ekb, cmp_res, Iv, prf_key);
}

void ClDecryption(ZZ &res, const Para &param, const VHSS_VK &vk,
                  const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0,
                  const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1,
                  const Vec<ZZ> &vvv_0, const Vec<ZZ> &vvv_1)
{
    for (int i = 0; i < param.k; ++i)
    {
        if (pc_1[i] - pc_0[i] == 0)
        {
            res = vv_1[i] - vv_0[i];
            if (vk * res == (vvv_1[i] - vvv_0[i]))
            {
                cout << "Verification Pass" << endl;
            }
            // break;
        }
    }
}