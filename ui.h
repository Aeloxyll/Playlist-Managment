#pragma once
#include <string>
#include "types.h"

using namespace std;

// ============================================================
// KONSTANTA WARNA & BORDER
// ============================================================

const string C_RESET  = "\033[0m";
const string C_RED    = "\033[31m";
const string C_GREEN  = "\033[32m";
const string C_YELLOW = "\033[33m";
const string C_BOLD   = "\033[1m";

const string LINE = "============================================================";
const string HEAD = "============================================================";

const int BOX_W = 36;

// ============================================================
// FUNGSI UTILITAS UI
// ============================================================

void   clearScreen();
void   printLine();
void   printHeader(const string& title);

string repeatStr(const string& s, int n);
int    visualWidth(const string& s);
string padLine(const string& s, int targetWidth);
string padBox(const string& content, int targetVW = 29);

void   boxTop();
void   boxBot();
void   boxSep();
void   boxLine(const string& s = "");
void   boxLineCenter(const string& s);
void   printSelectPrompt(int maxN);
void   pause();

// ============================================================
// FUNGSI TAMPILKAN KARTU PLAYLIST
// ============================================================

void tampilkanKartuPlaylistUser   (const Playlist&       pl, const string& pemilik, int nomor);
void tampilkanKartuPlaylistPublik (const Playlist&       pl, const string& pemilik, int nomor);
void tampilkanKartuPlaylistGlobal (const PlaylistGlobal& pg, int nomor);

// ============================================================
// FUNGSI TAMPILKAN DAFTAR LAGU (REKURSIF)
// ============================================================

void Daftarlagu      (const Playlist&       pl, size_t indexLagu = 0);
void DaftarLaguGlobal(const PlaylistGlobal& pg, size_t indexLagu = 0);

// ============================================================
// FUNGSI TAMPILKAN PLAYLIST
// ============================================================

void tampilkanPlaylistSaya         (const User& u);
void tampilkanSemuaPlaylistPublik  ();
void tampilkanPlaylistGlobal       ();
void tampilkanStatistikAdmin       ();
void tampilkanSemuaPlaylistDanGlobalAdmin();