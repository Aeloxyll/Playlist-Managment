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
    string privasi;   // "Publik" atau "Privat"
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
                cout << left << setw(12) << "Info" << ": Login berhasil" << endl;
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

void Datalagu(Musik *laguBaru) {
    cout << left << setw(12) << "Judul lagu" << ": ";
    cin.ignore();
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
            cout <<endl<<"Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!tahunvalid);
    laguBaru->tahun = Tahun;
}

void DatalaguGlobal(Musik *laguBaru) {
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
            cout <<endl<<"Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!tahunvalid);
    laguBaru->tahun = Tahun;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ==================== MODIFIKASI 1: buatPlaylist dengan privasi ====================
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
    
    // Input privasi
    string privasi;
    bool privasiValid = false;
    do {
        cout << left << setw(15) << "Privasi (Publik/Privat)" << ": ";
        cin >> privasi;
        privasi = toLowerCase(privasi);
        if (privasi == "publik" || privasi == "privat") {
            privasiValid = true;
            // Simpan dengan huruf pertama kapital
            if (privasi == "publik") u->musiklist[playlist].privasi = "Publik";
            else u->musiklist[playlist].privasi = "Privat";
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
            cout << '\n' <<"Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!jumlahvalid);

    for(int i = 0; i < jumlahlagu; i++){
        cout << endl << "Lagu ke-" << i + 1 << endl;   
        Datalagu(&u->musiklist[playlist].lagu[i]);
    }

    u->musiklist[playlist].jumlahlagu = jumlahlagu;
    u->jumlahPlaylist++; 
    cout << left << setw(12) << "Info" << ": Playlist berhasil dibuat" << endl;
    pause();
}

// ==================== MODIFIKASI 2: lihatPlaylist dengan 3 opsi ====================
void lihatPlaylist(User *u) {
    printHeader("LIHAT PLAYLIST");
    cout << "1. Playlist Saya" << endl;
    cout << "2. Playlist User Lain (Public)" << endl;
    cout << "3. Playlist Global" << endl;
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

    switch (opsi) {
        case 1: {
            // Playlist Saya
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
            break;
        }
        case 2: {
            // Playlist User Lain (Public)
            string targetUsername;
            cout << "Masukkan username: ";
            getline(cin, targetUsername);

            bool foundUser = false;
            for (int i = 0; i < jumlahuser; i++) {
                if (pengguna[i].username == targetUsername) {
                    foundUser = true;
                    if (pengguna[i].jumlahPlaylist == 0) {
                        cout << "User tersebut belum memiliki playlist." << endl;
                    } else {
                        cout << "Playlist public milik " << targetUsername << ":" << endl;
                        for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
                            if (pengguna[i].musiklist[j].privasi == "Publik") {
                                cout << HEAD << endl;
                                cout << left << setw(15) << "Nama Playlist" << ": " << pengguna[i].musiklist[j].judul << endl;
                                cout << left << setw(15) << "Jumlah Lagu" << ": " << pengguna[i].musiklist[j].jumlahlagu << " Lagu" << endl;
                                cout << HEAD << endl;
                                Daftarlagu(&pengguna[i].musiklist[j], 0);
                            }
                        }
                    }
                    break;
                }
            }
            if (!foundUser) {
                cout << "Username tidak ditemukan." << endl;
            }
            break;
        }
        case 3: {
            // Playlist Global (dari array playlistGlobal)
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
            break;
        }
        default:
            cout << "Pilihan tidak valid." << endl;
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
        if(u->musiklist[indexP].privasi == "Publik"){
            u->musiklist[indexP].privasi = "Privat";
        } else {
            u->musiklist[indexP].privasi = "Publik";
        }
        cout << left << setw(20) << "Status Baru" << ": " << u->musiklist[indexP].privasi << endl;
        cout << left << setw(12) << "Info" << ": Status privasi berhasil diperbarui" << endl;
    }
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
            cout << endl <<"Tekan Enter untuk melanjutkan...";
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
                cout << endl <<"Tekan Enter untuk melanjutkan...";
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
                cout << endl <<"Tekan Enter untuk melanjutkan...";
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
                    cout <<  '\n' <<"Tekan Enter untuk melanjutkan...";
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
        }
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
            cout << '\n' <<"Tekan Enter untuk melanjutkan...";
            cin.get();
        }
    }while(!jumlahvalid);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for(int i = 0; i < jumlahlagu; i++){
        cout << endl << "Lagu ke-" << i + 1 << endl;
        DatalaguGlobal(&playlistGlobal[idx].lagu[i]);
    }

    playlistGlobal[idx].jumlahlagu = jumlahlagu;
    playlistGlobal[idx].dibuatOleh = u->username;
    jumlahPlaylistGlobal++;
    cout << left << setw(12) << "Info" << ": Playlist Global berhasil dibuat" << endl;
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            DatalaguGlobal(&playlistGlobal[indexP].lagu[indexL]);
            cout << left << setw(12) << "Info" << ": Data lagu berhasil diubah" << endl;
        }
    }
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
        else if(arr[mid].judulLagu < target)
        {
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    return false;
}

void menuSearching(User *u){
    if(u->jumlahPlaylist == 0) {
        cout << left << setw(12) << "Info" << ": Anda belum memiliki Playlist untuk dicari" << endl;
        pause();
        return;
    }

    int metode;
    printHeader("MENU SEARCHING");
    cout << "1. Cari Artis" << endl;
    cout << "2. Cari Judul Lagu" << endl;
    cout << "Pilih metode\t: ";
    
    try {
        cin >> metode;
        if(cin.fail()){
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("angka");
        }
    } catch(const exception& e) {
        cout << left << setw(12) << "Info" << ": Input harus " << e.what() << endl;
        pause();
        return;
    }

    cout << endl << endl;
    int idx = -1;
    bool found = false;

    if(metode == 1) {
        string artisCari;
        cout << "Masukkan nama artis: ";
        cin.ignore();
        getline(cin, artisCari);
        
        bool foundAny = false;
        cout << "\n---| Hasil Pencarian |---" << endl;
        for(int p = 0; p < u->jumlahPlaylist; p++){
            for(int l = 0; l < u->musiklist[p].jumlahlagu; l++){
                if(linearSearchArtis(&u->musiklist[p].lagu[l], 1, artisCari, idx)) {
                    cout << "Playlist: " << u->musiklist[p].judul << endl;
                    cout << left << setw(10) << "Judul" << ": " << u->musiklist[p].lagu[l].judulLagu << endl;
                    cout << left << setw(10) << "Artis" << ": " << u->musiklist[p].lagu[l].artis << endl;
                    cout << left << setw(10) << "Genre" << ": " << u->musiklist[p].lagu[l].genre << endl;
                    cout << left << setw(10) << "Tahun" << ": " << u->musiklist[p].lagu[l].tahun << endl;
                    printLine();
                    foundAny = true;
                }
            }
        }
        if(!foundAny) cout << left << setw(12) << "Info" << ": Data artis tidak ditemukan" << endl;
    } 
    else if(metode == 2) {
        cout << "Daftar Playlist:" << endl;
        for(int i = 0; i < u->jumlahPlaylist; i++) {
            cout << i + 1 << ". " << u->musiklist[i].judul << endl;
        }
        
        int pIdx; 
        cout << "Pilih nomor playlist: "; 
        try {
            cin >> pIdx;
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("angka");
            }
            if (pIdx < 1 || pIdx > u->jumlahPlaylist) {
                throw out_of_range("Nomor playlist tidak valid");
            }
        } catch (const exception& e) {
            cout << left << setw(12) << "Info" << ": " << e.what() << endl;
            pause();
            return;
        }
        
        string judulCari; 
        cout << "Masukkan judul lagu: ";
        cin.ignore();
        getline(cin, judulCari);
        vector<Musik> temp(u->musiklist[pIdx-1].lagu, u->musiklist[pIdx-1].lagu + u->musiklist[pIdx-1].jumlahlagu);
        sort(temp.begin(), temp.end(), [](const Musik& a, const Musik& b){ return a.judulLagu < b.judulLagu; });
        
        found = binarySearchJudul(temp.data(), temp.size(), judulCari, idx);
        if(found){
            cout << "\n" << left << setw(12) << "Info" << ": Data ditemukan" << endl;
            cout << left << setw(10) << "Judul" << ": " << temp[idx].judulLagu << endl;
            cout << left << setw(10) << "Artis" << ": " << temp[idx].artis << endl;
            cout << left << setw(10) << "Genre" << ": " << temp[idx].genre << endl;
            cout << left << setw(10) << "Tahun" << ": " << temp[idx].tahun << endl;
        } else {
            cout << left << setw(12) << "Info" << ": Judul lagu tidak ditemukan" << endl;
        }
    }

    else {
        cout << left << setw(12) << "Info" << ": Pilihan metode tidak valid" << endl;
    }
    pause();
}

int dapatkanIndexPlaylist(const User* u) {
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
        }cout << endl << endl;

        if(opsi == 1)
        {
            Register(pengguna, jumlahuser);
        }
        else if(opsi == 2){
            int loginStatus = Login(pengguna, jumlahuser, userindex, bataslogin);
            if(loginStatus == 1)
            {
                return true;
            }
            else if(loginStatus == -1){
                return false;
            }
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

// ==================== MODIFIKASI 3: halamanUtamaAdmin dengan statistik dan lihat semua playlist ====================
void halamanUtamaAdmin(int &userindex) {
    while(true){
        clearScreen();
        int opsi;
        printHeader("HALAMAN UTAMA - ADMIN");
        cout << "1. Statistik Data" << endl;
        cout << "2. Lihat Seluruh Playlist (Semua User)" << endl;
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
        }cout << endl << endl;
        
        if (opsi == 1){
            // Statistik
            int totalUser = jumlahuser;
            int totalPlaylist = 0;
            int totalLagu = 0;
            for (int i = 0; i < jumlahuser; i++) {
                totalPlaylist += pengguna[i].jumlahPlaylist;
                for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
                    totalLagu += pengguna[i].musiklist[j].jumlahlagu;
                }
            }
            cout << "Statistik Data:" << endl;
            cout << left << setw(20) << "Total User" << ": " << totalUser << endl;
            cout << left << setw(20) << "Total Playlist" << ": " << totalPlaylist << endl;
            cout << left << setw(20) << "Total Lagu" << ": " << totalLagu << endl;
        }
        else if(opsi == 2){
            // Lihat seluruh playlist (semua user, semua privasi)
            bool ada = false;
            cout << "Seluruh Playlist (Semua User):" << endl;
            for (int i = 0; i < jumlahuser; i++) {
                if (pengguna[i].jumlahPlaylist > 0) {
                    ada = true;
                    cout << "User: " << pengguna[i].username << endl;
                    for (int j = 0; j < pengguna[i].jumlahPlaylist; j++) {
                        cout << HEAD << endl;
                        cout << left << setw(15) << "Nama Playlist" << ": " << pengguna[i].musiklist[j].judul << endl;
                        cout << left << setw(15) << "Privasi" << ": " << pengguna[i].musiklist[j].privasi << endl;
                        cout << left << setw(15) << "Jumlah Lagu" << ": " << pengguna[i].musiklist[j].jumlahlagu << " Lagu" << endl;
                        cout << HEAD << endl;
                        Daftarlagu(&pengguna[i].musiklist[j], 0);
                    }
                }
            }
            if (!ada) cout << "Belum ada playlist yang dibuat." << endl;
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
        // pause after each option (kecuali opsi 5 yang sudah pause sendiri)
        if (opsi != 5) pause();
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
        }cout << endl << endl;
        
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
            menuSearching(&pengguna[userindex]);
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
    pengguna[jumlahuser].username = "admin";
    pengguna[jumlahuser].password = "123";
    pengguna[jumlahuser].role = "admin";
    jumlahuser++;
    while(true) {
        int userindex = -1;
        if (!halamanMasuk(userindex)){
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