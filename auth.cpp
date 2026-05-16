#include "auth.h"
#include "ui.h"
#include "data.h"
#include "globals.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <stdexcept>

using namespace std;

// ============================================================
// FUNGSI UTILITAS INPUT
// ============================================================

bool usntersedia(const vector<User>& pengguna, const string& targetUsername) {
    for (const auto& user : pengguna) {
        if (user.username == targetUsername) return false;
    }
    return true;
}

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool validasigenre(const string& genre) {
    return genre == "klasik" || genre == "pop" || genre == "rock" ||
           genre == "jazz"   || genre == "hip hop";
}

void Datalagu(Musik* laguBaru, bool needIgnore) {
    if (needIgnore) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << left << setw(12) << "Judul lagu" << ": ";
    getline(cin, laguBaru->judulLagu);
    cout << left << setw(12) << "Artis" << ": ";
    getline(cin, laguBaru->artis);

    string Genre;
    bool genrevalid = false;
    do {
        try {
            cout << left << setw(12) << "Genre" << ": ";
            getline(cin, Genre);
            Genre = toLowerCase(Genre);
            if (!validasigenre(Genre))
                throw invalid_argument("klasik, pop, rock, jazz, atau hip hop");
            genrevalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    } while (!genrevalid);
    laguBaru->genre = Genre;

    int Tahun;
    bool tahunvalid = false;
    do {
        try {
            cout << left << setw(12) << "Tahun" << ": ";
            cin >> Tahun;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            if (Tahun < 1900 || Tahun > 2026)
                throw out_of_range("1900 hingga 2026");
            tahunvalid = true;
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": Tahun hanya dari rentang " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    } while (!tahunvalid);
    laguBaru->tahun = Tahun;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
// FUNGSI AUTENTIKASI
// ============================================================

void Register(vector<User>& pengguna) {
    printHeader("REGISTER");
    string username, password;
    cout << left << setw(12) << "Username" << ": ";
    cin >> username;

    if (!usntersedia(pengguna, username)) {
        cout << left << setw(12) << "Info" << ": Username tidak tersedia" << endl;
    } else {
        User newUser;
        newUser.username = username;
        newUser.role     = "user";
        cout << left << setw(12) << "Password" << ": ";
        cin >> password;
        newUser.password = password;
        pengguna.push_back(newUser);
        cout << left << setw(12) << "Info" << ": Akun berhasil dibuat" << endl;
        saveData();
    }
    pause();
}

int Login(const vector<User>& pengguna, int& userindex, int& bataslogin) {
    printHeader("LOGIN");
    string username, password;
    while (true) {
        cout << left << setw(12) << "Username" << ": ";
        cin >> username;
        cout << left << setw(12) << "Password" << ": ";
        cin >> password;

        for (size_t i = 0; i < pengguna.size(); i++) {
            if (pengguna[i].username == username &&
                pengguna[i].password == password) {
                cout << left << setw(12) << "Info" << ": Login berhasil" << endl;
                userindex  = (int)i;
                bataslogin = 0;
                pause();
                return 1;
            }
        }

        bataslogin++;
        if (bataslogin < 3) {
            cout << left << setw(12) << "Info" << ": Username atau Password salah\n" << endl;
        } else {
            cout << left << setw(12) << "Info"
                 << ": Gagal login 3 kali, keluar dari program\n" << endl;
            pause();
            return -1;
        }
    }
}
