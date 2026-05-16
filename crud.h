#pragma once
#include "types.h"

// ============================================================
// CRUD PLAYLIST USER
// ============================================================

void buatPlaylist  (User* u);
void lihatPlaylist (User* u);
void ubahPlaylist  (User* u);
bool hapusDataUser (int userindex);

// ============================================================
// CRUD PLAYLIST GLOBAL (ADMIN)
// ============================================================

void buatPlaylistGlobal    ();
void updatePlaylistGlobal  ();
void hapusPlaylistGlobal   ();