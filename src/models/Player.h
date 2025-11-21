#pragma once

#include <string>
#include "Person.h"

class Player : public Person
{
protected:
    std::string maCauThu, viTri, danhHieu, hopDong, vaiTro;
    std::string cccd;
    int banThang;
    int soAo; // Jersey number
    double mucLuong;

    // Achievement/History data (Thành tích)
    std::string ngayGiaNhap;     // Join date
    std::string doiBongTruoc;    // Previous teams
    int banThangChoClub;         // Goals for this club
    std::string danhHieuTaiClub; // Achievements at this club
    int soTranThiDau;            // Number of matches played
    int theVang;                 // Yellow cards
    int theDo;                   // Red cards

    // Health data (Sức khỏe)
    std::string trangThaiSucKhoe; // "Khỏe mạnh", "Chấn thương nhẹ", "Chấn thương nặng"
    std::string ngayBatDauChanThuong;
    std::string duKienHoiPhuc;
    std::string ghiChuSucKhoe;

    // Physical data (Thể lực)
    int mucTheLuc;   // 0-10
    double chieuCao; // cm
    double canNang;  // kg
    double tiLeMo;   // %

    // Performance data (Hiệu suất)
    int banThangHieuSuat;
    int kienTao;
    int chuyenDung;
    int diemPhongDo; // 0-10

    // Training data (Tập luyện)
    int diemDanhGiaTap;          // 0-10
    std::string cuongDoTapLuyen; // "Thấp", "Trung bình", "Cao"
    std::string ghiChuTapLuyen;

    // Discipline data (Kỷ luật)
    std::string thaiDo; // "Tốt", "Trung bình", "Kém"
    bool coViPham;
    std::string chiTietViPham;
    std::string ghiChuTinhThan;

public:
    Player() : Person("", "", "", "", ""), maCauThu(""), viTri(""), danhHieu(""), hopDong(""), vaiTro(""), cccd(""), banThang(0), soAo(0), mucLuong(0),
               ngayGiaNhap(""), doiBongTruoc(""), banThangChoClub(0), danhHieuTaiClub(""), soTranThiDau(0), theVang(0), theDo(0),
               trangThaiSucKhoe(u8"Khỏe mạnh"), ngayBatDauChanThuong(""), duKienHoiPhuc(""), ghiChuSucKhoe(""),
               mucTheLuc(5), chieuCao(0), canNang(0), tiLeMo(0),
               banThangHieuSuat(0), kienTao(0), chuyenDung(0), diemPhongDo(5),
               diemDanhGiaTap(5), cuongDoTapLuyen(u8"Trung bình"), ghiChuTapLuyen(""),
               thaiDo(u8"Tốt"), coViPham(false), chiTietViPham(""), ghiChuTinhThan("") {}

    ~Player() = default;

    Player(const std::string &id, const std::string &ten, const std::string &ns, const std::string &qq, const std::string &ttll,
           const std::string &ma, const std::string &vt, int bt, const std::string &dh, double luong, const std::string &hd, const std::string &vaitro = "", const std::string &cccdNum = "", int jersey = 0)
        : Person(id, ten, ns, qq, ttll), maCauThu(ma), viTri(vt), danhHieu(dh), hopDong(hd), vaiTro(vaitro), cccd(cccdNum), banThang(bt), soAo(jersey), mucLuong(luong),
          ngayGiaNhap(""), doiBongTruoc(""), banThangChoClub(0), danhHieuTaiClub(""), soTranThiDau(0), theVang(0), theDo(0),
          trangThaiSucKhoe(u8"Khỏe mạnh"), ngayBatDauChanThuong(""), duKienHoiPhuc(""), ghiChuSucKhoe(""),
          mucTheLuc(5), chieuCao(0), canNang(0), tiLeMo(0),
          banThangHieuSuat(0), kienTao(0), chuyenDung(0), diemPhongDo(5),
          diemDanhGiaTap(5), cuongDoTapLuyen(u8"Trung bình"), ghiChuTapLuyen(""),
          thaiDo(u8"Tốt"), coViPham(false), chiTietViPham(""), ghiChuTinhThan("") {}

    std::string getMaCauThu() const { return maCauThu; }
    std::string getViTri() const { return viTri; }
    void setViTri(const std::string &vt) { viTri = vt; }
    int getBanThang() const { return banThang; }
    int getSoAo() const { return soAo; }
    void setSoAo(int sa) { soAo = sa; }
    double getLuong() const { return mucLuong; }
    void setLuong(double l) { mucLuong = l; }
    std::string getDanhHieu() const { return danhHieu; }
    std::string getVaiTro() const { return vaiTro; }
    void setVaiTro(const std::string &vt) { vaiTro = vt; }
    std::string getCCCD() const { return cccd; }
    void setCCCD(const std::string &c) { cccd = c; }

    // Achievement/History getters/setters (Thành tích)
    std::string getNgayGiaNhap() const { return ngayGiaNhap; }
    void setNgayGiaNhap(const std::string &ngay) { ngayGiaNhap = ngay; }
    std::string getDoiBongTruoc() const { return doiBongTruoc; }
    void setDoiBongTruoc(const std::string &db) { doiBongTruoc = db; }
    int getBanThangChoClub() const { return banThangChoClub; }
    void setBanThangChoClub(int bt) { banThangChoClub = bt; }
    std::string getDanhHieuTaiClub() const { return danhHieuTaiClub; }
    void setDanhHieuTaiClub(const std::string &dh) { danhHieuTaiClub = dh; }
    int getSoTranThiDau() const { return soTranThiDau; }
    void setSoTranThiDau(int st) { soTranThiDau = st; }
    int getTheVang() const { return theVang; }
    void setTheVang(int tv) { theVang = tv; }
    int getTheDo() const { return theDo; }
    void setTheDo(int td) { theDo = td; }
    int getKienTaoHieuSuat() const { return kienTao; }

    // Health getters/setters (Sức khỏe)
    std::string getTrangThaiSucKhoe() const { return trangThaiSucKhoe; }
    void setTrangThaiSucKhoe(const std::string &tt) { trangThaiSucKhoe = tt; }
    std::string getNgayBatDauChanThuong() const { return ngayBatDauChanThuong; }
    void setNgayBatDauChanThuong(const std::string &ngay) { ngayBatDauChanThuong = ngay; }
    std::string getDuKienHoiPhuc() const { return duKienHoiPhuc; }
    void setDuKienHoiPhuc(const std::string &ngay) { duKienHoiPhuc = ngay; }
    std::string getGhiChuSucKhoe() const { return ghiChuSucKhoe; }
    void setGhiChuSucKhoe(const std::string &note) { ghiChuSucKhoe = note; }

    // Physical getters/setters (Thể lực)
    int getMucTheLuc() const { return mucTheLuc; }
    void setMucTheLuc(int mtl) { mucTheLuc = mtl; }
    double getChieuCao() const { return chieuCao; }
    void setChieuCao(double cc) { chieuCao = cc; }
    double getCanNang() const { return canNang; }
    void setCanNang(double cn) { canNang = cn; }
    double getTiLeMo() const { return tiLeMo; }
    void setTiLeMo(double tlm) { tiLeMo = tlm; }

    // Performance getters/setters (Hiệu suất)
    int getBanThangHieuSuat() const { return banThangHieuSuat; }
    void setBanThangHieuSuat(int bt) { banThangHieuSuat = bt; }
    int getKienTao() const { return kienTao; }
    void setKienTao(int kt) { kienTao = kt; }
    int getChuyenDung() const { return chuyenDung; }
    void setChuyenDung(int cd) { chuyenDung = cd; }
    int getDiemPhongDo() const { return diemPhongDo; }
    void setDiemPhongDo(int dpd) { diemPhongDo = dpd; }

    // Training getters/setters (Tập luyện)
    int getDiemDanhGiaTap() const { return diemDanhGiaTap; }
    void setDiemDanhGiaTap(int dgt) { diemDanhGiaTap = dgt; }
    std::string getCuongDoTapLuyen() const { return cuongDoTapLuyen; }
    void setCuongDoTapLuyen(const std::string &cd) { cuongDoTapLuyen = cd; }
    std::string getGhiChuTapLuyen() const { return ghiChuTapLuyen; }
    void setGhiChuTapLuyen(const std::string &note) { ghiChuTapLuyen = note; }

    // Discipline getters/setters (Kỷ luật)
    std::string getThaiDo() const { return thaiDo; }
    void setThaiDo(const std::string &td) { thaiDo = td; }
    bool getCoViPham() const { return coViPham; }
    void setCoViPham(bool vp) { coViPham = vp; }
    std::string getChiTietViPham() const { return chiTietViPham; }
    void setChiTietViPham(const std::string &ct) { chiTietViPham = ct; }
    std::string getGhiChuTinhThan() const { return ghiChuTinhThan; }
    void setGhiChuTinhThan(const std::string &note) { ghiChuTinhThan = note; }
};
