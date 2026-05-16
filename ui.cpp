#include "ui.h"
#include "globals.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// ============================================================
// FUNGSI UTILITAS UI
// ============================================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
        system("chcp 65001 >nul");
    #else
        system("clear");
    #endif
}

void printLine() {
    cout << LINE << endl;
}

void printHeader(const string& title) {
    int innerWidth = HEAD.length() - 4;
    int len = title.length();
    if (len > innerWidth) len = innerWidth;
    int leftPad  = (innerWidth - len) / 2;
    int rightPad = innerWidth - len - leftPad;
    cout << HEAD << endl;
    cout << "= " << string(leftPad, ' ')
         << title.substr(0, len)
         << string(rightPad, ' ') << " =" << endl;
    cout << HEAD << endl;
}

// ============================================================
// BOX DRAWING HELPERS
// ============================================================

string repeatStr(const string& s, int n) {
    string result;
    result.reserve(s.size() * n);
    for (int i = 0; i < n; i++) result += s;
    return result;
}

int visualWidth(const string& s) {
    int width = 0;
    size_t i  = 0;
    while (i < s.length()) {
        unsigned char c = (unsigned char)s[i];
        if (c == 0x1B && i + 1 < s.length() && s[i + 1] == '[') {
            i += 2;
            while (i < s.length() && s[i] != 'm') i++;
            i++;
            continue;
        }
        if      (c >= 0xF0) { width += 2; i += 4; }
        else if (c >= 0xE0) { width += 1; i += 3; }
        else if (c >= 0xC0) { width += 1; i += 2; }
        else                { width += 1; i++;     }
    }
    return width;
}

string padLine(const string& s, int targetWidth) {
    int vw = visualWidth(s);
    if (vw <= targetWidth) return s + string(targetWidth - vw, ' ');

    string truncated;
    int accum = 0;
    int limit = targetWidth - 3;
    size_t i = 0;

    while (i < s.length() && accum < limit) {
        unsigned char c = (unsigned char)s[i];
        if (c == 0x1B && i + 1 < s.length() && s[i+1] == '[') {
            size_t j = i + 2;
            while (j < s.length() && s[j] != 'm') j++;
            truncated += s.substr(i, j - i + 1);
            i = j + 1;
            continue;
        }
        int cw; size_t bytes;
        if      (c >= 0xF0) { cw = 2; bytes = 4; }
        else if (c >= 0xE0) { cw = 1; bytes = 3; }
        else if (c >= 0xC0) { cw = 1; bytes = 2; }
        else                { cw = 1; bytes = 1; }
        if (accum + cw > limit) break;
        truncated += s.substr(i, bytes);
        accum += cw;
        i += bytes;
    }

    truncated += "...";
    accum += 3;
    return truncated + string(max(0, targetWidth - accum), ' ');
}

string padBox(const string& content, int targetVW) {
    int vw = visualWidth(content);

    if (vw > targetVW) {
        int limit = targetVW - 3;
        string truncated;
        int accum = 0;
        size_t i = 0;

        while (i < content.length() && accum < limit) {
            unsigned char c = (unsigned char)content[i];
            if (c == 0x1B && i + 1 < content.length() && content[i+1] == '[') {
                size_t j = i + 2;
                while (j < content.length() && content[j] != 'm') j++;
                truncated += content.substr(i, j - i + 1);
                i = j + 1;
                continue;
            }
            int cw; size_t bytes;
            if      (c >= 0xF0) { cw = 2; bytes = 4; }
            else if (c >= 0xE0) { cw = 1; bytes = 3; }
            else if (c >= 0xC0) { cw = 1; bytes = 2; }
            else                { cw = 1; bytes = 1; }
            if (accum + cw > limit) break;
            truncated += content.substr(i, bytes);
            accum += cw;
            i += bytes;
        }

        truncated += "...";
        accum += 3;
        return truncated + string(max(0, targetVW - accum), ' ');
    }

    return content + string(max(0, targetVW - vw), ' ');
}

void boxTop() { cout << "╔" << repeatStr("═", BOX_W + 2) << "╗" << endl; }
void boxBot() { cout << "╚" << repeatStr("═", BOX_W + 2) << "╝" << endl; }
void boxSep() { cout << "╠" << repeatStr("═", BOX_W + 2) << "╣" << endl; }

void boxLine(const string& s) {
    cout << "║ " << padLine(s, BOX_W) << " ║" << endl;
}

void boxLineCenter(const string& s) {
    int vw = visualWidth(s);
    int lp = max(0, (BOX_W - vw) / 2);
    int rp = max(0, BOX_W - vw - lp);
    cout << "║ " << string(lp, ' ') << s << string(rp, ' ') << " ║" << endl;
}

void printSelectPrompt(int maxN) {
    string inner = "  🎯  Pilih: [1-" + to_string(maxN) + "]  |  0 = Batal";
    int vw = visualWidth(inner);
    cout << endl;
    cout << "  ╭" << repeatStr("─", vw + 2) << "╮" << endl;
    cout << "  │ " << inner << " │" << endl;
    cout << "  ╰" << repeatStr("─", vw + 2) << "╯" << endl;
    cout << "  ▶ Pilihan: ";
}

void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(1, '\n');
    cin.get();
    clearScreen();
}

// ============================================================
// FUNGSI TAMPILKAN KARTU PLAYLIST
// ============================================================

void tampilkanKartuPlaylistUser(const Playlist& pl, const string& pemilik, int nomor) {
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  🎵  PLAYLIST               │" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  📀 " + pl.judul) << "│" << endl;
    cout << "│" << padBox("  👤 " + pemilik)  << "│" << endl;
    string icon = (pl.privasi == "Publik") ? "🌍" : "🔒";
    cout << "│" << padBox("  " + icon + "  " + pl.privasi) << "│" << endl;
    cout << "│" << padBox("  🎶 " + to_string(pl.lagu.size()) + " Lagu") << "│" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  👉 Pilih: [" + to_string(nomor) + "]") << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
}

void tampilkanKartuPlaylistPublik(const Playlist& pl, const string& pemilik, int nomor) {
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  🌍  PUBLIC PLAYLIST        │" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  📀 " + pl.judul) << "│" << endl;
    cout << "│" << padBox("  👤 " + pemilik)  << "│" << endl;
    cout << "│" << padBox("  🌍  Publik")     << "│" << endl;
    cout << "│" << padBox("  🎶 " + to_string(pl.lagu.size()) + " Lagu") << "│" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  👉 Pilih: [" + to_string(nomor) + "]") << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
}

void tampilkanKartuPlaylistGlobal(const PlaylistGlobal& pg, int nomor) {
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  🌐  GLOBAL PLAYLIST        │" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  📀 " + pg.judul)       << "│" << endl;
    cout << "│" << padBox("  👑 " + pg.dibuatOleh)  << "│" << endl;
    cout << "│" << padBox("  🌍  Publik")            << "│" << endl;
    cout << "│" << padBox("  🎶 " + to_string(pg.lagu.size()) + " Lagu") << "│" << endl;
    cout << "│                             │" << endl;
    cout << "│" << padBox("  👉 Pilih: [" + to_string(nomor) + "]") << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
}

// ============================================================
// FUNGSI TAMPILKAN DAFTAR LAGU (REKURSIF)
// ============================================================

void Daftarlagu(const Playlist& pl, size_t indexLagu) {
    if (indexLagu >= pl.lagu.size()) return;
    const Musik& ptrLagu = pl.lagu[indexLagu];
    cout << "Lagu ke-" << indexLagu + 1 << endl;
    cout << left << setw(10) << "Judul" << ": " << ptrLagu.judulLagu << endl;
    cout << left << setw(10) << "Artis" << ": " << ptrLagu.artis     << endl;
    cout << left << setw(10) << "Genre" << ": " << ptrLagu.genre     << endl;
    cout << left << setw(10) << "Tahun" << ": " << ptrLagu.tahun     << endl;
    printLine();
    Daftarlagu(pl, indexLagu + 1);
}

void DaftarLaguGlobal(const PlaylistGlobal& pg, size_t indexLagu) {
    if (indexLagu >= pg.lagu.size()) return;
    const Musik& ptrLagu = pg.lagu[indexLagu];
    cout << "Lagu ke-" << indexLagu + 1 << endl;
    cout << left << setw(10) << "Judul" << ": " << ptrLagu.judulLagu << endl;
    cout << left << setw(10) << "Artis" << ": " << ptrLagu.artis     << endl;
    cout << left << setw(10) << "Genre" << ": " << ptrLagu.genre     << endl;
    cout << left << setw(10) << "Tahun" << ": " << ptrLagu.tahun     << endl;
    printLine();
    DaftarLaguGlobal(pg, indexLagu + 1);
}

// ============================================================
// FUNGSI TAMPILKAN PLAYLIST
// ============================================================

void tampilkanPlaylistSaya(const User& u) {
    if (u.musiklist.empty()) {
        cout << "Anda belum memiliki playlist." << endl;
    } else {
        for (const auto& pl : u.musiklist) {
            cout << HEAD << endl;
            cout << left << setw(15) << "Nama Playlist" << ": " << pl.judul        << endl;
            cout << left << setw(15) << "Jumlah Lagu"   << ": " << pl.lagu.size()  << " Lagu" << endl;
            cout << left << setw(15) << "Privasi"       << ": " << pl.privasi      << endl;
            cout << HEAD << endl;
            Daftarlagu(pl, 0);
        }
    }
}

void tampilkanSemuaPlaylistPublik() {
    bool adaPublik = false;
    printHeader("SELURUH PLAYLIST PUBLIK");
    for (const auto& user : pengguna) {
        bool userPunyaPublik = false;
        for (const auto& pl : user.musiklist) {
            if (pl.privasi == "Publik") {
                if (!userPunyaPublik) {
                    cout << ">> Playlist milik user [" << user.username << "] <<" << endl;
                    userPunyaPublik = true;
                    adaPublik = true;
                }
                cout << HEAD << endl;
                cout << left << setw(15) << "Nama Playlist" << ": " << pl.judul       << endl;
                cout << left << setw(15) << "Jumlah Lagu"   << ": " << pl.lagu.size() << " Lagu" << endl;
                cout << HEAD << endl;
                Daftarlagu(pl, 0);
            }
        }
        if (userPunyaPublik) cout << endl;
    }
    if (!adaPublik) cout << "Tidak ada playlist publik yang tersedia saat ini." << endl;
}

void tampilkanPlaylistGlobal() {
    if (playlistGlobal.empty()) {
        cout << "Belum ada playlist global." << endl;
    } else {
        for (const auto& pg : playlistGlobal) {
            cout << HEAD << endl;
            cout << left << setw(15) << "Nama Playlist" << ": " << pg.judul        << endl;
            cout << left << setw(15) << "Jumlah Lagu"   << ": " << pg.lagu.size()  << " Lagu" << endl;
            cout << left << setw(15) << "Dibuat Oleh"   << ": " << pg.dibuatOleh   << endl;
            cout << HEAD << endl;
            DaftarLaguGlobal(pg, 0);
        }
    }
}

void tampilkanStatistikAdmin() {
    int totalUser     = (int)pengguna.size();
    int totalPlaylist = 0;
    int totalLagu     = 0;
    for (const auto& user : pengguna) {
        totalPlaylist += (int)user.musiklist.size();
        for (const auto& pl : user.musiklist) totalLagu += (int)pl.lagu.size();
    }
    totalPlaylist += (int)playlistGlobal.size();
    for (const auto& pg : playlistGlobal) totalLagu += (int)pg.lagu.size();

    printHeader("STATISTIK DATA");
    cout << left << setw(20) << "Total User"     << ": " << totalUser     << endl;
    cout << left << setw(20) << "Total Playlist" << ": " << totalPlaylist
         << " (Termasuk " << playlistGlobal.size() << " Playlist Global)" << endl;
    cout << left << setw(20) << "Total Lagu"     << ": " << totalLagu     << endl;
}

void tampilkanSemuaPlaylistDanGlobalAdmin() {
    printHeader("SEMUA PLAYLIST");

    vector<PlaylistEntry> daftar;

    for (int u = 0; u < (int)pengguna.size(); u++) {
        for (int p = 0; p < (int)pengguna[u].musiklist.size(); p++) {
            PlaylistEntry e;
            e.tipe        = 0;
            e.userIdx     = u;
            e.playlistIdx = p;
            e.globalIdx   = -1;
            e.judul       = pengguna[u].musiklist[p].judul;
            e.pemilik     = pengguna[u].username;
            daftar.push_back(e);
        }
    }
    for (int g = 0; g < (int)playlistGlobal.size(); g++) {
        PlaylistEntry e;
        e.tipe        = 1;
        e.userIdx     = -1;
        e.playlistIdx = -1;
        e.globalIdx   = g;
        e.judul       = playlistGlobal[g].judul;
        e.pemilik     = playlistGlobal[g].dibuatOleh + " [ADMIN]";
        daftar.push_back(e);
    }

    if (daftar.empty()) {
        cout << "\n📭 Belum ada playlist yang dibuat oleh user manapun." << endl;
        return;
    }

    cout << "\n📋 Pilih playlist untuk melihat detail:\n" << endl;

    for (size_t i = 0; i < daftar.size(); i++) {
        const PlaylistEntry& e = daftar[i];
        string judul   = e.judul;
        string pemilik = e.pemilik;
        if (judul.length()   > 25) judul   = judul.substr(0, 22)   + "...";
        if (pemilik.length() > 25) pemilik = pemilik.substr(0, 22) + "...";

        if (e.tipe == 1) {
            cout << "┌─────────────────────────────┐" << endl;
            cout << "│  🌐  GLOBAL PLAYLIST        │" << endl;
            cout << "│                             │" << endl;
            cout << "│" << padBox("  📀 " + judul)   << "│" << endl;
            cout << "│" << padBox("  👑 " + pemilik)  << "│" << endl;
            cout << "│" << padBox("  🌍  Publik")     << "│" << endl;
            cout << "│" << padBox("  🎶 " + to_string(playlistGlobal[e.globalIdx].lagu.size()) + " Lagu") << "│" << endl;
            cout << "│                             │" << endl;
            cout << "│" << padBox("  👉 Pilih: [" + to_string(i+1) + "]") << "│" << endl;
            cout << "└─────────────────────────────┘" << endl;
        } else {
            const Playlist& pl = pengguna[e.userIdx].musiklist[e.playlistIdx];
            string iconPrivasi = (pl.privasi == "Publik") ? "🌍" : "🔒";
            cout << "┌─────────────────────────────┐" << endl;
            cout << "│  🎵  PLAYLIST               │" << endl;
            cout << "│                             │" << endl;
            cout << "│" << padBox("  📀 " + judul)                          << "│" << endl;
            cout << "│" << padBox("  👤 " + pemilik)                         << "│" << endl;
            cout << "│" << padBox("  " + iconPrivasi + "  " + pl.privasi)    << "│" << endl;
            cout << "│" << padBox("  🎶 " + to_string(pl.lagu.size()) + " Lagu") << "│" << endl;
            cout << "│                             │" << endl;
            cout << "│" << padBox("  👉 Pilih: [" + to_string(i+1) + "]")   << "│" << endl;
            cout << "└─────────────────────────────┘" << endl;
        }
        if (i < daftar.size() - 1) cout << endl;
    }

    printSelectPrompt((int)daftar.size());

    int pilihan;
    cin >> pilihan;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (pilihan >= 1 && pilihan <= (int)daftar.size()) {
        const PlaylistEntry& e = daftar[pilihan - 1];
        printHeader(e.judul);
        if (e.tipe == 1) {
            const PlaylistGlobal& pg = playlistGlobal[e.globalIdx];
            cout << "👑 Oleh: " << pg.dibuatOleh
                 << " | 🌐 Global | 🎶 " << pg.lagu.size() << " Lagu" << endl;
            printLine();
            DaftarLaguGlobal(pg, 0);
        } else {
            const Playlist& pl = pengguna[e.userIdx].musiklist[e.playlistIdx];
            cout << "👤 Oleh: " << pengguna[e.userIdx].username
                 << " | " << pl.privasi
                 << " | 🎶 " << pl.lagu.size() << " Lagu" << endl;
            printLine();
            Daftarlagu(pl, 0);
        }
    } else if (pilihan == 0) {
        cout << "\n✅ Batal memilih playlist." << endl;
    } else {
        cout << "\n❌ Nomor tidak valid." << endl;
    }
}