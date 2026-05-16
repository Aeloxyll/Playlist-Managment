#pragma once
#include <string>
#include <vector>
#include "types.h"

using namespace std;

// ============================================================
// FUNGSI UTILITAS INPUT
// ============================================================

bool   usntersedia   (const vector<User>& pengguna, const string& targetUsername);
string toLowerCase   (string str);
bool   validasigenre (const string& genre);
void   Datalagu      (Musik* laguBaru, bool needIgnore = true);

// ============================================================
// FUNGSI AUTENTIKASI
// ============================================================

void Register (vector<User>& pengguna);
int  Login    (const vector<User>& pengguna, int& userindex, int& bataslogin);