#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <fstream>          // Untuk baca/tulis file
#include <sstream>          // Untuk parsing string
using namespace std;

const string LINE = "-----------------------------------------";
const string HEAD = "=========================================";

void clearScreen() {
    #ifdef _WIN32
        system("cls");
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
    
    int leftPad = (innerWidth - len) / 2;
    int rightPad = innerWidth - len - leftPad;
    
    cout << HEAD << endl;
    cout << "= " << string(leftPad, ' ') << title.substr(0, len) << string(rightPad, ' ') << " =" << endl;
    cout << HEAD << endl;
}

// ============================================================
// HELPER: Padding khusus agar garis kotak TETAP RAPI dengan EMOJI
// ============================================================
string padBox(string content) {
    // Lebar dalam kotak HARUS tepat 29 karakter visual
    int emojiCount = 0;
    // Hitung emoji yang ada di string (setiap emoji +1 lebar visual)
    string emojis = "🎵📀👤👑🌍🔒🎶👉🌐";
    for (size_t i = 0; i < content.length(); ) {
        unsigned char c = static_cast<unsigned char>(content[i]);
        if (c >= 0xE0) { emojiCount++; i += 3; }      // Lewati byte UTF-8 emoji
        else if (c >= 0xC0) { i += 2; }                // Lewati 2-byte UTF-8
        else { i++; }
    }
    
    // Rumus: Spasi yang dibutuhkan = 29 - panjangString + jumlahEmoji
    int spacesNeeded = 29 - content.length() + emojiCount;
    return content + string(max(0, spacesNeeded), ' ');
}

void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

typedef struct {
    string judulLagu;
    string artis;
    string genre;
    int tahun;
} Musik;

typedef struct {
    string judul;
    int jumlahlagu = 0;
    Musik lagu[50];
    string privasi; 
} Playlist;

struct User {
    string username;
    string password;
    string role;
    int jumlahPlaylist = 0; 
    Playlist musiklist[50];
};

typedef struct {
    string judul;
    int jumlahlagu = 0;
    Musik lagu[50];
    string dibuatOleh;
} PlaylistGlobal;

PlaylistGlobal playlistGlobal[50];
int jumlahPlaylistGlobal = 0;

User pengguna[50];
int jumlahuser = 0;

// ============================================================
// FUNGSI KARTU PLAYLIST (SUDAH DIJAMIN RAPI)
// ============================================================
void tampilkanKartuPlaylistUser(const Playlist& pl, const string& pemilik, int nomor) {
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  🎵  PLAYLIST               │" << endl;
    cout << "│                             │" << endl;

    string line1 = "  📀 " + pl.judul;
    cout << "│" << padBox(line1) << "│" << endl;

    string line2 = "  👤 " + pemilik;
    cout << "│" << padBox(line2) << "│" << endl;

    string icon = (pl.privasi == "Publik") ? "🌍" : "🔒";
    string line3 = "  " + icon + "  " + pl.privasi;
    cout << "│" << padBox(line3) << "│" << endl;

    string line4 = "  🎶 " + to_string(pl.jumlahlagu) + " Lagu";
    cout << "│" << padBox(line4) << "│" << endl;

    cout << "│                             │" << endl;
    string line5 = "  👉 Pilih: [" + to_string(nomor) + "]";
    cout << "│" << padBox(line5) << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
}

void tampilkanKartuPlaylistPublik(const Playlist& pl, const string& pemilik, int nomor) {
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  🌍  PUBLIC PLAYLIST        │" << endl;
    cout << "│                             │" << endl;

    cout << "│" << padBox("  📀 " + pl.judul) << "│" << endl;
    cout << "│" << padBox("  👤 " + pemilik) << "│" << endl;
    cout << "│" << padBox("  🌍  Publik") << "│" << endl;
    cout << "│" << padBox("  🎶 " + to_string(pl.jumlahlagu) + " Lagu") << "│" << endl;
    
    cout << "│                             │" << endl;
    cout << "│" << padBox("  👉 Pilih: [" + to_string(nomor) + "]") << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
}

void tampilkanKartuPlaylistGlobal(const PlaylistGlobal& pg, int nomor) {
    cout << "┌─────────────────────────────┐" << endl;
    cout << "│  🌐  GLOBAL PLAYLIST        │" << endl;
    cout << "│                             │" << endl;

    cout << "│" << padBox("  📀 " + pg.judul) << "│" << endl;
    cout << "│" << padBox("  👑 " + pg.dibuatOleh) << "│" << endl;
    cout << "│" << padBox("  🌍  Publik") << "│" << endl;
    cout << "│" << padBox("  🎶 " + to_string(pg.jumlahlagu) + " Lagu") << "│" << endl;
    
    cout << "│                             │" << endl;
    cout << "│" << padBox("  👉 Pilih: [" + to_string(nomor) + "]") << "│" << endl;
    cout << "└─────────────────────────────┘" << endl;
}

void Daftarlagu(Playlist *p, int indexLagu) {
    if(indexLagu == (*p).jumlahlagu) return;
    
    Musik *ptrLagu = &p->lagu[indexLagu];
    cout << "Lagu ke-" << indexLagu + 1 << endl;
    cout << left << setw(10) << "Judul" << ": " << ptrLagu->judulLagu << endl;
    cout << left << setw(10) << "Artis" << ": " << ptrLagu->artis << endl;
    cout << left << setw(10) << "Genre" << ": " << ptrLagu->genre << endl;
    cout << left << setw(10) << "Tahun" << ": " << ptrLagu->tahun << endl;
    printLine();
    
    Daftarlagu(p, indexLagu + 1);
}

void DaftarLaguGlobal(PlaylistGlobal *pg, int indexLagu) {
    if(indexLagu == pg->jumlahlagu) return;
    
    Musik *ptrLagu = &pg->lagu[indexLagu];
    cout << "Lagu ke-" << indexLagu + 1 << endl;
    cout << left << setw(10) << "Judul" << ": " << ptrLagu->judulLagu << endl;
    cout << left << setw(10) << "Artis" << ": " << ptrLagu->artis << endl;
    cout << left << setw(10) << "Genre" << ": " << ptrLagu->genre << endl;
    cout << left << setw(10) << "Tahun" << ": " << ptrLagu->tahun << endl;
    printLine();
    
    DaftarLaguGlobal(pg, indexLagu + 1);
}

bool usntersedia(User pengguna[], int jumlahuser, string targetUsername) {
    for(int i = 0; i < jumlahuser; i++){
        if(pengguna[i].username == targetUsername){
            return false;
        } 
    }
    return true; 
}

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool validasigenre(string genre) {
    return genre == "klasik" || genre == "pop" || genre == "rock" || genre == "jazz" || genre == "hip hop";
}

// ============================================================
// TAMPILAN PLAYLIST
// ============================================================

void tampilkanPlaylistSaya(User *u) {
    if (u->jumlahPlaylist == 0) {
        cout << "Anda belum memiliki playlist." << endl;
    } else {
        for (int i = 0; i < u->jumlahPlaylist; i++) {
            cout << HEAD << endl;
            cout << left << setw(15) << "Nama Playlist" << ": " << u->musiklist[i].judul << endl;
            cout << left << setw(15) << "Jumlah Lagu" << ": " << u->musiklist[i].jumlahlagu << " Lagu" << endl;
            cout << left << setw(15) << "Privasi" << ": " << u->musiklist[i].privasi << endl;
            cout << HEAD << endl;
            Daftarlagu(&u->musiklist[i], 0);
        }
    }
}

void tampilkanSemuaPlaylistPublik() {
    bool adaPublik = false;
    printHeader("SELURUH PLAYLIST PUBLIK");
    for (int i = 0; i < jumlahuser; i++) {
        bool userPunyaPublik = false;
        for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
            if (pengguna[i].musiklist[j].privasi == "Publik") {
                if (!userPunyaPublik) {
                    cout << ">> Playlist milik user [" << pengguna[i].username << "] <<" << endl;
                    userPunyaPublik = true;
                    adaPublik = true;
                }
                cout << HEAD << endl;
                cout << left << setw(15) << "Nama Playlist" << ": " << pengguna[i].musiklist[j].judul << endl;
                cout << left << setw(15) << "Jumlah Lagu" << ": " << pengguna[i].musiklist[j].jumlahlagu << " Lagu" << endl;
                cout << HEAD << endl;
                Daftarlagu(&pengguna[i].musiklist[j], 0);
            }
        }
        if (userPunyaPublik) cout << endl;
    }
    if (!adaPublik) {
        cout << "Tidak ada playlist publik yang tersedia saat ini." << endl;
    }
}

void tampilkanPlaylistGlobal() {
    if (jumlahPlaylistGlobal == 0) {
        cout << "Belum ada playlist global." << endl;
    } else {
        for (int i = 0; i < jumlahPlaylistGlobal; i++) {
            cout << HEAD << endl;
            cout << left << setw(15) << "Nama Playlist" << ": " << playlistGlobal[i].judul << endl;
            cout << left << setw(15) << "Jumlah Lagu" << ": " << playlistGlobal[i].jumlahlagu << " Lagu" << endl;
            cout << left << setw(15) << "Dibuat Oleh" << ": " << playlistGlobal[i].dibuatOleh << endl;
            cout << HEAD << endl;
            DaftarLaguGlobal(&playlistGlobal[i], 0);
        }
    }
}

void tampilkanStatistikAdmin() {
    int totalUser = jumlahuser;
    int totalPlaylist = 0;
    int totalLagu = 0;
    
    for (int i = 0; i < jumlahuser; i++) {
        totalPlaylist += pengguna[i].jumlahPlaylist;
        for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
            totalLagu += pengguna[i].musiklist[j].jumlahlagu;
        }
    }

    totalPlaylist += jumlahPlaylistGlobal;
    for(int i = 0; i < jumlahPlaylistGlobal; i++) {
        totalLagu += playlistGlobal[i].jumlahlagu;
    }

    printHeader("STATISTIK DATA");
    cout << left << setw(20) << "Total User" << ": " << totalUser << endl;
    cout << left << setw(20) << "Total Playlist" << ": " << totalPlaylist << " (Termasuk " << jumlahPlaylistGlobal << " Playlist Global)" << endl;
    cout << left << setw(20) << "Total Lagu" << ": " << totalLagu << endl;
}

void tampilkanSemuaPlaylistDanGlobalAdmin() {
    printHeader("SEMUA PLAYLIST (ADMIN VIEW)");
    
    bool adaPlaylist = false;
    vector<tuple<Playlist*, string, string, int>> semuaPlaylist; 
    // {playlist, judul, pemilik, tipe(0=user,1=global)}
    
    // Kumpulkan playlist dari semua user
    for (int i = 0; i < jumlahuser; i++) {
        for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
            semuaPlaylist.push_back({
                &pengguna[i].musiklist[j], 
                pengguna[i].musiklist[j].judul,
                pengguna[i].username,
                0 // tipe: 0 = user playlist
            });
            adaPlaylist = true;
        }
    }
    
    // Kumpulkan playlist global
    for (int i = 0; i < jumlahPlaylistGlobal; i++) {
        semuaPlaylist.push_back({
            (Playlist*)&playlistGlobal[i], // Cast untuk reuse fungsi display
            playlistGlobal[i].judul,
            playlistGlobal[i].dibuatOleh + " [ADMIN]",
            1 // tipe: 1 = global playlist
        });
        adaPlaylist = true;
    }
    
    if (!adaPlaylist) {
        cout << "\n📭 Belum ada playlist yang dibuat oleh user manapun." << endl;
        return;
    }
    
    cout << "\n📋 Pilih playlist untuk melihat detail:\n" << endl;
    
    // Tampilkan semua dalam bentuk kartu
    for (int i = 0; i < (int)semuaPlaylist.size(); i++) {
        Playlist* pl = get<0>(semuaPlaylist[i]);
        string judul = get<1>(semuaPlaylist[i]);
        string pemilik = get<2>(semuaPlaylist[i]);
        int tipe = get<3>(semuaPlaylist[i]);
        
        if (tipe == 1) {
            // Global playlist - tampilkan dengan style khusus
            cout << "┌─────────────────────────────┐" << endl;
            cout << "│  🌐  GLOBAL PLAYLIST        │" << endl;
            cout << "│                             │" << endl;
            if(judul.length() > 25) judul = judul.substr(0, 22) + "...";
            cout << "│  📀 " << left << setw(20) << judul << " │" << endl;
            if(pemilik.length() > 25) pemilik = pemilik.substr(0, 22) + "...";
            cout << "│  👑 " << left << setw(20) << pemilik << " │" << endl;
            cout << "│  🌍 " << left << setw(20) << "Publik" << " │" << endl;
            cout << "│  🎶 " << left << setw(20) << to_string(pl->jumlahlagu) + " Lagu" << " │" << endl;
            cout << "│                             │" << endl;
            cout << "│  👉 Pilih: [" << right << setw(2) << i+1 << "]          │" << endl;
            cout << "└─────────────────────────────┘" << endl;
        } else {
            // User playlist
            string iconPrivasi = (pl->privasi == "Publik") ? "🌍" : "🔒";
            cout << "┌─────────────────────────────┐" << endl;
            cout << "│  🎵  PLAYLIST               │" << endl;
            cout << "│                             │" << endl;
            if(judul.length() > 25) judul = judul.substr(0, 22) + "...";
            cout << "│  📀 " << left << setw(20) << judul << " │" << endl;
            if(pemilik.length() > 25) pemilik = pemilik.substr(0, 22) + "...";
            cout << "│  👤 " << left << setw(20) << pemilik << " │" << endl;
            cout << "│  " << iconPrivasi << "  " << left << setw(20) << pl->privasi << " │" << endl;
            cout << "│  🎶 " << left << setw(20) << to_string(pl->jumlahlagu) + " Lagu" << " │" << endl;
            cout << "│                             │" << endl;
            cout << "│  👉 Pilih: [" << right << setw(2) << i+1 << "]          │" << endl;
            cout << "└─────────────────────────────┘" << endl;
        }
        if (i < (int)semuaPlaylist.size() - 1) cout << endl;
    }
    
    // Minta admin memilih
    cout << "\n┌─────────────────────────────┐" << endl;
    cout << "│  🔢 Masukkan nomor playlist:  │" << endl;
    cout << "└─────────────────────────────┘" << endl;
    cout << "Pilihan (1-" << semuaPlaylist.size() << ") atau 0 untuk batal: ";
    
    int pilihan;
    cin >> pilihan;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (pilihan >= 1 && pilihan <= (int)semuaPlaylist.size()) {
        int idx = pilihan - 1;
        Playlist* selected = get<0>(semuaPlaylist[idx]);
        string judul = get<1>(semuaPlaylist[idx]);
        string pemilik = get<2>(semuaPlaylist[idx]);
        int tipe = get<3>(semuaPlaylist[idx]);
        
        printHeader(judul);
        cout << "👤 Oleh: " << pemilik;
        if (tipe == 1) {
            cout << " | 🌐 Global";
        } else {
            cout << " | " << selected->privasi;
        }
        cout << " | 🎶 " << selected->jumlahlagu << " Lagu" << endl;
        printLine();
        
        // Tampilkan isi playlist
        if (tipe == 1) {
            DaftarLaguGlobal((PlaylistGlobal*)selected, 0);
        } else {
            Daftarlagu(selected, 0);
        }
        
    } else if (pilihan == 0) {
        cout << "\n✅ Batal memilih playlist." << endl;
    } else {
        cout << "\n❌ Nomor tidak valid." << endl;
    }
}

// ============================================================
// FUNGSI SAVE DATA KE FILE (PERSISTENCE)
// ============================================================

void saveData() {
    // ===== 1. SIMPAN DATA USER =====
    ofstream fileUser("data_users.txt");
    if (fileUser.is_open()) {
        for (int i = 0; i < jumlahuser; i++) {
            fileUser << pengguna[i].username << "|" 
                    << pengguna[i].password << "|" 
                    << pengguna[i].role << "|"
                    << pengguna[i].jumlahPlaylist << "\n";
        }
        fileUser.close();
    }

    // ===== 2. SIMPAN PLAYLIST USER =====
    ofstream filePlaylist("data_playlists.txt");
    if (filePlaylist.is_open()) {
        for (int u = 0; u < jumlahuser; u++) {
            for (int p = 0; p < pengguna[u].jumlahPlaylist; p++) {
                Playlist& pl = pengguna[u].musiklist[p];
                // Format: username|judul|jumlahlagu|privasi
                filePlaylist << pengguna[u].username << "|" 
                           << pl.judul << "|" 
                           << pl.jumlahlagu << "|" 
                           << pl.privasi << "\n";
                
                // Simpan detail lagu
                for (int l = 0; l < pl.jumlahlagu; l++) {
                    filePlaylist << "  "  // Indentasi penanda lagu
                               << pl.lagu[l].judulLagu << "|"
                               << pl.lagu[l].artis << "|"
                               << pl.lagu[l].genre << "|"
                               << pl.lagu[l].tahun << "\n";
                }
            }
        }
        filePlaylist.close();
    }

    // ===== 3. SIMPAN PLAYLIST GLOBAL =====
    ofstream fileGlobal("data_global.txt");
    if (fileGlobal.is_open()) {
        for (int i = 0; i < jumlahPlaylistGlobal; i++) {
            PlaylistGlobal& pg = playlistGlobal[i];
            fileGlobal << pg.judul << "|" 
                      << pg.jumlahlagu << "|" 
                      << pg.dibuatOleh << "\n";
            
            for (int l = 0; l < pg.jumlahlagu; l++) {
                fileGlobal << "  "
                          << pg.lagu[l].judulLagu << "|"
                          << pg.lagu[l].artis << "|"
                          << pg.lagu[l].genre << "|"
                          << pg.lagu[l].tahun << "\n";
            }
        }
        fileGlobal.close();
    }
    
    cout << "\n💾 Data berhasil disimpan ke file!" << endl;
}

// ============================================================
// FUNGSI RESET SEMUA DATA (MENCEGAH GHOST DATA)
// ============================================================
void resetAllData() {
    // Reset counter global
    jumlahuser = 0;
    jumlahPlaylistGlobal = 0;
    
    // Reset array pengguna[]
    for(int i = 0; i < 50; i++) {
        pengguna[i].username = "";
        pengguna[i].password = "";
        pengguna[i].role = "";
        pengguna[i].jumlahPlaylist = 0;
        
        // Reset semua playlist di user ini
        for(int p = 0; p < 50; p++) {
            pengguna[i].musiklist[p].judul = "";
            pengguna[i].musiklist[p].jumlahlagu = 0;
            pengguna[i].musiklist[p].privasi = "";
            
            // Reset semua lagu di playlist ini
            for(int l = 0; l < 50; l++) {
                pengguna[i].musiklist[p].lagu[l].judulLagu = "";
                pengguna[i].musiklist[p].lagu[l].artis = "";
                pengguna[i].musiklist[p].lagu[l].genre = "";
                pengguna[i].musiklist[p].lagu[l].tahun = 0;
            }
        }
    }
    
    // Reset array playlistGlobal[]
    for(int i = 0; i < 50; i++) {
        playlistGlobal[i].judul = "";
        playlistGlobal[i].jumlahlagu = 0;
        playlistGlobal[i].dibuatOleh = "";
        
        for(int l = 0; l < 50; l++) {
            playlistGlobal[i].lagu[l].judulLagu = "";
            playlistGlobal[i].lagu[l].artis = "";
            playlistGlobal[i].lagu[l].genre = "";
            playlistGlobal[i].lagu[l].tahun = 0;
        }
    }
}

// ============================================================
// FUNGSI LOAD DATA DARI FILE (PERSISTENCE)
// ============================================================

// ============================================================
// FUNGSI LOAD DATA DARI FILE (SUDAH DI-FIX)
// ============================================================

void loadData() {
    // 🔥 PENTING: Reset semua data dulu sebelum load!
    resetAllData();
    
    // ===== 1. LOAD DATA USER =====
    ifstream fileUser("data_users.txt");
    if (fileUser.is_open()) {
        string line;
        while (getline(fileUser, line) && jumlahuser < 50) {
            if(line.empty()) continue; // Skip baris kosong
            
            stringstream ss(line);
            string username, password, role, jmlPlaylist;
            
            getline(ss, username, '|');
            getline(ss, password, '|');
            getline(ss, role, '|');
            getline(ss, jmlPlaylist, '|');
            
            if(username.empty()) continue; // Skip data invalid
            
            pengguna[jumlahuser].username = username;
            pengguna[jumlahuser].password = password;
            pengguna[jumlahuser].role = role;
            pengguna[jumlahuser].jumlahPlaylist = stoi(jmlPlaylist);
            jumlahuser++;
        }
        fileUser.close();
    }

    // ===== 2. LOAD PLAYLIST USER =====
    ifstream filePlaylist("data_playlists.txt");
    if (filePlaylist.is_open()) {
        string line;
        while (getline(filePlaylist, line)) {
            if(line.empty()) continue;
            
            // Skip baris lagu (dimulai dengan spasi)
            if (line[0] == ' ') continue;
            
            stringstream ss(line);
            string username, judul, jmlLagu, privasi;
            
            getline(ss, username, '|');
            getline(ss, judul, '|');
            getline(ss, jmlLagu, '|');
            getline(ss, privasi, '|');
            
            if(username.empty() || judul.empty()) continue;
            
            // Cari user yang sesuai
            for (int u = 0; u < jumlahuser; u++) {
                if (pengguna[u].username == username) {
                    int idx = pengguna[u].jumlahPlaylist;
                    if(idx >= 50) break; // Safety check
                    
                    // Reset playlist dulu sebelum diisi
                    pengguna[u].musiklist[idx].judul = "";
                    pengguna[u].musiklist[idx].jumlahlagu = 0;
                    pengguna[u].musiklist[idx].privasi = "";
                    for(int l = 0; l < 50; l++) {
                        pengguna[u].musiklist[idx].lagu[l].judulLagu = "";
                        pengguna[u].musiklist[idx].lagu[l].artis = "";
                        pengguna[u].musiklist[idx].lagu[l].genre = "";
                        pengguna[u].musiklist[idx].lagu[l].tahun = 0;
                    }
                    
                    // Isi data playlist
                    pengguna[u].musiklist[idx].judul = judul;
                    pengguna[u].musiklist[idx].jumlahlagu = stoi(jmlLagu);
                    pengguna[u].musiklist[idx].privasi = privasi;
                    
                    // Baca lagu-lagu berikutnya (yang di-indent)
                    int laguDibaca = 0;
                    while (laguDibaca < pengguna[u].musiklist[idx].jumlahlagu && 
                           getline(filePlaylist, line)) {
                        if (line[0] == ' ') {
                            stringstream ls(line.substr(2)); // Hapus indentasi
                            string judulLagu, artis, genre, tahun;
                            
                            getline(ls, judulLagu, '|');
                            getline(ls, artis, '|');
                            getline(ls, genre, '|');
                            getline(ls, tahun, '|');
                            
                            pengguna[u].musiklist[idx].lagu[laguDibaca].judulLagu = judulLagu;
                            pengguna[u].musiklist[idx].lagu[laguDibaca].artis = artis;
                            pengguna[u].musiklist[idx].lagu[laguDibaca].genre = genre;
                            pengguna[u].musiklist[idx].lagu[laguDibaca].tahun = stoi(tahun);
                            laguDibaca++;
                        } else {
                            // Jika ketemu header playlist lain, push back line dan break
                            filePlaylist.putback(line.back());
                            for(int i = line.length()-2; i >= 0; i--) {
                                filePlaylist.putback(line[i]);
                            }
                            break;
                        }
                    }
                    pengguna[u].jumlahPlaylist++;
                    break;
                }
            }
        }
        filePlaylist.close();
    }

    // ===== 3. LOAD PLAYLIST GLOBAL =====
    ifstream fileGlobal("data_global.txt");
    if (fileGlobal.is_open()) {
        string line;
        while (getline(fileGlobal, line)) {
            if(line.empty()) continue;
            if (line[0] == ' ') continue; // Skip baris lagu
            
            stringstream ss(line);
            string judul, jmlLagu, dibuatOleh;
            
            getline(ss, judul, '|');
            getline(ss, jmlLagu, '|');
            getline(ss, dibuatOleh, '|');
            
            if(judul.empty()) continue;
            
            int idx = jumlahPlaylistGlobal;
            if(idx >= 50) break;
            
            // Reset playlist global dulu
            playlistGlobal[idx].judul = "";
            playlistGlobal[idx].jumlahlagu = 0;
            playlistGlobal[idx].dibuatOleh = "";
            for(int l = 0; l < 50; l++) {
                playlistGlobal[idx].lagu[l].judulLagu = "";
                playlistGlobal[idx].lagu[l].artis = "";
                playlistGlobal[idx].lagu[l].genre = "";
                playlistGlobal[idx].lagu[l].tahun = 0;
            }
            
            // Isi data
            playlistGlobal[idx].judul = judul;
            playlistGlobal[idx].jumlahlagu = stoi(jmlLagu);
            playlistGlobal[idx].dibuatOleh = dibuatOleh;
            
            // Baca lagu-lagu
            int laguDibaca = 0;
            while (laguDibaca < playlistGlobal[idx].jumlahlagu && 
                   getline(fileGlobal, line)) {
                if (line[0] == ' ') {
                    stringstream ls(line.substr(2));
                    string judulLagu, artis, genre, tahun;
                    
                    getline(ls, judulLagu, '|');
                    getline(ls, artis, '|');
                    getline(ls, genre, '|');
                    getline(ls, tahun, '|');
                    
                    playlistGlobal[idx].lagu[laguDibaca].judulLagu = judulLagu;
                    playlistGlobal[idx].lagu[laguDibaca].artis = artis;
                    playlistGlobal[idx].lagu[laguDibaca].genre = genre;
                    playlistGlobal[idx].lagu[laguDibaca].tahun = stoi(tahun);
                    laguDibaca++;
                } else {
                    fileGlobal.putback(line.back());
                    for(int i = line.length()-2; i >= 0; i--) {
                        fileGlobal.putback(line[i]);
                    }
                    break;
                }
            }
            jumlahPlaylistGlobal++;
        }
        fileGlobal.close();
    }
}


//============ HALAMAN MASUK ============

void Register(User pengguna[], int &jumlahuser) {
    if(jumlahuser >= 50){
        cout << left << setw(12) << "Info" << ": Kapasitas user sudah penuh" << endl;
        pause();
        return;
    }
    
    printHeader("REGISTER");
    string username, password;
    cout << left << setw(12) << "Username" << ": "; cin >> username;
    
    if(!usntersedia(pengguna, jumlahuser, username)){
        cout << left << setw(12) << "Info" << ": Username tidak tersedia" << endl;
    }
    else {
        pengguna[jumlahuser].username = username;
        pengguna[jumlahuser].role = "user";
        cout << left << setw(12) << "Password" << ": "; cin >> password;
        pengguna[jumlahuser].password = password;
        jumlahuser++; 
        cout << left << setw(12) << "Info" << ": Akun berhasil dibuat" << endl;
        
        // 🔥 AUTO-SAVE SETELAH REGISTER
        saveData();  // ← TAMBAHKAN INI
    }
    pause();
}

int Login(User pengguna[], int jumlahuser, int &userindex, int &bataslogin) {
    printHeader("LOGIN");
    string username, password;
    while(true){
        cout << left << setw(12) << "Username" << ": "; cin >> username; 
        cout << left << setw(12) << "Password" << ": "; cin >> password;  
        bool logvalid = false; 

        for(int i = 0; i < jumlahuser; i++){
            if(pengguna[i].username == username && pengguna[i].password == password){
                cout << left << setw(12)<<  "Info" << ": Login berhasil" << endl;
                userindex = i; 
                logvalid = true; 
                bataslogin = 0;
                pause();
                return 1; 
            }
        }
        if(!logvalid){
            bataslogin++;
            if(bataslogin < 3){
                cout << left << setw(12) << "Info" << ": Username atau Password salah\n" << endl;
            }
            else if(bataslogin == 3){
                cout << left << setw(12) << "Info" << ": Gagal login 3 kali, keluar dari program\n" << endl;
                pause();
                return -1; 
            }
        }
    }
}

void Datalagu(Musik *laguBaru, bool needIgnore = true) {
    if (needIgnore) cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << left << setw(12) << "Judul lagu" << ": ";
    getline(cin, laguBaru->judulLagu);
    
    cout << left << setw(12) << "Artis" << ": ";
    getline(cin, laguBaru->artis);
    
    string Genre;
    bool genrevalid = false;
    do{
        try {
            cout << left << setw(12) << "Genre" << ": ";
            getline(cin, Genre);
            Genre = toLowerCase(Genre);
            if(!validasigenre(Genre)){
                throw invalid_argument("klasik, pop, rock, jazz, atau hip hop");
            }
            genrevalid = true; 
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    } while (!genrevalid);
    laguBaru->genre = Genre;

    int Tahun;
    bool tahunvalid = false;
    do{
        try {
            cout << left << setw(12) << "Tahun" << ": ";
            cin >> Tahun;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            else if (Tahun < 1900 || Tahun > 2026) {
                throw out_of_range("1900 hingga 2026");
            }
            tahunvalid = true;
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Tahun hanya dari rentang " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!tahunvalid);
    laguBaru->tahun = Tahun;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void buatPlaylist(User *u) {
    int playlist = u->jumlahPlaylist; 
    if(playlist >= 50){
        cout << left << setw(12) << "Info" << ": Kapasitas Playlist sudah penuh" << endl;
        pause();
        return;
    }

    printHeader("BUAT PLAYLIST");
    cout << left << setw(15) << "Nama Playlist" << ": "; 
    cin.ignore();
    getline(cin, u->musiklist[playlist].judul);
    
    string privasi;
    bool privasiValid = false;
    do {
        cout << left << setw(15) << "Privasi (Publik/Privat)" << ": ";
        cin >> privasi;
        privasi = toLowerCase(privasi);
        if (privasi == "publik" || privasi == "privat") {
            privasiValid = true;
            u->musiklist[playlist].privasi = (privasi == "publik") ? "Publik" : "Privat";
        } else {
            cout << left << setw(12) << "Info" << ": Hanya 'Publik' atau 'Privat'" << endl;
        }
    } while (!privasiValid);

    int jumlahlagu;
    bool jumlahvalid = false;
    do{
        try {
            cout << left << setw(15) << "Jumlah lagu" << ": ";
            cin >> jumlahlagu;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            } 
            else if(jumlahlagu <= 0 || jumlahlagu > 50){
                throw out_of_range("1 sampai 50");
            } 
            else{
                jumlahvalid = true;
            }
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Jumlah lagu hanya dari rentang " << e.what() << endl;
            cout << '\n' << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!jumlahvalid);

    for(int i = 0; i < jumlahlagu; i++){
        cout << endl << "Lagu ke-" << i + 1 << endl;
        Datalagu(&u->musiklist[playlist].lagu[i], (i == 0));
    }

// Di dalam fungsi buatPlaylist(), setelah jumlahPlaylist++:
    u->musiklist[playlist].jumlahlagu = jumlahlagu;
    u->jumlahPlaylist++; 
    cout << left << setw(12) << "Info" << ": Playlist berhasil dibuat" << endl;
    
    // 🔥 AUTO-SAVE SETELAH BUAT PLAYLIST
    saveData();  // ← TAMBAHKAN INI
    pause();
}
void lihatPlaylist(User *u) {
    printHeader("LIHAT PLAYLIST");
    cout << "1. Playlist Saya" << endl;
    cout << "2. Playlist Publik & Global" << endl;
    cout << "Pilih opsi: ";

    int opsi;
    try {
        cin >> opsi;
        if (cin.fail()) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("angka");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } catch (const exception& e) {
        cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
        pause();
        return;
    }

    if (opsi == 1) {
        // ===== TAMPILKAN PLAYLIST SAYA DENGAN KARTU =====
        printHeader("PLAYLIST SAYA");
        
        if (u->jumlahPlaylist == 0) {
            cout << "\n📭 Anda belum memiliki playlist." << endl;
            cout << "💡 Tips: Buat playlist baru untuk mulai mengumpulkan lagu!" << endl;
        } else {
            cout << "\n📋 Pilih playlist yang ingin dibuka:\n" << endl;
            
            // Tampilkan semua playlist dalam bentuk kartu
            for (int i = 0; i < u->jumlahPlaylist; i++) {
                tampilkanKartuPlaylistUser(u->musiklist[i], "Saya", i + 1);
                if (i < u->jumlahPlaylist - 1) cout << endl; // Jarak antar kartu
            }
            
            // Minta user memilih
            cout << "\n┌─────────────────────────────┐" << endl;
            cout << "│  🔢 Masukkan nomor playlist:  │" << endl;
            cout << "└─────────────────────────────┘" << endl;
            cout << "Pilihan (1-" << u->jumlahPlaylist << ") atau 0 untuk batal: ";
            
            int pilihan;
            cin >> pilihan;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (pilihan >= 1 && pilihan <= u->jumlahPlaylist) {
                int idx = pilihan - 1;
                // Tampilkan isi playlist yang dipilih
                printHeader(u->musiklist[idx].judul);
                cout << "👤 Oleh: Saya | 🔒 " << u->musiklist[idx].privasi 
                     << " | 🎶 " << u->musiklist[idx].jumlahlagu << " Lagu" << endl;
                printLine();
                Daftarlagu(&u->musiklist[idx], 0);
            } else if (pilihan == 0) {
                cout << "\n✅ Batal memilih playlist." << endl;
            } else {
                cout << "\n❌ Nomor tidak valid." << endl;
            }
        }
        
    } 
    else if (opsi == 2) {
        // ===== TAMPILKAN PLAYLIST PUBLIK & GLOBAL DENGAN KARTU =====
        vector<pair<Playlist*, string>> poolPublik; // {playlist, pemilik}
        vector<pair<PlaylistGlobal*, string>> poolGlobal; // {playlist, label}
        
        // Kumpulkan playlist publik dari user lain
        for (int i = 0; i < jumlahuser; i++) {
            if (pengguna[i].username == u->username) continue; // Skip diri sendiri
            for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
                if (pengguna[i].musiklist[j].privasi == "Publik") {
                    poolPublik.push_back({&pengguna[i].musiklist[j], pengguna[i].username});
                }
            }
        }
        
        // Kumpulkan playlist global
        for (int i = 0; i < jumlahPlaylistGlobal; i++) {
            poolGlobal.push_back({&playlistGlobal[i], "Global"});
        }
        
        int totalKartu = poolPublik.size() + poolGlobal.size();
        
        if (totalKartu == 0) {
            cout << "\n📭 Belum ada playlist publik atau global yang tersedia." << endl;
        } else {
            cout << "\n🌍 PLAYLIST PUBLIK DARI USER LAIN:" << endl;
            printLine();
            
            if (poolPublik.empty()) {
                cout << "(Tidak ada)" << endl;
            } else {
                for (int i = 0; i < (int)poolPublik.size(); i++) {
                    tampilkanKartuPlaylistPublik(*poolPublik[i].first, poolPublik[i].second, i + 1);
                    if (i < (int)poolPublik.size() - 1) cout << endl;
                }
            }
            
            cout << "\n\n🌐 PLAYLIST GLOBAL (ADMIN):" << endl;
            printLine();
            
            if (poolGlobal.empty()) {
                cout << "(Tidak ada)" << endl;
            } else {
                for (int i = 0; i < (int)poolGlobal.size(); i++) {
                    tampilkanKartuPlaylistGlobal(*poolGlobal[i].first, (int)poolPublik.size() + i + 1);
                    if (i < (int)poolGlobal.size() - 1) cout << endl;
                }
            }
            
            // Minta user memilih
            cout << "\n┌─────────────────────────────┐" << endl;
            cout << "│  🔢 Masukkan nomor playlist:  │" << endl;
            cout << "└─────────────────────────────┘" << endl;
            cout << "Pilihan (1-" << totalKartu << ") atau 0 untuk batal: ";
            
            int pilihan;
            cin >> pilihan;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (pilihan >= 1 && pilihan <= (int)poolPublik.size()) {
                // Pilih dari pool publik
                int idx = pilihan - 1;
                Playlist* selected = poolPublik[idx].first;
                string pemilik = poolPublik[idx].second;
                
                printHeader(selected->judul);
                cout << "👤 Oleh: " << pemilik << " | 🌍 Publik | 🎶 " 
                     << selected->jumlahlagu << " Lagu" << endl;
                printLine();
                Daftarlagu(selected, 0);
                
            } else if (pilihan > (int)poolPublik.size() && pilihan <= totalKartu) {
                // Pilih dari pool global
                int idx = pilihan - (int)poolPublik.size() - 1;
                PlaylistGlobal* selected = poolGlobal[idx].first;
                
                printHeader(selected->judul);
                cout << "👑 Oleh: " << selected->dibuatOleh << " | 🌐 Global | 🎶 " 
                     << selected->jumlahlagu << " Lagu" << endl;
                printLine();
                DaftarLaguGlobal(selected, 0);
                
            } else if (pilihan == 0) {
                cout << "\n✅ Batal memilih playlist." << endl;
            } else {
                cout << "\n❌ Nomor tidak valid." << endl;
            }
        }
    }
    pause();
}
void ubahPlaylist(User *u) {
    int totalPlaylist = u->jumlahPlaylist; 
    if(totalPlaylist == 0){
        cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist" << endl;
        pause();
        return;
    }

    printHeader("UBAH PLAYLIST");
    cout << "Daftar Playlist:" << endl;
    for(int i = 0; i < totalPlaylist; i++){
        cout << i + 1 << ". " << u->musiklist[i].judul << " (" << u->musiklist[i].privasi << ")" << endl;
    }

    int pilihPlaylist;
    bool validPlaylist = false;
    do{
        try {
            cout << "Pilih nomor playlist yang ingin diubah (1 - " << totalPlaylist << "): ";
            cin >> pilihPlaylist;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka"); 
            }
            else if(pilihPlaylist < 1 || pilihPlaylist > totalPlaylist){
                throw out_of_range("Nomor playlist tidak ditemukan");
            }
            validPlaylist = true;
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!validPlaylist);
    
    int indexP = pilihPlaylist - 1;
    
    printLine();
    cout << left << setw(5) << " " << "1. Nama Playlist" << endl;
    cout << left << setw(5) << " " << "2. Data Lagu" << endl;
    cout << left << setw(5) << " " << "3. Ubah Status Privasi (Publik/Privat)" << endl;
    
    int Opsi;
    bool opsivalid = false;
    do{
        try {
            cout << "Pilih opsi (1/2/3): ";
            cin >> Opsi;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            else if(Opsi < 1 || Opsi > 3){
                throw out_of_range("1, 2, atau 3");
            }
            opsivalid = true;
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Pilihan hanya " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!opsivalid);

    if(Opsi == 1){
        cout << left << setw(20) << "Nama Playlist Baru" << ": ";
        cin.ignore();
        getline(cin, u->musiklist[indexP].judul);
        cout << left << setw(12) << "Info" << ": Judul Playlist berhasil diubah" << endl;
    }
    else if(Opsi == 2){
        int jumlahlagu = u->musiklist[indexP].jumlahlagu;
        if(jumlahlagu == 0){
            cout << left << setw(12) << "Info" << ": Playlist belum ada lagu" << endl;
        }
        else{
            cout << "\nDaftar Lagu:" << endl;
            for (int j = 0; j < jumlahlagu; j++) {
                cout << j + 1 << ". " << u->musiklist[indexP].lagu[j].judulLagu << " - " << u->musiklist[indexP].lagu[j].artis << endl;
            }
            
            int pilihLagu;
            bool validLagu = false;
            do{
                try {
                    cout << "Pilih nomor lagu yang ingin diubah (1 - " << jumlahlagu << "): ";
                    cin >> pilihLagu;
                    if(cin.fail()){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("angka");
                    }
                    else if(pilihLagu < 1 || pilihLagu > jumlahlagu){
                        throw out_of_range("Nomor lagu tidak ditemukan");
                    }
                    validLagu = true;
                }
                catch(const exception& e) {
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
    }
    else if (Opsi == 3){
        u->musiklist[indexP].privasi = (u->musiklist[indexP].privasi == "Publik") ? "Privat" : "Publik";
        cout << left << setw(20) << "Status Baru" << ": " << u->musiklist[indexP].privasi << endl;
        cout << left << setw(12) << "Info" << ": Status privasi berhasil diperbarui" << endl;
    }
    saveData(); 
    pause();
}

void hapusDataUser(User pengguna[], int &jumlahuser, int &userindex) {
    printHeader("HAPUS DATA");
    cout << left << setw(5) << " " << "1. Hapus Playlist" << endl;
    cout << left << setw(5) << " " << "2. Hapus Lagu dalam Playlist" << endl;
    cout << left << setw(5) << " " << "3. Hapus Akun Sendiri" << endl;
    
    int Opsi;
    bool opsivalid = false;
    do{
        try {
            cout << "Pilih opsi (1/2/3): ";
            cin >> Opsi;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            } 
            else if(Opsi < 1 || Opsi > 3){
                throw out_of_range("1, 2, atau 3");
            } 
            opsivalid = true;
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Pilihan hanya " << e.what() << endl;
            cout << endl << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }while(!opsivalid);

    if(Opsi == 1){
        User *u = &pengguna[userindex];
        int totalPlaylist = u->jumlahPlaylist;
        if(totalPlaylist == 0){
            cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist" << endl;
            pause();
            return;
        }
        cout << "Daftar Playlist:" << endl;
        for(int i = 0; i < totalPlaylist; i++){
            cout << i + 1 << ". " << u->musiklist[i].judul << endl;
        }
        int pilihPlaylist;
        bool validPlaylist = false;
        do{
            try {
                cout << "Pilih nomor playlist yang ingin dihapus (1 - " << totalPlaylist << "): ";
                cin >> pilihPlaylist;
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("angka");
                } else if(pilihPlaylist < 1 || pilihPlaylist > totalPlaylist){
                    throw out_of_range("Nomor playlist tidak ditemukan");
                } 
                validPlaylist = true;
            }
            catch(const exception& e) {
                cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                cout << endl << "Tekan Enter untuk melanjutkan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        }while (!validPlaylist);
        int indexP = pilihPlaylist - 1;
        for(int i = indexP; i < totalPlaylist - 1; i++){
            u->musiklist[i] = u->musiklist[i + 1];
        }
        u->jumlahPlaylist--;
        cout << left << setw(12) << "Info" << ": Playlist berhasil dihapus" << endl;
        saveData();
        return;
    } 
    else if(Opsi == 2){
        User *u = &pengguna[userindex];
        int totalPlaylist = u->jumlahPlaylist;
        if(totalPlaylist == 0){
            cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist" << endl;
            pause();
            return;
        }
        cout << "Daftar Playlist:" << endl;
        for(int i = 0; i < totalPlaylist; i++){
            cout << i + 1 << ". " << u->musiklist[i].judul << endl;
        }
        int pilihPlaylist;
        bool validPlaylist = false;
        do{
            try {
                cout << "Pilih nomor playlist yang ingin dipilih (1 - " << totalPlaylist << "): ";
                cin >> pilihPlaylist;
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("angka");
                } else if(pilihPlaylist < 1 || pilihPlaylist > totalPlaylist){
                    throw out_of_range("Nomor playlist tidak ditemukan");
                }
                validPlaylist = true;
            }
            catch(const exception& e) {
                cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                cout << endl << "Tekan Enter untuk melanjutkan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        }while(!validPlaylist);
        int indexP = pilihPlaylist - 1;
        int jumlahlagu = u->musiklist[indexP].jumlahlagu;
        if(jumlahlagu == 0){
            cout << left << setw(12) << "Info" << ": Playlist tidak memiliki lagu" << endl;
        } 
        else{
            cout << "\nDaftar Lagu di '" << u->musiklist[indexP].judul << "':" << endl;
            for(int j = 0; j < jumlahlagu; j++){
                cout << j + 1 << ". " << u->musiklist[indexP].lagu[j].judulLagu << " - " << u->musiklist[indexP].lagu[j].artis << endl;
            }
            int pilihLagu;
            bool validLagu = false;
            do{
                try {
                    cout << "Pilih nomor lagu yang ingin dihapus (1 - " << jumlahlagu << "): ";
                    cin >> pilihLagu;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("angka");
                    } else if(pilihLagu < 1 || pilihLagu > jumlahlagu){
                        throw out_of_range("Nomor lagu tidak ditemukan");
                    } 
                    validLagu = true;
                }
                catch(const exception& e) {
                    cout << left << setw(12) << "Info" << ": " << e.what() << endl;
                    cout << '\n' << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
            }while (!validLagu);
            int indexL = pilihLagu - 1;
            for(int k = indexL; k < jumlahlagu - 1; k++){
                u->musiklist[indexP].lagu[k] = u->musiklist[indexP].lagu[k + 1];
            }
            u->musiklist[indexP].jumlahlagu--;
            cout << left << setw(12) << "Info" << ": Lagu berhasil dihapus" << endl;
            saveData(); 
        }
        return;
    }
    else if(Opsi == 3){
        cout << "Apakah anda yakin ingin menghapus akun? (y/n): ";
        char konfirmasi;
        cin >> konfirmasi;
        if(tolower(konfirmasi) == 'y'){
            for(int i = userindex; i < jumlahuser - 1; i++){
                pengguna[i] = pengguna[i + 1];
            }
            jumlahuser--;
            userindex = -1;
            cout << left << setw(12) << "Info" << ": Akun berhasil dihapus" << endl;
            saveData(); 
            pause();
            return;
        }
    }
    pause();
}

void buatPlaylistGlobal(User *u) {
    if(jumlahPlaylistGlobal >= 50){
        cout << left << setw(12) << "Info" << ": Kapasitas Playlist Global sudah penuh" << endl;
        pause();
        return;
    }

    printHeader("BUAT PLAYLIST GLOBAL");
    int idx = jumlahPlaylistGlobal;
    cout << left << setw(15) << "Nama Playlist" << ": ";
    cin.ignore();
    getline(cin, playlistGlobal[idx].judul);

    int jumlahlagu;
    bool jumlahvalid = false;
    do{
        try {
            cout << left << setw(15) << "Jumlah lagu" << ": ";
            cin >> jumlahlagu;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            } 
            else if(jumlahlagu <= 0 || jumlahlagu > 50){
                throw out_of_range("1 sampai 50");
            } 
            else{
                jumlahvalid = true;
            }
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Jumlah lagu hanya dari rentang " << e.what() << endl;
            cout << '\n' << "Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!jumlahvalid);

    for(int i = 0; i < jumlahlagu; i++){
        cout << endl << "Lagu ke-" << i + 1 << endl;
        Datalagu(&playlistGlobal[idx].lagu[i], (i == 0));
    }

    playlistGlobal[idx].jumlahlagu = jumlahlagu;
    playlistGlobal[idx].dibuatOleh = u->username;
    jumlahPlaylistGlobal++;
    cout << left << setw(12) << "Info" << ": Playlist Global berhasil dibuat" << endl;
    saveData();
    pause();
}

void updatePlaylistGlobal() {
    if(jumlahPlaylistGlobal == 0){
        cout << left << setw(12) << "Info" << ": Belum ada Playlist Global" << endl;
        pause();
        return;
    }
    printHeader("UPDATE PLAYLIST GLOBAL");
    cout << "Daftar Playlist Global:" << endl;
    for(int i = 0; i < jumlahPlaylistGlobal; i++){
        cout << i + 1 << ". " << playlistGlobal[i].judul << " (dibuat oleh: " << playlistGlobal[i].dibuatOleh << ")" << endl;
    }
    int pilihPlaylist;
    bool validPlaylist = false;
    do{
        try {
            cout << "Pilih nomor playlist yang ingin diupdate (1 - " << jumlahPlaylistGlobal << "): ";
            cin >> pilihPlaylist;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            else if(pilihPlaylist < 1 || pilihPlaylist > jumlahPlaylistGlobal){
                throw out_of_range("Nomor playlist tidak ditemukan");
            }
            validPlaylist = true;
        }
        catch(const exception& e) {
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

    int Opsi;
    bool opsivalid = false;
    do{
        try {
            cout << "Pilih opsi (1/2): ";
            cin >> Opsi;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            else if(Opsi < 1 || Opsi > 2){
                throw out_of_range("1 atau 2");
            }
            opsivalid = true;
        }
        catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Pilihan hanya " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (!opsivalid);

    if(Opsi == 1){
        cout << left << setw(20) << "Nama Playlist Baru" << ": ";
        cin.ignore();
        getline(cin, playlistGlobal[indexP].judul);
        cout << left << setw(12) << "Info" << ": Judul Playlist Global berhasil diubah" << endl;
    }
    else if(Opsi == 2){
        int jumlahlagu = playlistGlobal[indexP].jumlahlagu;
        if(jumlahlagu == 0){
            cout << left << setw(12) << "Info" << ": Playlist belum ada lagu" << endl;
        }
        else{
            cout << "\nDaftar Lagu:" << endl;
            for(int j = 0; j < jumlahlagu; j++){
                cout << j + 1 << ". " << playlistGlobal[indexP].lagu[j].judulLagu << " - " << playlistGlobal[indexP].lagu[j].artis << endl;
            }

            int pilihLagu;
            bool validLagu = false;
            do{
                try {
                    cout << "Pilih nomor lagu yang ingin diubah (1 - " << jumlahlagu << "): ";
                    cin >> pilihLagu;
                    if(cin.fail()){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("angka");
                    }
                    else if(pilihLagu < 1 || pilihLagu > jumlahlagu){
                        throw out_of_range("Nomor lagu tidak ditemukan");
                    }
                    validLagu = true;
                }
                catch(const exception& e) {
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
    }
    saveData();
    pause();
}

void bubbleSortPlaylistNaik(User *u){
    int n = u->jumlahPlaylist;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (u->musiklist[j].judul > u->musiklist[j+1].judul) {
                swap(u->musiklist[j], u->musiklist[j+1]);
            }
        }
    }
}

void selectionSortLaguDSC(Playlist *p){
    int n = p->jumlahlagu;
    for(int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for(int j = i + 1; j < n; j++) {
            if(p->lagu[j].tahun > p->lagu[maxIdx].tahun){
                maxIdx = j;
            }
        }
        swap(p->lagu[i], p->lagu[maxIdx]);
    }
}

void insertionSortLaguASC(Playlist *p){
    int n = p->jumlahlagu;
    for(int i = 1; i < n; i++){
        Musik key = p->lagu[i];
        int j = i - 1;
        while (j >= 0 && p->lagu[j].judulLagu > key.judulLagu) {
            p->lagu[j+1] = p->lagu[j];
            j--;
        }
        p->lagu[j+1] = key;
    }
}

int dapatkanIndexPlaylist(const User* u) {
    cout << "Daftar Playlist:" << endl;
    for(int i = 0; i < u->jumlahPlaylist; i++) {
        cout << i + 1 << ". " << u->musiklist[i].judul
             << " (" << u->musiklist[i].jumlahlagu << " lagu)" << endl;
    }
    printLine();
    
    int pilihan;
    while(true) {
        cout << "Pilih nomor playlist: ";
        try {
            cin >> pilihan;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(pilihan < 1 || pilihan > u->jumlahPlaylist) {
                throw out_of_range("Nomor playlist tidak valid");
            }
            return pilihan - 1;
        } catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
        }
    }
}

void menuSorting(User *u) {
    if(u->jumlahPlaylist == 0) {
        cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist untuk diurutkan" << endl;
        pause();
        return;
    }

    int opsi;
    printHeader("MENU SORTING");
    cout << "1. Urutkan Playlist berdasarkan Judul" << endl;
    cout << "2. Urutkan Lagu dalam Playlist berdasarkan Tahun" << endl;
    cout << "3. Urutkan Lagu dalam Playlist berdasarkan Judul Lagu" << endl;
    cout << "4. Menu Utama" << endl;
    cout << "Pilih opsi\t: ";

    try {
        cin >> opsi;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("angka");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } catch(const exception& e) {
        cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
        pause();
        return;
    }
    cout << endl;

    switch(opsi) {
        case 1: {
            bubbleSortPlaylistNaik(u);
            cout << left << setw(12) << "Info" << ": Playlist berhasil diurutkan secara ascending" << endl;
            for(int i = 0; i < u->jumlahPlaylist; i++) {
                cout << i + 1 << ". " << u->musiklist[i].judul << endl;
            }
            pause();
            break;
        }
        case 2: {
            int idx = dapatkanIndexPlaylist(u);
            if(u->musiklist[idx].jumlahlagu == 0) {
                cout << left << setw(12) << "Info" << ": Playlist tidak memiliki lagu" << endl;
            } else {
                selectionSortLaguDSC(&u->musiklist[idx]);
                cout << left << setw(12) << "Info" << ": Lagu berhasil diurutkan secara descending berdasarkan tahun" << endl;
                Daftarlagu(&u->musiklist[idx], 0);
            }
            pause();
            break;
        }
        case 3: {
            int idx = dapatkanIndexPlaylist(u);
            if(u->musiklist[idx].jumlahlagu == 0) {
                cout << left << setw(12) << "Info" << ": Playlist tidak memiliki lagu" << endl;
            } else {
                insertionSortLaguASC(&u->musiklist[idx]);
                cout << left << setw(12) << "Info" << ": Lagu berhasil diurutkan secara ascending berdasarkan judul lagu" << endl;
                Daftarlagu(&u->musiklist[idx], 0);
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

struct HasilCari {
    Musik lagu;
    string namaPlaylist;
    string pemilik;      
};

bool linearSearchArtis(const Musik* arr, int n, const string& target, int& idx){
    for(int i = 0; i < n; i++) {
        if(arr[i].artis == target) {
            idx = i; 
            return true;
        }
    }
    return false;
}

bool binarySearchJudul(const Musik* arr, int n, const string& target, int& idx){
    int left = 0;
    int right = n - 1;
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if(arr[mid].judulLagu == target) {
            idx = mid; 
            return true; 
        }
        else if(arr[mid].judulLagu < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return false;
}


void kumpulkanPoolLagu(int userindex, vector<HasilCari>& pool) {
    pool.clear();

    User *u = &pengguna[userindex];
    for(int j = 0; j < u->jumlahPlaylist; j++) {
        for(int k = 0; k < u->musiklist[j].jumlahlagu; k++) {
            HasilCari h;
            h.lagu        = u->musiklist[j].lagu[k];
            h.namaPlaylist = u->musiklist[j].judul;
            h.pemilik     = "Saya";
            pool.push_back(h);
        }
    }

    for(int i = 0; i < jumlahuser; i++) {
        if(i == userindex) continue;
        for(int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
            if(pengguna[i].musiklist[j].privasi != "Publik") continue;
            for(int k = 0; k < pengguna[i].musiklist[j].jumlahlagu; k++) {
                HasilCari h;
                h.lagu        = pengguna[i].musiklist[j].lagu[k];
                h.namaPlaylist = pengguna[i].musiklist[j].judul;
                h.pemilik     = pengguna[i].username;
                pool.push_back(h);
            }
        }
    }

    for(int i = 0; i < jumlahPlaylistGlobal; i++) {
        for(int k = 0; k < playlistGlobal[i].jumlahlagu; k++) {
            HasilCari h;
            h.lagu        = playlistGlobal[i].lagu[k];
            h.namaPlaylist = playlistGlobal[i].judul;
            h.pemilik     = "Global";
            pool.push_back(h);
        }
    }
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

void menuSearching(int userindex){
    vector<HasilCari> pool;
    kumpulkanPoolLagu(userindex, pool);

    if(pool.empty()) {
        cout << left << setw(12) << "Info" << ": Tidak ada lagu yang dapat dicari saat ini" << endl;
        pause();
        return;
    }

    int metode;
    printHeader("MENU SEARCHING");
    cout << "Cari berdasarkan:" << endl;
    cout << "1. Nama Artis" << endl;
    cout << "2. Judul Lagu" << endl;
    cout << "Pilih metode\t: ";
    
    try {
        cin >> metode;
        if(cin.fail()){
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("angka");
        }
        if(metode < 1 || metode > 2) throw out_of_range("1 atau 2");
    } catch(const exception& e) {
        cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
        pause();
        return;
    }

    if(metode == 1) {
        string artisCari;
        cout << "Masukkan nama artis\t: ";
        cin.ignore();
        getline(cin, artisCari);

        cout << "\n---| Hasil Pencarian Artis: " << artisCari << " |---" << endl;
        bool foundAny = false;
        for(int i = 0; i < (int)pool.size(); i++) {
            if(pool[i].lagu.artis == artisCari) {
                cetakHasilCari(pool[i]);
                foundAny = true;
            }
        }
        if(!foundAny) {
            cout << left << setw(12) << "Info" << ": Artis '" << artisCari << "' tidak ditemukan" << endl;
        }
    } 
    else {
        string judulCari;
        cout << "Masukkan judul lagu\t: ";
        cin.ignore();
        getline(cin, judulCari);

        vector<HasilCari> sortedPool = pool;
        sort(sortedPool.begin(), sortedPool.end(), [](const HasilCari& a, const HasilCari& b){
            return a.lagu.judulLagu < b.lagu.judulLagu;
        });

        vector<Musik> lagusorted;
        for(auto& h : sortedPool) lagusorted.push_back(h.lagu);

        int idx = -1;
        bool found = binarySearchJudul(lagusorted.data(), (int)lagusorted.size(), judulCari, idx);

        cout << "\n---| Hasil Pencarian Judul: " << judulCari << " |---" << endl;
        if(found) {
            int start = idx;
            while(start > 0 && sortedPool[start-1].lagu.judulLagu == judulCari) start--;
            int end = idx;
            while(end < (int)sortedPool.size()-1 && sortedPool[end+1].lagu.judulLagu == judulCari) end++;

            for(int i = start; i <= end; i++) {
                cetakHasilCari(sortedPool[i]);
            }
        } else {
            cout << left << setw(12) << "Info" << ": Judul '" << judulCari << "' tidak ditemukan" << endl;
        }
    }
    pause();
}

bool halamanMasuk(int &userindex) {
    int bataslogin = 0; 
    while(true){
        clearScreen();
        int opsi;
        printHeader("HALAMAN MASUK");
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihlah opsi\t: ";
        
        try {
            cin >> opsi;
            if(cin.fail()){
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
        } catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
            continue;
        }
        cout << endl << endl;

        if(opsi == 1) {
            Register(pengguna, jumlahuser);
        }
        else if(opsi == 2){
            int loginStatus = Login(pengguna, jumlahuser, userindex, bataslogin);
            if(loginStatus == 1)       return true;
            else if(loginStatus == -1) return false;
        }
        else if(opsi == 3){
            cout << left << setw(12) << "Info" << ": Keluar dari program" << endl;
            pause();
            return false;
        }
        else {
            cout << left << setw(12) << "Info" << ": Pilihlah berdasarkan opsi yang valid" << endl;
            pause();
        }
    }
}

void halamanUtamaAdmin(int &userindex) {
    while(true){
        clearScreen();
        int opsi;
        printHeader("HALAMAN UTAMA - ADMIN");
        cout << "1. Statistik Data" << endl;
        cout << "2. Lihat Seluruh Playlist" << endl;
        cout << "3. Buat Playlist Global" << endl;
        cout << "4. Update Playlist Global" << endl;
        cout << "5. Halaman Masuk" << endl;
        printLine();
        cout << "Pilih opsi\t: "; 
        
        try {
            cin >> opsi;
            if(cin.fail()){
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
        } catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
            continue;
        }
        cout << endl << endl;
        
        if(opsi == 1){
            tampilkanStatistikAdmin();
            pause(); 
        }
        else if(opsi == 2){
            tampilkanSemuaPlaylistDanGlobalAdmin();
            pause(); 
        }
        else if(opsi == 3){
            buatPlaylistGlobal(&pengguna[userindex]); 
        }
        else if(opsi == 4){
            updatePlaylistGlobal(); 
        }
        else if(opsi == 5){
            cout << left << setw(12) << "Info" << ": Kembali ke halaman masuk" << endl;
            pause();
            return;
        }
        else {
            cout << left << setw(12) << "Info" << ": Pilihan tidak valid" << endl;
            pause();
        }
    }
}

void halamanUtamaUser(int &userindex) {
    while(true){
        clearScreen();
        int opsi;
        printHeader("HALAMAN UTAMA");
        cout << "1. Buat Playlist" << endl;
        cout << "2. Lihat Playlist" << endl;
        cout << "3. Ubah Playlist" << endl;
        cout << "4. Hapus Data" << endl;
        cout << "5. Urutkan Data" << endl;
        cout << "6. Cari Data" << endl;
        cout << "7. Halaman Masuk" << endl;
        printLine();
        cout << "Pilih opsi\t: "; 
        
        try {
            cin >> opsi;
            if(cin.fail()){
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
        } catch(const exception& e) {
            cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
            continue;
        }
        cout << endl << endl;
        
        if(opsi == 1){
            buatPlaylist(&pengguna[userindex]);
        }
        else if(opsi == 2){
            lihatPlaylist(&pengguna[userindex]);
        }
        else if(opsi == 3){
            ubahPlaylist(&pengguna[userindex]);
        }
        else if(opsi == 4){
            hapusDataUser(pengguna, jumlahuser, userindex);
            if(userindex == -1) return;
        }
        else if(opsi == 5){
            menuSorting(&pengguna[userindex]);
        }
        else if(opsi == 6){
            menuSearching(userindex);
        }
        else if(opsi == 7){
            cout << left << setw(12) << "Info" << ": Kembali ke halaman masuk" << endl;
            pause();
            return;
        }
        else {
            cout << left << setw(12) << "Info" << ": Pilihan tidak valid" << endl;
            pause();
        }
    }
}

int main(){
    #ifdef _WIN32
        system("chcp 65001 >nul");
    #endif
    
    resetAllData();
    loadData();
    
    if (jumlahuser == 0) {
        pengguna[jumlahuser].username = "admin";
        pengguna[jumlahuser].password = "123";
        pengguna[jumlahuser].role = "admin";
        jumlahuser++;
    }
    
    while(true) {
        int userindex = -1;
        if(!halamanMasuk(userindex)) {
            saveData(); 
            break;
        }
        if(pengguna[userindex].role == "admin"){
            halamanUtamaAdmin(userindex);
        } else {
            halamanUtamaUser(userindex);
        }
    }
    return 0;
}