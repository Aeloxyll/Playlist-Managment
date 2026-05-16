#include "crud.h"
#include "ui.h"
#include "auth.h"
#include "data.h"
#include "globals.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// ============================================================
// CRUD PLAYLIST USER
// ============================================================

void buatPlaylist(User* u) {
    printHeader("BUAT PLAYLIST");
    cout << left << setw(15) << "Nama Playlist" << ": ";
    cin.ignore();
    string judul;
    getline(cin, judul);

    string privasi;
    bool privasiValid = false;
    do {
        cout << left << setw(15) << "Privasi (Publik/Privat)" << ": ";
        cin >> privasi;
        privasi = toLowerCase(privasi);
        if (privasi == "publik" || privasi == "privat") {
            privasiValid = true;
            privasi = (privasi == "publik") ? "Publik" : "Privat";
        } else {
            cout << left << setw(12) << "Info" << ": Hanya 'Publik' atau 'Privat'" << endl;
        }
    } while (!privasiValid);

    int jumlahlagu;
    bool jumlahvalid = false;
    do {
        try {
            cout << left << setw(15) << "Jumlah lagu" << ": ";
            cin >> jumlahlagu;
            if (cin.fail() || jumlahlagu <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka positif");
            }
            jumlahvalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
            cout << '\n' << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    } while (!jumlahvalid);

    Playlist baru;
    baru.judul   = judul;
    baru.privasi = privasi;

    for (int i = 0; i < jumlahlagu; i++) {
        cout << endl << "Lagu ke-" << i + 1 << endl;
        Musik laguBaru;
        Datalagu(&laguBaru, (i == 0));
        baru.lagu.push_back(laguBaru);
    }

    u->musiklist.push_back(baru);
    cout << left << setw(12) << "Info" << ": Playlist berhasil dibuat" << endl;
    saveData();
    pause();
}

void lihatPlaylist(User* u) {
    printHeader("LIHAT PLAYLIST");

    cout << "1. Playlist Saya" << endl;
    cout << "2. Playlist Publik & Global" << endl;
    cout << "Pilih opsi: ";

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

    if (opsi == 1) {
        printHeader("PLAYLIST SAYA");

        if (u->musiklist.empty()) {
            cout << "\n📭 Anda belum memiliki playlist." << endl;
        } else {
            for (size_t i = 0; i < u->musiklist.size(); i++) {
                tampilkanKartuPlaylistUser(u->musiklist[i], "Saya", (int)i + 1);
                if (i < u->musiklist.size() - 1) cout << endl;
            }

            printSelectPrompt((int)u->musiklist.size());

            int pilihan;
            cin >> pilihan;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (pilihan >= 1 && pilihan <= (int)u->musiklist.size()) {
                int idx = pilihan - 1;
                printHeader(u->musiklist[idx].judul);
                cout << "👤 Oleh: Saya | 🔒 " << u->musiklist[idx].privasi
                     << " | 🎶 " << u->musiklist[idx].lagu.size() << " Lagu" << endl;
                printLine();
                Daftarlagu(u->musiklist[idx], 0);
            } else if (pilihan == 0) {
                cout << "\n✅ Batal memilih playlist." << endl;
            } else {
                cout << "\n❌ Nomor tidak valid." << endl;
            }
        }

    } else if (opsi == 2) {
        vector<pair<const Playlist*, string>>       poolPublik;
        vector<pair<const PlaylistGlobal*, string>> poolGlobal;

        for (const auto& user : pengguna) {
            if (user.username == u->username) continue;
            for (const auto& pl : user.musiklist) {
                if (pl.privasi == "Publik")
                    poolPublik.push_back({&pl, user.username});
            }
        }
        for (const auto& pg : playlistGlobal)
            poolGlobal.push_back({&pg, "Global"});

        int totalKartu = (int)poolPublik.size() + (int)poolGlobal.size();

        if (totalKartu == 0) {
            cout << "\n📭 Belum ada playlist publik atau global yang tersedia." << endl;
        } else {
            cout << "\n🌍 PLAYLIST PUBLIK DARI USER LAIN:" << endl;
            printLine();
            if (poolPublik.empty()) {
                cout << "(Tidak ada)" << endl;
            } else {
                for (size_t i = 0; i < poolPublik.size(); i++) {
                    tampilkanKartuPlaylistPublik(*poolPublik[i].first, poolPublik[i].second, (int)i + 1);
                    if (i < poolPublik.size() - 1) cout << endl;
                }
            }

            cout << "\n\n🌐 PLAYLIST GLOBAL (ADMIN):" << endl;
            printLine();
            if (poolGlobal.empty()) {
                cout << "(Tidak ada)" << endl;
            } else {
                for (size_t i = 0; i < poolGlobal.size(); i++) {
                    tampilkanKartuPlaylistGlobal(*poolGlobal[i].first,
                        (int)poolPublik.size() + (int)i + 1);
                    if (i < poolGlobal.size() - 1) cout << endl;
                }
            }

            printSelectPrompt(totalKartu);

            int pilihan;
            cin >> pilihan;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (pilihan >= 1 && pilihan <= (int)poolPublik.size()) {
                int idx = pilihan - 1;
                const Playlist* selected = poolPublik[idx].first;
                string pemilik = poolPublik[idx].second;
                printHeader(selected->judul);
                cout << "👤 Oleh: " << pemilik << " | 🌍 Publik | 🎶 "
                     << selected->lagu.size() << " Lagu" << endl;
                printLine();
                Daftarlagu(*selected, 0);
            } else if (pilihan > (int)poolPublik.size() && pilihan <= totalKartu) {
                int idx = pilihan - (int)poolPublik.size() - 1;
                const PlaylistGlobal* selected = poolGlobal[idx].first;
                printHeader(selected->judul);
                cout << "👑 Oleh: " << selected->dibuatOleh
                     << " | 🌐 Global | 🎶 " << selected->lagu.size() << " Lagu" << endl;
                printLine();
                DaftarLaguGlobal(*selected, 0);
            } else if (pilihan == 0) {
                cout << "\n✅ Batal memilih playlist." << endl;
            } else {
                cout << "\n❌ Nomor tidak valid." << endl;
            }
        }
    }
    pause();
}

void ubahPlaylist(User* u) {
    if (u->musiklist.empty()) {
        cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist" << endl;
        pause();
        return;
    }

    printHeader("UBAH PLAYLIST");
    cout << "Daftar Playlist:" << endl;
    for (size_t i = 0; i < u->musiklist.size(); i++) {
        cout << i + 1 << ". " << u->musiklist[i].judul
             << " (" << u->musiklist[i].privasi << ")" << endl;
    }

    int pilihPlaylist;
    bool validPlaylist = false;
    do {
        try {
            cout << "Pilih nomor playlist yang ingin diubah (1 - " << u->musiklist.size() << "): ";
            cin >> pilihPlaylist;
            if (cin.fail() || pilihPlaylist < 1 || pilihPlaylist > (int)u->musiklist.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw out_of_range("Nomor playlist tidak ditemukan");
            }
            validPlaylist = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!validPlaylist);

    int indexP = pilihPlaylist - 1;
    printLine();

    cout << left << setw(5) << " " << "1. Ubah Nama Playlist" << endl;
    cout << left << setw(5) << " " << "2. Ubah Data Lagu" << endl;
    cout << left << setw(5) << " " << "3. Tambah Lagu Baru" << endl;
    cout << left << setw(5) << " " << "4. Ubah Status Privasi (Publik/Privat)" << endl;

    int Opsi;
    bool opsivalid = false;
    do {
        try {
            cout << "Pilih opsi (1/2/3/4): ";
            cin >> Opsi;
            if (cin.fail() || Opsi < 1 || Opsi > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw out_of_range("1, 2, 3, atau 4");
            }
            opsivalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Pilihan hanya " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!opsivalid);

    if (Opsi == 1) {
        cout << left << setw(20) << "Nama Playlist Baru" << ": ";
        cin.ignore();
        getline(cin, u->musiklist[indexP].judul);
        cout << left << setw(12) << "Info" << ": Judul Playlist berhasil diubah" << endl;

    } else if (Opsi == 2) {
        if (u->musiklist[indexP].lagu.empty()) {
            cout << left << setw(12) << "Info"
                 << ": Playlist belum ada lagu. Gunakan opsi [3] untuk menambahkan lagu." << endl;
        } else {
            cout << "\nDaftar Lagu:" << endl;
            for (size_t j = 0; j < u->musiklist[indexP].lagu.size(); j++) {
                cout << j + 1 << ". " << u->musiklist[indexP].lagu[j].judulLagu
                     << " - " << u->musiklist[indexP].lagu[j].artis << endl;
            }
            int pilihLagu;
            bool validLagu = false;
            do {
                try {
                    cout << "Pilih nomor lagu yang ingin diubah (1 - "
                         << u->musiklist[indexP].lagu.size() << "): ";
                    cin >> pilihLagu;
                    if (cin.fail() || pilihLagu < 1 ||
                        pilihLagu > (int)u->musiklist[indexP].lagu.size()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw out_of_range("Nomor lagu tidak ditemukan");
                    }
                    validLagu = true;
                } catch (const exception& e) {
                    cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                    cout << endl << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
            } while (!validLagu);
            int indexL = pilihLagu - 1;
            cout << "\n--- Data Lagu Baru ---" << endl;
            Datalagu(&u->musiklist[indexP].lagu[indexL]);
            cout << left << setw(12) << "Info" << ": Data lagu berhasil diubah" << endl;
        }

    } else if (Opsi == 3) {
        char lagi = 'y';
        do {
            cout << "\n--- Tambah Lagu ke \"" << u->musiklist[indexP].judul << "\" ---" << endl;
            Musik laguBaru;
            Datalagu(&laguBaru);
            u->musiklist[indexP].lagu.push_back(laguBaru);
            cout << left << setw(12) << "Info" << ": Lagu berhasil ditambahkan!" << endl;
            cout << "Tambah lagu lagi? (y/n): ";
            cin >> lagi;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (tolower(lagi) == 'y');

    } else if (Opsi == 4) {
        u->musiklist[indexP].privasi =
            (u->musiklist[indexP].privasi == "Publik") ? "Privat" : "Publik";
        cout << left << setw(20) << "Status Baru" << ": " << u->musiklist[indexP].privasi << endl;
        cout << left << setw(12) << "Info" << ": Status privasi berhasil diperbarui" << endl;
    }

    saveData();
    pause();
}

bool hapusDataUser(int userindex) {
    printHeader("HAPUS DATA");

    cout << left << setw(5) << " " << "1. Hapus Playlist" << endl;
    cout << left << setw(5) << " " << "2. Hapus Lagu dalam Playlist" << endl;
    cout << left << setw(5) << " " << "3. Hapus Akun Sendiri" << endl;

    int Opsi;
    bool opsivalid = false;
    do {
        try {
            cout << "Pilih opsi (1/2/3): ";
            cin >> Opsi;
            if (cin.fail() || Opsi < 1 || Opsi > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw out_of_range("1, 2, atau 3");
            }
            opsivalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Pilihan hanya " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!opsivalid);

    User* u = &pengguna[userindex];

    if (Opsi == 1) {
        if (u->musiklist.empty()) {
            cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist" << endl;
            pause();
            return false;
        }
        cout << "Daftar Playlist:" << endl;
        for (size_t i = 0; i < u->musiklist.size(); i++) {
            cout << i + 1 << ". " << u->musiklist[i].judul << endl;
        }
        int pilihPlaylist;
        bool validPlaylist = false;
        do {
            try {
                cout << "Pilih nomor playlist yang ingin dihapus (1 - "
                     << u->musiklist.size() << "): ";
                cin >> pilihPlaylist;
                if (cin.fail() || pilihPlaylist < 1 ||
                    pilihPlaylist > (int)u->musiklist.size()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw out_of_range("Nomor playlist tidak ditemukan");
                }
                validPlaylist = true;
            } catch (const exception& e) {
                cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                cout << endl << "Tekan Enter untuk melanjutkan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        } while (!validPlaylist);
        int indexP = pilihPlaylist - 1;
        u->musiklist.erase(u->musiklist.begin() + indexP);
        cout << left << setw(12) << "Info" << ": Playlist berhasil dihapus" << endl;
        saveData();
        pause();
        return false;

    } else if (Opsi == 2) {
        if (u->musiklist.empty()) {
            cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist" << endl;
            pause();
            return false;
        }
        cout << "Daftar Playlist:" << endl;
        for (size_t i = 0; i < u->musiklist.size(); i++) {
            cout << i + 1 << ". " << u->musiklist[i].judul << endl;
        }
        int pilihPlaylist;
        bool validPlaylist = false;
        do {
            try {
                cout << "Pilih nomor playlist yang ingin dipilih (1 - "
                     << u->musiklist.size() << "): ";
                cin >> pilihPlaylist;
                if (cin.fail() || pilihPlaylist < 1 ||
                    pilihPlaylist > (int)u->musiklist.size()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw out_of_range("Nomor playlist tidak ditemukan");
                }
                validPlaylist = true;
            } catch (const exception& e) {
                cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                cout << endl << "Tekan Enter untuk melanjutkan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        } while (!validPlaylist);
        int indexP = pilihPlaylist - 1;

        if (u->musiklist[indexP].lagu.empty()) {
            cout << left << setw(12) << "Info" << ": Playlist tidak memiliki lagu" << endl;
        } else {
            cout << "\nDaftar Lagu di '" << u->musiklist[indexP].judul << "':" << endl;
            for (size_t j = 0; j < u->musiklist[indexP].lagu.size(); j++) {
                cout << j + 1 << ". " << u->musiklist[indexP].lagu[j].judulLagu
                     << " - " << u->musiklist[indexP].lagu[j].artis << endl;
            }
            int pilihLagu;
            bool validLagu = false;
            do {
                try {
                    cout << "Pilih nomor lagu yang ingin dihapus (1 - "
                         << u->musiklist[indexP].lagu.size() << "): ";
                    cin >> pilihLagu;
                    if (cin.fail() || pilihLagu < 1 ||
                        pilihLagu > (int)u->musiklist[indexP].lagu.size()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw out_of_range("Nomor lagu tidak ditemukan");
                    }
                    validLagu = true;
                } catch (const exception& e) {
                    cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                    cout << '\n' << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
            } while (!validLagu);
            int indexL = pilihLagu - 1;
            u->musiklist[indexP].lagu.erase(u->musiklist[indexP].lagu.begin() + indexL);
            cout << left << setw(12) << "Info" << ": Lagu berhasil dihapus" << endl;
            saveData();
        }
        pause();
        return false;

    } else { // Opsi == 3
        cout << "Apakah anda yakin ingin menghapus akun? (y/n): ";
        char konfirmasi;
        cin >> konfirmasi;
        if (tolower(konfirmasi) == 'y') {
            pengguna.erase(pengguna.begin() + userindex);
            cout << left << setw(12) << "Info" << ": Akun berhasil dihapus" << endl;
            saveData();
            pause();
            return true;
        }
    }

    pause();
    return false;
}

// ============================================================
// CRUD PLAYLIST GLOBAL (ADMIN)
// ============================================================

void buatPlaylistGlobal() {
    printHeader("BUAT PLAYLIST GLOBAL");
    cout << left << setw(15) << "Nama Playlist" << ": ";
    cin.ignore();
    string judul;
    getline(cin, judul);

    int jumlahlagu;
    bool jumlahvalid = false;
    do {
        try {
            cout << left << setw(15) << "Jumlah lagu" << ": ";
            cin >> jumlahlagu;
            if (cin.fail() || jumlahlagu <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka positif");
            }
            jumlahvalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
            cout << '\n' << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    } while (!jumlahvalid);

    PlaylistGlobal baru;
    baru.judul      = judul;
    baru.dibuatOleh = "admin";

    for (int i = 0; i < jumlahlagu; i++) {
        cout << endl << "Lagu ke-" << i + 1 << endl;
        Musik laguBaru;
        Datalagu(&laguBaru, (i == 0));
        baru.lagu.push_back(laguBaru);
    }

    playlistGlobal.push_back(baru);
    cout << left << setw(12) << "Info" << ": Playlist Global berhasil dibuat" << endl;
    saveData();
    pause();
}

void updatePlaylistGlobal() {
    if (playlistGlobal.empty()) {
        cout << left << setw(12) << "Info" << ": Belum ada Playlist Global" << endl;
        pause();
        return;
    }

    printHeader("UPDATE PLAYLIST GLOBAL");
    cout << "Daftar Playlist Global:" << endl;
    for (size_t i = 0; i < playlistGlobal.size(); i++) {
        cout << i + 1 << ". " << playlistGlobal[i].judul << endl;
    }

    int pilihPlaylist;
    bool validPlaylist = false;
    do {
        try {
            cout << "Pilih nomor playlist yang ingin diupdate (1 - "
                 << playlistGlobal.size() << "): ";
            cin >> pilihPlaylist;
            if (cin.fail() || pilihPlaylist < 1 ||
                pilihPlaylist > (int)playlistGlobal.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw out_of_range("Nomor playlist tidak ditemukan");
            }
            validPlaylist = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!validPlaylist);

    int indexP = pilihPlaylist - 1;
    printLine();

    cout << left << setw(5) << " " << "1. Ubah Nama Playlist" << endl;
    cout << left << setw(5) << " " << "2. Ubah Data Lagu" << endl;
    cout << left << setw(5) << " " << "3. Tambah Lagu Baru" << endl;

    int Opsi;
    bool opsivalid = false;
    do {
        try {
            cout << "Pilih opsi (1/2/3): ";
            cin >> Opsi;
            if (cin.fail() || Opsi < 1 || Opsi > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw out_of_range("1, 2, atau 3");
            }
            opsivalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Pilihan hanya " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!opsivalid);

    if (Opsi == 1) {
        cout << left << setw(20) << "Nama Playlist Baru" << ": ";
        cin.ignore();
        getline(cin, playlistGlobal[indexP].judul);
        cout << left << setw(12) << "Info" << ": Judul Playlist Global berhasil diubah" << endl;

    } else if (Opsi == 2) {
        if (playlistGlobal[indexP].lagu.empty()) {
            cout << left << setw(12) << "Info"
                 << ": Playlist belum ada lagu. Gunakan opsi [3] untuk menambahkan lagu." << endl;
        } else {
            cout << "\nDaftar Lagu:" << endl;
            for (size_t j = 0; j < playlistGlobal[indexP].lagu.size(); j++) {
                cout << j + 1 << ". " << playlistGlobal[indexP].lagu[j].judulLagu
                     << " - " << playlistGlobal[indexP].lagu[j].artis << endl;
            }
            int pilihLagu;
            bool validLagu = false;
            do {
                try {
                    cout << "Pilih nomor lagu yang ingin diubah (1 - "
                         << playlistGlobal[indexP].lagu.size() << "): ";
                    cin >> pilihLagu;
                    if (cin.fail() || pilihLagu < 1 ||
                        pilihLagu > (int)playlistGlobal[indexP].lagu.size()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw out_of_range("Nomor lagu tidak ditemukan");
                    }
                    validLagu = true;
                } catch (const exception& e) {
                    cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                    cout << endl << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
            } while (!validLagu);
            int indexL = pilihLagu - 1;
            cout << "\n--- Data Lagu Baru ---" << endl;
            Datalagu(&playlistGlobal[indexP].lagu[indexL]);
            cout << left << setw(12) << "Info" << ": Data lagu berhasil diubah" << endl;
        }

    } else if (Opsi == 3) {
        char lagi = 'y';
        do {
            cout << "\n--- Tambah Lagu ke \"" << playlistGlobal[indexP].judul << "\" ---" << endl;
            Musik laguBaru;
            Datalagu(&laguBaru);
            playlistGlobal[indexP].lagu.push_back(laguBaru);
            cout << left << setw(12) << "Info" << ": Lagu berhasil ditambahkan!" << endl;
            cout << "Tambah lagu lagi? (y/n): ";
            cin >> lagi;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (tolower(lagi) == 'y');
    }

    saveData();
    pause();
}

void hapusPlaylistGlobal() {
    printHeader("HAPUS PLAYLIST GLOBAL");

    if (playlistGlobal.empty()) {
        cout << "\n📭 Belum ada playlist global untuk dihapus." << endl;
        pause();
        return;
    }

    cout << "\n📋 Pilih playlist global yang ingin dihapus:\n" << endl;

    for (size_t i = 0; i < playlistGlobal.size(); i++) {
        const PlaylistGlobal& pg = playlistGlobal[i];
        cout << "┌─────────────────────────────┐" << endl;
        cout << "│  🌐  GLOBAL PLAYLIST        │" << endl;
        cout << "│                             │" << endl;
        cout << "│" << padBox("  📀 " + pg.judul)            << "│" << endl;
        cout << "│" << padBox("  👑 " + pg.dibuatOleh)       << "│" << endl;
        cout << "│" << padBox("  🎶 " + to_string(pg.lagu.size()) + " Lagu") << "│" << endl;
        cout << "│                             │" << endl;
        cout << "│" << padBox("  👉 Pilih: [" + to_string(i+1) + "]") << "│" << endl;
        cout << "└─────────────────────────────┘" << endl;
        if (i < playlistGlobal.size() - 1) cout << endl;
    }

    printSelectPrompt((int)playlistGlobal.size());

    int pilihan;
    cin >> pilihan;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (pilihan == 0) {
        cout << "\n" << C_YELLOW << "ℹ️  Penghapusan dibatalkan." << C_RESET << endl;
        pause();
        return;
    }

    if (pilihan < 1 || pilihan > (int)playlistGlobal.size()) {
        cout << "\n" << C_RED << "❌ Nomor tidak valid." << C_RESET << endl;
        pause();
        return;
    }

    int indexP = pilihan - 1;
    const PlaylistGlobal& pg = playlistGlobal[indexP];

    cout << "\n";
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  " << C_RED << C_BOLD << "🗑️  HAPUS PLAYLIST GLOBAL" << C_RESET << "  │" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  📀 " + pg.judul)                       << "│" << endl;
    cout << "│" << padBox("  👑 " + pg.dibuatOleh)                  << "│" << endl;
    cout << "│" << padBox("  🎶 " + to_string(pg.lagu.size()) + " Lagu") << "│" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  " + C_RED + "❗ Aksi ini tidak dapat" + C_RESET)  << "│" << endl;
    cout << "│" << padBox("  " + C_RED + "   dibatalkan!" + C_RESET)           << "│" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  [" + C_GREEN + "Y" + C_RESET + "] Hapus  |  [" + C_YELLOW + "N" + C_RESET + "] Batal") << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
    cout << "▶ Konfirmasi [y/n]: ";

    char konfirmasi;
    cin >> konfirmasi;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(konfirmasi) == 'y') {
        string judulHapus = pg.judul;
        playlistGlobal.erase(playlistGlobal.begin() + indexP);
        saveData();
        cout << "\n" << C_GREEN << "✅ Playlist global \"" << judulHapus
             << "\" berhasil dihapus!" << C_RESET << endl;
    } else {
        cout << "\n" << C_YELLOW << "ℹ️  Penghapusan dibatalkan." << C_RESET << endl;
    }

    pause();
}