#include "globals.h"
#include "data.h"
#include "pages.h"
#include <clocale>

// ============================================================
// MAIN
// ============================================================

int main() {
    #ifdef _WIN32
        system("chcp 65001 >nul");
        setlocale(LC_ALL, "en_US.UTF-8");
    #endif

    loadData();

    // Pastikan akun admin selalu ada
    bool adminExists = false;
    for (const auto& user : pengguna) {
        if (user.username == "admin" && user.role == "admin") {
            adminExists = true;
            break;
        }
    }
    if (!adminExists) {
        User admin;
        admin.username = "admin";
        admin.password = "123";
        admin.role     = "admin";
        pengguna.push_back(admin);
    }

    while (true) {
        int userindex = -1;
        if (!halamanMasuk(userindex)) { saveData(); break; }

        if (pengguna[userindex].role == "admin") {
            halamanUtamaAdmin(userindex);
        } else {
            halamanUtamaUser(userindex);
        }
    }

    return 0;
}