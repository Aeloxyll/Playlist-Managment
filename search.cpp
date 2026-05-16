#include "search.h"
#include "ui.h"
#include "globals.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <stdexcept>

using namespace std;

// ============================================================
// FUNGSI SEARCHING
// ============================================================

bool linearSearchArtis(const vector<Musik>& arr, const string& target, size_t& idx) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i].artis == target) { idx = i; return true; }
    }
    return false;
}

bool binarySearchJudul(const vector<Musik>& arr, const string& target, int& idx) {
    int left  = 0;
    int right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if      (arr[mid].judulLagu == target) { idx = mid; return true; }
        else if (arr[mid].judulLagu < target)    left  = mid + 1;
        else                                     right = mid - 1;
    }
    return false;
}

void kumpulkanPoolLagu(int userindex, vector<HasilCari>& pool) {
    pool.clear();
    const User* u = &pengguna[userindex];

    for (const auto& pl : u->musiklist)
        for (const auto& lagu : pl.lagu)
            pool.push_back({lagu, pl.judul, "Saya"});

    for (const auto& user : pengguna) {
        if (&user == u) continue;
        for (const auto& pl : user.musiklist)
            if (pl.privasi == "Publik")
                for (const auto& lagu : pl.lagu)
                    pool.push_back({lagu, pl.judul, user.username});
    }

    for (const auto& pg : playlistGlobal)
        for (const auto& lagu : pg.lagu)
            pool.push_back({lagu, pg.judul, "Global"});
}

void cetakHasilCari(const HasilCari& h) {
    cout << left << setw(12) << "Playlist" << ": " << h.namaPlaylist
         << " [" << h.pemilik << "]" << endl;
    cout << left << setw(12) << "Judul"    << ": " << h.lagu.judulLagu << endl;
    cout << left << setw(12) << "Artis"    << ": " << h.lagu.artis     << endl;
    cout << left << setw(12) << "Genre"    << ": " << h.lagu.genre     << endl;
    cout << left << setw(12) << "Tahun"    << ": " << h.lagu.tahun     << endl;
    printLine();
}

void menuSearching(int userindex) {
    vector<HasilCari> pool;
    kumpulkanPoolLagu(userindex, pool);

    if (pool.empty()) {
        cout << left << setw(12) << "Info"
             << ": Tidak ada lagu yang dapat dicari saat ini" << endl;
        pause();
        return;
    }

    printHeader("MENU SEARCHING");
    cout << "Cari berdasarkan:" << endl;
    cout << "1. Nama Artis" << endl;
    cout << "2. Judul Lagu" << endl;
    cout << "Pilih metode\t: ";

    int metode;
    try {
        cin >> metode;
        if (cin.fail() || metode < 1 || metode > 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw out_of_range("1 atau 2");
        }
    } catch (const exception& e) {
        cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
        pause();
        return;
    }

    if (metode == 1) {
        string artisCari;
        cout << "Masukkan nama artis\t: ";
        cin.ignore();
        getline(cin, artisCari);

        cout << "\n---| Hasil Pencarian Artis: " << artisCari << " |---" << endl;
        bool foundAny = false;
        for (const auto& h : pool) {
            if (h.lagu.artis == artisCari) { cetakHasilCari(h); foundAny = true; }
        }
        if (!foundAny)
            cout << left << setw(12) << "Info"
                 << ": Artis '" << artisCari << "' tidak ditemukan" << endl;

    } else {
        string judulCari;
        cout << "Masukkan judul lagu\t: ";
        cin.ignore();
        getline(cin, judulCari);

        vector<HasilCari> sortedPool = pool;
        sort(sortedPool.begin(), sortedPool.end(),
            [](const HasilCari& a, const HasilCari& b) {
                return a.lagu.judulLagu < b.lagu.judulLagu;
            });

        vector<Musik> lagusorted;
        for (const auto& h : sortedPool) lagusorted.push_back(h.lagu);

        int idx = -1;
        bool found = binarySearchJudul(lagusorted, judulCari, idx);

        cout << "\n---| Hasil Pencarian Judul: " << judulCari << " |---" << endl;
        if (found) {
            int start = idx;
            while (start > 0 && sortedPool[start-1].lagu.judulLagu == judulCari) start--;
            int end = idx;
            while (end < (int)sortedPool.size()-1 &&
                   sortedPool[end+1].lagu.judulLagu == judulCari) end++;
            for (int i = start; i <= end; i++) cetakHasilCari(sortedPool[i]);
        } else {
            cout << left << setw(12) << "Info"
                 << ": Judul '" << judulCari << "' tidak ditemukan" << endl;
        }
    }
    pause();
}