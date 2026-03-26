#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <vector>

#include "NTL/ZZ.h"
#include "NTL/ZZX.h"

using std::array;
using std::begin;
using std::copy;
using std::cout;
using std::end;
using std::endl;
using std::vector;
using namespace NTL;

void DataProcess(double &mean, double &stdev, const vector<double> &timings);
ZZ PRF_ZZ(const int &prfkey, const ZZ &mmod);
void GenerateMatrix(int m, int n, vector<vector<int>> &delta);
