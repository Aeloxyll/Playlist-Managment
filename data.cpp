#include "data.h"
#include "globals.h"
#include <fstream>
#include <sstream>

using namespace std;

// ============================================================
// FUNGSI SAVE DATA
// ============================================================

void saveData() {
    ofstream fileUser("data_users.txt");
    if (fileUser.is_open()) {
        for (const auto& user : pengguna) {
            fileUser << user.username << "|"
                     << user.password << "|"
                     << user.role     << "|"
                     << user.musiklist.size() << "\n";
        }
        fileUser.close();
    }

    ofstream filePlaylist("data_playlists.txt");
    if (filePlaylist.is_open()) {
        for (const auto& user : pengguna) {
            for (const auto& pl : user.musiklist) {
                filePlaylist << user.username << "|"
                             << pl.judul      << "|"
                             << pl.lagu.size()<< "|"
                             << pl.privasi    << "\n";
                for (const auto& lagu : pl.lagu) {
                    filePlaylist << "  "
                                 << lagu.judulLagu << "|"
                                 << lagu.artis     << "|"
                                 << lagu.genre     << "|"
                                 << lagu.tahun     << "\n";
                }
            }
        }
        filePlaylist.close();
    }

    ofstream fileGlobal("data_global.txt");
    if (fileGlobal.is_open()) {
        for (const auto& pg : playlistGlobal) {
            fileGlobal << pg.judul      << "|"
                       << pg.lagu.size()<< "|"
                       << pg.dibuatOleh << "\n";
            for (const auto& lagu : pg.lagu) {
                fileGlobal << "  "
                           << lagu.judulLagu << "|"
                           << lagu.artis     << "|"
                           << lagu.genre     << "|"
                           << lagu.tahun     << "\n";
            }
        }
        fileGlobal.close();
    }
}

// ============================================================
// FUNGSI LOAD DATA
// ============================================================

void loadData() {
    pengguna.clear();
    playlistGlobal.clear();

    // 1. Load user
    {
        ifstream fileUser("data_users.txt");
        if (fileUser.is_open()) {
            string line;
            while (getline(fileUser, line)) {
                if (line.empty()) continue;
                stringstream ss(line);
                string username, password, role, jmlStr;
                getline(ss, username, '|');
                getline(ss, password, '|');
                getline(ss, role,     '|');
                getline(ss, jmlStr,   '|');
                if (username.empty()) continue;
                User newUser;
                newUser.username = username;
                newUser.password = password;
                newUser.role     = role;
                pengguna.push_back(newUser);
            }
        }
    }

    // 2. Load playlist user
    {
        ifstream filePlaylist("data_playlists.txt");
        if (filePlaylist.is_open()) {
            string line;
            while (getline(filePlaylist, line)) {
                if (line.empty() || line[0] == ' ') continue;
                stringstream ss(line);
                string username, judul, jmlStr, privasi;
                getline(ss, username, '|');
                getline(ss, judul,    '|');
                getline(ss, jmlStr,   '|');
                getline(ss, privasi,  '|');
                if (username.empty() || judul.empty()) continue;

                int jmlLagu = 0;
                try { jmlLagu = stoi(jmlStr); } catch (...) { jmlLagu = 0; }

                Playlist newPl;
                newPl.judul   = judul;
                newPl.privasi = privasi;

                for (int l = 0; l < jmlLagu; l++) {
                    string songLine;
                    if (!getline(filePlaylist, songLine)) break;
                    if (songLine.size() < 2 || songLine[0] != ' ') continue;
                    stringstream ls(songLine.substr(2));
                    string jl, a, g, tStr;
                    getline(ls, jl,   '|');
                    getline(ls, a,    '|');
                    getline(ls, g,    '|');
                    getline(ls, tStr, '|');
                    Musik laguBaru;
                    laguBaru.judulLagu = jl;
                    laguBaru.artis     = a;
                    laguBaru.genre     = g;
                    try { laguBaru.tahun = stoi(tStr); } catch (...) { laguBaru.tahun = 2024; }
                    newPl.lagu.push_back(laguBaru);
                }

                for (auto& user : pengguna) {
                    if (user.username == username) {
                        user.musiklist.push_back(newPl);
                        break;
                    }
                }
            }
        }
    }

    // 3. Load playlist global
    {
        ifstream fileGlobal("data_global.txt");
        if (fileGlobal.is_open()) {
            string line;
            while (getline(fileGlobal, line)) {
                if (line.empty() || line[0] == ' ') continue;
                stringstream ss(line);
                string judul, jmlStr, dibuatOleh;
                getline(ss, judul,      '|');
                getline(ss, jmlStr,     '|');
                getline(ss, dibuatOleh, '|');
                if (judul.empty()) continue;

                int jmlLagu = 0;
                try { jmlLagu = stoi(jmlStr); } catch (...) { jmlLagu = 0; }

                PlaylistGlobal newPg;
                newPg.judul      = judul;
                newPg.dibuatOleh = dibuatOleh;

                for (int l = 0; l < jmlLagu; l++) {
                    string songLine;
                    if (!getline(fileGlobal, songLine)) break;
                    if (songLine.size() < 2 || songLine[0] != ' ') continue;
                    stringstream ls(songLine.substr(2));
                    string jl, a, g, tStr;
                    getline(ls, jl,   '|');
                    getline(ls, a,    '|');
                    getline(ls, g,    '|');
                    getline(ls, tStr, '|');
                    Musik laguBaru;
                    laguBaru.judulLagu = jl;
                    laguBaru.artis     = a;
                    laguBaru.genre     = g;
                    try { laguBaru.tahun = stoi(tStr); } catch (...) { laguBaru.tahun = 2024; }
                    newPg.lagu.push_back(laguBaru);
                }

                playlistGlobal.push_back(newPg);
            }
        }
    }
}