#include "ODTE_SH_tester.h"

void ODTE_SETUP_TEST(Para &param, HSS_EK &ek0, HSS_EK &ek1, int depth, int N_attribute, int msgbit, int cyctimes, bool debug)
{
    (void)debug;
    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        Para paramTEST;
        HSS_EK ek0TEST, ek1TEST;
        paramTEST.h = depth;
        paramTEST.k = 2 << (depth - 1);
        paramTEST.m = paramTEST.k - 1;
        paramTEST.t = msgbit;
        paramTEST.n = N_attribute;
        KeyGen(paramTEST, ek0TEST, ek1TEST);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "Setup algo time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
    param.h = depth;
    param.k = 2 << (depth - 1);
    param.m = param.k - 1;
    param.t = msgbit;
    param.n = N_attribute;
    KeyGen(param, ek0, ek1);
}

void ODTE_ProviderEnc_TEST(Mat<HSS_CT> &Iy,
                          Vec<HSS_CT> &Iv, 
                          Mat<HSS_CT> &Idelta,
                          const vec_ZZ &Y,
                          const vec_ZZ &V,
                          const vector<vector<int>> &delta,
                          const Para &param, int cyctimes, bool debug)
{
    (void)debug;
    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        Mat<HSS_CT> IyTEST, IdeltaTEST;
        Vec<HSS_CT> IvTEST;
        ProviderEnc(IdeltaTEST, IyTEST, IvTEST, param, Y, V, delta);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "Provider encryption time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
    ProviderEnc(Idelta, Iy, Iv, param, Y, V, delta);
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

void ODTE_FeatureSelection2_TEST(Mat<HSS_CT> &Ix,
                                 const Mat<HSS_CT> &Idelta,
                                 const vec_ZZ &X,
                                 const Para &param, int cyctimes, bool debug)
{
    (void)debug;
    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        Ix.kill();
        time = GetTime();
        FeatureSelection2(Ix, param, X, Idelta);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "Feature Selection time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_HSSCMP_TEST(const Para &param, int b, const HSS_EK &ek0, const HSS_EK &ek1, int cyctimes)
{
    int prf_key = 0;
    Vec<HSS_CT> Ix, Iy;
    ZZ x, y, c_b;
    RandomBits(x, param.t);
    RandomBits(y, param.t);

    HSS_CT tmp;
    for (int i = 0; i < param.t; ++i)
    {
        HSS_Input(tmp, param.pk, ZZ(bit(x, i)));
        Ix.append(tmp);
        HSS_Input(tmp, param.pk, ZZ(bit(y, i)));
        Iy.append(tmp);
    }

    HSS_EK ekb;
    ekb = b ? ek1 : ek0;

    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        HSSCMP(c_b, b, param, ekb, Ix, Iy, prf_key);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "HSSCMP time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_ClassificationGen_TEST(const Para &param, int b, const HSS_EK &ek0, const HSS_EK &ek1, int cyctimes)
{
    Vec<ZZ> cmp_resb;
    cmp_resb.SetLength(param.m);
    for (int i = 0; i < param.m; ++i)
    {
        RandomBits(cmp_resb[i], 1);
    }

    int prf_key = 0;
    HSS_EK ekb;
    ekb = b ? ek1 : ek0;
    Vec<ZZ> pcb, vvb;

    vec_ZZ V;
    V.SetLength(param.k); // Classification
    for (int i = 0; i < param.k; ++i)
    {
        RandomBits(V[i], param.t);
    }

    Vec<HSS_CT> Iv;
    Iv.SetLength(param.k);
    for (int j = 0; j < param.k; ++j)
    {
        HSS_Input(Iv[j], param.pk, V[j]);
    }

    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        pcb.kill();
        vvb.kill();
        time = GetTime();
        ClassificationGen(pcb, vvb, b, param, ekb, cmp_resb, Iv, prf_key);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "ClassificationGen time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_DTEvaluation_TEST(Vec<ZZ> &pc0, Vec<ZZ> &vv0, Vec<ZZ> &pc1, Vec<ZZ> &vv1,
                            const Para &param, const HSS_EK &ek0, const HSS_EK &ek1, const Mat<HSS_CT> &Ix, const Mat<HSS_CT> &Iy, const Vec<HSS_CT> &Iv,
                            int cyctimes)
{
    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        pc0.kill();
        vv0.kill();
        time = GetTime();
        DTEvaluation(pc0, vv0, 0, param, ek0, Ix, Iy, Iv);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "DTevaluation 0 time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";

    for (int i = 0; i < cyctimes; ++i)
    {
        pc1.kill();
        vv1.kill();
        time = GetTime();
        DTEvaluation(pc1, vv1, 1, param, ek1, Ix, Iy, Iv);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "DTevaluation 1 time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_Decryption_TEST(ZZ &res, const Para &param,
                          const Vec<ZZ> &pc_0, const Vec<ZZ> &vv_0,
                          const Vec<ZZ> &pc_1, const Vec<ZZ> &vv_1,
                          int cyctimes)
{
    vector<double> timings(cyctimes);
    double time, mean, stdev;
    for (int i = 0; i < cyctimes; ++i)
    {
        time = GetTime();
        ClDecryption(res, param, pc_0, vv_0, pc_1, vv_1);
        timings[i] = GetTime() - time;
    }
    DataProcess(mean, stdev, timings);
    cout << "Decryption time: " << mean * 1000 << " ms  RSD: " << stdev * 100 << "%\n";
}

void ODTE_TIME_TEST(int depth, int N_attribute, int msgbit, int cyctimes, bool debug)
{
    // Start: Setup Test
    Para param;
    HSS_EK ek0, ek1;
    ODTE_SETUP_TEST(param, ek0, ek1, depth, N_attribute, msgbit, cyctimes, debug);
    // End: Setup Test

    // Start: Random Input
    Vec<ZZ> X, Y, V;
    vector<std::vector<int>> delta;
    ODTE_DATA_PREPARATION(X, Y, V, delta, param);
    // End: Random Input

    // Start: Provider Input Test
    Mat<HSS_CT> Iy;
    Vec<HSS_CT> Iv;
    Mat<HSS_CT> Idelta;
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
    Mat<HSS_CT> Ix;
    ODTE_FeatureSelection2_TEST(Ix, Idelta, X, param, cyctimes, debug);
    // End: Client Input Test
    
    // Start: HSSCMP Test
    ODTE_HSSCMP_TEST(param, 0, ek0, ek1, cyctimes);
    // End: HSSCMP Test

    // Start: ClassificationGen Test
    ODTE_ClassificationGen_TEST(param, 0, ek0, ek1, cyctimes);
    // End: ClassificationGen Test

    // Start: Evaluation Test
    vec_ZZ pc0, vv0, pc1, vv1;
    ODTE_DTEvaluation_TEST(pc0, vv0, pc1, vv1, param, ek0, ek1, Ix, Iy, Iv, cyctimes);
    // End: Evaluation Test

    // Start: Decryption Test
    ZZ Predict_v;
    ODTE_Decryption_TEST(Predict_v, param, pc0, vv0, pc1, vv1,cyctimes);
    // End: Decryption Test
}
