#include "ODTE_SH.h"

void KeyGen(Para &param, HSS_EK &ek0, HSS_EK &ek1)
{
    HSS_Gen(param.pk, ek0, ek1, param.t);
}

void KeyFree(Para &param, HSS_EK &ek0, HSS_EK &ek1)
{
    // Nothing to free for HSS_EK type (ZZ)
}

void FeatureSelection2(Mat<HSS_CT> &Ix, const Para &param, const vec_ZZ &x, const Mat<HSS_CT> &Idelta)
{
    bool first;
    HSS_CT temp;
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
                        HSS_AddInput(temp, param.pk, temp, Idelta[i][k]);
                    }
                }
            }
            Ix[i][j] = temp;
        }
    }
}

void ProviderEnc(Mat<HSS_CT> &Idelta, Mat<HSS_CT> &Iy, Vec<HSS_CT> &Iv, const Para &param, const vec_ZZ &y, const vec_ZZ &v, const vector<vector<int>> &delta)
{

    Idelta.SetDims(param.m, param.n);
    Iy.SetDims(param.m, param.t);
    Iv.SetLength(param.k);

    // Encode delta matrix
    for (int i = 0; i < param.m; i++)
    {
        for (int j = 0; j < param.n; j++)
        {
            HSS_Input(Idelta[i][j], param.pk, conv<ZZ>(delta[i][j]));
        }
    }

    // Encode thresholds
    for (int i = 0; i < param.m; ++i)
    {
        for (int j = 0; j < param.t; ++j)
        {
            HSS_Input(Iy[i][j], param.pk, ZZ(bit(y[i], j)));
        }
    }

    // Encode leaf values
    for (int i = 0; i < param.k; i++)
    {
        HSS_Input(Iv[i], param.pk, v[i]);
    }
}

void HSSCMP(ZZ &c_b, int b, const Para &param, const HSS_EK &ekb, const Vec<HSS_CT> &Ix, const Vec<HSS_CT> &Iy, int &prf_key)
{
    HSS_MV M1b, Mxy, Mcb, Mx, Mcxy, M2c, M2c1;
    HSS_CT Cxy;

    // Initialize M1b
    M1b[0] = b;
    M1b[1] = ekb;


    HSS_Mul(Mx, b, param.pk, Ix[0], M1b, prf_key);
    HSS_Mul(Mxy, b, param.pk, Iy[0], Mx, prf_key);
    HSS_SubMemory(Mcb, param.pk, Mx, Mxy);

    for (int i = 1; i < param.t; ++i)
    {
        HSS_Mul(Mx, b, param.pk, Ix[i], M1b, prf_key);
        HSS_AddInput(Cxy, param.pk, Ix[i], Iy[i]);
        HSS_Mul(Mcxy, b, param.pk, Cxy, Mcb, prf_key);
        HSS_AddMemory(M2c, param.pk, Mcb, Mcb);
        HSS_SubMemory(M2c, param.pk, M2c, M1b);
        HSS_Mul(M2c1, b, param.pk, Ix[i], M2c, prf_key);
        HSS_Mul(M2c, b, param.pk, Iy[i], M2c1, prf_key);
        HSS_SubMemory(Mcb, param.pk, Mcb, Mcxy);
        HSS_AddMemory(Mcb, param.pk, Mcb, M2c);
        HSS_AddMemory(Mcb, param.pk, Mcb, Mx);

        // HSS_Mul(Mcxb, b, param.pk, Ix[i], Mcb, prf_key);
        // HSS_Mul(Mcyb, b, param.pk, Iy[i], Mcb, prf_key);
        // HSS_Mul(Mcxyb, b, param.pk, Ix[i], Mcyb, prf_key);
        // HSS_Mul(Mxb, b, param.pk, Ix[i], M1b, prf_key);
        // HSS_Mul(Mxyb, b, param.pk, Iy[i], Mxb, prf_key);
        // HSS_SubMemory(Mcb, param.pk, Mcb, Mcxb);
        // HSS_SubMemory(Mcb, param.pk, Mcb, Mcyb);
        // HSS_AddMemory(Mcb, param.pk, Mcb, Mcxyb);
        // HSS_AddMemory(Mcb, param.pk, Mcb, Mcxyb);
        // HSS_AddMemory(Mcb, param.pk, Mcb, Mxb);
        // HSS_SubMemory(Mcb, param.pk, Mcb, Mxyb);
    }
    c_b = Mcb[0];
}

void ClassificationGen(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b,int b,
                       const Para &param, const HSS_EK &ekb,
                       const Vec<ZZ> &cmp_res, const Vec<HSS_CT> &Iv, int &prf_key)
{
    ZZ pc, v, vv, r_0, r_1;
    int idx;

    HSS_MV M1b, vb, tvb;
    M1b[0] = b;
    M1b[1] = ekb;

    
    for (int i = 0; i < param.k; ++i)
    {
        HSS_Mul(vb, b, param.pk, Iv[i], M1b, prf_key);
        //HSS_Mul(tvb, b, param.pk, param.IA, vb, prf_key);

        // compute pc_k
        pc = 0;
        idx = param.m + i;

        while (idx >0)
        {
            idx = (idx - 1) / 2;
            if ((i % 2 == 0))
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
        //AddMod(vv, r_1, tvb[0], param.pk.N);

        MulMod(pc, r_0, pc, param.pk.N);

        pc_b.append(pc);
        vv_b.append(v);

        //vvv_b.append(vv);
    }

}

void DTEvaluation(Vec<ZZ> &pc_b, Vec<ZZ> &vv_b, int b,
                  const Para &param, const HSS_EK &ekb,
                  const Mat<HSS_CT> &Ix, const Mat<HSS_CT> &Iy,
                  const Vec<HSS_CT> &Iv)
{
    int prf_key = 0;

    // Compare at each node
    Vec<ZZ> cmp_res;
    cmp_res.SetLength(param.m);
    for (int i = 0; i < param.m; i++)
    {
        HSSCMP(cmp_res[i], b, param, ekb, Ix[i], Iy[i], prf_key);
    }

    // Generate classification
    ClassificationGen(pc_b, vv_b, b, param, ekb, cmp_res, Iv, prf_key);
}

void ClDecryption(ZZ &res, const Para &param,
                  const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0,
                  const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1)
{
    for (int i = 0; i < param.k; ++i)
    {
        if (pc_1[i] - pc_0[i] == 0)
        {
            res = vv_1[i] - vv_0[i];
            // break;
        }
    }
}