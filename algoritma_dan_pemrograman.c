#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Delay


struct User {
    char username[50];
    char password[50];
};

struct Admin {
    char username[50];
    char password[50];
};

struct Barang {
    char nama[50];
    float harga;
    int stok;
    char satuan[20];
};

struct Transaksi {
    char username[50];
    char barang[50];
    int jumlahBarang;
    float hargaTotal;
    char satuan[20];
};


int pelangganIndexLogin = -1;

struct Barang barang[100];  
int jumlahBarang = 0;  
struct Transaksi transaksi[50]; 
int jumlahTransaksi = 0; 
struct Barang daftarBarang[100];
int jumlahBarangDaftar = 0; 
struct User daftarPelanggan[50];

void daftarUser();
int loginAdmin();
int loginUser();
void bacaDataBarang(struct Barang *barang, int *jumlahBarang);

// Fungsi admin
void pengelolaanPengguna();
void tampilkanLaporan(struct Transaksi *transaksi, int jumlahTransaksi, const char *jenisTransaksi);
void laporanImpor();
void laporanEkspor();

void tampilkanListBarang(struct Barang *barang, int jumlahBarang);
void updateBarang(struct Barang *barang, int *jumlahBarang);
void pengaturanSistem(struct Barang *barang, int *jumlahBarang);
void menuAdmin();

// Fungsi user
void opsiMasukUser();
void prosesImpor();
void prosesEkspor();
void menuUser();
void gantiPasswordUser();

int main() {

    FILE *fileDataBarang = fopen("data_barang.dat", "ab+");
    if (fileDataBarang == NULL) {
    printf("Error: File data_barang.dat tidak dapat dibuka atau dibuat.\n");
    exit(1);
    }
    fclose(fileDataBarang);

    int pilihan;
    bacaDataBarang(daftarBarang, &jumlahBarang);

    do {
    system("cls");
    printf("\n===== SELAMAT DATANG DI SISTEM EKSPOR-IMPOR =====\n");
    printf("||  1. Pengguna Admin                         ||\n");
    printf("||  2. Pengguna User                          ||\n");
    printf("||  3. Keluar                                 ||\n");
    printf("================================================\n");
    printf("    Pilih menu (1-3): ");
    scanf("%d", &pilihan);
    
    switch (pilihan) {
        case 1:
            if (loginAdmin()) {
                menuAdmin();
            } else {
                printf("\nLogin sebagai Admin Gagal.\n");
                printf("Silahkan login kembali\n");
                sleep(1.5);
            }
            break;
        case 2:
            opsiMasukUser();
        case 3:
            printf("Keluar dari Sistem Ekspor-Impor.\n");
            pelangganIndexLogin = -1;
            break;
        default:
            printf("Pilihan tidak valid.\n");
            break;
        }
    } while (pilihan != 3);

    return 0;
}

//=== Fungsi-Fungsi ===

void bacaDataBarang(struct Barang *barang, int *jumlahBarang) {
    FILE *file = fopen("data_barang.dat", "rb");
    if (file == NULL) {
        printf("Error: File data_barang.dat tidak dapat dibuka.\n");
        exit(1);
    }

    *jumlahBarang = fread(barang, sizeof(struct Barang), 100, file);
    fclose(file);
}

void daftarUser() {
    FILE *file;
    struct User newUser;

    file = fopen("user_data.dat", "ab");
    if (file == NULL) {
        printf("Error: Gagal membuka file untuk penulisan.\n");
        exit(1);
    }

    printf("\n===Daftar User===\n");
    printf("Masukkan username: ");
    scanf("%s", newUser.username);

    printf("Masukkan password: ");
    scanf("%s", newUser.password);

    fwrite(&newUser, sizeof(struct User), 1, file);

    fclose(file);

    printf("Pengguna baru berhasil didaftarkan.\n");
    sleep(1.5);
    system("cls");
    opsiMasukUser();
}


int loginAdmin() {
    char username[20];
    char password[20];
    printf("\n============ LOGIN ADMIN ============\n");
    printf("Masukkan username Admin: ");
    scanf("%s", username);

    printf("Masukkan password Admin: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        printf("Login sebagai Admin berhasil.\n");
        return 1; // Return 1
    } else {
        return 0; // Return 0
    }
    sleep(1);
}

int loginUser() {
    FILE *file;
    struct User userLogin;
    char userUsername[50];
    char userPassword[50];

    file = fopen("user_data.dat", "rb");
    if (file == NULL) {
        printf("Error: File user_data.dat tidak ditemukan.\n");
        return 0; 
    }

    printf("\n============ LOGIN USER ============\n");
    printf("Masukkan username User: ");
    scanf("%s", userUsername);

    printf("Masukkan password User: ");
    scanf("%s", userPassword);

    while (fread(&userLogin, sizeof(struct User), 1, file) == 1) {
        if (strcmp(userUsername, userLogin.username) == 0 && strcmp(userPassword, userLogin.password) == 0) {
            pelangganIndexLogin = ftell(file) / sizeof(struct User) - 1; 
            fclose(file);
            printf("Login User berhasil.\n");
            return 1; 
        }
    }

    fclose(file);

    printf("Login User gagal.\n");
    return 0;
    sleep(1);
}

void pengelolaanPengguna() {
    FILE *file;
    char username[50];
    char password[50];

    file = fopen("user_data.dat", "rb"); 
    if (file == NULL) {
        printf("Error: File user_data.dat tidak ditemukan.\n");
        return;
    }

    printf("\n=== Daftar Pengguna ===");

    int nomorPengguna = 1;

    while (fread(&username, sizeof(char[50]), 1, file) == 1) {
        fread(&password, sizeof(char[50]), 1, file);
        printf("\n%d. Username:%s\n   Password:%s\n", nomorPengguna, username, password);
        nomorPengguna++ ;
    }

    fclose(file);
}

void tampilkanLaporan(struct Transaksi *transaksi, int jumlahTransaksi, const char *jenisTransaksi) {
    printf("\n=== Laporan %s ===\n", jenisTransaksi);
    printf("[No. Barang - Jumlah, Harga Total:]\n");

    for (int i = 0; i < jumlahTransaksi; i++) {
        if (strcmp(jenisTransaksi, "Impor") == 0) {
            printf("%d. %s - %d %s, %.2f IDR\n", i + 1, transaksi[i].barang, transaksi[i].jumlahBarang, transaksi[i].satuan, transaksi[i].hargaTotal);
        } else if (strcmp(jenisTransaksi, "Ekspor") == 0) {
            printf("%d. %s - %d %s, %.2f USD\n", i + 1, transaksi[i].barang, transaksi[i].jumlahBarang, transaksi[i].satuan, transaksi[i].hargaTotal);
        }
    }
}

void laporanImpor() {
    system("cls");
    FILE *file;
    struct Transaksi transaksi[50]; 
    int jumlahTransaksi = 0; 

    file = fopen("laporan_impor.dat", "rb");
    if (file == NULL) {
        printf("Error: File laporan_impor.dat tidak ditemukan.\n");
        return;
    }

    while (fread(&transaksi[jumlahTransaksi], sizeof(struct Transaksi), 1, file) == 1) {
        jumlahTransaksi++;
    }

    fclose(file);

    if (jumlahTransaksi > 0) {
        tampilkanLaporan(transaksi, jumlahTransaksi, "Impor");
    } else {
        printf("Tidak ada laporan impor.\n");
    }
}

void laporanEkspor() {
    FILE *file;
    struct Transaksi transaksi[50]; 
    int jumlahTransaksi = 0; 

    file = fopen("laporan_ekspor.dat", "rb");
    if (file == NULL) {
        printf("Error: File laporan_ekspor.dat tidak ditemukan.\n");
        return;
    }

    while (fread(&transaksi[jumlahTransaksi], sizeof(struct Transaksi), 1, file) == 1) {
        jumlahTransaksi++;
    }

    fclose(file);

    if (jumlahTransaksi > 0) {
        tampilkanLaporan(transaksi, jumlahTransaksi, "Ekspor");
    } else {
        printf("Tidak ada laporan ekspor.\n");
    }
}


void tampilkanListBarang(struct Barang *barang, int jumlahBarang) {
    printf("\n============= List Barang =============\n");
    for (int i = 0; i < jumlahBarang; i++) {
        // TEST
        if (barang[i].harga >= 0.01 && barang[i].harga <= 1999.0) {
            printf("%d. %s - Harga: %.2f USD/%s, Stok: %d %s\n", i + 1, barang[i].nama, barang[i].harga, barang[i].satuan, barang[i].stok, barang[i].satuan);
        } else {
            printf("%d. %s - Harga: %.2f IDR/%s, Stok: %d %s\n", i + 1, barang[i].nama, barang[i].harga, barang[i].satuan, barang[i].stok, barang[i].satuan);
        }
    }
}

void updateBarang(struct Barang *barang, int *jumlahBarang) {
    FILE *file = fopen("data_barang.dat", "rb+");
    if (file == NULL) {
        printf("Error: File data_barang.dat tidak dapat dibuka.\n");
        return;
    }

    fread(barang, sizeof(struct Barang), *jumlahBarang, file);

    fclose(file);

    tampilkanListBarang(barang, *jumlahBarang);

    int indeksBarang;
    printf("Masukkan nomor barang yang akan diubah: ");
    scanf("%d", &indeksBarang);

    if (indeksBarang < 1 || indeksBarang > *jumlahBarang) {
        printf("Error: Nomor barang tidak valid.\n");
        return;
    }

    indeksBarang--;

    printf("Masukkan harga baru untuk barang '%s': ", barang[indeksBarang].nama);
    scanf("%f", &barang[indeksBarang].harga);

    printf("Masukkan stok baru untuk barang '%s': ", barang[indeksBarang].nama);
    scanf("%d", &barang[indeksBarang].stok);

    file = fopen("data_barang.dat", "wb");
    if (file == NULL) {
        printf("Error: File data_barang.dat tidak dapat dibuka.\n");
        return;
    }

    fwrite(barang, sizeof(struct Barang), *jumlahBarang, file);

    fclose(file);

    printf("Barang '%s' berhasil diubah.\n", barang[indeksBarang].nama);

    printf("\nList Barang Setelah Diubah:");
    tampilkanListBarang(barang, *jumlahBarang);

}


void pengaturanSistem(struct Barang *barang, int *jumlahBarang) {
    int pilihan;

    do {
        printf("\n=== Menu Pengaturan Barang ===\n");
        printf("1. Update Barang\n");
        printf("2. Kembali ke Menu Admin\n");
        printf("Pilih menu (1-2): ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                updateBarang(barang, jumlahBarang);
                system("pause");
                system("cls");
                break;
            case 2:
                printf("Kembali ke Menu Admin.\n");
                system("cls");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
    } while (pilihan != 2);
}

void menuAdmin() {
    int pilihan;

    do {
        printf("\n======== Menu Admin ========\n");
        printf("||  1. Lihat Daftar Pengguna ||\n");
        printf("||  2. Laporan Transaksi     ||\n");
        printf("||  3. Pengaturan Barang     ||\n");
        printf("||  4. Keluar                ||\n");
        printf("===============================\n");
        printf("    Pilih menu (1-4): ");
        scanf("%d", &pilihan);
        system("cls");

        switch (pilihan) {
            case 1:
                pengelolaanPengguna();
                system("pause");
                system("cls");
                break;
            case 2:
                printf("\n=== Laporan Transaksi ===\n");
                printf("1. Laporan Impor\n");
                printf("2. Laporan Ekspor\n");
                printf("Pilih laporan (1-2): ");
                scanf("%d", &pilihan);
                if (pilihan == 1) {
                    laporanImpor();
                    system("pause");
                    system("cls");
                } else if (pilihan == 2) {
                    laporanEkspor();
                    system("pause");
                    system("cls");
                } else {
                    printf("Pilihan tidak valid.\n");
                }
                break;
            case 3:
                pengaturanSistem(daftarBarang, &jumlahBarang);
                break;
            case 4:
                printf("Keluar dari Menu Admin.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
    } while (pilihan != 4);
}

void prosesImpor() {
    FILE* fileImpor;
    struct Transaksi transaksi;
    int indeksBarang, jumlahBarangImpor;
    float hargaTotal;

    fileImpor = fopen("laporan_impor.dat", "ab+");
    if (fileImpor == NULL) {
        printf("Error: File laporan_impor.dat tidak dapat diakses.\n");
        return;
    }

    FILE* fileBarang = fopen("data_barang.dat", "rb");
    if (fileBarang == NULL) {
        printf("Error: File data_barang.dat tidak dapat diakses.\n");
        fclose(fileImpor);
        return;
    }

    printf("\n============= List Barang =============\n");
    struct Barang tempBarang;
    int nomorBarang = 1;

    while (fread(&tempBarang, sizeof(struct Barang), 1, fileBarang) == 1) {
        printf("%d. %s - Harga: %.2f USD/%s, Stok: %d %s\n", nomorBarang, tempBarang.nama, tempBarang.harga, tempBarang.satuan, tempBarang.stok, tempBarang.satuan);
        nomorBarang++;
    }

    fclose(fileBarang);

    printf("Masukkan nomor barang yang akan diimpor: ");
    scanf("%d", &indeksBarang);

    if (indeksBarang < 1 || indeksBarang > nomorBarang - 1) {
        printf("Error: Nomor barang tidak valid.\n");
        fclose(fileImpor);
        return;
    }

    fileBarang = fopen("data_barang.dat", "rb+");
    if (fileBarang == NULL) {
        printf("Error: File data_barang.dat tidak dapat diakses.\n");
        fclose(fileImpor);
        return;
    }

    fseek(fileBarang, (indeksBarang - 1) * sizeof(struct Barang), SEEK_SET);
    fread(&tempBarang, sizeof(struct Barang), 1, fileBarang);

    printf("Masukkan jumlah barang yang diimpor: ");
    scanf("%d", &jumlahBarangImpor);

    if (jumlahBarangImpor > tempBarang.stok) {
        printf("Error: Stok barang tidak cukup untuk diimpor.\n");
        fclose(fileImpor);
        fclose(fileBarang);
        return;
    }

    // buffer input clear
    getchar();

    hargaTotal = jumlahBarangImpor * tempBarang.harga * 15000;

    strcpy(transaksi.barang, tempBarang.nama);
    transaksi.jumlahBarang = jumlahBarangImpor;
    transaksi.hargaTotal = hargaTotal;
    strcpy(transaksi.satuan, tempBarang.satuan);

    tempBarang.stok -= jumlahBarangImpor;

    fseek(fileBarang, (indeksBarang - 1) * sizeof(struct Barang), SEEK_SET);
    fwrite(&tempBarang, sizeof(struct Barang), 1, fileBarang);

    fwrite(&transaksi, sizeof(struct Transaksi), 1, fileImpor);

    fclose(fileImpor);
    fclose(fileBarang);

    printf("\nProses impor telah berhasil:\n");
    printf("Barang: %s\n", transaksi.barang);
    printf("Harga Total (IDR): %.2f IDR\n", transaksi.hargaTotal);
    printf("Jumlah Stok yang Diimpor: %d %s\n", transaksi.jumlahBarang, tempBarang.satuan);

    bacaDataBarang(daftarBarang, &jumlahBarang);
}

void prosesEkspor() {
    FILE* file;
    struct Barang newBarang;

    file = fopen("data_barang.dat", "ab+");
    if (file == NULL) {
        printf("Error: File data_barang.dat tidak dapat diakses.\n");
        return;
    }

    printf("\n===Proses Ekspor===\n");
    printf("Masukkan nama barang: ");
    scanf("%s", newBarang.nama);

    printf("Masukkan harga barang (IDR): ");
    scanf("%f", &newBarang.harga);

    printf("Masukkan stok barang: ");
    scanf("%d", &newBarang.stok);

    printf("Masukkan satuan untuk barang (misalnya: item, kg): ");
    scanf("%s", newBarang.satuan);

    // USD convert
    newBarang.harga /= 15000;

    fwrite(&newBarang, sizeof(struct Barang), 1, file);

    fclose(file);

    struct Transaksi transaksi;
    transaksi.jumlahBarang = newBarang.stok; 
    transaksi.hargaTotal = newBarang.harga * transaksi.jumlahBarang; 
    strcpy(transaksi.satuan, newBarang.satuan);

    file = fopen("laporan_ekspor.dat", "ab+");
    if (file == NULL) {
        printf("Error: File laporan_ekspor.dat tidak dapat diakses.\n");
        return;
    }

    strcpy(transaksi.barang, newBarang.nama);
    
    fwrite(&transaksi, sizeof(struct Transaksi), 1, file);

    fclose(file);

    printf("Proses ekspor telah berhasil!!\n");

    printf("\nList Barang Setelah Proses Ekspor:\n");
    bacaDataBarang(daftarBarang, &jumlahBarang);
    file = fopen("data_barang.dat", "rb");

    if (file == NULL) {
        printf("Error: File data_barang.dat tidak dapat diakses.\n");
        return;
    }

    int nomorBarang = 1;
    while (fread(&newBarang, sizeof(struct Barang), 1, file) == 1) {
        printf("%d. %s - Harga: %.2f USD/%s, Stok: %d %s\n", nomorBarang, newBarang.nama, newBarang.harga, newBarang.satuan, newBarang.stok, newBarang.satuan);
        nomorBarang++;
    }

    system("pause");
    system("cls");

    fclose(file);
}

void opsiMasukUser() {
            printf("\nPilih opsi User:\n");
            printf("1. Login User\n");
            printf("2. Daftar User\n");
            printf("3. Kembali ke Login\n");
            printf("Masukkan pilihan Anda: ");
            int opsiUser;
            scanf("%d", &opsiUser);

            switch (opsiUser) {
                case 1:
                    if (loginUser()) {
                        menuUser();
                    } else {
                        printf("Login User gagal.\n");
                        sleep(1);
                    }
                    break;
                case 2:
                    daftarUser();
                    break;
                case 3:
                    main();
                    break;
                default:
                    printf("Opsi tidak valid. Silakan coba lagi.\n");
                    break;
}
}

void menuUser() {
    int pilihan;

    do {
        printf("\n======== Menu User ========\n");
        printf("||  1. Sub Menu Impor     ||\n");
        printf("||  2. Sub Menu Ekspor    ||\n");
        printf("||  3. Ganti Password     ||\n");
        printf("||  4. Keluar             ||\n");
        printf("============================\n");
        printf("    Pilih menu (1-4): ");
        scanf("%d", &pilihan);
        system("cls");

        switch (pilihan) {
            case 1:
                if (pelangganIndexLogin = -1) {
                    prosesImpor();
                } else {
                    printf("Anda belum login sebagai User.\n");
                }
                break;
            case 2:
                if (pelangganIndexLogin = -1) {
                    prosesEkspor();
                } else {
                    printf("Anda belum login sebagai User.\n");
                }
                break;
            case 3:
                gantiPasswordUser();
                break;
            case 4:
                printf("Keluar dari Menu User.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
    } while (pilihan != 4);
}

void gantiPasswordUser() {
    FILE *file;
    struct User user;
    char newPassword[50];

    file = fopen("user_data.dat", "rb+");
    if (file == NULL) {
        printf("Error: File user_data.dat tidak ditemukan.\n");
        return;
    }

    fseek(file, pelangganIndexLogin * sizeof(struct User), SEEK_SET);

    fread(&user, sizeof(struct User), 1, file);

    printf("\n=== Ganti Password ===\n");
    printf("Masukkan password baru: ");
    scanf("%s", newPassword);

    strcpy(user.password, newPassword);

    fseek(file, pelangganIndexLogin * sizeof(struct User), SEEK_SET);

    fwrite(&user, sizeof(struct User), 1, file);

    fclose(file);

    printf("Password berhasil diganti.\n");

    system("pause");
    system("cls");
}
