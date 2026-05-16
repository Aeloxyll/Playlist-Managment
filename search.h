#pragma once
#include <vector>
#include "types.h"

using namespace std;

// ============================================================
// FUNGSI SEARCHING
// ============================================================

bool linearSearchArtis  (const vector<Musik>& arr, const string& target, size_t& idx);
bool binarySearchJudul  (const vector<Musik>& arr, const string& target, int& idx);
void kumpulkanPoolLagu  (int userindex, vector<HasilCari>& pool);
void cetakHasilCari     (const HasilCari& h);
void menuSearching      (int userindex);