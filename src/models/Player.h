#pragma once

#include <string>
#include <vector>
#include "Person.h"

// Struct để lưu thông tin từng trận đấu
struct MatchRecord {
    std::string ngayThiDau;      // Ngày thi đấu (dd/mm/yyyy)
    std::string doiDoi;          // Đội đối thủ
    std::string giaiDau;         // Tên giải đấu
    int banThang;                // Bàn thắng trong trận
    int kienTao;                 // Kiến tạo trong trận
    int theVang;                 // Thẻ vàng (0 hoặc 1)
    int theDo;                   // Thẻ đỏ (0 hoặc 1)
    std::string ghiChu;          // Ghi chú thêm
    
    MatchRecord() : ngayThiDau(""), doiDoi(""), giaiDau(""), banThang(0), kienTao(0), theVang(0), theDo(0), ghiChu("") {}
    
    MatchRecord(const std::string& ngay, const std::string& doi, const std::string& giai, int bt, int kt, int tv, int td, const std::string& gc = "")
        : ngayThiDau(ngay), doiDoi(doi), giaiDau(giai), banThang(bt), kienTao(kt), theVang(tv), theDo(td), ghiChu(gc) {}
};

// Struct cho giải thưởng cá nhân (không trùng lặp trong cùng năm)
struct Award {
    std::string tenGiai;         // Tên giải: "Quả bóng vàng", "Vua phá lưới", etc.
    int nam;                     // Năm nhận giải
    int xepHang;                 // 1=Vàng, 2=Bạc, 3=Đồng
    std::string ghiChu;          // Ghi chú thêm
    
    Award() : tenGiai(""), nam(0), xepHang(1), ghiChu("") {}
    
    Award(const std::string& ten, int y, int rank, const std::string& note = "")
        : tenGiai(ten), nam(y), xepHang(rank), ghiChu(note) {}
};

// Struct cho thành tích cầu thủ đạt được cùng đội bóng
struct TeamAchievement {
    std::string tenGiai;         // Tên giải: "V-League", "Cup Quoc Gia", "AFC Champions League"
    int nam;                     // Năm đạt giải
    int xepHang;                 // 1=Vô địch, 2=Á quân, 3=Hạng ba
    std::string ghiChu;          // Ghi chú thêm
    
    TeamAchievement() : tenGiai(""), nam(0), xepHang(1), ghiChu("") {}
    
    TeamAchievement(const std::string& ten, int y, int rank, const std::string& note = "")
        : tenGiai(ten), nam(y), xepHang(rank), ghiChu(note) {}
};

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

    // Lịch sử các trận đấu
    std::vector<MatchRecord> lichSuTranDau;
    
    // Danh sách giải thưởng cá nhân (hiển thị ở Thống kê)
    std::vector<Award> danhSachGiaiThuong;
    
    // Danh sách thành tích cùng đội bóng (hiển thị ở Thành tích)
    std::vector<TeamAchievement> thanhTichCungDoi;

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

    // Match history getters/setters (Lịch sử trận đấu)
    std::vector<MatchRecord>& getLichSuTranDau() { return lichSuTranDau; }
    const std::vector<MatchRecord>& getLichSuTranDau() const { return lichSuTranDau; }
    void addTranDau(const MatchRecord& tran) { lichSuTranDau.push_back(tran); }
    void clearLichSuTranDau() { lichSuTranDau.clear(); }
    
    // Tính tổng từ lịch sử trận đấu
    int getTongBanThangTuLichSu() const {
        int tong = 0;
        for (const auto& tran : lichSuTranDau) tong += tran.banThang;
        return tong;
    }
    int getTongKienTaoTuLichSu() const {
        int tong = 0;
        for (const auto& tran : lichSuTranDau) tong += tran.kienTao;
        return tong;
    }
    int getTongTheVangTuLichSu() const {
        int tong = 0;
        for (const auto& tran : lichSuTranDau) tong += tran.theVang;
        return tong;
    }
    int getTongTheDoTuLichSu() const {
        int tong = 0;
        for (const auto& tran : lichSuTranDau) tong += tran.theDo;
        return tong;
    }
    int getSoTranTuLichSu() const {
        return (int)lichSuTranDau.size();
    }
    
    // Danh sách giải thưởng cá nhân
    std::vector<Award>& getDanhSachGiaiThuong() { return danhSachGiaiThuong; }
    const std::vector<Award>& getDanhSachGiaiThuong() const { return danhSachGiaiThuong; }
    void addGiaiThuong(const Award& award) { danhSachGiaiThuong.push_back(award); }
    void removeGiaiThuong(int index) { 
        if (index >= 0 && index < (int)danhSachGiaiThuong.size()) 
            danhSachGiaiThuong.erase(danhSachGiaiThuong.begin() + index); 
    }
    
    // Danh sách thành tích cùng đội bóng
    std::vector<TeamAchievement>& getThanhTichCungDoi() { return thanhTichCungDoi; }
    const std::vector<TeamAchievement>& getThanhTichCungDoi() const { return thanhTichCungDoi; }
    void addThanhTichDoi(const TeamAchievement& tt) { thanhTichCungDoi.push_back(tt); }
    void removeThanhTichDoi(int index) { 
        if (index >= 0 && index < (int)thanhTichCungDoi.size()) 
            thanhTichCungDoi.erase(thanhTichCungDoi.begin() + index); 
    }
};
