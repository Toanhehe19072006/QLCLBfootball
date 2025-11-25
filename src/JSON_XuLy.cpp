#include "ClubGUI.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <direct.h>

using json = nlohmann::json;
using namespace std;

string formatVND(double value, bool valueIsInK)
{
    long long v = llround(value * (valueIsInK ? 1000.0 : 1.0));
    string s = to_string(v);
    int insertPos = (int)s.length() - 3;
    while (insertPos > 0)
    {
        s.insert(insertPos, ",");
        insertPos -= 3;
    }
    return s + " VND";
}

void loadFromJson(Club &clb)
{
    std::cout << "\n=== BAT DAU LOAD JSON ===\n";

    std::ifstream in("team.json");
    std::string filePath = "team.json";

    if (!in.is_open())
    {
        std::cout << "Khong mo duoc team.json\n";
        return;
    }

    std::cout << "Mo file thanh cong: " << filePath << "\n";

    std::string fileContent((std::istreambuf_iterator<char>(in)),
                            std::istreambuf_iterator<char>());

    std::cout << "Kich thuoc file: " << fileContent.size() << " bytes\n";
    std::cout << "100 ky tu dau tien:\n"
              << fileContent.substr(0, 100) << "\n";

    in.close();

    try
    {
        json j = json::parse(fileContent);

        std::cout << "Parse JSON thanh cong!\n";
        std::cout << "Noi dung JSON:\n"
                  << j.dump(2) << "\n";

        std::cout << "Co key 'teams' khong? " << (j.contains("teams") ? "Co" : "Khong") << "\n";

        if (j.contains("teams") && j["teams"].is_array())
        {
            std::cout << "So doi trong JSON: " << j["teams"].size() << "\n";

            for (const auto &teamJson : j["teams"])
            {
                std::cout << "Dang xu ly doi: " << teamJson["name"] << "\n";

                Team team(teamJson["id"], teamJson["name"]);

                if (teamJson.contains("hlvTruong"))
                    team.setHLVTruong(teamJson["hlvTruong"]);
                if (teamJson.contains("hlvPho"))
                    team.setHLVPho(teamJson["hlvPho"]);

                if (teamJson.contains("players") && teamJson["players"].is_array())
                {
                    std::cout << "  So cau thu: " << teamJson["players"].size() << "\n";

                    for (const auto &pJson : teamJson["players"])
                    {
                        try
                        {
                            Player p(
                                pJson["id"],
                                pJson["name"],
                                pJson["dob"],
                                pJson["address"],
                                pJson["contact"],
                                pJson["playerID"],
                                pJson["position"],
                                pJson["goals"],
                                pJson["honor"],
                                pJson["salary"],
                                pJson.value("contract", ""),
                                pJson.value("role", ""),
                                pJson.value("cccd", ""));

                            if (pJson.contains("soAo"))
                                p.setSoAo(pJson["soAo"]);

                            if (pJson.contains("thanhTich"))
                            {
                                auto tt = pJson["thanhTich"];
                                if (tt.contains("ngayGiaNhap"))
                                    p.setNgayGiaNhap(tt["ngayGiaNhap"]);
                                if (tt.contains("doiBongTruoc"))
                                    p.setDoiBongTruoc(tt["doiBongTruoc"]);
                                if (tt.contains("banThangChoClub"))
                                    p.setBanThangChoClub(tt["banThangChoClub"]);
                                if (tt.contains("danhHieuTaiClub"))
                                    p.setDanhHieuTaiClub(tt["danhHieuTaiClub"]);
                                if (tt.contains("soTranThiDau"))
                                    p.setSoTranThiDau(tt["soTranThiDau"]);
                                if (tt.contains("theVang"))
                                    p.setTheVang(tt["theVang"]);
                                if (tt.contains("theDo"))
                                    p.setTheDo(tt["theDo"]);
                            }

                            if (pJson.contains("sucKhoe"))
                            {
                                auto sk = pJson["sucKhoe"];
                                if (sk.contains("trangThai"))
                                    p.setTrangThaiSucKhoe(sk["trangThai"]);
                                if (sk.contains("ngayBatDau"))
                                    p.setNgayBatDauChanThuong(sk["ngayBatDau"]);
                                if (sk.contains("duKienHoiPhuc"))
                                    p.setDuKienHoiPhuc(sk["duKienHoiPhuc"]);
                                if (sk.contains("ghiChu"))
                                    p.setGhiChuSucKhoe(sk["ghiChu"]);
                            }

                            if (pJson.contains("theLuc"))
                            {
                                auto tl = pJson["theLuc"];
                                if (tl.contains("mucTheLuc"))
                                    p.setMucTheLuc(tl["mucTheLuc"]);
                                if (tl.contains("chieuCao"))
                                    p.setChieuCao(tl["chieuCao"]);
                                if (tl.contains("canNang"))
                                    p.setCanNang(tl["canNang"]);
                                if (tl.contains("tiLeMo"))
                                    p.setTiLeMo(tl["tiLeMo"]);
                            }

                            if (pJson.contains("hieuSuat"))
                            {
                                auto hs = pJson["hieuSuat"];
                                if (hs.contains("banThang"))
                                    p.setBanThangHieuSuat(hs["banThang"]);
                                if (hs.contains("kienTao"))
                                    p.setKienTao(hs["kienTao"]);
                                if (hs.contains("chuyenDung"))
                                    p.setChuyenDung(hs["chuyenDung"]);
                                if (hs.contains("diemPhongDo"))
                                    p.setDiemPhongDo(hs["diemPhongDo"]);
                            }

                            if (pJson.contains("tapLuyen"))
                            {
                                auto tl = pJson["tapLuyen"];
                                if (tl.contains("diemDanhGia"))
                                    p.setDiemDanhGiaTap(tl["diemDanhGia"]);
                                if (tl.contains("cuongDo"))
                                    p.setCuongDoTapLuyen(tl["cuongDo"]);
                                if (tl.contains("ghiChu"))
                                    p.setGhiChuTapLuyen(tl["ghiChu"]);
                            }

                            if (pJson.contains("kyLuat"))
                            {
                                auto kl = pJson["kyLuat"];
                                if (kl.contains("thaiDo"))
                                    p.setThaiDo(kl["thaiDo"]);
                                if (kl.contains("coViPham"))
                                    p.setCoViPham(kl["coViPham"]);
                                if (kl.contains("chiTietViPham"))
                                    p.setChiTietViPham(kl["chiTietViPham"]);
                                if (kl.contains("ghiChuTinhThan"))
                                    p.setGhiChuTinhThan(kl["ghiChuTinhThan"]);
                            }

                            team.themCauThu(p);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << "Error loading player: " << e.what() << "\n";
                        }
                    }
                }

                std::cout << "Them doi vao club...\n";
                clb.themTeam(team);
                std::cout << "So doi hien tai: " << clb.getTeams().size() << "\n";
            }
        }
        else
        {
            std::cout << "KHONG TIM THAY KEY 'teams' HOAC KHONG PHAI ARRAY!\n";
        }

        std::cout << "\n==========================================\n";
        std::cout << "LOAD DU LIEU THANH CONG!\n";
        std::cout << "So doi: " << clb.getTeams().size() << "\n";

        for (const auto &team : clb.getTeams())
        {
            std::cout << "  + " << team.getTenDoi() << ": "
                      << team.getPlayers().size() << " cau thu\n";
        }
        std::cout << "==========================================\n\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Loi parse JSON: " << e.what() << "\n";
    }
}

void saveToJson(const Club &clb)
{
    std::cout << "\n=== BAT DAU LUU DU LIEU ===\n";

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << "Thu muc hien tai: " << cwd << "\n";
    std::cout << "File se duoc luu vao: " << cwd << "\\team.json\n";

    std::cout << "So doi can luu: " << clb.getTeams().size() << "\n";

    json j;

    for (const auto &team : clb.getTeams())
    {
        std::cout << "Dang luu doi: " << team.getTenDoi() << " (ID: " << team.getIDDoi() << ")\n";

        json teamJson;
        teamJson["id"] = team.getIDDoi();
        teamJson["name"] = team.getTenDoi();
        teamJson["hlvTruong"] = team.getHLVTruong();
        teamJson["hlvPho"] = team.getHLVPho();

        for (const auto &p : team.getPlayers())
        {
            json pJson = {
                {"id", p.getID()},
                {"name", p.getTen()},
                {"dob", p.getNgaySinh()},
                {"address", p.getQueQuan()},
                {"contact", p.getThongTinLienLac()},
                {"playerID", p.getMaCauThu()},
                {"position", p.getViTri()},
                {"goals", p.getBanThang()},
                {"honor", p.getDanhHieu()},
                {"salary", p.getLuong()},
                {"role", p.getVaiTro()},
                {"cccd", p.getCCCD()},
                {"soAo", p.getSoAo()},
                {"thanhTich", {{"ngayGiaNhap", p.getNgayGiaNhap()}, {"doiBongTruoc", p.getDoiBongTruoc()}, {"banThangChoClub", p.getBanThangChoClub()}, {"danhHieuTaiClub", p.getDanhHieuTaiClub()}, {"soTranThiDau", p.getSoTranThiDau()}, {"theVang", p.getTheVang()}, {"theDo", p.getTheDo()}}},
                {"sucKhoe", {{"trangThai", p.getTrangThaiSucKhoe()}, {"ngayBatDau", p.getNgayBatDauChanThuong()}, {"duKienHoiPhuc", p.getDuKienHoiPhuc()}, {"ghiChu", p.getGhiChuSucKhoe()}}},
                {"theLuc", {{"mucTheLuc", p.getMucTheLuc()}, {"chieuCao", p.getChieuCao()}, {"canNang", p.getCanNang()}, {"tiLeMo", p.getTiLeMo()}}},
                {"hieuSuat", {{"banThang", p.getBanThangHieuSuat()}, {"kienTao", p.getKienTao()}, {"chuyenDung", p.getChuyenDung()}, {"diemPhongDo", p.getDiemPhongDo()}}},
                {"tapLuyen", {{"diemDanhGia", p.getDiemDanhGiaTap()}, {"cuongDo", p.getCuongDoTapLuyen()}, {"ghiChu", p.getGhiChuTapLuyen()}}},
                {"kyLuat", {{"thaiDo", p.getThaiDo()}, {"coViPham", p.getCoViPham()}, {"chiTietViPham", p.getChiTietViPham()}, {"ghiChuTinhThan", p.getGhiChuTinhThan()}}}};
            teamJson["players"].push_back(pJson);
        }

        j["teams"].push_back(teamJson);
    }

    std::string jsonString = j.dump(4);
    std::cout << "Kich thuoc JSON can ghi: " << jsonString.size() << " bytes\n";

    std::ofstream out("team.json", std::ios::out | std::ios::trunc);

    if (!out.is_open())
    {
        std::cout << "LOI: Khong the mo file de ghi!\n";
        return;
    }

    out << jsonString;
    out.flush();
    out.close();

    std::cout << "Da ghi xong vao team.json\n";

    std::ifstream verify("team.json");
    std::string verifyContent((std::istreambuf_iterator<char>(verify)),
                              std::istreambuf_iterator<char>());
    verify.close();

    std::cout << "Kich thuoc file sau khi ghi: " << verifyContent.size() << " bytes\n";

    if (verifyContent.size() != jsonString.size())
    {
        std::cout << "CANH BAO: Kich thuoc khong khop!\n";
    }
    else
    {
        std::cout << "XAC NHAN: File da duoc luu thanh cong!\n";
    }

    std::cout << "=== KET THUC LUU DU LIEU ===\n\n";
}
