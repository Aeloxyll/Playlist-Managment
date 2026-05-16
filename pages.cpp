#include "pages.h"
#include "ui.h"
#include "auth.h"
#include "crud.h"
#include "sort.h"
#include "search.h"
#include "data.h"
#include "globals.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// ============================================================
// HALAMAN MASUK
// ============================================================

bool halamanMasuk(int& userindex) {
    int bataslogin = 0;

    while (true) {
        clearScreen();

        cout << "\n";
        boxTop();
        boxLineCenter("🎵  MUSIC PLAYLIST MANAGER  🎵");
        boxSep();
        boxLine();
        boxLine("   👋  Selamat Datang!");
        boxLine();
        boxSep();
        boxLine("   [1]   📝   Register");
        boxLine("   [2]   🔐   Login");
        boxLine("   [3]   🚪   Keluar");
        boxLine();
        boxBot();
        cout << "\n  ▶ Pilih opsi [1-3]: ";

        int opsi;
        try {
            cin >> opsi;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
            continue;
        }

        cout << endl << endl;

        if (opsi == 1) {
            Register(pengguna);
        } else if (opsi == 2) {
            int status = Login(pengguna, userindex, bataslogin);
            if (status ==  1) return true;
            if (status == -1) return false;
        } else if (opsi == 3) {
            cout << left << setw(12) << "Info" << ": Keluar dari program" << endl;
            pause();
            return false;
        } else {
            cout << left << setw(12) << "Info"
                 << ": Pilihlah berdasarkan opsi yang valid" << endl;
            pause();
        }
    }
}

// ============================================================
// HALAMAN UTAMA ADMIN
// ============================================================

void halamanUtamaAdmin(int userindex) {
    while (true) {
        clearScreen();

        int totalUser     = (int)pengguna.size();
        int totalPlaylist = (int)playlistGlobal.size();
        int totalLagu     = 0;
        for (const auto& pg : playlistGlobal)   totalLagu     += (int)pg.lagu.size();
        for (const auto& user : pengguna) {
            totalPlaylist += (int)user.musiklist.size();
            for (const auto& pl : user.musiklist) totalLagu += (int)pl.lagu.size();
        }

        cout << "\n";
        boxTop();
        boxLine("  👑  ADMIN MODE");
        boxLine("  🌐  DASHBOARD ADMIN");
        boxSep();
        boxLine("  👥  " + to_string(totalUser) + " User  •  "
                + to_string(totalPlaylist) + " Playlist");
        boxLine("  🎶  " + to_string(totalLagu) + " Lagu Total");
        boxSep();
        boxLine("  [1]   📊   Statistik Data");
        boxLine("  [2]   📋   Semua Playlist");
        boxLine("  [3]   🌐   Buat Playlist Global");
        boxLine("  [4]   📝   Update Playlist Global");
        boxLine("  [5]   🗑️    Hapus Playlist Global");
        boxLine("  [6]   🚪   Keluar");
        boxSep();
        boxLine("  🔔  Aksi admin berlaku untuk semua user");
        boxBot();
        cout << "\n  ▶ Pilih menu [1-6]: ";

        int opsi;
        try {
            cin >> opsi;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
            continue;
        }

        cout << endl << endl;

        if      (opsi == 1) { tampilkanStatistikAdmin(); pause(); }
        else if (opsi == 2) { tampilkanSemuaPlaylistDanGlobalAdmin(); pause(); }
        else if (opsi == 3) { buatPlaylistGlobal(); }
        else if (opsi == 4) { updatePlaylistGlobal(); }
        else if (opsi == 5) { hapusPlaylistGlobal(); }
        else if (opsi == 6) {
            cout << left << setw(12) << "Info" << ": Kembali ke halaman masuk" << endl;
            pause();
            return;
        } else {
            cout << left << setw(12) << "Info" << ": Pilihan tidak valid" << endl;
            pause();
        }
    }
}

// ============================================================
// HALAMAN UTAMA USER
// ============================================================

void halamanUtamaUser(int userindex) {
    while (true) {
        clearScreen();

        const User& u = pengguna[userindex];
        int totalPL   = (int)u.musiklist.size();
        int totalLagu = 0;
        for (const auto& pl : u.musiklist) totalLagu += (int)pl.lagu.size();

        string nama = u.username.length() > 20
                      ? u.username.substr(0, 17) + "..."
                      : u.username;

        cout << "\n";
        boxTop();
        boxLine("  👤  Halo, " + nama + "!");
        boxLine("  🎵  HALAMAN UTAMA");
        boxSep();
        boxLine("  📊  " + to_string(totalPL) + " Playlist  •  "
                + to_string(totalLagu) + " Lagu");
        boxSep();
        boxLine("  [1]   📁   Buat Playlist");
        boxLine("  [2]   📋   Lihat Playlist");
        boxLine("  [3]   📝   Ubah Playlist");
        boxLine("  [4]   🗑   Hapus Data");
        boxLine("  [5]   🔀   Urutkan Data");
        boxLine("  [6]   🔍   Cari Data");
        boxLine("  [7]   🚪   Keluar");
        boxLine();
        boxBot();
        cout << "\n  ▶ Pilih menu [1-7]: ";

        int opsi;
        try {
            cin >> opsi;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
            continue;
        }

        cout << endl << endl;

        if      (opsi == 1) { buatPlaylist(&pengguna[userindex]); }
        else if (opsi == 2) { lihatPlaylist(&pengguna[userindex]); }
        else if (opsi == 3) { ubahPlaylist(&pengguna[userindex]); }
        else if (opsi == 4) {
            bool accountDeleted = hapusDataUser(userindex);
            if (accountDeleted) return;
        }
        else if (opsi == 5) { menuSorting(&pengguna[userindex]); }
        else if (opsi == 6) { menuSearching(userindex); }
        else if (opsi == 7) {
            cout << left << setw(12) << "Info" << ": Kembali ke halaman masuk" << endl;
            pause();
            return;
        } else {
            cout << left << setw(12) << "Info" << ": Pilihan tidak valid" << endl;
            pause();
        }
    }
}