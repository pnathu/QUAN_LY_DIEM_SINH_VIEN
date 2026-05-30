#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include<time.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define HIGHLIGHT_BLUE "\033[46;30m"
#define HIGHLIGHT_YELLOW "\033[43;30m"
#define BOLD_CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BOLD "\x1b[1m"

#define MK_SV_MAC_DINH "123456"
#define MAX_GV_MON 3
#define mx 40

typedef struct {
    char maGV[20];
    char tenGV[50];
    char taiKhoan[30];
    char matKhau[30];
} GiangVien;

typedef struct {
    char maSV[20];
    char tenSV[50];
    char tenLopGoc[20];
    float lab1;
    float lab2;
    float pt1;
    float pt2;
    float pre;
    float final;
} SinhVien;

typedef struct {
    char maHP[10];
    char tenMon[50];
    float tinChi;       
    char congThuc[100];    
    int soLuongGVDay;
    char danhSachMaGV[3][20];
    int soLuongSV;
    int isRequired;
    SinhVien dsSV[mx];
} HocPhan;

typedef struct {
    char tenLop[20];
    int soLuongSV;
    SinhVien dsSV[mx];
} LopSinhHoat;

typedef struct {
    char tenLop[20];
    int soLuongHocPhan;
    HocPhan dsHocPhan[10];
    int tongTinChiHienTai; 
} LopHocPhan;

LopSinhHoat dsLopSinhHoat[2]; 
LopHocPhan dsLopHocPhan[10];  
int soLuongLopHP = 0;

GiangVien dsGiangVien[20];
GiangVien gvDangNhap;
int soLuongGV = 0; 

int lopDangChon, hocPhanDangChon;

void lockConsoleSize(int width, int height) {
    HWND consoleWindow = GetConsoleWindow();
    RECT r;
    GetWindowRect(consoleWindow, &r); 
    MoveWindow(consoleWindow, r.left, r.top, width, height, TRUE);
    HMENU hm = GetSystemMenu(consoleWindow, FALSE);
    RemoveMenu(hm, SC_MAXIMIZE, MF_BYCOMMAND);
    DrawMenuBar(consoleWindow);
}

void restoreConsoleSize(RECT *oldRect) {
    HWND consoleWindow = GetConsoleWindow();
    MoveWindow(consoleWindow, oldRect->left, oldRect->top,  oldRect->right - oldRect->left,  oldRect->bottom - oldRect->top, TRUE);
}

void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

int realLen(const char *text) {
    int len = 0;
    int i = 0;
    while (text[i] != '\0') {
        if (text[i] == '\033') { 
            while (text[i] != 'm' && text[i] != '\0') i++;
        } else {
            len++;
        }
        if (text[i] != '\0') i++;
    }
    return len;
}

void printCenter(const char *text) {
    int visibleLen = realLen(text);
    int pos = (120 - visibleLen) / 2; 
    if (pos < 0) pos = 0;
    
    int i;
    for (i = 0; i < pos; i++) printf(" ");
    printf("%s\n", text);
}

void xoaDongVuaNhap() {
    printf("\033[A\33[2K\r");
}

void taoSinhVien40(SinhVien ds[], int start, char* tenLop) {
    int i, j, trung;
    char *ho[] = {
        "Nguyen","Tran","Le","Pham","Hoang","Bui","Vu","Dang",
        "Do","Ngo","Duong","Ly","Dinh","Truong","Phan","Vo",
        "Huynh","Mai","Ta","Cao"
    };
    char *tenLot[] = {
        "Van","Thi","Minh","Duc","Quoc","Thanh","Gia","Bao",
        "Ngoc","Anh","Tuan","Khanh","Hong","Xuan","Huu"
    };
    char *ten[] = {
        "An","Binh","Cuong","Dung","Huy","Kiet","Lan","Nga",
        "Nam","Phuc","Son","Trang","Phuong","Linh","Thao",
        "Vy","Quan","Dat","Nhi","Yen","Tam","Long","Hieu",
        "Khanh","Tuan","My","Chau","Khoa","Truc","Ngan"
    };

    int soHo = sizeof(ho) / sizeof(ho[0]);
    int soTenLot = sizeof(tenLot) / sizeof(tenLot[0]);
    int soTen = sizeof(ten) / sizeof(ten[0]);

    for (i = 0; i < 40; i++) {
        do {
            trung = 0;

            char hoTmp[30];
            char lotTmp[30];
            char tenTmp[30];
            char tenDayDu[100];

            strcpy(hoTmp, ho[rand() % soHo]);
            strcpy(lotTmp, tenLot[rand() % soTenLot]);
            strcpy(tenTmp, ten[rand() % soTen]);

            sprintf(tenDayDu, "%s %s %s", hoTmp, lotTmp, tenTmp);

            for (j = 0; j < i; j++) {
                if (strcmp(ds[j].tenSV, tenDayDu) == 0) {
                    trung = 1;
                    break;
                }
            }
            if (!trung) {
                sprintf(ds[i].maSV, "10225%03d", start + i);
                strcpy(ds[i].tenSV, tenDayDu);
                strcpy(ds[i].tenLopGoc, tenLop);

                ds[i].lab1 = ds[i].lab2 = ds[i].pt1 =
                ds[i].pt2 = ds[i].pre = ds[i].final = -1;
            }
        } while (trung);
    }
}

void khoiTaoLopSinhHoat() {
    strcpy(dsLopSinhHoat[0].tenLop, "25T_DT1");
    dsLopSinhHoat[0].soLuongSV = 40;
    taoSinhVien40(dsLopSinhHoat[0].dsSV, 1, "25T_DT1");

    strcpy(dsLopSinhHoat[1].tenLop, "25T_DT2");
    dsLopSinhHoat[1].soLuongSV = 40;
    taoSinhVien40(dsLopSinhHoat[1].dsSV, 41, "25T_DT2");
}

float tinhTB_hp(SinhVien sv) {
    if (sv.lab1 == -1 || sv.lab2 == -1 || sv.pt1 == -1 || sv.pt2 == -1 || sv.pre == -1 || sv.final == -1) 
	    return -1;
    return sv.lab1 * 0.1 + sv.lab2 * 0.1 + sv.pt1  * 0.15 + sv.pt2  * 0.15 + sv.pre  * 0.2 + sv.final * 0.3;
}


void themGiangVien(char* ma, char* ten, char* tk, char* mk) {
    if (soLuongGV >= 20) {
        printf("Danh sach giang vien da day!\n");
        return; 
    }
    strcpy(dsGiangVien[soLuongGV].maGV, ma);
    strcpy(dsGiangVien[soLuongGV].tenGV, ten);
    strcpy(dsGiangVien[soLuongGV].taiKhoan, tk);
    strcpy(dsGiangVien[soLuongGV].matKhau, mk);
    soLuongGV++;
}

void khoiTaoTaiKhoanHeThong() {
    soLuongGV = 0;
    themGiangVien("GV01", "Tran Thi Binh", "giangvien1", "gv001");
    themGiangVien("GV02", "Le Van Chau", "giangvien2", "gv002");
    themGiangVien("GV03", "Nguyen Van Minh", "giangvien3", "gv003");
}

void loadDanhSachGVTuFile() {
    FILE *f = fopen("giangvien.txt", "r");
    if (!f) {
        printf("Khong tim thay file giangvien.txt, dung du lieu mac dinh.\n");
        khoiTaoTaiKhoanHeThong();
        return;
    }
    soLuongGV = 0;
    while (soLuongGV < 20 &&
           fscanf(f, "%s %s %s %s",
                  dsGiangVien[soLuongGV].maGV,
                  dsGiangVien[soLuongGV].taiKhoan,
                  dsGiangVien[soLuongGV].matKhau,
                  dsGiangVien[soLuongGV].tenGV) == 4) {
        soLuongGV++;
    }
    fclose(f);
}

void initMon(HocPhan *hp, char *ma, char *ten, char *gv, float tinChi) {
    strcpy(hp->maHP, ma);
    strcpy(hp->tenMon, ten);
    hp->tinChi = tinChi;
    hp->soLuongGVDay = 1;
    strcpy(hp->danhSachMaGV[0], gv);
    hp->soLuongSV = 0;
}

void khoiTaoDuLieuGoc() {
    soLuongLopHP = 2;
    
    strcpy(dsLopHocPhan[0].tenLop, "25Nh10");
    dsLopHocPhan[0].soLuongHocPhan = 6;


    initMon(&dsLopHocPhan[0].dsHocPhan[0], "KTLT", "Ky Thuat Lap Trinh", "GV01", 3);
    initMon(&dsLopHocPhan[0].dsHocPhan[1], "CTMT", "Cau Truc May Tinh", "GV02", 2);
    initMon(&dsLopHocPhan[0].dsHocPhan[2], "CTDL", "Cau Truc Du Lieu", "GV03", 2);
    initMon(&dsLopHocPhan[0].dsHocPhan[3], "TRR",  "Toan Roi Rac", "GV01", 3);
    initMon(&dsLopHocPhan[0].dsHocPhan[4], "PTT",  "Phuong Phap Tinh", "GV02", 3);
    initMon(&dsLopHocPhan[0].dsHocPhan[5], "CSDL", "Co So Du Lieu", "GV03", 2);

    strcpy(dsLopHocPhan[1].tenLop, "25Nh11");
    dsLopHocPhan[1].soLuongHocPhan = 6;

    initMon(&dsLopHocPhan[1].dsHocPhan[0], "KTLT", "Ky Thuat Lap Trinh", "GV02", 2);
    initMon(&dsLopHocPhan[1].dsHocPhan[1], "CTMT", "Cau Truc May Tinh", "GV01", 2);
    initMon(&dsLopHocPhan[1].dsHocPhan[2], "CTDL", "Cau Truc Du Lieu", "GV02", 2);
    initMon(&dsLopHocPhan[1].dsHocPhan[3], "TRR",  "Toan Roi Rac", "GV03", 3);
    initMon(&dsLopHocPhan[1].dsHocPhan[4], "PTT",  "Phuong Phap Tinh", "GV01", 3);
    initMon(&dsLopHocPhan[1].dsHocPhan[5], "CSDL", "Co So Du Lieu", "GV02", 2);
}

int lopDangChon;
int hocPhanDangChon;

int sinhVienDaCoLop(char *maSV) {
    int l, h, i;
    for (l = 0; l < soLuongLopHP; l++) {
        for (h = 0; h < dsLopHocPhan[l].soLuongHocPhan; h++) {
            HocPhan *hp = &dsLopHocPhan[l].dsHocPhan[h];
            for (i = 0; i < hp->soLuongSV; i++) {
                if (strcmp(hp->dsSV[i].maSV, maSV) == 0) {
                    if (hp->dsSV[i].final != -1)
                        return 1;
                }
            }
        }
    }
    return 0;
}

int sinhVienDaHocMonNayChua(char *maSV, char *tenMon) {
    int l, h, i;
    for (l = 0; l < soLuongLopHP; l++) {
        for (h = 0; h < dsLopHocPhan[l].soLuongHocPhan; h++) {
            HocPhan *hp = &dsLopHocPhan[l].dsHocPhan[h];
            if (strcmp(hp->tenMon, tenMon) == 0) {
                for (i = 0; i < hp->soLuongSV; i++) {
                    if (strcmp(hp->dsSV[i].maSV, maSV) == 0)
                        return 1;
                }
            }
        }
    }
    return 0;
}

void phanBoSinhVienVaoLop(LopHocPhan *lop, HocPhan *hp) {
    int count = 0;
    int maxSVPerHP = 40;
    int i, j;
    for (i = 0; i < 2; i++) {
        LopSinhHoat *lsh = &dsLopSinhHoat[i];
        for (j = 0; j < lsh->soLuongSV; j++) {
            if (count >= maxSVPerHP)
                break;
            if (sinhVienDaCoLop(lsh->dsSV[j].maSV) == 0) {
                hp->dsSV[count] = lsh->dsSV[j];
                count++;
            }
        }
        if (count >= maxSVPerHP)
            break;
    }
    hp->soLuongSV = count;
}

void luuFile(LopHocPhan *lop, HocPhan *hp) {
    char filename[100];
    sprintf(filename, "%s_%s.txt", lop->tenLop, hp->maHP);

    FILE *f = fopen(filename, "w");
    if (f == NULL) return;
    fprintf(f, "%d\n", hp->soLuongSV);
    int i;
    for (i = 0; i < hp->soLuongSV; i++) {
        SinhVien *sv = &hp->dsSV[i];
        fprintf(f, "%s|%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f\n", sv->maSV, sv->tenSV, sv->tenLopGoc, sv->lab1, sv->lab2, sv->pt1, sv->pt2, sv->pre, sv->final);
    }
    fclose(f);
}

void docFile(LopHocPhan *lop, HocPhan *hp) {
    char filename[100];
    sprintf(filename, "%s_%s.txt", lop->tenLop, hp->maHP);
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        phanBoSinhVienVaoLop(lop, hp);
        luuFile(lop, hp);
        return;
    }
    fscanf(f, "%d\n", &hp->soLuongSV);
    int i;
    for (i = 0; i < hp->soLuongSV; i++) {
        SinhVien *sv = &hp->dsSV[i];
        fscanf(f, "%19[^|]|%49[^|]|%19[^|]|%f|%f|%f|%f|%f|%f\n",
               sv->maSV, sv->tenSV,sv->tenLopGoc, &sv->lab1, &sv->lab2, &sv->pt1, &sv->pt2, &sv->pre, &sv->final);
    }
    fclose(f);
}

void luuToanBoHeThong() {
    FILE *f = fopen("hethong.txt", "w");
    if (!f) return;

    fprintf(f, "%d\n", 2); 

    int l, h;

    for (l = 0; l < 2; l++) {
        LopHocPhan *lop = &dsLopHocPhan[l];

        fprintf(f, "%s\n", lop->tenLop);
        fprintf(f, "%d\n", lop->soLuongHocPhan);

        for (h = 0; h < lop->soLuongHocPhan; h++) {
            HocPhan *hp = &lop->dsHocPhan[h];

            fprintf(f,
                    "%s|%s|%.1f|%d\n",
                    hp->maHP,
                    hp->tenMon,
                    hp->tinChi,
                    hp->isRequired);

            luuFile(lop, hp);
        }
    }

    fclose(f);
}

void docToanBoHeThong() {
    FILE *f = fopen("hethong.txt", "r");
    if (!f) return;

    int soLop;
    fscanf(f, "%d\n", &soLop);

    int l, h;

    for (l = 0; l < soLop; l++) {
        LopHocPhan *lop = &dsLopHocPhan[l];

        fgets(lop->tenLop, 20, f);
        lop->tenLop[strcspn(lop->tenLop, "\n")] = '\0';

        fscanf(f, "%d\n", &lop->soLuongHocPhan);

        for (h = 0; h < lop->soLuongHocPhan; h++) {
            HocPhan *hp = &lop->dsHocPhan[h];

            fscanf(f,
                   "%9[^|]|%49[^|]|%f|%d\n",
                   hp->maHP,
                   hp->tenMon,
                   &hp->tinChi,
                   &hp->isRequired);

            docFile(lop, hp);

            if (hp->soLuongSV == 0) {
                phanBoSinhVienVaoLop(lop, hp);
                luuFile(lop, hp);
            }
        }
    }
    fclose(f);
}

float thang4(float diem10) {
    if (diem10 >= 8.5) return 4.0;
    if (diem10 >= 7.0) return 3.0;
    if (diem10 >= 5.5) return 2.0;
    if (diem10 >= 4.0) return 1.0;
    return 0.0;
}

char xepLoai(float d) {
    if (d >= 8.5) return 'A';
    if (d >= 7.0) return 'B';
    if (d >= 5.5) return 'C';
    if (d >= 4.0) return 'D';
    return 'F';
}

void DanhSachSV(HocPhan *hp, char* tenLopSinhHoat) {
    printf(BOLD_CYAN "GV PHU TRACH: %s\n" RESET, gvDangNhap.tenGV);
    char title[200];
    int j, k;
    sprintf(title, BOLD_CYAN "\nMON: %s | LOP HOC PHAN: %s" RESET, hp->tenMon, tenLopSinhHoat);
    printCenter(title);
    printf(CYAN "+-----+------------+---------------------+--------------+------+------+------+------+------+------+------+-----------+\n" RESET);
    printf("| STT |  Ma SV     |      Ho ten         | Lop SH       | Lab1 | Lab2 | PT1  | PT2  | Pre  | Final|  TB  | Xep loai  |\n");
    printf(CYAN "+-----+------------+---------------------+--------------+------+------+------+------+------+------+------+-----------+\n" RESET);

    for (j = 0; j < hp->soLuongSV; j++) {
        printf("| %-3d | %-10s | %-19s | %-12s | ", j + 1, hp->dsSV[j].maSV, hp->dsSV[j].tenSV, hp->dsSV[j].tenLopGoc);
        float diem[] = { hp->dsSV[j].lab1, hp->dsSV[j].lab2, hp->dsSV[j].pt1, hp->dsSV[j].pt2, hp->dsSV[j].pre, hp->dsSV[j].final};
        for (k = 0; k < 6; k++) {
            if (diem[k] == -1)
                printf("%-4s | ", "N/A");
            else
                printf("%-4.1f | ", diem[k]);
        }
        float tb = tinhTB_hp(hp->dsSV[j]);
        if (tb == -1) {
            printf("%-4s | %-9s |\n", "N/A", "-");
        } else {
            printf("%-4.1f | %-9c |\n", tb, xepLoai(tb));
        }
    }
    printf(CYAN
    "+-----+------------+---------------------+--------------+------+------+------+------+------+------+------+-----------+\n" RESET);
}

void chooseRoleBox() {
    system("cls");
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();
    gotoxy(12, 3);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    gotoxy(12, 4);  printf(CYAN "|" RESET);
    gotoxy(46, 4);  printf(RED "HE THONG QUAN LY DIEM SINH VIEN" RESET);
    gotoxy(106, 4); printf(CYAN "|" RESET);
    gotoxy(12, 5);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    int y;
    for (y = 6; y <= 22; y++) {
        gotoxy(12, y);  printf(CYAN "|" RESET);
        gotoxy(106, y); printf(CYAN "|" RESET);
    }
    gotoxy(12, 23);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    gotoxy(35, 8);
    printf(YELLOW "+----------------------------------------------+" RESET);
    gotoxy(35, 9);
    printf(GREEN  "|                CHON DOI TUONG                |" RESET);    
    gotoxy(35, 10);
    printf(YELLOW "+----------------------------------------------+" RESET); 
    gotoxy(35, 11);
    printf(YELLOW "|                                              |" RESET);
    gotoxy(35, 12);
    printf(YELLOW "|        1. DANG NHAP GIANG VIEN               |" RESET);
    gotoxy(35, 13);
    printf(YELLOW "|                                              |" RESET);
    gotoxy(35, 14);
    printf(YELLOW "|        2. DANG NHAP SINH VIEN                |" RESET);
    gotoxy(35, 15);
    printf(YELLOW "|                                              |" RESET);
    gotoxy(35, 16);
    printf(YELLOW "|        0. THOAT                              |" RESET);
    gotoxy(35, 17);
    printf(YELLOW "|                                              |" RESET);
    gotoxy(35, 18);
    printf(YELLOW "+----------------------------------------------+" RESET);
    gotoxy(35, 20);
    printf(GREEN "Nhap lua chon cua ban: " RESET);
    showCursor();
}

void lecturerLoginBox() {
    system("cls");
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();
    gotoxy(12, 3);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    int y;
    for (y = 4; y <= 22; y++) {
        gotoxy(12, y);  printf(CYAN "|" RESET);
        gotoxy(106, y); printf(CYAN "|" RESET);
    }
    gotoxy(46, 4);
    printf(RED "HE THONG QUAN LY DIEM SINH VIEN" RESET);
    gotoxy(12, 5);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    gotoxy(12, 23);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    gotoxy(30, 8);
    printf(MAGENTA "+------------------------------------------------------------+" RESET);
    gotoxy(30, 9);
    printf(MAGENTA "|                  DANG NHAP TAI KHOAN GIANG VIEN            |" RESET);
    gotoxy(30, 10);
    printf(MAGENTA "+------------------------------------------------------------+" RESET);
    for (y = 11; y <= 18; y++) {
        gotoxy(30, y);
        printf(MAGENTA "|                                                            |" RESET);
    }
    gotoxy(32, 12);
    printf(MAGENTA "TEN DANG NHAP" RESET);
    gotoxy(50, 11);
    printf(MAGENTA "     +---------------------------+" RESET);
    gotoxy(50, 12);
    printf(MAGENTA "     |                           |" RESET);
    gotoxy(50, 13);
    printf(MAGENTA "     +---------------------------+" RESET);
    gotoxy(32, 16);
    printf(MAGENTA "MAT KHAU" RESET);
    gotoxy(50, 15);
    printf(MAGENTA "     +---------------------------+" RESET);
    gotoxy(50, 16);
    printf(MAGENTA "     |                           |" RESET);
    gotoxy(50, 17);
    printf(MAGENTA "     +---------------------------+" RESET);
    gotoxy(30, 19);
    printf(MAGENTA "+------------------------------------------------------------+" RESET);
}

void studentLoginBox() {
    system("cls");
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();
    gotoxy(12, 3);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    int y;
    for (y = 4; y <= 22; y++) {
        gotoxy(12, y);  printf(CYAN "|" RESET);
        gotoxy(106, y); printf(CYAN "|" RESET);
    }
    gotoxy(46, 4);
    printf(RED "HE THONG QUAN LY DIEM SINH VIEN" RESET);
    gotoxy(12, 5);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    gotoxy(12, 23);
    printf(CYAN "+---------------------------------------------------------------------------------------------+" RESET);
    gotoxy(30, 8);
    printf(GREEN "+------------------------------------------------------------+" RESET);
    gotoxy(30, 9);
    printf(GREEN "|                    DANG NHAP TAI KHOAN SINH VIEN           |" RESET);
    gotoxy(30, 10);
    printf(GREEN "+------------------------------------------------------------+" RESET);
    for (y = 11; y <= 18; y++) {
        gotoxy(30, y);
        printf(GREEN "|                                                            |" RESET);
    }
    gotoxy(32, 12);
    printf(GREEN "MA SINH VIEN" RESET);
    gotoxy(50, 11); printf(GREEN "     +---------------------------+" RESET);
    gotoxy(50, 12); printf(GREEN "     |                           |" RESET);
    gotoxy(50, 13); printf(GREEN "     +---------------------------+" RESET);
    gotoxy(32, 16);
    printf(GREEN "MAT KHAU" RESET);
    gotoxy(50, 15); printf(GREEN "     +---------------------------+" RESET);
    gotoxy(50, 16); printf(GREEN "     |                           |" RESET);
    gotoxy(50, 17); printf(GREEN "     +---------------------------+" RESET);
    gotoxy(30, 19);
    printf(GREEN "+------------------------------------------------------------+" RESET);
}

void loadingEffect(int x, int y) {
    gotoxy(x, y);
    printf(CYAN "Dang xu ly" RESET);
    int i;
    for (i = 0; i < 4; i++) {
        printf(CYAN "." RESET);
        fflush(stdout);
        Sleep(250);
    }
}

void nhapMatKhau(char mk[], int x, int y) {
    int i = 0;
    char ch;
    int hienMK = 0;
    while (1) {
        gotoxy(82, y);
        if(hienMK) {
            printf(GREEN "[SHOW]" RESET);
        }
        else {
            printf(YELLOW "[HIDE]" RESET);
        }
        gotoxy(x + i, y);
        ch = getch();
        if(ch == 13) {
            mk[i] = '\0';
            break;
        }
        else if(ch == 27) {
            strcpy(mk, "ESC_EXIT");
            return;
        }
        else if(ch == 8) {
            if(i > 0) {
                i--;
                mk[i] = '\0';
                gotoxy(x + i, y);
                printf(" ");
                gotoxy(x + i, y);
            }
        }
        else if(ch == 9) {
            hienMK = !hienMK;
            int j;
            gotoxy(x, y);
            printf("                    ");
            gotoxy(x, y);
            for(j = 0; j < i; j++) {
                if(hienMK) {
                    printf("%c", mk[j]);
                }
                else {
                    printf("*");
                }
            }
        }
        else if(i < 19 && ch != 32) {
            mk[i++] = ch;
            if(hienMK) {
                printf("%c", ch);
            }
            else {
                printf("*");
            }
        }
    }
}

int dangNhapHeThong() {
    char tk[30], mk[30];
    int i;
    while (1) {
        system("cls");
        lecturerLoginBox();
        gotoxy(57, 12);
        showCursor();
        scanf("%29s", tk);
        if (strcmp(tk, "0") == 0)
            return 0;
        gotoxy(57, 16);
        nhapMatKhau(mk, 57, 16);
        if(strcmp(mk, "ESC_EXIT") == 0) {
            return 0;
        }
        hideCursor();
        loadingEffect(52, 21);
        if (soLuongGV == 0) {
            gotoxy(42, 21);
            printf(RED "CHUA CO GIANG VIEN TRONG HE THONG!" RESET);
            getch();
            return 0;
        }
        int timThay = 0;
        for (i = 0; i < soLuongGV; i++) {
            if (strcmp(tk, dsGiangVien[i].taiKhoan) == 0 &&
                strcmp(mk, dsGiangVien[i].matKhau) == 0) {
                gvDangNhap = dsGiangVien[i];
                timThay = 1;
                break;
            }
        }
        if (timThay) {
            gotoxy(42, 21);
            printf("                                                          ");
            gotoxy(34, 21);
            printf(GREEN "[ V ] CHAO MUNG GV: %s DANG NHAP THANH CONG!" RESET, gvDangNhap.tenGV);
            fflush(stdout);
            Sleep(1200);
            return 1; 
        }
        else {
            gotoxy(42, 21);
            printf("                                             ");
            gotoxy(42, 21);
            printf(RED "[ X ] TAI KHOAN HOAC MAT KHAU KHONG CHINH XAC!" RESET);
            gotoxy(45, 22);
            printf(YELLOW "Nhan phim bat ky de thu lai..." RESET);
            getch();
        }
    }
}

char svDangNhap[20];

int timKiemSinhVienToanLop(char ma[], LopHocPhan *lop) {
	int i, j;
    for (j = 0; j < lop->soLuongHocPhan; j++) {
        HocPhan *hp = &lop->dsHocPhan[j];
        for (i = 0; i < hp->soLuongSV; i++) {
            if (strcmp(ma, hp->dsSV[i].maSV) == 0)
                return i;
        }
    }
    return -1;
}

int timKiemSinhVienHeThong(char *maSV) {
    int i, j, k;
    for (i = 0; i < soLuongLopHP; i++) {
        LopHocPhan *lop = &dsLopHocPhan[i];
        for (j = 0; j < lop->soLuongHocPhan; j++) {
            HocPhan *hp = &lop->dsHocPhan[j];
            for (k = 0; k < hp->soLuongSV; k++) {
                if (strcmp(hp->dsSV[k].maSV, maSV) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int dangNhapSinhVien() {
    char tk[30], mk[30];
    while (1) {
        system("cls");
        studentLoginBox();
        gotoxy(57, 12);
        showCursor();
        scanf("%29s", tk);
        if (strcmp(tk, "0") == 0)
            return 0;
        gotoxy(57, 16);
        nhapMatKhau(mk, 57, 16);
        if(strcmp(mk, "ESC_EXIT") == 0) {
            return 0;
        }
        hideCursor();
        loadingEffect(52, 21);
        int tonTai = timKiemSinhVienHeThong(tk);
		if (tonTai == 1 && strcmp(mk, MK_SV_MAC_DINH) == 0) {
            strcpy(svDangNhap, tk);
            gotoxy(48, 21);
            printf(GREEN "[ V ] DANG NHAP THANH CONG!" RESET);
            Sleep(1200);
            return 1;
        } else {
            gotoxy(42, 21);
            printf(RED "[ X ] TEN DANG NHAP HOAC MAT KHAU SAI!" RESET);
            gotoxy(45, 22);
            printf(YELLOW "Nhan phim bat ky de nhap lai..." RESET);
            getch();
        }
    }
}

int gvCoDayHocPhan(char* maGV, HocPhan *hp) {
    int i;
    for (i = 0; i < hp->soLuongGVDay; i++) {
        if (strcmp(hp->danhSachMaGV[i], maGV) == 0)
            return 1;
    }
    return 0;
}

void ganGVVaoHP(HocPhan *hp, char* maGV) {
    if (!hp || !maGV) return;
    if (gvCoDayHocPhan(maGV, hp)) {
        printf("Giang vien nay da duoc gan cho hoc phan nay!\n");
        return;
    }
    if (hp->soLuongGVDay < MAX_GV_MON) {
        int n = hp->soLuongGVDay;
        strcpy(hp->danhSachMaGV[n], maGV);
        hp->soLuongGVDay++;
        printf("Da gan thanh cong GV %s vao hoc phan %s.\n",
               maGV, hp->tenMon);
    } else {
        printf("Loi: Hoc phan %s da du %d giang vien!\n",
               hp->tenMon, MAX_GV_MON);
    }
}

int menuHocPhan() {
    int chonHP;
    int l, j;
    system("cls");
    int d;
    for (d = 0; d < 8; d++)
        printf("\n");
    printCenter(CYAN "+------------------------------------------------+" RESET);
    printCenter(CYAN "|                 CHON HOC PHAN                  |" RESET);
    printCenter(CYAN "+------------------------------------------------+" RESET);
    int listLopIdx[50];
    int listHPIdx[50];
    int count = 0;
    for (l = 0; l < soLuongLopHP; l++) {
        LopHocPhan *lop = &dsLopHocPhan[l];
        for (j = 0; j < lop->soLuongHocPhan; j++) {
            if (gvCoDayHocPhan(gvDangNhap.maGV, &lop->dsHocPhan[j])) {
                char dong[120];
                sprintf(dong, "| %d. %-18s (Lop: %-8s)          |", count + 1, lop->dsHocPhan[j].tenMon, lop->tenLop);
                printCenter(dong);
                listLopIdx[count] = l;
                listHPIdx[count] = j;
                count++;
            }
        }
    }
    printCenter(CYAN "+------------------------------------------------+" RESET);
    if (count == 0) {
        printf("\n");
        printCenter(RED "BAN KHONG DAY MON NAO!" RESET);
        getch();
        return 0;
    }
    printf("\n");
    printf("%48s", "");
    printf(YELLOW "NHAP LUA CHON: " RESET);

    if (scanf("%d", &chonHP) != 1 || chonHP < 1 || chonHP > count)
        return 0;
    lopDangChon = listLopIdx[chonHP - 1];
    hocPhanDangChon = listHPIdx[chonHP - 1];
    
    LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
    HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];

    system("cls");
    char thongBao[120];
    sprintf(thongBao, GREEN "BAN DA CHON MON: %s - LOP: %s" RESET, hp->tenMon, lop->tenLop);
    printCenter(thongBao);
    docFile(lop, hp);
    return 1;
}

void menuGiangVien(HocPhan *hp, char* tenLop) {
    if (!hp || !tenLop) return;
    system("cls");
    int d;
    for (d = 0; d < 8; d++) printf("\n");
    printf(CYAN);
    printCenter("+----------------------------------------------------------+" );
    char row1[120], row2[120], row3[120];
    sprintf(row1, "|%23s%s%23s|", "", "QUAN LY DIEM", "");
    sprintf(row2, "| GV: %-52s |", gvDangNhap.tenGV);
    printCenter(row1);
    printCenter(row2);
    printCenter("|----------------------------------------------------------|");
    printCenter("| 1. Nhap diem sinh vien                                   |");
    printCenter("| 2. Xem diem chi tiet                                     |");
    printCenter("| 3. Sua diem da nhap                                      |");
    printCenter("| 4. Sap xep danh sach                                     |");
    printCenter("| 5. In bang thong ke                                      |");
    printCenter("| 6. Thoat                                                 |");
    printCenter("+----------------------------------------------------------+");
    printf(RESET);
    printf("\n%40s" YELLOW "NHAP LUA CHON CUA BAN: " RESET, "");
}

void layTenChinh(char hoTen[], char tenChinh[]) {
    int len = strlen(hoTen);
    int i = len - 1;
    while (i >= 0 && hoTen[i] == ' ')
        i--;
    if (i < 0) {
        tenChinh[0] = '\0';
        return;
    }
    int ketThucTen = i;
    while (i >= 0 && hoTen[i] != ' ')
        i--;
    int idx = 0;
    int j;
    for (j = i + 1; j <= ketThucTen; j++) {
        tenChinh[idx++] = hoTen[j];
    }
    tenChinh[idx] = '\0';
}

const char* layChuoiCongThuc() {
    return "L1*0.1+L2*0.1+P1*0.15+P2*0.15+Pre*0.2+Final*0.3";
}

void fmt(float diem, char s[]) {
    if (diem == -1)
        sprintf(s, "%-4s", "N/A");
    else
        sprintf(s, "%-4.1f", diem);
}

void inBangTongHop(char maSV[]) {
    printCenter("+----------------------+----+---------------------------------------------------+------+------+------+------+------+-------+------+------+----+");
    printCenter("| TEN HOC PHAN         | TC | CONG THUC                                         | L1   | L2   | P1   | P2   | PRE  | FINAL | T10  | T4   | XL |");
    printCenter("+----------------------+----+---------------------------------------------------+------+------+------+------+------+-------+------+------+----+");
    float tongDiemTichLuy10 = 0;
    float tongDiemTichLuy4  = 0;
    int tongTinChi = 0;
    int i, j, s;
    for (i = 0; i < soLuongLopHP; i++) {
        LopHocPhan lop = dsLopHocPhan[i];
        for (j = 0; j < lop.soLuongHocPhan; j++) {
            HocPhan hp = lop.dsHocPhan[j];
            for (s = 0; s < hp.soLuongSV; s++) {
                if (strcmp(hp.dsSV[s].maSV, maSV) == 0) {
                    SinhVien sv = hp.dsSV[s];
                    float d10 = tinhTB_hp(sv);
                    float d4 = (d10 < 0) ? 0.0 : thang4(d10);
                    char xl = (d10 < 0) ? '-' : xepLoai(d10);
                    
                    char sD4[10];
					if (d10 < 0)
    					sprintf(sD4, "%4s", "N/A");
					else
    					sprintf(sD4,  "%4.1f", d4);
                    
                    if (d10 >= 0) {
                        tongDiemTichLuy10 += d10 * hp.tinChi;
                        tongDiemTichLuy4 += d4 * hp.tinChi;
                        tongTinChi += (int)hp.tinChi;
                    }
                    char sL1[10], sL2[10];
                    char sP1[10], sP2[10];
                    char sPre[10], sFi[10];
                    char sD10[10];

                    fmt(sv.lab1, sL1);
                    fmt(sv.lab2, sL2);

                    fmt(sv.pt1, sP1);
                    fmt(sv.pt2, sP2);

                    fmt(sv.pre, sPre);
                    fmt(sv.final, sFi);

                    if (d10 < 0)
                        sprintf(sD10, "%-4s", "N/A");
                    else
                        sprintf(sD10, "%-4.1f", d10);

                    char row[400];
					sprintf(row, "| %-20s | %2.0f | %-49s | %-4s | %-4s | %-4s | %-4s | %-4s | %-5s | %-4s | %-4s | %-2c |",
							hp.tenMon, hp.tinChi, layChuoiCongThuc(), sL1, sL2, sP1, sP2, sPre, sFi, sD10, sD4, xl);
                    printCenter(row);
                    break;
                }
            }
        }
    }
    printCenter("+----------------------+----+---------------------------------------------------+------+------+------+------+------+-------+------+------+----+");
    if (tongTinChi > 0) {
        float gpa10 = tongDiemTichLuy10 / tongTinChi;
        float gpa4 = tongDiemTichLuy4 / tongTinChi;
        printf("\n");
        printf(GREEN ">> GPA THANG 10: %.2f\n" ">> GPA THANG 4 : %.2f\n" ">> TONG TIN CHI: %d\n" RESET, gpa10, gpa4, tongTinChi);
    }
    else {
        printf(YELLOW "\n>> CHUA CO DU LIEU DIEM!\n" RESET);
    }
}

void menuSinhVien(char maTruyenVao[]) {
    int i, k, s;
    int timThay = 0;
    system("cls");
    RECT oldRect;
    HWND hwnd = GetConsoleWindow();
    GetWindowRect(hwnd, &oldRect);
    lockConsoleSize(1100, 600);
    printf("\n" GREEN "----- THONG TIN CA NHAN -----" RESET "\n");
    for (i = 0; i < soLuongLopHP && !timThay; i++) {
        LopHocPhan lop = dsLopHocPhan[i];
        for (k = 0; k < lop.soLuongHocPhan && !timThay; k++) {
            HocPhan hp = lop.dsHocPhan[k];
            for (s = 0; s < hp.soLuongSV; s++) {
                if (strcmp(hp.dsSV[s].maSV, maTruyenVao) == 0) {
                    printf(YELLOW "   MSSV          : " RESET "%s\n", hp.dsSV[s].maSV);
                    printf(YELLOW "   HO VA TEN     : " RESET "%s\n", hp.dsSV[s].tenSV);
                    printf(YELLOW "   LOP SINH HOAT : " RESET "%s\n", hp.dsSV[s].tenLopGoc);
                    timThay = 1;
                    break;
                }
            }
        }
    }
    if (!timThay) {
        printf(RED "Khong tim thay sinh vien!" RESET);
        getch();
        return;
    }
    printf("\n");
    inBangTongHop(maTruyenVao);
    printf("\n");
    printCenter(YELLOW "Nhan phim bat ky de tiep tuc..." RESET);
    getch();
    restoreConsoleSize(&oldRect);
}

int khoaLab1 = 0;
int khoaLab2 = 0;
int khoaPT1 = 0;
int khoaPT2 = 0;
int khoaPre = 0;
int khoaFinal = 0;

void luuTrangThaiCot(HocPhan *hp) {
    char fileTrangThai[100];
    sprintf(fileTrangThai, "%s_trangthai.txt", hp->maHP);
    FILE *f = fopen(fileTrangThai, "w");
    if (f == NULL)
        return;
    fprintf(f, "%d %d %d %d %d %d", khoaLab1, khoaLab2, khoaPT1, khoaPT2, khoaPre, khoaFinal);
    fclose(f);
}

void docTrangThaiCot(HocPhan *hp) {
    char fileTrangThai[100];
    sprintf(fileTrangThai, "%s_trangthai.txt", hp->maHP);
    FILE *f = fopen(fileTrangThai, "r");
    if (f == NULL) {
        khoaLab1 = 0;
        khoaLab2 = 0;
        khoaPT1 = 0;
        khoaPT2 = 0;
        khoaPre = 0;
        khoaFinal = 0;
        return;
    }
    fscanf(f, "%d %d %d %d %d %d", &khoaLab1, &khoaLab2, &khoaPT1, &khoaPT2, &khoaPre, &khoaFinal);
    fclose(f);
}

float NhapMotDiem(HocPhan *hp, char tenCot[], float diemCu) {
    char s[50];
    float diem;
    int daKhoa = 0;
    if (strcmp(tenCot, "Lab1") == 0) daKhoa = khoaLab1;
    else if (strcmp(tenCot, "Lab2") == 0) daKhoa = khoaLab2;
    else if (strcmp(tenCot, "PT1") == 0) daKhoa = khoaPT1;
    else if (strcmp(tenCot, "PT2") == 0) daKhoa = khoaPT2;
    else if (strcmp(tenCot, "Pre") == 0) daKhoa = khoaPre;
    else if (strcmp(tenCot, "Final") == 0) daKhoa = khoaFinal;
    if (daKhoa == 1) {
        printCenter(RED "[!] Cot diem da duoc XAC NHAN va BI KHOA!" RESET);
        Sleep(1000);
        return diemCu;
    }
    while (1) {
        if (diemCu != -1) printf("Nhap %s (Hien tai: %.1f, ENTER = GIU NGUYEN, q = DUNG): ", tenCot, diemCu);
        else printf("Nhap %s (q = DUNG): ", tenCot);
        if (fgets(s, sizeof(s), stdin) == NULL) continue;
        s[strcspn(s, "\n")] = '\0';
        if (strcmp(s, "q") == 0 || strcmp(s, "Q") == 0) return -999;
        if (strlen(s) == 0 && diemCu != -1) return diemCu;
        char *endPtr;
        diem = strtof(s, &endPtr);
        if (*endPtr == '\0' && diem >= 0.0 && diem <= 10.0) return diem;
        printf(RED "\n[!] Diem khong hop le. Vui long nhap tu 0 -> 10!\n" RESET);
        Sleep(800);
        xoaDongVuaNhap();
        xoaDongVuaNhap();
    }
}

void NhapTheoCot(LopHocPhan *lop, HocPhan *hp, char tenCot[]) {
    int i;
    int *khoaCot = NULL;
    float *diemSV;
    if (strcmp(tenCot, "Lab1") == 0) khoaCot = &khoaLab1;
    else if (strcmp(tenCot, "Lab2") == 0) khoaCot = &khoaLab2;
    else if (strcmp(tenCot, "PT1") == 0) khoaCot = &khoaPT1;
    else if (strcmp(tenCot, "PT2") == 0) khoaCot = &khoaPT2;
    else if (strcmp(tenCot, "Pre") == 0) khoaCot = &khoaPre;
    else if (strcmp(tenCot, "Final") == 0) khoaCot = &khoaFinal;
    if (khoaCot == NULL) return;
    if (*khoaCot == 1) {
    	system("cls");
    	char buffer[100];
    	sprintf(buffer, "COT %s DA DUOC XAC NHAN", tenCot);
	    printf(GREEN);
	    printCenter(buffer);
	    printf(RESET);
    	printCenter(YELLOW "NHAN PHIM BAT KY DE QUAY LAI" RESET);
	    getch();
    	return;
	}
    int daNhapDu = 1;
    for (i = 0; i < hp->soLuongSV; i++) {
        if (strcmp(tenCot, "Lab1") == 0) diemSV = &hp->dsSV[i].lab1;
        else if (strcmp(tenCot, "Lab2") == 0) diemSV = &hp->dsSV[i].lab2;
        else if (strcmp(tenCot, "PT1") == 0) diemSV = &hp->dsSV[i].pt1;
        else if (strcmp(tenCot, "PT2") == 0) diemSV = &hp->dsSV[i].pt2;
        else if (strcmp(tenCot, "Pre") == 0) diemSV = &hp->dsSV[i].pre;
        else diemSV = &hp->dsSV[i].final;
        if (*diemSV == -1) { daNhapDu = 0; break; }
    }
    if (daNhapDu == 1) {
        while (1) {
            system("cls");
			char buffer[200];
			sprintf(buffer, BOLD_CYAN "============= COT DIEM %s =============" RESET, tenCot);
			printCenter(buffer);
			printCenter(GREEN "Cot diem da duoc nhap day du!" RESET);
			printCenter("1. Xac nhan cot diem");
			printCenter("0. Quay lai");
			printf("\n");
			printCenter("Nhap lua chon: ");
            int chon;
            if (scanf("%d", &chon) != 1) { while(getchar() != '\n'); continue; }
            while (getchar() != '\n');
            if (chon == 1) {
                *khoaCot = 1;
                luuTrangThaiCot(hp);
                luuFile(lop, hp);
                printf(GREEN "\n[THANH CONG] Da xac nhan cot %s!" RESET, tenCot);
                Sleep(1200);
                return;
            } else if (chon == 0) return;
            printf(RED "\n[!] Lua chon khong hop le!" RESET);
            Sleep(700);
        }
    }
    system("cls");
    printf(BOLD_CYAN "\n================ NHAP DIEM %s ================\n" RESET, tenCot);
    int soDaNhap = 0;
	for(i = 0; i < hp->soLuongSV; i++) {
    	float *diem;
	    if (strcmp(tenCot, "Lab1") == 0)
    	    diem = &hp->dsSV[i].lab1;
    	else if (strcmp(tenCot, "Lab2") == 0)
        	diem = &hp->dsSV[i].lab2;
    	else if (strcmp(tenCot, "PT1") == 0)
        	diem = &hp->dsSV[i].pt1;
    	else if (strcmp(tenCot, "PT2") == 0)
        	diem = &hp->dsSV[i].pt2;
    	else if (strcmp(tenCot, "Pre") == 0)
        	diem = &hp->dsSV[i].pre;
    	else
        	diem = &hp->dsSV[i].final;
	    if (*diem != -1) soDaNhap++;
	}
	printf(GREEN "\nDa nhap %d/%d sinh vien\n" RESET, soDaNhap, hp->soLuongSV);
	for (i = 0; i < hp->soLuongSV; i++) {
	    if (strcmp(tenCot, "Lab1") == 0)
    	    diemSV = &hp->dsSV[i].lab1;
    	else if (strcmp(tenCot, "Lab2") == 0)
        	diemSV = &hp->dsSV[i].lab2;
    	else if (strcmp(tenCot, "PT1") == 0)
        	diemSV = &hp->dsSV[i].pt1;
    	else if (strcmp(tenCot, "PT2") == 0)
        	diemSV = &hp->dsSV[i].pt2;
    	else if (strcmp(tenCot, "Pre") == 0)
        	diemSV = &hp->dsSV[i].pre;
    	else
        	diemSV = &hp->dsSV[i].final;
	    if (*diemSV != -1) continue;
	    printf("\n" CYAN "[%02d] %-30s" RESET, i + 1, hp->dsSV[i].tenSV);
	    float tam = NhapMotDiem(hp, tenCot, -1);
	    if (tam == -999) {
    	    printCenter(YELLOW "\n[THONG BAO] Da dung nhap giua chung!" RESET);
        	Sleep(1200);
        	return;
    	}
	    *diemSV = tam;
    	luuFile(lop, hp);
    	printf(GREEN "    -> Da luu %.1f" RESET, tam);
	}
    printCenter(GREEN "\n[THONG BAO] Da nhap xong cot diem!" RESET);
    Sleep(1000);
}

void InMenuCot(int stt, HocPhan *hp, char tenCot[]) {
    int *daKhoa = NULL;
    int daNhapDu = 1, i;
    if (strcmp(tenCot, "Lab1") == 0) daKhoa = &khoaLab1;
    else if (strcmp(tenCot, "Lab2") == 0) daKhoa = &khoaLab2;
    else if (strcmp(tenCot, "PT1") == 0) daKhoa = &khoaPT1;
    else if (strcmp(tenCot, "PT2") == 0) daKhoa = &khoaPT2;
    else if (strcmp(tenCot, "Pre") == 0) daKhoa = &khoaPre;
    else if (strcmp(tenCot, "Final") == 0) daKhoa = &khoaFinal;
    for (i = 0; i < hp->soLuongSV; i++) {
        float *diem;
        if (strcmp(tenCot, "Lab1") == 0) diem = &hp->dsSV[i].lab1;
        else if (strcmp(tenCot, "Lab2") == 0) diem = &hp->dsSV[i].lab2;
        else if (strcmp(tenCot, "PT1") == 0) diem = &hp->dsSV[i].pt1;
        else if (strcmp(tenCot, "PT2") == 0) diem = &hp->dsSV[i].pt2;
        else if (strcmp(tenCot, "Pre") == 0) diem = &hp->dsSV[i].pre;
        else diem = &hp->dsSV[i].final;

        if (*diem == -1) { daNhapDu = 0; break; }
    }
    if (daKhoa && *daKhoa == 1) 
        printf(GREEN "%d. Nhap cot %-8s [DA XAC NHAN]\n" RESET, stt, tenCot);
    else if (daNhapDu == 1) 
        printf(GREEN "%d. Nhap cot %-8s [DA NHAP - CHO XAC NHAN]\n" RESET, stt, tenCot);
    else 
        printf(YELLOW "%d. Nhap cot %-8s [CHUA XAC NHAN]\n" RESET, stt, tenCot);
}

void Nhap() {
    char *dsTenCot[] = {"Lab1", "Lab2", "PT1", "PT2", "Pre", "Final"};
    while (1) {
    	if (menuHocPhan() == 0) return;
    	int i;
        LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
        HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];
        docTrangThaiCot(hp);
        while (1) {
            system("cls");
            char line[150];
            sprintf(line, "=============== DU LIEU DIEM: %s ===============", hp->tenMon);
            printf(BOLD_CYAN); printCenter(line); printf(RESET);
            DanhSachSV(hp, lop->tenLop);
            printf(BOLD_CYAN "\n+------------------------------------------+\n"
                   "|               MENU NHAP DIEM             |\n"
                   "+------------------------------------------+\n" RESET);

            for (i = 0; i < 6; i++) InMenuCot(i + 1, hp, dsTenCot[i]);
            printf("\n0. Quay lai" GREEN "\n\nNhap lua chon: " RESET);
            int chon;
            if (scanf("%d", &chon) != 1) { 
				while(getchar() != '\n'); 
				continue; 
			}
            while (getchar() != '\n');

            if (chon >= 1 && chon <= 6) {
                NhapTheoCot(lop, hp, dsTenCot[chon - 1]);
                luuFile(lop, hp);
            } else if (chon == 0) {
                break;
            } else {
                printf(RED "\n[!] Lua chon khong hop le!" RESET);
                Sleep(700);
            }
        }
    }
}


int binary_search(char x[]) {
    return -1;
}

void XemDiemMotSV(LopHocPhan *lop, char maTruyenVao[], char monSua[], int cotSua) {
    char nhap[50], nhapUP[50];
    int i, j, k, d, cheDoXemDocLap = 0;
    float l1, l2, p1, p2, pr, fi, tb;
    int indexGoc[40];
    int soLuongTimThay = 0;
    int nCuaLop = lop->dsHocPhan[0].soLuongSV;
    if (strcmp(maTruyenVao, "") == 0) {
        cheDoXemDocLap = 1;
    } else {
        strcpy(nhap, maTruyenVao);
    }
    while (1) {
        if (cheDoXemDocLap) {
            system("cls");
            printf("\n");
            printCenter(BOLD_CYAN "+------------------------------------------+" RESET);
            printCenter(BOLD_CYAN "|          TRA CUU DIEM MOT SINH VIEN      |" RESET);
            printCenter(BOLD_CYAN "+------------------------------------------+" RESET);
            while (1) {
                soLuongTimThay = 0;
                printf("\n");
                for(d = 0; d < 40; d++) printf(" ");
                printf(YELLOW "Nhap MSSV hoac TEN SINH VIEN (0 de huy): " RESET);
                fgets(nhap, sizeof(nhap), stdin);
                nhap[strcspn(nhap, "\n")] = '\0';
                if (strcmp(nhap, "0") == 0) return;
                if (strlen(nhap) == 0) {
                    xoaDongVuaNhap();
                    continue;
                }
				k = timKiemSinhVienToanLop(nhap, lop);
				if (k != -1)
    				break;
                strcpy(nhapUP, nhap);
                strupr(nhapUP);
                for (i = 0; i < nCuaLop; i++) {
                    char tChinh[50];
                    layTenChinh(lop->dsHocPhan[0].dsSV[i].tenSV, tChinh);
                    strupr(tChinh);
                    if (strstr(tChinh, nhapUP) != NULL) {
                        indexGoc[soLuongTimThay] = i;
                        soLuongTimThay++;
                    }
                }
                if (soLuongTimThay == 0) {
                    xoaDongVuaNhap();
                    continue;
                }
                if (soLuongTimThay == 1) {
                    k = indexGoc[0];
                    break;
                }
                char tempRow[200];
                printf("\n");
                sprintf(tempRow, GREEN "Tim thay %d sinh vien phu hop:" RESET, soLuongTimThay);
                printCenter(tempRow);
                printCenter("+-----+---------------+--------------------------------+");
                printCenter("| STT |     MSSV      |           HO VA TEN            |");
                printCenter("+-----+---------------+--------------------------------+");
                for (i = 0; i < soLuongTimThay; i++) {
                    int idx = indexGoc[i];
                    sprintf(tempRow, "| %-3d | %-13s | %-30s |", i + 1, lop->dsHocPhan[0].dsSV[idx].maSV, lop->dsHocPhan[0].dsSV[idx].tenSV);
                    printCenter(tempRow);
                }
                printCenter("+-----+---------------+--------------------------------+");
                int chon;
                while (1) {
                    printf("\n");
                    printf("                     " BOLD_CYAN "Nhap STT sinh vien muon xem (0 de nhap lai): " RESET);
                    if (scanf("%d", &chon) != 1) {
                        while (getchar() != '\n');
                        xoaDongVuaNhap();
                        continue;
                    }
                    while (getchar() != '\n');
                    if (chon == 0) break;
                    if (chon >= 1 && chon <= soLuongTimThay) {
                        k = indexGoc[chon - 1];
                        break;
                    }
                    xoaDongVuaNhap();
                }
                if (chon == 0) continue;
                break;
            }
        } else {
            k = timKiemSinhVienToanLop(nhap, lop);
            if (k == -1) return;
        }
        system("cls");
        printf("\n");
        printf("\n" GREEN "----- THONG TIN SINH VIEN -----" RESET "\n");
        printf(YELLOW "   MSSV      : " RESET "%s\n", lop->dsHocPhan[0].dsSV[k].maSV);
        printf(YELLOW "   HO VA TEN : " RESET "%s\n", lop->dsHocPhan[0].dsSV[k].tenSV);
        printf(YELLOW "   LOP SINH HOAT : " RESET "%s\n", lop->dsHocPhan[0].dsSV[k].tenLopGoc);
        printf("\n");
        printCenter("----------------------- BANG DIEM CUA SINH VIEN -----------------------");
        printf("\n");
        printCenter("+------------------------+------+------+------+------+------+-------+------+");
		printCenter("|        Hoc phan        | Lab1 | Lab2 | PT1  | PT2  | Pre  | Final |  TB  |");
		printCenter("+------------------------+------+------+------+------+------+-------+------+");
		int soHpCuaLop = lop->soLuongHocPhan;
		for (i = 0; i < soHpCuaLop; i++) {
    		if (!gvCoDayHocPhan(gvDangNhap.maGV, &lop->dsHocPhan[i]))
        		continue;
    		char file[100], row[300];
		    sprintf(file, "%s_%s.txt", lop->tenLop, lop->dsHocPhan[i].maHP);
		    FILE *f = fopen(file, "r");
            int found = 0;
            l1 = l2 = p1 = p2 = pr = fi = -1.0;
            if (f) {
                int sl;
                if (fscanf(f, "%d\n", &sl) == 1) {
					char maF[20], tenTam[100], lopTam[30];
					for (j = 0; j < sl; j++) {
						if (fscanf(f, "%[^|]|%[^|]|%[^|]|%f|%f|%f|%f|%f|%f\n", maF,tenTam,lopTam,&l1, &l2, &p1, &p2, &pr, &fi) == 9) {
                            if (strcmp(maF, lop->dsHocPhan[0].dsSV[k].maSV) == 0) {
                                found = 1;
                                break;
                            }
                        }
                    }
                }
                fclose(f);
            }
            char sL1[30], sL2[30], sP1[30], sP2[30];
            char sPr[30], sFi[30], sTb[30];
            int laMonSua =
                (strcmp(lop->dsHocPhan[i].tenMon, monSua) == 0);
            if (found) {

                if (l1 == -1.0) strcpy(sL1, "N/A ");
                else if (laMonSua && cotSua == 1)
                    sprintf(sL1, GREEN "%-4.1f" RESET, l1);
                else sprintf(sL1, "%-4.1f", l1);

                if (l2 == -1.0) strcpy(sL2, "N/A ");
                else if (laMonSua && cotSua == 2)
                    sprintf(sL2, GREEN "%-4.1f" RESET, l2);
                else sprintf(sL2, "%-4.1f", l2);

                if (p1 == -1.0) strcpy(sP1, "N/A ");
                else if (laMonSua && cotSua == 3)
                    sprintf(sP1, GREEN "%-4.1f" RESET, p1);
                else sprintf(sP1, "%-4.1f", p1);

                if (p2 == -1.0) strcpy(sP2, "N/A ");
                else if (laMonSua && cotSua == 4)
                    sprintf(sP2, GREEN "%-4.1f" RESET, p2);
                else sprintf(sP2, "%-4.1f", p2);

                if (pr == -1.0) strcpy(sPr, "N/A ");
                else if (laMonSua && cotSua == 5)
                    sprintf(sPr, GREEN "%-4.1f" RESET, pr);
                else sprintf(sPr, "%-4.1f", pr);

                if (fi == -1.0) strcpy(sFi, "N/A ");
                else if (laMonSua && cotSua == 6)
                    sprintf(sFi, GREEN "%-4.1f" RESET, fi);
                else sprintf(sFi, "%-4.1f", fi);
                
				if (l1 == -1.0 || l2 == -1.0 || p1 == -1.0 || p2 == -1.0 || pr == -1.0 || fi == -1.0) { 
				    strcpy(sTb, "N/A ");
				} else {
    				SinhVien svTam;
    				svTam.lab1  = l1;
    				svTam.lab2  = l2;
    				svTam.pt1   = p1;
    				svTam.pt2   = p2;
    				svTam.pre   = pr;
    				svTam.final = fi;
				    tb = tinhTB_hp(svTam);
				    sprintf(sTb, "%-4.1f", tb);
				}
				sprintf(row, "| %-22s | %-4s | %-4s | %-4s | %-4s | %-4s | %-5s | %-4s |", lop->dsHocPhan[i].tenMon, sL1, sL2, sP1, sP2, sPr, sFi, sTb);
            	} else {
					sprintf(row, "| %-22s | %-4s | %-4s | %-4s | %-4s | %-4s | %-5s | %-4s |", lop->dsHocPhan[i].tenMon,"N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A");
            }
            printCenter(row);
        }
        printCenter("+------------------------+------+------+------+------+------+-------+------+");
		printf("\n");
        if (!cheDoXemDocLap) return;
        printCenter(YELLOW "Nhan phim bat ky de tiep tuc tra cuu..." RESET);
        getch();
    }
}

void XemDiemMotLopHocPhan() {
    while (1) {
        if (menuHocPhan() == 0) return;
        LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
        HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];
        system("cls");
        printf("\n");
        DanhSachSV(hp, lop->tenLop);
        printf("\n");
        printCenter(YELLOW "Nhan phim bat ky de quay lai..." RESET);
        getch();
    }
}

void Xem() {
    int ch;
    int d;
    do {
        system("cls");
        for (d = 0; d < 10; d++)
            printf("\n");
        printf(GREEN);
        printCenter("+-------------------------------------------+");
        printCenter("|               MUC XEM DIEM                |");
        printCenter("+-------------------------------------------+");
        printf(RESET);
        printCenter("|  1. Xem diem mot sinh vien                |");
        printCenter("|  2. Xem diem mot lop hoc phan             |");
        printCenter("|  0. Quay lai Menu chinh                   |");
        printf(GREEN);
        printCenter("+-------------------------------------------+");
        printf(RESET);
        printf("\n");
        for (d = 0; d < 48; d++)
            printf(" ");
        printf("Lua chon cua ban: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        switch (ch) {
            case 1:
                XemDiemMotSV(&dsLopHocPhan[lopDangChon], "", "", 0);
                break;
            case 2:
                XemDiemMotLopHocPhan();
                break;
            case 0:
                break;
            default:
                printCenter(RED "Lua chon khong hop le!" RESET);
                getch();
        }
    } while (ch != 0);
}

int daSuaCot(int cot, int dsCotSua[], int soCotSua) {
    int i;
    for(i = 0; i < soCotSua; i++) {
        if(dsCotSua[i] == cot) return 1;
    }
    return 0;
}

int cotDaKhoaTheoMon(int cot) {
    if (cot == 1) return khoaLab1;
    if (cot == 2) return khoaLab2;
    if (cot == 3) return khoaPT1;
    if (cot == 4) return khoaPT2;
    if (cot == 5) return khoaPre;
    if (cot == 6) return khoaFinal;
    return 0;
}

void Sua() {
    char nhapTimKiem[100], tuKhoaUp[100], ma[20];
    int chonCot, tiep, i, j, k, d, soLuongTimThay, chon, indexGoc[100];
    while (1) {
    	if (menuHocPhan() == 0) return;
    	while (1) {
    		LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
    		HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];
        while (1) {
            system("cls");
            DanhSachSV(hp, lop->tenLop);
            printCenter(BOLD_CYAN "+------------------------------------------+" RESET);
            printCenter(BOLD_CYAN "|          CHINH SUA DIEM SINH VIEN        |" RESET);
            printCenter(BOLD_CYAN "+------------------------------------------+" RESET);
            printf("\n");
            for(d = 0; d < 40; d++) printf(" ");
            printf(YELLOW "Nhap MSSV hoac TEN SINH VIEN (0 de huy): " RESET);
            fflush(stdin);
            fgets(nhapTimKiem, sizeof(nhapTimKiem), stdin);
            nhapTimKiem[strcspn(nhapTimKiem, "\n")] = '\0';
            if (strcmp(nhapTimKiem, "0") == 0) break;
            if (strlen(nhapTimKiem) == 0) continue;
			k = timKiemSinhVienToanLop(nhapTimKiem, lop);
			if (k != -1) {
			    strcpy(ma, nhapTimKiem);
    			break;
			}      
    		strcpy(tuKhoaUp, nhapTimKiem);
            strupr(tuKhoaUp);
            soLuongTimThay = 0;
            for (i = 0; i < hp->soLuongSV; i++) {
                char tChinh[50];
                layTenChinh(hp->dsSV[i].tenSV, tChinh);
                strupr(tChinh);
                if (strstr(tChinh, tuKhoaUp) != NULL) { 
					indexGoc[soLuongTimThay++] = i; 
				}
            }
            if (soLuongTimThay == 0) {
                printf("\n");
                printCenter(RED "(!) Khong tim thay sinh vien nao phu hop. Vui long nhap lai!\n" RESET);
                Sleep(1200);
                continue;
            }
            if (soLuongTimThay == 1) { 
				k = indexGoc[0]; 
				strcpy(ma, hp->dsSV[k].maSV); 
				break; 
			}
            printf("\n");
            char tieuDe[100];
            sprintf(tieuDe, GREEN "Tim thay %d sinh vien phu hop:" RESET, soLuongTimThay);
            printCenter(tieuDe);
            printCenter(BOLD_CYAN "+-----+---------------+-------------------------------------+" RESET);
            printCenter(BOLD_CYAN "| STT |      MSSV     |             HO VA TEN               |" RESET);
            printCenter(BOLD_CYAN "+-----+---------------+-------------------------------------+" RESET);
            for (i = 0; i < soLuongTimThay; i++) {
                char row[150];
                sprintf(row, "| %-3d | %-13s | %-35s |", i + 1, hp->dsSV[indexGoc[i]].maSV, hp->dsSV[indexGoc[i]].tenSV);
                printCenter(row);
            }
            printCenter(BOLD_CYAN "+-----+---------------+-------------------------------------+" RESET);
            int hopLe = 0;
            while (1) {
                printf("\n");
                for(d = 0; d < 22; d++) printf(" ");
                printf(CYAN "Nhap STT sinh vien muon sua (0 de nhap lai): " RESET);
                if (scanf("%d", &chon) != 1) { 
					while (getchar() != '\n'); 
					xoaDongVuaNhap(); 
					continue; 
				}
                if (chon == 0) break;
                if (chon >= 1 && chon <= soLuongTimThay) { 
					k = indexGoc[chon - 1]; 
					strcpy(ma,hp->dsSV[k].maSV); 
					hopLe = 1; 
					break; 
				}
                xoaDongVuaNhap();
            }
			if (hopLe) break;
		}
		if (strcmp(nhapTimKiem, "0") == 0)
    		break;
		int daCoItNhatMotDauDiem = 0;
        char fileCheck[100];
        sprintf(fileCheck, "%s_%s.txt", lop->tenLop, hp->maHP);
        FILE *fCheck = fopen(fileCheck, "r");
        if (fCheck) {
        	int slCheck;
            if (fscanf(fCheck, "%d\n", &slCheck) == 1) {
                char maF[20], tenT[100];
                float l1, l2, p1, p2, pr, fi;
                for (j = 0; j < slCheck; j++) {
                    char lopT[50]; 
					if (fscanf(fCheck, "%[^|]|%[^|]|%[^|]|%f|%f|%f|%f|%f|%f\n",
    					maF, tenT, lopT, &l1, &l2, &p1, &p2, &pr, &fi) == 9) {
						if (strcmp(maF, ma) == 0 && (l1 != -1.0 || l2 != -1.0 || p1 != -1.0 || p2 != -1.0 || pr != -1.0 || fi != -1.0)) {
                            daCoItNhatMotDauDiem = 1;
                        }
                    }
                }
            }
            fclose(fCheck);
        }
        if (!daCoItNhatMotDauDiem) {
            system("cls");
            printf("\n\n\n");
            printCenter(RED "(!) Sinh vien nay chua duoc nhap bat ky diem nao. Khong the sua!" RESET);
            fflush(stdout); Sleep(1500);
            continue;
        }
        char fileSua[100];
        sprintf(fileSua, "%s_%s.txt", lop->tenLop, hp->maHP);
        docTrangThaiCot(hp);
        int slTam, timThaySVTrongFile = 0;
        char maTam[40][20], tenTam[40][100], lopTam[40][50];;
        float bL1[40], bL2[40], bP1[40], bP2[40], bPr[40], bFi[40], diemHienTai = -1.0;
        FILE *fIn = fopen(fileSua, "r");
        if (fIn) {
            if (fscanf(fIn, "%d\n", &slTam) == 1) {
                for (i = 0; i < slTam; i++) {
            		if (fscanf(fIn, "%[^|]|%[^|]|%[^|]|%f|%f|%f|%f|%f|%f\n",
                		maTam[i], tenTam[i], lopTam[i], &bL1[i], &bL2[i], &bP1[i], &bP2[i], &bPr[i], &bFi[i]) == 9) {
                            if (strcmp(maTam[i], ma) == 0) { 
								timThaySVTrongFile = 1; 
								k = i; 
							}
                        }
                    }
                }
                fclose(fIn);
            }
            if (!timThaySVTrongFile) { printf(RED "(!) Sinh vien khong co trong hoc phan nay.\n" RESET); getch(); continue; }
            system("cls");
            printf("\n");
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printCenter(BOLD_CYAN "|                                BANG DIEM HOC PHAN CAN CHINH SUA                                  |" RESET);
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            char dongMon[150];
            sprintf(dongMon, YELLOW "MON HOC: %s" RESET, hp->tenMon);
            printCenter(dongMon);
            printf("\n");
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printCenter(BOLD_CYAN "|      MSSV       |          HO TEN         |  Lab1  |  Lab2  |  PT1   |  PT2   |   Pre  |  Final  |" RESET);
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            char row[300];
            char sL1[10], sL2[10], sP1[10], sP2[10], sPr[10], sFi[10];
			if (bL1[k] == -1.0) strcpy(sL1, "N/A");
			else sprintf(sL1, "%.1f", bL1[k]);

			if (bL2[k] == -1.0) strcpy(sL2, "N/A");
			else sprintf(sL2, "%.1f", bL2[k]);
			
			if (bP1[k] == -1.0) strcpy(sP1, "N/A");
			else sprintf(sP1, "%.1f", bP1[k]);

			if (bP2[k] == -1.0) strcpy(sP2, "N/A");
			else sprintf(sP2, "%.1f", bP2[k]);

			if (bPr[k] == -1.0) strcpy(sPr, "N/A");
			else sprintf(sPr, "%.1f", bPr[k]);

			if (bFi[k] == -1.0) strcpy(sFi, "N/A");
			else sprintf(sFi, "%.1f", bFi[k]);

			sprintf(row, "| %-15s | %-23s | %-6s | %-6s | %-6s | %-6s | %-6s | %-7s |",
					maTam[k], tenTam[k], sL1, sL2, sP1, sP2, sPr, sFi);
            printCenter(row);
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printf("\n");
            int soCotSua;
            printf(GREEN "BAN MUON SUA BAO NHIEU COT DIEM? " RESET);
            while (1) {
                printf(CYAN "(0 de thoat, 1 - 6): " RESET);
                    if (scanf("%d", &soCotSua) == 1) {
        				while(getchar() != '\n');
				        if (soCotSua == 0)
            				break;
				        if (soCotSua >= 1 && soCotSua <= 6)
            				break;
    				}
				xoaDongVuaNhap();
            }
            if (soCotSua == 0)
    			continue;
            int dsCotSua[6];
            printf(GREEN "CHON CAC COT CAN SUA CUA MON %s:" RESET "\n" MAGENTA "1.Lab1 2.Lab2 3.PT1 4.PT2 5.Pre 6.Final" RESET "\n",hp->maHP);
            for(i = 0; i < soCotSua; i++) {
                while (1) {
                    printf(CYAN "Nhap cot thu %d (1-6): " RESET, i + 1);
                    if (scanf("%d", &chonCot) != 1) { 
						while(getchar() != '\n'); 
						xoaDongVuaNhap(); 
						continue; 
					}
                    while(getchar() != '\n');
                    if (chonCot < 1 || chonCot > 6) { 
						printf("Cot khong hop le!\n"); 
						continue; 
					}
                    if (cotDaKhoaTheoMon(chonCot)) {
    					printf(RED "(!) Cot nay da DUOC KHOA - KHONG DUOC SUA!\n" RESET);
    					Sleep(1200);
    					continue;
					}
					int trung = 0;
                    for(j = 0; j < i; j++) 
						if(dsCotSua[j] == chonCot) 
							trung = 1;
                    if(trung) { 
						printf(RED "(!) Cot nay da duoc chon!\n" RESET); 
						Sleep(1000); 
						continue; 
					}
                    float dHienTai = (chonCot==1) ? bL1[k] : (chonCot==2) ? bL2[k] : (chonCot==3) ? bP1[k] : (chonCot==4) ? bP2[k] : (chonCot==5) ? bPr[k] : bFi[k];
                    if (dHienTai == -1.0) { 
						printf(RED "(!) Cot nay chua co diem de sua!\n" RESET); 
						Sleep(1000); 
						continue; 
					}
                    dsCotSua[i] = chonCot; 
					break;
                }
            }
            int dungSua = 0;
            for(i = 0; i < soCotSua; i++) {
                chonCot = dsCotSua[i];
                if (cotDaKhoaTheoMon(chonCot)) {
    				printf(RED "(!) Cot da khoa - KHONG DUOC SUA\n" RESET);
    				continue;
				}
				diemHienTai = (chonCot==1) ? bL1[k] : (chonCot==2) ? bL2[k] : (chonCot==3) ? bP1[k] : (chonCot==4) ? bP2[k] : (chonCot==5) ? bPr[k] : bFi[k];
				char tenCot[20];
				if(chonCot == 1) strcpy(tenCot, "Lab1");
				else if(chonCot == 2) strcpy(tenCot, "Lab2");
				else if(chonCot == 3) strcpy(tenCot, "PT1");
				else if(chonCot == 4) strcpy(tenCot, "PT2");
				else if(chonCot == 5) strcpy(tenCot, "Pre");
				else strcpy(tenCot, "Final");
                float diem = NhapMotDiem(hp, tenCot, diemHienTai);
                if (diem == -999) { 
					printf(RED "\n[THONG BAO] Da dung sua!\n" RESET); 
					dungSua = 1; 
					Sleep(1200); 
					break; 
				}
                if (diem == -2.0) { 
					printf(RED "\n[THONG BAO] Da huy!\n" RESET); 
					Sleep(1200); 
					break; 
				}
                if(chonCot==1) bL1[k]=diem; 
				else if(chonCot==2) bL2[k]=diem; 
				else if(chonCot==3) bP1[k]=diem; 
				else if(chonCot==4) bP2[k]=diem; 
				else if(chonCot==5) bPr[k]=diem; 
				else bFi[k]=diem;
            }
            if (dungSua) continue;
            system("cls");
            printf("\n");
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printCenter(BOLD_CYAN "|                                      CAP NHAT DIEM THANH CONG                                    |" RESET);
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printf("\n");
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printCenter(BOLD_CYAN "|       MSSV        |         HO TEN         |  Lab1  |  Lab2  |  PT1   |  PT2   |   Pre  |  Final |" RESET);
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET);
            printf("          | %-17s | %-22s | ", maTam[k], tenTam[k]);
            for(j = 1; j <= 6; j++) {
    			float val = (j==1) ? bL1[k] : (j==2) ? bL2[k] : (j==3) ? bP1[k] : (j==4) ? bP2[k] : (j==5) ? bPr[k] : bFi[k];
			    char diemStr[10];
			    if (val == -1.0)
        			strcpy(diemStr, "N/A");
    			else
        			sprintf(diemStr, "%.1f", val);
			    if (daSuaCot(j, dsCotSua, soCotSua))
    			    printf(GREEN "%-6s" RESET " | ", diemStr);
    			else
        			printf("%-6s | ", diemStr);
			}
            printf("\n");
            printCenter(BOLD_CYAN "+--------------------------------------------------------------------------------------------------+" RESET );
            printf("\n");
            printCenter(GREEN "[THANH CONG] Da cap nhat diem!" RESET);
            int xacNhan;
            printf("\n"); 
			printCenter(YELLOW "BAN CO CHAC CHAN MUON SUA CAC DIEM VUA NHAP KHONG?" RESET "\n\n");
            for(d=0; d<45; d++) 
				printf(" "); 
			printf(GREEN "[1] Dong y    [0] Huy sua: " RESET);
            while (1) {
                if (scanf("%d", &xacNhan) == 1 && (xacNhan==0 || xacNhan==1)) { 
					while(getchar()!='\n'); 
					break; 
				}
                while(getchar()!='\n'); xoaDongVuaNhap();
            }
            if (xacNhan == 0) { 
				printCenter(RED "\n[THONG BAO] Da huy!" RESET); 
				Sleep(1200); 
				continue; 
			}
			for(i = 0; i < hp->soLuongSV; i++) {
    			hp->dsSV[i].lab1  = bL1[i];
    			hp->dsSV[i].lab2  = bL2[i];
    			hp->dsSV[i].pt1   = bP1[i];
    			hp->dsSV[i].pt2   = bP2[i];
    			hp->dsSV[i].pre   = bPr[i];
    			hp->dsSV[i].final = bFi[i];
			}
            FILE *fOut = fopen(fileSua, "w");
            if (fOut) {
                fprintf(fOut, "%d\n", slTam);
                for(i=0; i<slTam; i++) 
					fprintf(fOut, "%s|%s|%s|%.1f|%.1f|%.1f|%.1f|%.1f|%.1f\n", 
					maTam[i],tenTam[i],hp->dsSV[i].tenLopGoc, bL1[i], bL2[i], bP1[i], bP2[i], bPr[i], bFi[i]);
                fclose(fOut);
            }
            Sleep(1200);
			continue;
		}
    }
}

float temp_dtb[mx];

void swap1(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void swapSV(SinhVien *a, SinhVien *b) {
    SinhVien temp = *a;
    *a = *b;
    *b = temp;
}

void QuickSort(HocPhan *hp, int left, int right, int chon) {
    int i = left;
    int j = right;
    float pivot = temp_dtb[(left + right) / 2];
    do {
        if (chon == 1) {
            while (temp_dtb[i] < pivot)
                i++;
            while (temp_dtb[j] > pivot)
                j--;
        } else {
            while (temp_dtb[i] > pivot)
                i++;
            while (temp_dtb[j] < pivot)
                j--;
        }
        if (i <= j) {
            swapSV(&hp->dsSV[i], &hp->dsSV[j]);
            swap1(&temp_dtb[i], &temp_dtb[j]);
            i++;
            j--;
        }
    } while (i <= j);
    if (left < j)
        QuickSort(hp, left, j, chon);
    if (i < right)
        QuickSort(hp, i, right, chon);
}

void SapXep(HocPhan *hp, LopHocPhan *lop) {
    char line[100];
    int chon, i;
    while (1) {
    	if (menuHocPhan() == 0) return;
    	lop = &dsLopHocPhan[lopDangChon];
    	hp = &lop->dsHocPhan[hocPhanDangChon];
    	while (1) {
        system("cls");
        int d;
        for (d = 0; d < 10; d++)
            printf("\n");
        printf(BOLD_CYAN);
        sprintf(line, "--- SAP XEP (Hoc phan: %s) ---", hp->tenMon);
        printCenter(line);
        printf(RESET);
        printCenter("+------------------------------------------+");
        printCenter("|  1. Diem trung binh Tang dan             |");
        printCenter("|  2. Diem trung binh Giam dan             |");
        printCenter("|  0. Quay lai                             |");
        printCenter("+------------------------------------------+");
        printf("\n");
        printf("%48s", "");
        printf("Chon thao tac: ");
        if (scanf("%d", &chon) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (chon == 0)
            break;
        if (chon != 1 && chon != 2)
            continue;

        for (i = 0; i < hp->soLuongSV; i++) {
            float tb = tinhTB_hp(hp->dsSV[i]);
            if (tb == -1) {
                if (chon == 1)
                    temp_dtb[i] = 999;
                else
                    temp_dtb[i] = -999;

            } else {
                temp_dtb[i] = tb;
            }
        }

        QuickSort(hp, 0, hp->soLuongSV - 1, chon);
        system("cls");
        printf("\n" BOLD_CYAN "-------------------------------- BANG DIEM SAU SAP XEP ------------------------------\n\n" RESET);
        DanhSachSV(hp, lop->tenLop);
        printf("\n");
        printCenter(GREEN "=> DA SAP XEP THANH CONG!" RESET);
        printCenter(YELLOW "Nhan phim bat ky de tiep tuc..." RESET);
        getch();
    }
    }
}

void ThongKeHocVu() {
    int i, j, k;

    system("cls");
    printf("\n");
    char tieuDe[300];
	sprintf(tieuDe, MAGENTA "|     CHAO MUNG GIANG VIEN %s - DANH SACH BAO CAO THONG KE HOC VU     |" RESET, gvDangNhap.tenGV);
	printCenter(tieuDe);
	printf("\n");
    int coHocPhan = 0;

    for (i = 0; i < soLuongLopHP; i++) {
        LopHocPhan *lop = &dsLopHocPhan[i];
        for (j = 0; j < lop->soLuongHocPhan; j++) {
            HocPhan *hp = &lop->dsHocPhan[j];
            if (!gvCoDayHocPhan(gvDangNhap.maGV, hp))
                continue;
            coHocPhan = 1;
            int demA = 0, demB = 0, demC = 0, demD = 0, demF = 0;
            int coDiem = 0, soRot = 0;
            float tongTB = 0;
            float maxTB = -1;
            SinhVien thuKhoa;

            for (k = 0; k < hp->soLuongSV; k++) {
                float tb = tinhTB_hp(hp->dsSV[k]);
                if (tb == -1)
                    continue;
                coDiem++;
                tongTB += tb;
                char xl = xepLoai(tb);
                if (xl == 'A') demA++;
                else if (xl == 'B') demB++;
                else if (xl == 'C') demC++;
                else if (xl == 'D') demD++;
                else demF++;
                if (tb > maxTB) {
                    maxTB = tb;
                    thuKhoa = hp->dsSV[k];
                }
                if (tb < 4 || hp->dsSV[k].final < 4)
                    soRot++;
            }
            printf(CYAN "-------------------------------------------------------------------------\n" RESET);
            printf(BOLD_CYAN "  HOC PHAN : %s\n" RESET, hp->tenMon);
            printf("  LOP HP   : %s\n", lop->tenLop);
            printf("  Tong SV      : %d\n", hp->soLuongSV);
            printf("  Da co diem   : %d\n", coDiem);
            if (coDiem == 0) {
                printf(RED "  Chua du du lieu thong ke!\n" RESET);
                printf(CYAN "-------------------------------------------------------------------------\n\n" RESET);
                continue;
            }
            printf("\n");
            printf(GREEN   "  A: %.1f%%   " RESET, demA * 100.0 / coDiem);
            printf(CYAN    "  B: %.1f%%   " RESET, demB * 100.0 / coDiem);
            printf(MAGENTA "  C: %.1f%%   " RESET, demC * 100.0 / coDiem);
            printf(YELLOW  "  D: %.1f%%   " RESET, demD * 100.0 / coDiem);
            printf(RED     "  F: %.1f%%\n" RESET, demF * 100.0 / coDiem);
            printf("\n");
            printf("  DIEM TB LOP : %.1f\n", tongTB / coDiem);
            printf("\n");
            printf(YELLOW "  THU KHOA HOC PHAN\n" RESET);
            printf("      MSSV : %s\n", thuKhoa.maSV);
            printf("      Ten  : %s\n", thuKhoa.tenSV);
            printf("      TB   : %.1f\n", maxTB);
            printf("\n");
            printf(RED "   SO SV NGUY CO ROT MON : %d\n" RESET, soRot);
            printf(CYAN "-------------------------------------------------------------------------\n\n" RESET);
        }
    }
    if (!coHocPhan) {
        printCenter(RED "Ban khong duoc phan cong hoc phan nao!" RESET);
    }
    printCenter(YELLOW "Nhan phim bat ky de hien menu hoc phan..." RESET);
    getch();
}

void In(HocPhan *hp, LopHocPhan *lop) {
    ThongKeHocVu();
    int loaiLoc = 0;
    while (1) {
    	if (menuHocPhan() == 0) return;
    	int loaiLoc = 0;
    	while (1) {
        int demA = 0, demB = 0, demC = 0, demD = 0, demF = 0;
        float maxTB = -1.0;
        char mssvThuKhoa[20] = "";
        char tenThuKhoa[50] = "";
        char dsHocLaiMa[40][20];
        char dsHocLaiTen[40][50];
        float dsHocLaiDiem[40];
        int demHocLai = 0;
        system("cls");
        printf("\n" GREEN
        "---------------------------------- BANG DIEM CHI TIET MON: %-5s -----------------------------------"
        RESET "\n\n", hp->tenMon);
        int i;
        for (i = 0; i < hp->soLuongSV; i++) {
            int coDiem = (hp->dsSV[i].lab1  != -1 && hp->dsSV[i].lab2  != -1 && hp->dsSV[i].pt1   != -1 &&
                		  hp->dsSV[i].pt2   != -1 && hp->dsSV[i].pre   != -1 && hp->dsSV[i].final != -1);
            if (coDiem) {
                float tb = tinhTB_hp(hp->dsSV[i]);
                char xl = xepLoai(tb);
                if (xl == 'A') demA++;
                else if (xl == 'B') demB++;
                else if (xl == 'C') demC++;
                else if (xl == 'D') demD++;
                else demF++;
                if (tb > maxTB) {
                    maxTB = tb;
                    strcpy(mssvThuKhoa, hp->dsSV[i].maSV);
                    strcpy(tenThuKhoa, hp->dsSV[i].tenSV);
                }
                if (tb < 4.0 || hp->dsSV[i].final < 4.0) {
                    strcpy(dsHocLaiMa[demHocLai], hp->dsSV[i].maSV);
                    strcpy(dsHocLaiTen[demHocLai], hp->dsSV[i].tenSV);
                    if (tb < hp->dsSV[i].final)
                        dsHocLaiDiem[demHocLai] = tb;
                    else
                        dsHocLaiDiem[demHocLai] = hp->dsSV[i].final;
                    demHocLai++;
                }
            }
        }
        if (loaiLoc == 0) {
            DanhSachSV(hp, lop->tenLop);
        } else {
            if (loaiLoc == 1)
                printf(YELLOW " [CHE DO LOC]: Chi hien thi sinh vien Xep loai A\n\n" RESET);
            else if (loaiLoc == 2)
                printf(YELLOW " [CHE DO LOC]: Chi hien thi sinh vien Xep loai B\n\n" RESET);
            else if (loaiLoc == 3)
                printf(YELLOW " [CHE DO LOC]: Chi hien thi sinh vien Xep loai C\n\n" RESET);
            else if (loaiLoc == 4)
                printf(YELLOW " [CHE DO LOC]: Chi hien thi sinh vien Xep loai D\n\n" RESET);
            else if (loaiLoc == 5)
                printf(RED " [CHE DO LOC]: Chi hien thi sinh vien Xep loai F\n\n" RESET);
            printf("+----+-----------+---------------------+------+------+------+------+------+------+------+----------+\n");
            printf("| STT|   MSSV    |           Ten       | L1   | L2   | PT1  | PT2  | Pre  | Fin  |  TB  | XEP LOAI |\n");
            printf("+----+-----------+---------------------+------+------+------+------+------+------+------+----------+\n");
            int hienThiSTT = 1;
            int coSinhVienThoanMan = 0;
            for (i = 0; i < hp->soLuongSV; i++) {
                int coDiem =( hp->dsSV[i].lab1  != -1 && hp->dsSV[i].lab2  != -1 && hp->dsSV[i].pt1   != -1 &&
                    		  hp->dsSV[i].pt2   != -1 && hp->dsSV[i].pre   != -1 && hp->dsSV[i].final != -1);
                float tb = coDiem ? tinhTB_hp(hp->dsSV[i]) : 0;
                char xl = coDiem ? xepLoai(tb) : ' ';
                if (loaiLoc == 1 && xl != 'A') continue;
                if (loaiLoc == 2 && xl != 'B') continue;
                if (loaiLoc == 3 && xl != 'C') continue;
                if (loaiLoc == 4 && xl != 'D') continue;
                if (loaiLoc == 5 && xl != 'F') continue;
                coSinhVienThoanMan = 1;
                printf("| %-2d | %-9s | %-19s | ", hienThiSTT++, hp->dsSV[i].maSV, hp->dsSV[i].tenSV);
                if (hp->dsSV[i].lab1 == -1)
                    printf("N/A  | ");
                else
                    printf("%-4.1f | ", hp->dsSV[i].lab1);

                if (hp->dsSV[i].lab2 == -1)
                    printf("N/A  | ");
                else
                    printf("%-4.1f | ", hp->dsSV[i].lab2);

                if (hp->dsSV[i].pt1 == -1)
                    printf("N/A  | ");
                else
                    printf("%-4.1f | ", hp->dsSV[i].pt1);

                if (hp->dsSV[i].pt2 == -1)
                    printf("N/A  | ");
                else
                    printf("%-4.1f | ", hp->dsSV[i].pt2);

                if (hp->dsSV[i].pre == -1)
                    printf("N/A  | ");
                else
                    printf("%-4.1f | ", hp->dsSV[i].pre);

                if (hp->dsSV[i].final == -1)
                    printf("N/A  | ");
                else
                    printf("%-4.1f | ", hp->dsSV[i].final);

                if (coDiem)
                    printf("%-4.1f |    %-5c |\n", tb, xl);
                else
                    printf("N/A  |    N/A   |\n");
            }
            if (coSinhVienThoanMan == 0) {
                printf("|    |           | Khong co sinh vien thuoc xep loai nay                              |\n");
            }
            printf("+----+-----------+---------------------+------+------+------+------+------+------+------+----------+\n");
        }
        if (loaiLoc == 0) {
            int tongCoDiem = demA + demB + demC + demD + demF;
            printf("\n");
            printCenter(BOLD_CYAN "[ THONG KE TY LE HOC LUC ]" RESET);
            printCenter(CYAN
            " +----------+----------+----------+----------+----------+----------+"
            RESET);
            printCenter(CYAN
            " | Xep loai |    A     |    B     |    C     |    D     |    F     |"
            RESET);
            printCenter(CYAN
            " +----------+----------+----------+----------+----------+----------+"
            RESET);
            if (tongCoDiem > 0) {
                char s_sl[150];
                char s_tl[150];
                char tempColor[200];
                sprintf(s_sl, " | So luong |    %-2d    |    %-2d    |    %-2d    |    %-2d    |    %-2d    |", demA, demB, demC, demD, demF);
                sprintf(s_tl," | Ty le %%  |  %5.1f%%  |  %5.1f%%  |  %5.1f%%  |  %5.1f%%  |  %5.1f%%  |",
                	demA * 100.0 / tongCoDiem, demB * 100.0 / tongCoDiem, demC * 100.0 / tongCoDiem, demD * 100.0 / tongCoDiem, demF * 100.0 / tongCoDiem);
                sprintf(tempColor, "%s%s%s", YELLOW, s_sl, RESET);
                printCenter(tempColor);
                sprintf(tempColor, "%s%s%s", YELLOW, s_tl, RESET);
                printCenter(tempColor);
                printCenter(CYAN
                " +----------+----------+----------+----------+----------+----------+"
                RESET);
                printf("\n");
                printCenter(HIGHLIGHT_BLUE
                "=== BAO CAO CHI TIET HOC PHAN ==="
                RESET);
                printf("   -> Thu khoa hoc phan : "
                       YELLOW "%s" RESET
                       " - MSSV: %s - Diem TB: "
                       GREEN "%.1f" RESET "\n",
                       tenThuKhoa,
                       mssvThuKhoa,
                       maxTB);
                if (demHocLai > 0) {
                    printf("   -> " RED "DANH SACH NGUY CO HOC LAI (%d SV):"
                           RESET "\n", demHocLai);
                    int k;
                    for (k = 0; k < demHocLai; k++) {
                        printf("      + %s - %-20s (Diem thap nhat: " RED "%.1f" RESET ")\n",dsHocLaiMa[k],dsHocLaiTen[k], dsHocLaiDiem[k]);
                    }
                } else {
                    printf("   -> Canh bao hoc lai  : "
                           GREEN "Khong co sinh vien nguy co rot mon."
                           RESET "\n");
                }
                printCenter(HIGHLIGHT_BLUE "================================="RESET);
            } else {
                printCenter(RED "  Chua co du lieu thong ke cho mon hoc nay " RESET);
            }
        }
        printf("\n");
        printCenter(MAGENTA
        "--- CONG CU TUONG TAC BANG DIEM TOAN DIEN ---"
        RESET);
        printCenter("+---------------------------------------------------+");
        printCenter("| 1: Loc Loai A  | 2: Loc Loai B  | 3: Loc Loai C   |");
        printCenter("| 4: Loc Loai D  | 5: Loc Loai F                    |");
        printCenter("| 0: Quay lai                                       |");
        printCenter("+---------------------------------------------------+");
        printf("\n");
        printf("%45s", "");
        printf("Nhap lua chon cua ban: ");
        int luaChonTuongTac;
        if (scanf("%d", &luaChonTuongTac) != 1) {
            while (getchar() != '\n'){
                continue;
            }
        }
        if (luaChonTuongTac == 0)
            break;
        else if (luaChonTuongTac == 1)
            loaiLoc = 1;
        else if (luaChonTuongTac == 2)
            loaiLoc = 2;
        else if (luaChonTuongTac == 3)
            loaiLoc = 3;
        else if (luaChonTuongTac == 4)
            loaiLoc = 4;
        else if (luaChonTuongTac == 5)
            loaiLoc = 5;
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL)); 
    
    loadDanhSachGVTuFile();
    khoiTaoTaiKhoanHeThong();
    khoiTaoDuLieuGoc();
    khoiTaoLopSinhHoat();
    docToanBoHeThong();

    int k, l, h;
	for (l = 0; l < soLuongLopHP; l++) {
    	LopHocPhan *lop = &dsLopHocPhan[l];
    	for (h = 0; h < lop->soLuongHocPhan; h++) {
        	docFile(lop, &lop->dsHocPhan[h]);
    	}
	}

    int chon;

    while (1) {
        chooseRoleBox();
        gotoxy(60, 20);
        showCursor();
        if (scanf("%d", &chon) != 1) {
            while (getchar() != '\n');
            gotoxy(35, 22);
            printf(RED "VUI LONG CHI NHAP SO!" RESET);
            Sleep(1200);
            continue;
        }
        switch (chon) {
            case 1: {
                if (dangNhapHeThong() == 1) {
            			LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
            			HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];
                    int dangtrongMenu = 1;
                    while (dangtrongMenu) {
                    	menuGiangVien(hp, lop->tenLop);
                        int chonGV;
                        scanf("%d", &chonGV);
                        switch (chonGV) {
                            case 1: Nhap(); break;
                            case 2: Xem(); break;
                            case 3: Sua(); break;
							case 4: { 
    							LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
    							HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];
    							SapXep(hp, lop);
							} break;
							case 5: { 
    							LopHocPhan *lop = &dsLopHocPhan[lopDangChon];
    							HocPhan *hp = &lop->dsHocPhan[hocPhanDangChon];
    							In(hp, lop);
							} break;
                            case 6: dangtrongMenu = 0; break;
                            default:
                                printf("\n");
                                printCenter(RED "LUA CHON KHONG HOP LE!" RESET);
                                Sleep(1200);
                                break;
                        }
                    }
                }
                break;
            }
			case 2: {
				if (dangNhapSinhVien()) {
    			menuSinhVien(svDangNhap);
				} break;
			}
            case 0: {
                system("cls");
                int i;
                for (i=0; i<6; i++)
                   printf("\n");
                printCenter(GREEN "        CAM ON BAN DA SU DUNG HE THONG !!! " RESET);
                loadingEffect(53, 8);
                Sleep(1500);
                return 0;
            }
            default: {
                gotoxy(35, 22);
                printf(RED "LUA CHON KHONG HOP LE!" RESET);
                Sleep(1200);
                break;
            }
        }
    }
    return 0;
}
