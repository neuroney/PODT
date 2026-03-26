#include "helper.h"

void DataProcess(double &mean, double &stdev, const vector<double> &timings)
{
    if (timings.empty())
    {
        mean = 0.0;
        stdev = 0.0;
        return;
    }

    double sum = 0.0;
    for (double timing : timings)
    {
        sum += timing;
    }
    mean = sum / static_cast<double>(timings.size());

    double variance_sum = 0.0;
    for (double timing : timings)
    {
        const double delta = mean - timing;
        variance_sum += delta * delta;
    }

    stdev = std::sqrt(variance_sum / static_cast<double>(timings.size()));
    if (mean != 0.0)
    {
        stdev /= mean;
    }
    else
    {
        stdev = 0.0;
    }
}

ZZ PRF_ZZ(const int &prfkey, const ZZ &mmod)
{
    ZZ res;
    SetSeed(ZZ(prfkey));
    RandomBnd(res, mmod);
    return res;
}

void GenerateMatrix(int m, int n, vector<vector<int>> &delta)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, n - 1);

    for (int i = 0; i < m; ++i)
    {
        vector<int> row(n, 0);
        const int randomIndex = distribution(generator);
        row[randomIndex] = 1;
        delta[i] = row;
    }
}
