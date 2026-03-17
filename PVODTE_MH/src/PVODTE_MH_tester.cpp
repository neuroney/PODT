#include "PVODTE_MH_tester.h"

void ODTE_SETUP_TEST(Para &param, VHSS_EK &ek0, VHSS_EK &ek1, VHSS_VK &vk, int depth, int N_attribute, int msgbit, int cyctimes, bool debug)
{
    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        Para paramTEST;
        VHSS_EK ek0TEST, ek1TEST;
        VHSS_VK vkTEST;
        paramTEST.h = depth;
        paramTEST.k = 2 << (depth - 1);
        paramTEST.m = paramTEST.k - 1;
        paramTEST.t = msgbit;
        paramTEST.n = N_attribute;
        KeyGen(paramTEST, ek0TEST, ek1TEST, vkTEST);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "Setup algo time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
    param.h = depth;
    param.k = 2 << (depth - 1);
    param.m = param.k - 1;
    param.t = msgbit;
    param.n = N_attribute;
    KeyGen(param, ek0, ek1, vk);
    delete[] Time;
}

void ODTE_ProviderEnc_TEST(Mat<VHSS_CT> &Iy,
                          Vec<VHSS_CT> &Iv, 
                          Mat<VHSS_CT> &Idelta,
                          const vec_ZZ &Y,
                          const vec_ZZ &V,
                          const vector<vector<int>> &delta,
                          const Para &param, int cyctimes, bool debug)
{
    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        Mat<VHSS_CT> IyTEST, IdeltaTEST;
        Vec<VHSS_CT> IvTEST;
        ProviderEnc(IdeltaTEST, IyTEST, IvTEST, param, Y, V, delta);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "Provider encryption time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
    ProviderEnc(Idelta, Iy, Iv, param, Y, V, delta);
    //delete[] Time;
}

void ODTE_DATA_PREPARATION(Vec<ZZ> &X, Vec<ZZ> &Y, Vec<ZZ> &V, vector<vector<int>> &delta, const Para &param)
{
    X.SetLength(param.n); // Feature vector
    Y.SetLength(param.m); // Threshold value
    V.SetLength(param.k); // Classification
    delta.assign(param.m, std::vector<int>(param.n, 0));
    GenerateMatrix(param.m, param.n, delta);
    for (int i = 0; i < param.n; ++i)
    {
        RandomBits(X[i], param.t);
    }

    for (int i = 0; i < param.m; ++i)
    {
        RandomBits(Y[i], param.t);
    }
    for (int i = 0; i < param.k; ++i)
    {
        RandomBits(V[i], param.t);
    }
}

void ODTE_FeatureSelection2_TEST(Mat<VHSS_CT> &Ix,
                                 const Mat<VHSS_CT> &Idelta,
                                 const vec_ZZ &X,
                                 const Para &param, int cyctimes, bool debug)
{
    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        Ix.kill();
        time = GetTime();
        FeatureSelection2(Ix, param, X, Idelta);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "Feature Selection time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_VHSSCMP_TEST(const Para &param, int b, const VHSS_EK &ek0, const VHSS_EK &ek1, int cyctimes)
{
    int prf_key = 0;
    Vec<VHSS_CT> Ix, Iy;
    ZZ x, y, c_b;
    RandomBits(x, param.t);
    RandomBits(y, param.t);

    VHSS_CT tmp;
    for (int i = 0; i < param.t; ++i)
    {
        VHSS_Input(tmp, param.pk, ZZ(bit(x, i)));
        Ix.append(tmp);
        VHSS_Input(tmp, param.pk, ZZ(bit(y, i)));
        Iy.append(tmp);
    }

    VHSS_EK ekb;
    ekb = b ? ek1 : ek0;

    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        VHSSCMP(c_b, b, param, ekb, Ix, Iy, prf_key);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "VHSSCMP time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_ClassificationGen_TEST(const Para &param, int b, const VHSS_EK &ek0, const VHSS_EK &ek1, int cyctimes)
{
    Vec<ZZ> cmp_resb;
    cmp_resb.SetLength(param.m);
    for (int i = 0; i < param.m; ++i)
    {
        RandomBits(cmp_resb[i], 1);
    }

    int prf_key = 0;
    VHSS_EK ekb;
    ekb = b ? ek1 : ek0;
    Vec<ZZ> pcb, vvb, vvvb;

    vec_ZZ V;
    V.SetLength(param.k); // Classification
    for (int i = 0; i < param.k; ++i)
    {
        RandomBits(V[i], param.t);
    }

    Vec<VHSS_CT> Iv;
    Iv.SetLength(param.k);
    for (int j = 0; j < param.k; ++j)
    {
        VHSS_Input(Iv[j], param.pk, V[j]);
    }

    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        pcb.kill();
        vvb.kill();
        vvvb.kill();
        time = GetTime();
        ClassificationGen(pcb, vvb, vvvb, b, param, ekb, cmp_resb, Iv, prf_key);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "ClassificationGen time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_DTEvaluation_TEST(Vec<ZZ> &pc0, Vec<ZZ> &vv0, Vec<ZZ> &pc1, Vec<ZZ> &vv1, Vec<ZZ> &vvv0, Vec<ZZ> &vvv1,
                            const Para &param, const VHSS_EK &ek0, const VHSS_EK &ek1, const Mat<VHSS_CT> &Ix, const Mat<VHSS_CT> &Iy, const Vec<VHSS_CT> &Iv,
                            int cyctimes)
{
    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        pc0.kill();
        vv0.kill();
        time = GetTime();
        DTEvaluation(pc0, vv0, vvv0, 0, param, ek0, Ix, Iy, Iv);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "DTevaluation 0 time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";

    for (int i = 0; i < cyctimes; ++i)
    {
        pc1.kill();
        vv1.kill();
        time = GetTime();
        DTEvaluation(pc1, vv1, vvv1, 1, param, ek1, Ix, Iy, Iv);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "DTevaluation 1 time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_Decryption_TEST(ZZ &res, const Para &param, const VHSS_VK &vk,
                          const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0,
                          const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1,const Vec<ZZ> &vvv_0, const Vec<ZZ> &vvv_1,
                          int cyctimes)
{
    double *Time = new double[cyctimes];
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        ClDecryption(res, param, vk, pc_0, vv_0, pc_1, vv_1, vvv_0, vvv_1);
        Time[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, Time, cyctimes);
    cout << "Decryption time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_TIME_TEST(int depth, int N_attribute, int msgbit, int cyctimes, bool debug)
{
    // Start: Setup Test
    Para param;
    VHSS_EK ek0, ek1;
    VHSS_VK vk;
    ODTE_SETUP_TEST(param, ek0, ek1, vk, depth, N_attribute, msgbit, cyctimes, debug);
    // End: Setup Test

    // Start: Random Input
    Vec<ZZ> X, Y, V;
    vector<std::vector<int>> delta;
    ODTE_DATA_PREPARATION(X, Y, V, delta, param);
    // End: Random Input

    // Start: Provider Input Test
    Mat<VHSS_CT> Iy;
    Vec<VHSS_CT> Iv;
    Mat<VHSS_CT> Idelta;
    ODTE_ProviderEnc_TEST(Iy, Iv, Idelta, Y, V, delta, param, 1, debug);
    // End: Provider Input Test

    // randomize Iy, Iv, Idelta
    // Iy.SetDims(param.m, param.t);
    // Iv.SetLength(param.k);
    // Idelta.SetDims(param.m, param.n);

    // for (int i = 0; i < param.m; i++) {
    //     for (int j = 0; j < param.t; j++) {
    //         RandomBnd(Iy[i][j][0][0], param.pk.N2);
    //         RandomBnd(Iy[i][j][0][1], param.pk.N2);
    //         RandomBnd(Iy[i][j][1][0], param.pk.N2);
    //         RandomBnd(Iy[i][j][1][1], param.pk.N2);
    //     }
    // }

    // for (int i = 0; i < param.k; i++) {
    //     RandomBnd(Iv[i][0][0], param.pk.N2);
    //     RandomBnd(Iv[i][0][1], param.pk.N2);
    //     RandomBnd(Iv[i][1][0], param.pk.N2);
    //     RandomBnd(Iv[i][1][1], param.pk.N2);

    // }

    // for (int i = 0; i < param.m; i++) {
    //     for (int j = 0; j < param.n; j++) {
    //         RandomBnd(Idelta[i][j][0][0], param.pk.N2);
    //         RandomBnd(Idelta[i][j][0][1], param.pk.N2);
    //         RandomBnd(Idelta[i][j][1][0], param.pk.N2);
    //         RandomBnd(Idelta[i][j][1][1], param.pk.N2);
    //     }
    // }

    // Start: Feature Selection Test
    Mat<VHSS_CT> Ix;
    ODTE_FeatureSelection2_TEST(Ix, Idelta, X, param, cyctimes, debug);
    // End: Client Input Test

    // Start: VHSSCMP Test
    ODTE_VHSSCMP_TEST(param, 0, ek0, ek1, cyctimes);
    // End: VHSSCMP Test

    // Start: ClassificationGen Test
    ODTE_ClassificationGen_TEST(param, 0, ek0, ek1, cyctimes);
    // End: ClassificationGen Test

    // Start: Evaluation Test
    vec_ZZ pc0, vv0, pc1, vv1, vvv0, vvv1;
    ODTE_DTEvaluation_TEST(pc0, vv0, pc1, vv1, vvv0, vvv1, param, ek0, ek1, Ix, Iy, Iv, cyctimes);
    // End: Evaluation Test

    // Start: Decryption Test
    ZZ Predict_v;
    ODTE_Decryption_TEST(Predict_v, param, vk, pc0, vv0, pc1, vv1, vvv0, vvv1,cyctimes);
    // End: Decryption Test
}