#include "sort.h"
#include "ui.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <algorithm>

using namespace std;

// ============================================================
// FUNGSI SORTING
// ============================================================

void bubbleSortPlaylistNaik(User* u) {
    for (size_t i = 0; i < u->musiklist.size() - 1; i++)
        for (size_t j = 0; j < u->musiklist.size() - i - 1; j++)
            if (u->musiklist[j].judul > u->musiklist[j+1].judul)
                swap(u->musiklist[j], u->musiklist[j+1]);
}

void selectionSortLaguDSC(Playlist* p) {
    for (size_t i = 0; i < p->lagu.size() - 1; i++) {
        size_t maxIdx = i;
        for (size_t j = i + 1; j < p->lagu.size(); j++)
            if (p->lagu[j].tahun > p->lagu[maxIdx].tahun) maxIdx = j;
        swap(p->lagu[i], p->lagu[maxIdx]);
    }
}

void insertionSortLaguASC(Playlist* p) {
    for (size_t i = 1; i < p->lagu.size(); i++) {
        Musik key = p->lagu[i];
        size_t j  = i;
        while (j > 0 && p->lagu[j-1].judulLagu > key.judulLagu) {
            p->lagu[j] = p->lagu[j-1];
            j--;
        }
        p->lagu[j] = key;
    }
}

int dapatkanIndexPlaylist(const User* u) {
    cout << "Daftar Playlist:" << endl;
    for (size_t i = 0; i < u->musiklist.size(); i++) {
        cout << i + 1 << ". " << u->musiklist[i].judul
             << " (" << u->musiklist[i].lagu.size() << " lagu)" << endl;
    }
    printLine();
    while (true) {
        cout << "Pilih nomor playlist: ";
        int pilihan;
        try {
            cin >> pilihan;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (pilihan < 1 || pilihan > (int)u->musiklist.size())
                throw out_of_range("Nomor playlist tidak valid");
            return pilihan - 1;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
        }
    }
}

void menuSorting(User* u) {
    if (u->musiklist.empty()) {
        cout << left << setw(12) << "Info"
             << ": Anda belum memiliki Playlist untuk diurutkan" << endl;
        pause();
        return;
    }

    printHeader("MENU SORTING");
    cout << "1. Urutkan Playlist berdasarkan Judul" << endl;
    cout << "2. Urutkan Lagu dalam Playlist berdasarkan Tahun" << endl;
    cout << "3. Urutkan Lagu dalam Playlist berdasarkan Judul Lagu" << endl;
    cout << "4. Menu Utama" << endl;
    cout << "Pilih opsi\t: ";

    int opsi;
    try {
        cin >> opsi;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("angka");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } catch (const exception& e) {
        cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
        pause();
        return;
    }

    cout << endl;

    switch (opsi) {
        case 1:
            bubbleSortPlaylistNaik(u);
            cout << left << setw(12) << "Info"
                 << ": Playlist berhasil diurutkan secara ascending" << endl;
            for (size_t i = 0; i < u->musiklist.size(); i++)
                cout << i + 1 << ". " << u->musiklist[i].judul << endl;
            pause();
            break;
        case 2: {
            int idx = dapatkanIndexPlaylist(u);
            if (u->musiklist[idx].lagu.empty()) {
                cout << left << setw(12) << "Info" << ": Playlist tidak memiliki lagu" << endl;
            } else {
                selectionSortLaguDSC(&u->musiklist[idx]);
                cout << left << setw(12) << "Info"
                     << ": Lagu berhasil diurutkan secara descending berdasarkan tahun" << endl;
                Daftarlagu(u->musiklist[idx], 0);
            }
            pause();
            break;
        }
        case 3: {
            int idx = dapatkanIndexPlaylist(u);
            if (u->musiklist[idx].lagu.empty()) {
                cout << left << setw(12) << "Info" << ": Playlist tidak memiliki lagu" << endl;
            } else {
                insertionSortLaguASC(&u->musiklist[idx]);
                cout << left << setw(12) << "Info"
                     << ": Lagu berhasil diurutkan secara ascending berdasarkan judul lagu" << endl;
                Daftarlagu(u->musiklist[idx], 0);
            }
            pause();
            break;
        }
        case 4:
            return;
        default:
            cout << left << setw(12) << "Info" << ": Pilihan tidak valid" << endl;
            pause();
            break;
    }
}