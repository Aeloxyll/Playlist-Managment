#pragma once
#include <string>
#include <vector>

using namespace std;

// ============================================================
// STRUCT DATA
// ============================================================

typedef struct {
    string judulLagu;
    string artis;
    string genre;
    int tahun;
} Musik;

typedef struct {
    string judul;
    string privasi;
    vector<Musik> lagu;
} Playlist;

struct User {
    string username;
    string password;
    string role;
    vector<Playlist> musiklist;
};

typedef struct {
    string judul;
    vector<Musik> lagu;
    string dibuatOleh;
} PlaylistGlobal;

// ============================================================
// STRUCT PLAYLIST ENTRY (untuk admin view)
// ============================================================

struct PlaylistEntry {
    int  tipe;
    int  userIdx;
    int  playlistIdx;
    int  globalIdx;
    string judul;
    string pemilik;
};

// ============================================================
// STRUCT HASIL CARI (untuk searching)
// ============================================================

struct HasilCari {
    Musik  lagu;
    string namaPlaylist;
    string pemilik;
};