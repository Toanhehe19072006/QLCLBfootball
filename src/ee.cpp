#include "raylib.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "json.hpp"
#include <fstream>
#include <direct.h>
#include <cmath>
#include "MyVector.h"
#include "MyMap.h"
#include "models/Person.h"
#include "models/Player.h"
#include "models/Team.h"
#include "models/Club.h"
#include "utf8_helper.h" // Must be included AFTER raylib

using json = nlohmann::json;
using namespace std;

// Format a numeric salary value into VND string with thousand separators.
// Assumption: existing salary values are stored in 'k' (thousands). If so,
// pass valueIsInK=true (default) to multiply by 1000. Returns e.g. "50,500 VND".
static string formatVND(double value, bool valueIsInK = true)
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

// ========================
// ========================
// CLASSES (moved to headers in src/models)
// ========================

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

    // Đọc toàn bộ nội dung file
    std::string fileContent((std::istreambuf_iterator<char>(in)),
                            std::istreambuf_iterator<char>());

    std::cout << "Kich thuoc file: " << fileContent.size() << " bytes\n";
    std::cout << "100 ky tu dau tien:\n"
              << fileContent.substr(0, 100) << "\n";

    in.close();

    try
    {
        json j = json::parse(fileContent); // ← QUAN TRỌNG: Parse từ string

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
                                pJson.value("contract", ""), // hopDong
                                pJson.value("role", ""),     // vaiTro
                                pJson.value("cccd", ""));    // cccd

                            // Load số áo
                            if (pJson.contains("soAo"))
                                p.setSoAo(pJson["soAo"]);

                            // Load thành tích
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

                            // Load sức khỏe
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

                            // Load thể lực
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

                            // Load hiệu suất
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

                            // Load tập luyện
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

                            // Load kỷ luật
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

    // Lấy đường dẫn tuyệt đối
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
// ========================
// GUI COMPONENTS
// ========================
Color PRIMARY_BG, SIDEBAR_BG, CONTENT_BG, ACCENT_1, ACCENT_2, TEXT_DARK, TEXT_LIGHT, CARD_BG, BORDER_LIGHT;
Color CAPTAIN_COLOR, VICE_CAPTAIN_COLOR, COACH_COLOR, ASSISTANT_COACH_COLOR;
Font customFont;

void initColors()
{
    PRIMARY_BG = {41, 50, 65, 255};
    SIDEBAR_BG = {52, 73, 94, 255};
    CONTENT_BG = {236, 240, 241, 255};
    ACCENT_1 = {52, 152, 219, 255};
    ACCENT_2 = {231, 76, 60, 255};
    TEXT_DARK = {44, 62, 80, 255};
    TEXT_LIGHT = {236, 240, 241, 255};
    CARD_BG = {255, 255, 255, 255};
    BORDER_LIGHT = {200, 200, 200, 255};
    CAPTAIN_COLOR = {243, 156, 18, 255};
    VICE_CAPTAIN_COLOR = {46, 204, 113, 255};
    COACH_COLOR = {52, 152, 219, 255};
    ASSISTANT_COACH_COLOR = {155, 89, 182, 255};
}

void DrawTextEx2(string text, int x, int y, int fontSize, Color color)
{
    DrawTextEx(customFont, text.c_str(), {(float)x, (float)y}, (float)fontSize, 1.0f, color);
}

int MeasureTextEx2(string text, int fontSize)
{
    Vector2 size = MeasureTextEx(customFont, text.c_str(), (float)fontSize, 1.0f);
    return (int)size.x;
}

struct Button
{
    Rectangle rect;
    string text;
    Color color;
    Color hoverColor;
    bool isHovered(Vector2 mousePos)
    {
        return CheckCollisionPointRec(mousePos, rect);
    }
    void draw()
    {
        Vector2 mousePos = GetMousePosition();
        Color currentColor = isHovered(mousePos) ? hoverColor : color;
        DrawRectangleRounded(rect, 0.15f, 10, currentColor);
        int textWidth = MeasureTextEx2(text, 18);
        DrawTextEx2(text, (int)(rect.x + (rect.width - textWidth) / 2),
                    (int)(rect.y + (rect.height - 18) / 2), 18, TEXT_LIGHT);
    }
    bool isClicked()
    {
        return isHovered(GetMousePosition()) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
};

struct InputField
{
    Rectangle rect;
    string label, text;
    bool active;
    int maxLength;
    string compositionText; // For IME composition
    bool multiline;         // Support multiline input

    InputField() : rect({0, 0, 0, 0}), label(""), text(""), active(false), maxLength(50), compositionText(""), multiline(false) {}
    InputField(Rectangle r, string l, int max = 50, bool multi = false)
        : rect(r), label(l), text(""), active(false), maxLength(max), compositionText(""), multiline(multi) {}

    void draw()
    {
        DrawTextEx2(label, (int)rect.x, (int)rect.y - 22, 16, TEXT_DARK);
        Color borderColor = active ? ACCENT_1 : (Color){189, 195, 199, 255};
        DrawRectangleRec(rect, CARD_BG);
        DrawRectangleLinesEx(rect, 2.0f, borderColor);

        // Draw text with composition text (for Vietnamese IME)
        string displayText = text;
        if (active && !compositionText.empty())
        {
            displayText += compositionText;
        }

        // Draw text with multiline support
        if (multiline)
        {
            int lineHeight = 22;
            int currentY = (int)rect.y + 10;
            int startX = (int)rect.x + 12;
            string line = "";

            for (size_t i = 0; i < displayText.length(); i++)
            {
                if (displayText[i] == '\n')
                {
                    DrawTextEx2(line, startX, currentY, 16, TEXT_DARK);
                    currentY += lineHeight;
                    line = "";
                }
                else
                {
                    line += displayText[i];
                }
            }
            if (!line.empty())
            {
                DrawTextEx2(line, startX, currentY, 16, TEXT_DARK);
            }
        }
        else
        {
            DrawTextEx2(displayText, (int)rect.x + 12, (int)rect.y + 10, 18, TEXT_DARK);
        }

        if (active && ((int)(GetTime() * 2) % 2 == 0))
        {
            int fontSize = multiline ? 16 : 18;
            int textWidth = MeasureTextEx(customFont, displayText.c_str(), (float)fontSize, 1.0f).x;
            int cursorHeight = fontSize + 4;
            DrawLine((int)rect.x + 12 + textWidth, (int)rect.y + 12,
                     (int)rect.x + 12 + textWidth, (int)rect.y + 12 + cursorHeight, ACCENT_1);
        }
    }

    void update()
    {
        if (CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            active = true;
        }
        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            active = false;
            compositionText = ""; // Clear composition when losing focus
        }
        if (active)
        {
            compositionText = ""; // Reset composition each frame

            int key = GetCharPressed();
            while (key > 0)
            {
                // Accept all Unicode characters (including Vietnamese)
                if ((key >= 32 && key <= 126) || key >= 128)
                {
                    if (text.length() < (size_t)maxLength * 4) // Allow more space for UTF-8 multi-byte chars
                    {
                        // For multi-byte UTF-8 characters
                        if (key < 128)
                        {
                            text += (char)key;
                        }
                        else
                        {
                            // Convert Unicode codepoint to UTF-8
                            if (key <= 0x7FF)
                            {
                                text += (char)(0xC0 | (key >> 6));
                                text += (char)(0x80 | (key & 0x3F));
                            }
                            else if (key <= 0xFFFF)
                            {
                                text += (char)(0xE0 | (key >> 12));
                                text += (char)(0x80 | ((key >> 6) & 0x3F));
                                text += (char)(0x80 | (key & 0x3F));
                            }
                            else if (key <= 0x10FFFF)
                            {
                                text += (char)(0xF0 | (key >> 18));
                                text += (char)(0x80 | ((key >> 12) & 0x3F));
                                text += (char)(0x80 | ((key >> 6) & 0x3F));
                                text += (char)(0x80 | (key & 0x3F));
                            }
                        }
                    }
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && text.length() > 0)
            {
                // Handle UTF-8 multi-byte character deletion
                text.pop_back();
                // If we removed a continuation byte, keep removing until we hit a start byte
                while (!text.empty() && (text.back() & 0xC0) == 0x80)
                {
                    text.pop_back();
                }
            }

            // Handle Enter key
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            {
                if (multiline && text.length() < (size_t)maxLength * 4)
                {
                    text += '\n'; // Add newline for multiline fields
                }
                else
                {
                    active = false; // Deactivate for single-line fields
                }
            }
        }
    }
};

// ========================
// MAIN GUI CLASS
// ========================
class ClubGUI
{
private:
    Club *clb;
    int currentTab;
    MyVector<InputField> inputs;
    string message;
    float messageTimer;
    int scrollOffset;
    bool isSearching;
    MyVector<pair<Player, string>> searchResults;
    int screenWidth, screenHeight;

    bool showPlayerDetail;
    Player selectedPlayer;
    string selectedPlayerTeam;
    Texture2D defaultPlayerPhoto;
    MyMap<string, Texture2D> playerPhotos;
    // Thêm vào class ClubGUI (trong phần private):
    bool showEditPlayer;
    Player editingPlayer;
    string editingPlayerTeam;
    int editingTeamIndex;
    int editingPlayerIndex;
    MyVector<InputField> editInputs; // Form riêng cho chỉnh sửa // Form riêng cho chỉnh sửa

public:
    int selectedTeamIndex;

private:
    int healthTab;
    int selectedHealthPlayerIndex;
    string selectedHealthTeam;

    bool showEditHLV;
    int editingHLVTeamIndex;
    MyVector<InputField> hlvInputs;
    MyVector<InputField> healthInputs;

    bool showEditTeam;
    int editingTeamIndex2;
    MyVector<InputField> teamEditInputs;

    // Temporary variables for health data editing
    int lastLoadedHealthPlayerIndex;
    int selectedPlayerIndex; // For achievements tab
    int lastLoadedAchievementPlayerIndex;
    MyVector<InputField> achievementInputs;

    // Temporary variables for achievement data
    string tempNgayGiaNhap;
    string tempDoiBongTruoc;
    int tempBanThangChoClub;
    string tempDanhHieuTaiClub;
    int tempSoTranThiDau;
    int tempTheVang;
    int tempTheDo;

    string tempTrangThaiSucKhoe;
    string tempNgayBatDauChanThuong;
    string tempDuKienHoiPhuc;
    string tempGhiChuSucKhoe;
    int tempMucTheLuc;
    double tempChieuCao;
    double tempCanNang;
    double tempTiLeMo;
    int tempBanThangHieuSuat;
    int tempKienTao;
    int tempChuyenDung;
    int tempDiemPhongDo;
    int tempDiemDanhGiaTap;
    string tempCuongDoTapLuyen;
    string tempGhiChuTapLuyen;
    string tempThaiDo;
    bool tempCoViPham;
    string tempChiTietViPham;
    string tempGhiChuTinhThan;

public:
    ClubGUI(Club *clubPtr) : clb(clubPtr), currentTab(0), messageTimer(0), scrollOffset(0), isSearching(false),
                             screenWidth(1400), screenHeight(800), showPlayerDetail(false), selectedPlayerTeam(""),
                             defaultPlayerPhoto(), showEditPlayer(false), editingPlayerTeam(""), editingTeamIndex(-1), editingPlayerIndex(-1),
                             selectedTeamIndex(-1), healthTab(0), selectedHealthPlayerIndex(-1), selectedHealthTeam(""),
                             showEditHLV(false), editingHLVTeamIndex(-1), lastLoadedHealthPlayerIndex(-1),
                             showEditTeam(false), editingTeamIndex2(-1),
                             selectedPlayerIndex(-1), lastLoadedAchievementPlayerIndex(-1),
                             tempNgayGiaNhap(""), tempDoiBongTruoc(""), tempBanThangChoClub(0), tempDanhHieuTaiClub(""), tempSoTranThiDau(0), tempTheVang(0), tempTheDo(0),
                             tempTrangThaiSucKhoe(""), tempNgayBatDauChanThuong(""), tempDuKienHoiPhuc(""), tempGhiChuSucKhoe(""),
                             tempMucTheLuc(0), tempChieuCao(0), tempCanNang(0), tempTiLeMo(0),
                             tempBanThangHieuSuat(0), tempKienTao(0), tempChuyenDung(0), tempDiemPhongDo(0),
                             tempDiemDanhGiaTap(0), tempCuongDoTapLuyen(""), tempGhiChuTapLuyen(""),
                             tempThaiDo(""), tempCoViPham(false), tempChiTietViPham(""), tempGhiChuTinhThan("")
    {
    }

    ~ClubGUI()
    {
        UnloadTexture(defaultPlayerPhoto);
        // Note: playerPhotos (MyMap) cleanup handled by destructor
    }

    Texture2D loadPlayerPhoto(const string &playerID)
    {
        if (playerPhotos.find(playerID))
        {
            return playerPhotos[playerID];
        }

        string photoPath = "assets/players/" + playerID + ".png";

        if (FileExists(photoPath.c_str()))
        {
            Texture2D texture = LoadTexture(photoPath.c_str());
            playerPhotos[playerID] = texture;
            return texture;
        }

        photoPath = "assets/players/" + playerID + ".jpg";
        if (FileExists(photoPath.c_str()))
        {
            Texture2D texture = LoadTexture(photoPath.c_str());
            playerPhotos[playerID] = texture;
            return texture;
        }

        return defaultPlayerPhoto;
    }

    void showMessage(const string &msg)
    {
        message = msg;
        messageTimer = 3.0f;
    }

    void loadHealthDataFromPlayer(const Player &player)
    {
        tempTrangThaiSucKhoe = player.getTrangThaiSucKhoe();
        tempNgayBatDauChanThuong = player.getNgayBatDauChanThuong();
        tempDuKienHoiPhuc = player.getDuKienHoiPhuc();
        tempGhiChuSucKhoe = player.getGhiChuSucKhoe();
        tempMucTheLuc = player.getMucTheLuc();
        tempChieuCao = player.getChieuCao();
        tempCanNang = player.getCanNang();
        tempTiLeMo = player.getTiLeMo();
        tempBanThangHieuSuat = player.getBanThangHieuSuat();
        tempKienTao = player.getKienTao();
        tempChuyenDung = player.getChuyenDung();
        tempDiemPhongDo = player.getDiemPhongDo();
        tempDiemDanhGiaTap = player.getDiemDanhGiaTap();
        tempCuongDoTapLuyen = player.getCuongDoTapLuyen();
        tempGhiChuTapLuyen = player.getGhiChuTapLuyen();
        tempThaiDo = player.getThaiDo();
        tempCoViPham = player.getCoViPham();
        tempChiTietViPham = player.getChiTietViPham();
        tempGhiChuTinhThan = player.getGhiChuTinhThan();

        // Update input fields with loaded data
        if (healthInputs.size() >= 9)
        {
            healthInputs[0].text = tempNgayBatDauChanThuong;
            healthInputs[1].text = tempDuKienHoiPhuc;
            healthInputs[2].text = tempGhiChuSucKhoe;
            char buf[20];
            sprintf(buf, "%.0f", tempChieuCao);
            healthInputs[3].text = (tempChieuCao > 0) ? string(buf) : "";
            sprintf(buf, "%.0f", tempCanNang);
            healthInputs[4].text = (tempCanNang > 0) ? string(buf) : "";
            sprintf(buf, "%.1f", tempTiLeMo);
            healthInputs[5].text = (tempTiLeMo > 0) ? string(buf) : "";
            healthInputs[6].text = tempGhiChuTapLuyen;
            healthInputs[7].text = tempChiTietViPham;
            healthInputs[8].text = tempGhiChuTinhThan;
        }
    }

    void drawSidebar()
    {
        int sidebarWidth = max(200, screenWidth / 7);
        DrawRectangle(0, 0, sidebarWidth, screenHeight, SIDEBAR_BG);

        int fontSize = max(16, screenWidth / 70);
        DrawTextEx2(u8"QUẢN LÝ CLB", 25, 30, fontSize + 6, TEXT_LIGHT);
        DrawTextEx2(u8"BÓNG ĐÁ", 25, 55, fontSize + 6, ACCENT_1);
        DrawRectangle(25, 85, sidebarWidth - 50, 3, ACCENT_1);

        string tabs[] = {u8"TỔNG QUAN", u8"ĐỘI BÓNG", u8"CẦU THỦ", u8"SỨC KHỎE", u8"THÀNH TÍCH", u8"THỐNG KÊ", u8"TÌM KIẾM & SỬA"};
        for (int i = 0; i < 7; i++)
        {
            Color tabColor = (i == currentTab) ? ACCENT_1 : SIDEBAR_BG;
            DrawRectangle(10, 120 + i * 60, sidebarWidth - 20, 50, tabColor);
            DrawTextEx2(tabs[i], 25, 132 + i * 60, fontSize + 2, TEXT_LIGHT);

            if (CheckCollisionPointRec(GetMousePosition(),
                                       {10, (float)(120 + i * 60), (float)(sidebarWidth - 20), 50}) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentTab = i;
                selectedTeamIndex = -1;
                scrollOffset = 0;
                isSearching = false;
                searchResults.clear();
                inputs.clear();
                showPlayerDetail = false;
            }
        }
    }

    void drawHeader(string title)
    {
        int sidebarWidth = max(200, screenWidth / 7);
        DrawRectangle(sidebarWidth, 0, screenWidth - sidebarWidth, 80, CARD_BG);
        int fontSize = max(20, screenWidth / 50);
        DrawTextEx2(title, sidebarWidth + 30, 25, fontSize, TEXT_DARK);
        DrawRectangle(sidebarWidth, 78, screenWidth - sidebarWidth, 2, ACCENT_1);
    }

    void drawMessage()
    {
        if (messageTimer > 0)
        {
            messageTimer -= GetFrameTime();

            // Tính toán vị trí content (không tính sidebar)
            int sidebarWidth = max(200, screenWidth / 7);
            int contentX = sidebarWidth + 30;
            int contentWidth = screenWidth - sidebarWidth - 60;

            int textWidth = MeasureTextEx2(message, 20);
            int msgWidth = textWidth + 80;
            int msgHeight = 60;

            // Căn giữa theo content area, không phải toàn màn hình
            int msgX = contentX + (contentWidth - msgWidth) / 2;
            int msgY = screenHeight - 150; // Điều chỉnh khoảng cách từ đáy

            float alpha = messageTimer / 2.0f;
            if (alpha > 1.0f)
                alpha = 1.0f;

            Color bgColor = ACCENT_1;
            bgColor.a = (unsigned char)(255 * alpha);
            Color textColor = TEXT_LIGHT;
            textColor.a = (unsigned char)(255 * alpha);

            // Vẽ bóng đổ
            DrawRectangleRounded(
                {(float)(msgX + 3), (float)(msgY + 3), (float)msgWidth, (float)msgHeight},
                0.15f, 10, (Color){0, 0, 0, (unsigned char)(100 * alpha)});

            // Vẽ thanh thông báo
            DrawRectangleRounded(
                {(float)msgX, (float)msgY, (float)msgWidth, (float)msgHeight},
                0.15f, 10, bgColor);

            // Vẽ text căn giữa
            DrawTextEx2(message, msgX + (msgWidth - textWidth) / 2, msgY + (msgHeight - 20) / 2, 20, textColor);
        }
    }
    void drawScrollbar(int contentHeight, int viewHeight, int startY)
    {
        if (contentHeight <= viewHeight)
            return;

        float mouseWheel = GetMouseWheelMove();
        if (mouseWheel != 0)
        {
            scrollOffset -= (int)(mouseWheel * 30);
            scrollOffset = max(0, min(scrollOffset, contentHeight - viewHeight));
        }

        int scrollbarX = screenWidth - 30;
        float scrollbarHeight = (float)viewHeight * viewHeight / contentHeight;
        float scrollbarY = startY + ((float)scrollOffset / (contentHeight - viewHeight)) * (viewHeight - scrollbarHeight);

        DrawRectangle(scrollbarX, startY, 10, viewHeight, (Color){200, 200, 200, 255});
        DrawRectangleRounded({(float)scrollbarX, scrollbarY, 10, scrollbarHeight}, 0.5f, 10, ACCENT_1);
    }

    void drawPlayerCard(Player &p, string teamName, int x, int y, int cardWidth, Team *team = nullptr)
    {
        DrawRectangleRounded({(float)x, (float)y, (float)cardWidth, 70}, 0.1f, 10, CARD_BG);

        int badgeX = x + 20;
        if (p.getVaiTro() == "DT")
        {
            DrawRectangleRounded({(float)badgeX, (float)(y + 8), 40, 24}, 0.3f, 8, CAPTAIN_COLOR);
            DrawTextEx2("DT", badgeX + 8, y + 11, 14, TEXT_LIGHT);
            badgeX += 50;
        }
        else if (p.getVaiTro() == "DP")
        {
            DrawRectangleRounded({(float)badgeX, (float)(y + 8), 40, 24}, 0.3f, 8, VICE_CAPTAIN_COLOR);
            DrawTextEx2("DP", badgeX + 8, y + 11, 14, TEXT_LIGHT);
            badgeX += 50;
        }

        DrawTextEx2(p.getTen(), badgeX, y + 10, 18, TEXT_DARK);

        string info = u8"Mã: " + p.getMaCauThu() + " | " + teamName + " | " + p.getViTri() +
                      " | BT: " + to_string(p.getBanThang()) +
                      " | Lương: " + formatVND(p.getLuong(), true);
        DrawTextEx2(info, x + 20, y + 38, 13, ACCENT_1);

        Button cccdBtn;
        cccdBtn.rect = {(float)(x + cardWidth - 190), (float)(y + 10), 80, 40};
        cccdBtn.text = "CCCD";
        cccdBtn.color = {46, 204, 113, 255};
        cccdBtn.hoverColor = {39, 174, 96, 255};
        cccdBtn.draw();

        if (cccdBtn.isClicked())
        {
            selectedPlayer = p;
            selectedPlayerTeam = teamName;
            showPlayerDetail = true;
        }

        Button deleteBtn;
        deleteBtn.rect = {(float)(x + cardWidth - 100), (float)(y + 10), 80, 40};
        deleteBtn.text = u8"XÓA";
        deleteBtn.color = ACCENT_2;
        deleteBtn.hoverColor = {192, 57, 43, 255};
        deleteBtn.draw();

        if (deleteBtn.isClicked() && team != nullptr)
        {
            team->xoaCauThu(p.getID());
            saveToJson(*clb);
            showMessage(u8"Đã xoá cầu thủ: " + p.getTen());
        }

        Rectangle cardRect = {(float)x, (float)y, (float)(cardWidth - 200), 70};
        if (CheckCollisionPointRec(GetMousePosition(), cardRect) && IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON) && team != nullptr)
        {
            if (p.getVaiTro() == "")
            {
                team->capNhatDoiTruong(p.getID());
                saveToJson(*clb);
                showMessage(u8"Đã đặt " + p.getTen() + u8" làm Đội trưởng");
            }
            else if (p.getVaiTro() == "DT")
            {
                team->capNhatDoiPho(p.getID());
                saveToJson(*clb);
                showMessage(u8"Đã đặt " + p.getTen() + u8" làm Đội phó");
            }
            else
            {
                p.setVaiTro("");
                saveToJson(*clb);
                showMessage(u8"Đã xoá vai trò của " + p.getTen());
            }
        }
    }

    void drawPlayerDetailPopup()
    {
        if (!showPlayerDetail)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 700;
        int popupHeight = 500;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, ACCENT_1);
        DrawTextEx2(u8"GIẤY CHỨNG MINH NHÂN DÂN", popupX + 30, popupY + 20, 22, TEXT_LIGHT);
        DrawTextEx2("CITIZEN IDENTIFICATION", popupX + 30, popupY + 45, 14, (Color){200, 230, 255, 255});

        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 100), (float)(popupY + 10), 80, 40};
        closeBtn.text = "DONG";
        closeBtn.color = ACCENT_2;
        closeBtn.hoverColor = {192, 57, 43, 255};
        closeBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showPlayerDetail = false;
            return;
        }

        int contentY = popupY + 80;
        int leftCol = popupX + 40;
        int rightCol = popupX + 380;

        Texture2D photo = loadPlayerPhoto(selectedPlayer.getID());

        int photoWidth = 250;
        int photoHeight = 320;
        int photoX = rightCol;
        int photoY = contentY;

        DrawRectangleRounded({(float)photoX, (float)photoY, (float)photoWidth, (float)photoHeight}, 0.05f, 10, (Color){240, 240, 240, 255});

        if (photo.id > 0)
        {
            float scaleX = (float)photoWidth / photo.width;
            float scaleY = (float)photoHeight / photo.height;
            float scale = min(scaleX, scaleY);

            int drawWidth = (int)(photo.width * scale);
            int drawHeight = (int)(photo.height * scale);
            int drawX = photoX + (photoWidth - drawWidth) / 2;
            int drawY = photoY + (photoHeight - drawHeight) / 2;

            DrawTexturePro(photo,
                           {0, 0, (float)photo.width, (float)photo.height},
                           {(float)drawX, (float)drawY, (float)drawWidth, (float)drawHeight},
                           {0, 0}, 0, WHITE);
        }
        else
        {
            DrawTextEx2(u8"CHƯA CÓ ẢNH", photoX + 60, photoY + 135, 18, (Color){150, 150, 150, 255});
        }

        DrawRectangleLinesEx({(float)photoX, (float)photoY, (float)photoWidth, (float)photoHeight}, 2, ACCENT_1);

        int lineHeight = 35;
        int currentY = contentY;

        DrawTextEx2(u8"Họ và tên:", leftCol, currentY, 16, (Color){100, 100, 100, 255});
        currentY += 20;
        DrawTextEx2(selectedPlayer.getTen(), leftCol, currentY, 20, TEXT_DARK);
        currentY += lineHeight + 10;

        DrawTextEx2(u8"Ngày sinh:", leftCol, currentY, 16, (Color){100, 100, 100, 255});
        currentY += 20;
        DrawTextEx2(selectedPlayer.getNgaySinh(), leftCol, currentY, 20, TEXT_DARK);
        currentY += lineHeight + 10;

        DrawTextEx2(u8"Số CCCD:", leftCol, currentY, 16, (Color){100, 100, 100, 255});
        currentY += 20;
        string cccd = selectedPlayer.getCCCD().empty() ? u8"Chưa cập nhật" : selectedPlayer.getCCCD();
        DrawTextEx2(cccd, leftCol, currentY, 20, ACCENT_2);
        currentY += lineHeight + 10;

        DrawTextEx2(u8"Quê quán:", leftCol, currentY, 16, (Color){100, 100, 100, 255});
        currentY += 20;
        DrawTextEx2(selectedPlayer.getQueQuan(), leftCol, currentY, 18, TEXT_DARK);
        currentY += lineHeight + 10;

        DrawTextEx2(u8"SĐT:", leftCol, currentY, 16, (Color){100, 100, 100, 255});
        currentY += 20;
        string sdt = selectedPlayer.getThongTinLienLac().empty() ? u8"Chưa cập nhật" : selectedPlayer.getThongTinLienLac();
        DrawTextEx2(sdt, leftCol, currentY, 18, ACCENT_1);
        currentY += lineHeight + 10;

        DrawRectangle(popupX + 30, popupY + popupHeight - 100, popupWidth - 60, 70, (Color){240, 248, 255, 255});
        DrawTextEx2(u8"ĐỘI BÓNG: " + selectedPlayerTeam, leftCol, popupY + popupHeight - 85, 16, ACCENT_1);
        DrawTextEx2(u8"VỊ TRÍ: " + selectedPlayer.getViTri(), leftCol, popupY + popupHeight - 60, 16, ACCENT_1);
        DrawTextEx2(u8"BÀN THẮNG: " + to_string(selectedPlayer.getBanThangHieuSuat()), rightCol - 50, popupY + popupHeight - 85, 16, ACCENT_2);
        DrawTextEx2(u8"LƯƠNG: " + formatVND(selectedPlayer.getLuong(), true), rightCol - 50, popupY + popupHeight - 60, 16, (Color){46, 204, 113, 255});
    }

    void drawDashboard()
    {
        drawHeader(u8"TỔNG QUAN CLB BÓNG ĐÁ");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        if (selectedTeamIndex >= 0 && selectedTeamIndex < (int)clb->getTeams().size())
        {
            auto &team = clb->getTeams()[selectedTeamIndex];

            Button backBtn;
            backBtn.rect = {(float)contentX, 100, 150, 45};
            backBtn.text = u8"<< QUAY LẠI";
            backBtn.color = {149, 165, 166, 255};
            backBtn.hoverColor = {127, 140, 141, 255};
            backBtn.draw();

            if (backBtn.isClicked())
            {
                selectedTeamIndex = -1;
                scrollOffset = 0;
                return;
            }

            DrawRectangleRounded({(float)contentX, 160, (float)(contentWidth), 130}, 0.1f, 10, ACCENT_1);
            DrawTextEx2(team.getTenDoi(), contentX + 40, 170, 28, TEXT_LIGHT);
            string info = u8"Số cầu thủ: " + to_string(team.getPlayers().size()) +
                          u8" | Tổng bàn thắng: " + to_string(team.tongBanThang());
            DrawTextEx2(info, contentX + 40, 205, 18, TEXT_LIGHT);

            string hlvInfo = u8"HLV trưởng: " + (team.getHLVTruong().empty() ? u8"Chưa có" : team.getHLVTruong()) +
                             u8"  |  HLV phó: " + (team.getHLVPho().empty() ? u8"Chưa có" : team.getHLVPho());
            DrawTextEx2(hlvInfo, contentX + 40, 235, 16, (Color){255, 255, 255, 200});

            Button editHLVBtn;
            editHLVBtn.rect = {(float)(contentX + contentWidth - 150), 165, 130, 35};
            editHLVBtn.text = u8"SỬA HLV";
            editHLVBtn.color = {52, 152, 219, 255};
            editHLVBtn.hoverColor = {41, 128, 185, 255};
            editHLVBtn.draw();

            if (editHLVBtn.isClicked())
            {
                showEditHLV = true;
                editingHLVTeamIndex = selectedTeamIndex;
                hlvInputs.clear();
                // Input fields sẽ được tạo trong drawEditHLVPopup() với vị trí động
                hlvInputs.push_back(InputField({0, 0, 500, 40}, u8"HLV trưởng:", 50));
                hlvInputs.push_back(InputField({0, 0, 500, 40}, u8"HLV phó:", 50));
                hlvInputs[0].text = team.getHLVTruong();
                hlvInputs[1].text = team.getHLVPho();
            }

            DrawTextEx2(u8"DANH SÁCH CẦU THỦ (Click chuột giữa để đổi vai trò)", contentX + 20, 310, 20, TEXT_DARK);

            int contentHeight = team.getPlayers().size() * 80 + 20;
            int viewHeight = screenHeight - 380;
            int startY = 350;

            BeginScissorMode(contentX, startY, contentWidth, viewHeight);

            int y = startY - scrollOffset;
            if (team.getPlayers().empty())
            {
                DrawTextEx2(u8"Chưa có cầu thủ nào!", contentX + 100, y + 50, 20, ACCENT_2);
            }
            else
            {
                for (auto &p : team.getPlayers())
                {
                    drawPlayerCard(p, team.getTenDoi(), contentX + 20, y, contentWidth - 40, &team);
                    y += 80;
                }
            }

            EndScissorMode();
            drawScrollbar(contentHeight, viewHeight, startY);

            // Vẽ chú thích đẹp hơn với background
            int legendY = screenHeight - 65;
            int legendHeight = 50;
            DrawRectangleRounded({(float)contentX, (float)legendY, (float)contentWidth, (float)legendHeight}, 0.1f, 10, (Color){240, 248, 255, 255});

            DrawTextEx2(u8"CHÚ THÍCH", contentX + 20, legendY + 10, 16, TEXT_DARK);

            // Đội trưởng
            DrawRectangleRounded({(float)(contentX + 120), (float)(legendY + 10), 35, 22}, 0.3f, 8, CAPTAIN_COLOR);
            DrawTextEx2("DT", contentX + 128, legendY + 13, 14, TEXT_LIGHT);
            DrawTextEx2(u8"= Đội trưởng", contentX + 160, legendY + 12, 16, TEXT_DARK);

            // Đội phó
            DrawRectangleRounded({(float)(contentX + 280), (float)(legendY + 10), 35, 22}, 0.3f, 8, VICE_CAPTAIN_COLOR);
            DrawTextEx2("DP", contentX + 288, legendY + 13, 14, TEXT_LIGHT);
            DrawTextEx2(u8"= Đội phó", contentX + 320, legendY + 12, 16, TEXT_DARK);

            // Hướng dẫn
            DrawTextEx2("| Click chuột giữa (scroll) để thay đổi vai trò", contentX + 440, legendY + 12, 15, ACCENT_1);

            return;
        }

        int totalTeams = clb->getTeams().size();
        int totalPlayers = 0, totalGoals = 0;
        for (auto &team : clb->getTeams())
        {
            totalPlayers += team.getPlayers().size();
            totalGoals += team.tongBanThang();
        }

        int cardWidth = (contentWidth - 60) / 3;
        DrawRectangleRounded({(float)contentX, 120, (float)cardWidth, 150}, 0.15f, 10, ACCENT_1);
        DrawTextEx2(to_string(totalTeams), contentX + cardWidth / 2 - 20, 150, 50, TEXT_LIGHT);
        DrawTextEx2(u8"ĐỘI BÓNG", contentX + cardWidth / 2 - 50, 220, 20, TEXT_LIGHT);

        DrawRectangleRounded({(float)(contentX + cardWidth + 30), 120, (float)cardWidth, 150}, 0.15f, 10, ACCENT_1);
        DrawTextEx2(to_string(totalPlayers), contentX + cardWidth + 30 + cardWidth / 2 - 20, 150, 50, TEXT_LIGHT);
        DrawTextEx2(u8"CẦU THỦ", contentX + cardWidth + 30 + cardWidth / 2 - 40, 220, 20, TEXT_LIGHT);

        DrawRectangleRounded({(float)(contentX + 2 * cardWidth + 60), 120, (float)cardWidth, 150}, 0.15f, 10, ACCENT_1);
        DrawTextEx2(to_string(totalGoals), contentX + 2 * cardWidth + 60 + cardWidth / 2 - 20, 150, 50, TEXT_LIGHT);
        DrawTextEx2(u8"BÀN THẮNG", contentX + 2 * cardWidth + 60 + cardWidth / 2 - 50, 220, 20, TEXT_LIGHT);

        DrawTextEx2(u8"DANH SÁCH ĐỘI BÓNG", contentX, 310, 24, TEXT_DARK);

        int contentHeight = clb->getTeams().size() * 100 + 20;
        int viewHeight = screenHeight - 390;
        int startY = 360;

        BeginScissorMode(contentX, startY, contentWidth, viewHeight);

        int y = startY - scrollOffset;
        int teamIndex = 0;
        for (auto &team : clb->getTeams())
        {
            Rectangle teamRect = {(float)contentX, (float)y, (float)contentWidth, 80};
            bool isHovered = CheckCollisionPointRec(GetMousePosition(), teamRect);
            Color cardColor = isHovered ? (Color){245, 246, 250, 255} : CARD_BG;

            DrawRectangleRounded(teamRect, 0.1f, 10, cardColor);
            DrawTextEx2(team.getTenDoi(), contentX + 30, y + 15, 22, TEXT_DARK);
            string info = "So cau thu: " + to_string(team.getPlayers().size()) +
                          " | Bàn thắng: " + to_string(team.tongBanThang());
            DrawTextEx2(info, contentX + 30, y + 45, 16, ACCENT_1);

            if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedTeamIndex = teamIndex;
                scrollOffset = 0;
            }

            y += 100;
            teamIndex++;
        }

        EndScissorMode();
        drawScrollbar(contentHeight, viewHeight, startY);
    }

    void drawTeams()
    {
        drawHeader(u8"QUẢN LÝ ĐỘI BÓNG");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        if (inputs.empty())
        {
            inputs.push_back(InputField({(float)contentX, 150, (float)(contentWidth / 2 - 30), 45}, u8"Mã Đội:"));
            inputs.push_back(InputField({(float)(contentX + contentWidth / 2 + 30), 150, (float)(contentWidth / 2 - 30), 45}, u8"Tên Đội:"));
        }

        DrawRectangleRounded({(float)contentX, 120, (float)contentWidth, 120}, 0.1f, 10, CARD_BG);
        for (auto &input : inputs)
        {
            input.draw();
            input.update();
        }

        Button addBtn;
        addBtn.rect = {(float)(contentX + contentWidth - 200), 250, 200, 45};
        addBtn.text = u8"THÊM ĐỘI";
        addBtn.color = ACCENT_1;
        addBtn.hoverColor = {41, 128, 185, 255};
        addBtn.draw();

        if (addBtn.isClicked() && !inputs[0].text.empty() && !inputs[1].text.empty())
        {
            string newTeamID = inputs[0].text;
            string newTeamName = inputs[1].text;

            // Loại bỏ khoảng trắng ở đầu và cuối
            while (!newTeamID.empty() && (newTeamID[0] == ' ' || newTeamID[0] == '\t'))
                newTeamID.erase(0, 1);
            while (!newTeamID.empty() && (newTeamID.back() == ' ' || newTeamID.back() == '\t'))
                newTeamID.pop_back();

            // Kiểm tra trùng mã đội (không phân biệt hoa thường)
            bool isDuplicate = false;
            for (auto &team : clb->getTeams())
            {
                string existingID = team.getIDDoi();
                // Trim existing ID
                while (!existingID.empty() && (existingID[0] == ' ' || existingID[0] == '\t'))
                    existingID.erase(0, 1);
                while (!existingID.empty() && (existingID.back() == ' ' || existingID.back() == '\t'))
                    existingID.pop_back();

                // So sánh không phân biệt hoa thường
                string newIDLower = newTeamID;
                string existingIDLower = existingID;
                transform(newIDLower.begin(), newIDLower.end(), newIDLower.begin(), ::tolower);
                transform(existingIDLower.begin(), existingIDLower.end(), existingIDLower.begin(), ::tolower);

                if (newIDLower == existingIDLower)
                {
                    showMessage(u8"Mã đội đã tồn tại!");
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate)
            {
                Team newTeam(newTeamID, newTeamName);
                clb->themTeam(newTeam);
                saveToJson(*clb);
                showMessage(u8"Đã thêm đội thành công!");
                inputs.clear();
            }
        }

        DrawTextEx2(u8"DANH SÁCH ĐỘI BÓNG", contentX, 330, 22, TEXT_DARK);

        int contentHeight = clb->getTeams().size() * 85 + 20;
        int viewHeight = screenHeight - 410;
        int startY = 380;

        BeginScissorMode(contentX, startY, contentWidth, viewHeight);

        int y = startY - scrollOffset;
        int teamIndex = 0;
        for (auto it = clb->getTeams().begin(); it != clb->getTeams().end();)
        {
            auto &team = *it;
            bool teamDeleted = false;

            DrawRectangleRounded({(float)contentX, (float)y, (float)(contentWidth - 20), 70}, 0.1f, 10, CARD_BG);
            DrawTextEx2(team.getTenDoi(), contentX + 30, y + 10, 20, TEXT_DARK);
            string info = u8"Mã: " + team.getIDDoi() + u8" | Cầu thủ: " +
                          to_string(team.getPlayers().size());
            DrawTextEx2(info, contentX + 30, y + 38, 16, ACCENT_1);

            Button editTeamBtn;
            editTeamBtn.rect = {(float)(contentX + contentWidth - 240), (float)(y + 15), 110, 40};
            editTeamBtn.text = u8"ĐỔI TÊN";
            editTeamBtn.color = ACCENT_1;
            editTeamBtn.hoverColor = {41, 128, 185, 255};
            editTeamBtn.draw();

            Button deleteBtn;
            deleteBtn.rect = {(float)(contentX + contentWidth - 120), (float)(y + 15), 100, 40};
            deleteBtn.text = u8"XOÁ ĐỘI";
            deleteBtn.color = ACCENT_2;
            deleteBtn.hoverColor = {192, 57, 43, 255};
            deleteBtn.draw();

            if (editTeamBtn.isClicked())
            {
                editingTeamIndex2 = teamIndex;
                showEditTeam = true;
                teamEditInputs.clear();
            }

            if (deleteBtn.isClicked())
            {
                if (team.getPlayers().size() > 0)
                {
                    showMessage(u8"Không thể xóa đội còn thành viên!");
                }
                else
                {
                    string teamName = team.getTenDoi();
                    it = clb->getTeams().erase(it);
                    saveToJson(*clb);
                    showMessage(u8"Đã xoá đội: " + teamName);
                    teamDeleted = true;
                }
            }

            if (!teamDeleted)
            {
                ++it;
                y += 85;
                teamIndex++;
            }
        }

        EndScissorMode();
        drawScrollbar(contentHeight, viewHeight, startY);
    }
    void drawPlayers()
    {
        auto trimCopy = [](const string &s)
        {
            size_t a = s.find_first_not_of(" \t\r\n");
            if (a == string::npos)
                return string("");
            size_t b = s.find_last_not_of(" \t\r\n");
            return s.substr(a, b - a + 1);
        };

        auto toLowerCopy = [](string s)
        {
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        };

        auto isIntegerString = [&](const string &s)
        {
            string t = trimCopy(s);
            if (t.empty())
                return false;
            size_t i = 0;
            if (t[0] == '+' || t[0] == '-')
            {
                if (t.size() == 1)
                    return false;
                i = 1;
            }
            for (; i < t.size(); ++i)
                if (!isdigit((unsigned char)t[i]))
                    return false;
            return true;
        };

        auto isDoubleString = [&](const string &s)
        {
            string t = trimCopy(s);
            if (t.empty())
                return false;
            bool dotSeen = false;
            size_t i = 0;
            if (t[0] == '+' || t[0] == '-')
            {
                if (t.size() == 1)
                    return false;
                i = 1;
            }
            for (; i < t.size(); ++i)
            {
                if (t[i] == '.')
                {
                    if (dotSeen)
                        return false;
                    dotSeen = true;
                    continue;
                }
                if (!isdigit((unsigned char)t[i]))
                    return false;
            }
            return true;
        };

        drawHeader(u8"QUẢN LÝ CẦU THỦ");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        if (inputs.size() != 9)
        {
            inputs.clear();
            int fieldWidth = contentWidth / 5 - 16;
            inputs.push_back(InputField({(float)contentX, 120, (float)fieldWidth, 40}, u8"Mã CT: (VD: P001)", 20));
            inputs.push_back(InputField({(float)(contentX + fieldWidth + 20), 120, (float)fieldWidth, 40}, u8"Họ Tên: (VD: Nguyễn Văn A)", 40));
            inputs.push_back(InputField({(float)(contentX + 2 * (fieldWidth + 20)), 120, (float)fieldWidth, 40}, u8"Vị Trí: (VD: FW)", 20));
            inputs.push_back(InputField({(float)(contentX + 3 * (fieldWidth + 20)), 120, (float)fieldWidth, 40}, u8"Ngày Sinh: (DD/MM/YYYY)", 20));
            inputs.push_back(InputField({(float)(contentX + 4 * (fieldWidth + 20)), 120, (float)fieldWidth, 40}, u8"CCCD: (12 số)", 20));

            inputs.push_back(InputField({(float)contentX, 200, (float)fieldWidth, 40}, u8"Quê Quán: (VD: Hà Nội)", 40));
            inputs.push_back(InputField({(float)(contentX + fieldWidth + 20), 200, (float)fieldWidth, 40}, u8"SĐT: (10-11 số)", 11));
            inputs.push_back(InputField({(float)(contentX + 2 * (fieldWidth + 20)), 200, (float)fieldWidth, 40}, u8"Lương: (VD: 50.5)", 20));
            inputs.push_back(InputField({(float)(contentX + 3 * (fieldWidth + 20)), 200, (float)fieldWidth, 40}, u8"Mã/Tên Đội: (VD: T001)", 40));
        }

        for (auto &input : inputs)
        {
            input.draw();
            input.update();
        }

        Button addBtn;
        addBtn.rect = {(float)(contentX + contentWidth - 340), 270, 140, 45};
        addBtn.text = u8"THÊM";
        addBtn.color = ACCENT_1;
        addBtn.hoverColor = {41, 128, 185, 255};
        addBtn.draw();

        Button clearBtn;
        clearBtn.rect = {(float)(contentX + contentWidth - 180), 270, 140, 45};
        clearBtn.text = u8"XOÁ FORM";
        clearBtn.color = {149, 165, 166, 255};
        clearBtn.hoverColor = {127, 140, 141, 255};
        clearBtn.draw();

        if (clearBtn.isClicked())
        {
            inputs.clear();
            showMessage(u8"Đã xoá form!");
        }

        if (addBtn.isClicked())
        {
            try
            {
                if (inputs.size() < 9)
                {
                    showMessage(u8"Form không hợp lệ!");
                }
                else
                {
                    string sSalary = trimCopy(inputs[7].text);

                    double salary = 0;
                    if (!sSalary.empty())
                    {
                        if (!isDoubleString(sSalary))
                        {
                            showMessage(u8"Lương phải là số!");
                            goto SKIP_ADD_PLAYER;
                        }
                        salary = stod(sSalary);
                    }

                    string idCT = trimCopy(inputs[0].text);
                    string hoTen = trimCopy(inputs[1].text);
                    string viTri = trimCopy(inputs[2].text);
                    string ngaySinh = trimCopy(inputs[3].text);
                    string cccd = trimCopy(inputs[4].text);
                    string queQuan = trimCopy(inputs[5].text);
                    string sdt = trimCopy(inputs[6].text);
                    string teamKeyRaw = trimCopy(inputs[8].text);

                    if (idCT.empty() || hoTen.empty() || teamKeyRaw.empty())
                    {
                        showMessage(u8"Vui lòng nhập Mã CT, Họ Tên và Mã/Tên Đội!");
                        goto SKIP_ADD_PLAYER;
                    }

                    // Kiểm tra trùng mã cầu thủ (không phân biệt hoa thường)
                    string idCTLower = idCT;
                    transform(idCTLower.begin(), idCTLower.end(), idCTLower.begin(), ::tolower);

                    for (auto &team : clb->getTeams())
                    {
                        for (auto &p : team.getPlayers())
                        {
                            string existingID = p.getMaCauThu();
                            string existingIDLower = existingID;
                            transform(existingIDLower.begin(), existingIDLower.end(), existingIDLower.begin(), ::tolower);

                            if (existingIDLower == idCTLower)
                            {
                                showMessage(u8"Mã cầu thủ đã tồn tại!");
                                goto SKIP_ADD_PLAYER;
                            }
                        }
                    }

                    Player newPlayer(idCT, hoTen, ngaySinh, queQuan, sdt, idCT, viTri,
                                     0, "", salary, "", "", cccd);

                    string teamKeyLower = toLowerCopy(teamKeyRaw);
                    bool found = false;

                    for (auto &team : clb->getTeams())
                    {
                        string teamId = trimCopy(team.getIDDoi());
                        string teamName = trimCopy(team.getTenDoi());
                        string teamIdLower = toLowerCopy(teamId);
                        string teamNameLower = toLowerCopy(teamName);

                        if (!teamId.empty() && teamIdLower == teamKeyLower)
                        {
                            team.themCauThu(newPlayer);
                            found = true;
                            saveToJson(*clb);
                            showMessage(u8"Đã thêm cầu thủ thành công!");
                            inputs.clear();
                            break;
                        }

                        if (!teamNameLower.empty() && teamNameLower.find(teamKeyLower) != string::npos)
                        {
                            team.themCauThu(newPlayer);
                            found = true;
                            saveToJson(*clb);
                            showMessage(u8"Đã thêm cầu thủ thành công!");
                            inputs.clear();
                            break;
                        }
                    }

                    if (!found)
                    {
                        showMessage(u8"Không tìm thấy đội!");
                    }
                }
            }
            catch (...)
            {
                showMessage(u8"Lỗi nhập dữ liệu!");
            }
        }
    SKIP_ADD_PLAYER:;

        DrawTextEx2(u8"DANH SÁCH CẦU THỦ", contentX, 340, 22, TEXT_DARK);

        int totalPlayers = 0;
        for (auto &team : clb->getTeams())
            totalPlayers += team.getPlayers().size();

        int contentHeight = max(1, totalPlayers) * 80 + 20;
        int viewHeight = screenHeight - 420;
        int startY = 390;

        BeginScissorMode(contentX, startY, contentWidth, viewHeight);

        int y = startY - scrollOffset;
        for (auto &team : clb->getTeams())
        {
            for (auto &p : team.getPlayers())
            {
                drawPlayerCard(p, team.getTenDoi(), contentX, y, contentWidth - 20, &team);
                y += 80;
            }
        }

        EndScissorMode();
        drawScrollbar(contentHeight, viewHeight, startY);
    }

    void drawStatistics()
    {
        drawHeader(u8"THỐNG KÊ THÀNH TÍCH CẦU THỦ TRONG NĂM");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        auto &teams = clb->getTeams();

        if (teams.empty())
        {
            DrawTextEx2(u8"Chưa có dữ liệu thống kê!", contentX + 100, 300, 24, ACCENT_2);
            return;
        }

        // Thu thập dữ liệu
        MyVector<pair<string, int>> topScorers;
        MyVector<pair<string, string>> playersWithHonors;
        int totalPlayers = 0;
        int totalGoals = 0;
        double totalSalary = 0;

        int maxGoals = 1;
        for (auto &team : teams)
        {
            totalPlayers += team.getPlayers().size();
            for (auto &p : team.getPlayers())
            {
                totalGoals += p.getBanThangHieuSuat();
                totalSalary += p.getLuong();
                topScorers.push_back({p.getTen(), p.getBanThangHieuSuat()});
                if (!p.getDanhHieu().empty())
                    playersWithHonors.push_back({p.getTen(), p.getDanhHieu()});
                if (p.getBanThangHieuSuat() > maxGoals)
                    maxGoals = p.getBanThangHieuSuat();
            }
        }

        sort(topScorers.begin(), topScorers.end(),
             [](const pair<string, int> &a, const pair<string, int> &b)
             { return a.second > b.second; });

        // ===== THẺ THỐNG KÊ TỔNG QUAN 3D =====
        int statsY = 110;
        int cardWidth = (contentWidth - 60) / 3;

        // Card 1: Tổng cầu thủ
        {
            int cardX = contentX;
            Color shadowColor = {0, 0, 0, 40};
            DrawRectangleRounded({(float)(cardX + 8), (float)(statsY + 8), (float)cardWidth, 140}, 0.15f, 10, shadowColor);

            // Gradient background
            Rectangle cardRect = {(float)cardX, (float)statsY, (float)cardWidth, 140};
            DrawRectangleGradientEx(cardRect,
                                    (Color){52, 152, 219, 255},
                                    (Color){41, 128, 185, 255},
                                    (Color){41, 128, 185, 255},
                                    (Color){52, 152, 219, 255});
            // DrawRectangleRoundedLines(cardRect, 0.15f, 10, 3);  // Border

            // Icon 3D
            int iconX = cardX + cardWidth / 2;
            int iconY = statsY + 45;
            DrawCircle(iconX + 3, iconY + 3, 28, (Color){0, 0, 0, 60});
            DrawCircle(iconX, iconY, 28, (Color){255, 255, 255, 255});
            DrawCircle(iconX, iconY, 24, ACCENT_1);
            DrawTextEx2("P", iconX - 10, iconY - 12, 28, TEXT_LIGHT);

            // Số liệu với hiệu ứng
            string playerCount = to_string(totalPlayers);
            int numWidth = MeasureTextEx2(playerCount, 42);
            DrawTextEx2(playerCount, cardX + (cardWidth - numWidth) / 2 + 2, statsY + 92, 42, (Color){0, 0, 0, 80});
            DrawTextEx2(playerCount, cardX + (cardWidth - numWidth) / 2, statsY + 90, 42, TEXT_LIGHT);

            int labelWidth = MeasureTextEx2(u8"CẦU THỦ", 16);
            DrawTextEx2(u8"CẦU THỦ", cardX + (cardWidth - labelWidth) / 2, statsY + 145, 16, (Color){255, 255, 255, 200});
        }

        // Card 2: Tổng bàn thắng
        {
            int cardX = contentX + cardWidth + 30;
            Color shadowColor = {0, 0, 0, 40};
            DrawRectangleRounded({(float)(cardX + 8), (float)(statsY + 8), (float)cardWidth, 140}, 0.15f, 10, shadowColor);

            Rectangle cardRect = {(float)cardX, (float)statsY, (float)cardWidth, 140};
            DrawRectangleGradientEx(cardRect,
                                    (Color){231, 76, 60, 255},
                                    (Color){192, 57, 43, 255},
                                    (Color){192, 57, 43, 255},
                                    (Color){231, 76, 60, 255});
            // DrawRectangleRoundedLines(cardRect, 0.15f, 10, 3);

            int iconX = cardX + cardWidth / 2;
            int iconY = statsY + 45;
            DrawCircle(iconX + 3, iconY + 3, 28, (Color){0, 0, 0, 60});
            DrawCircle(iconX, iconY, 28, (Color){255, 255, 255, 255});
            DrawCircle(iconX, iconY, 24, ACCENT_2);
            DrawTextEx2("G", iconX - 9, iconY - 12, 28, TEXT_LIGHT);

            string goalCount = to_string(totalGoals);
            int numWidth = MeasureTextEx2(goalCount, 42);
            DrawTextEx2(goalCount, cardX + (cardWidth - numWidth) / 2 + 2, statsY + 92, 42, (Color){0, 0, 0, 80});
            DrawTextEx2(goalCount, cardX + (cardWidth - numWidth) / 2, statsY + 90, 42, TEXT_LIGHT);

            int labelWidth = MeasureTextEx2(u8"BÀN THẮNG", 16);
            DrawTextEx2(u8"BÀN THẮNG", cardX + (cardWidth - labelWidth) / 2, statsY + 145, 16, (Color){255, 255, 255, 200});
        }

        // Card 3: Tổng lương
        {
            int cardX = contentX + 2 * (cardWidth + 30);
            Color shadowColor = {0, 0, 0, 40};
            DrawRectangleRounded({(float)(cardX + 8), (float)(statsY + 8), (float)cardWidth, 140}, 0.15f, 10, shadowColor);

            Rectangle cardRect = {(float)cardX, (float)statsY, (float)cardWidth, 140};
            DrawRectangleGradientEx(cardRect,
                                    (Color){46, 204, 113, 255},
                                    (Color){39, 174, 96, 255},
                                    (Color){39, 174, 96, 255},
                                    (Color){46, 204, 113, 255});
            // DrawRectangleRoundedLines(cardRect, 0.15f, 10, 3);

            int iconX = cardX + cardWidth / 2;
            int iconY = statsY + 45;
            DrawCircle(iconX + 3, iconY + 3, 28, (Color){0, 0, 0, 60});
            DrawCircle(iconX, iconY, 28, (Color){255, 255, 255, 255});
            DrawCircle(iconX, iconY, 24, (Color){46, 204, 113, 255});
            DrawTextEx2("$", iconX - 8, iconY - 12, 28, TEXT_LIGHT);

            string salaryText = to_string((int)(totalSalary / 1000)) + "M";
            int numWidth = MeasureTextEx2(salaryText, 42);
            DrawTextEx2(salaryText, cardX + (cardWidth - numWidth) / 2 + 2, statsY + 92, 42, (Color){0, 0, 0, 80});
            DrawTextEx2(salaryText, cardX + (cardWidth - numWidth) / 2, statsY + 90, 42, TEXT_LIGHT);

            int labelWidth = MeasureTextEx2(u8"TỔNG LƯƠNG", 16);
            DrawTextEx2(u8"TỔNG LƯƠNG", cardX + (cardWidth - labelWidth) / 2, statsY + 145, 16, (Color){255, 255, 255, 200});
        }

        // ===== PANEL TOP GHI BÀN VỚI BIỂU ĐỒ 3D =====
        int panelY = 280;
        int panelWidth = (contentWidth - 20) / 2;
        int panelHeight = screenHeight - panelY - 50;

        // Panel trái: Top ghi bàn
        {
            int panelX = contentX;

            // Shadow
            DrawRectangleRounded({(float)(panelX + 6), (float)(panelY + 6), (float)panelWidth, (float)panelHeight}, 0.1f, 10, (Color){0, 0, 0, 30});

            // Background gradient
            DrawRectangleGradientV(panelX, panelY, panelWidth, panelHeight, (Color){248, 249, 250, 255}, (Color){233, 236, 239, 255});
            DrawRectangleRounded({(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 0.1f, 10, (Color){255, 255, 255, 0});
            // DrawRectangleRoundedLines({(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 0.1f, 10, 2);

            // Header với gradient
            DrawRectangleGradientEx({(float)panelX, (float)panelY, (float)panelWidth, 50},
                                    (Color){52, 152, 219, 255},
                                    (Color){41, 128, 185, 255},
                                    (Color){41, 128, 185, 255},
                                    (Color){52, 152, 219, 255});
            // DrawRectangleRoundedLines({(float)panelX, (float)panelY, (float)panelWidth, 50}, 0.1f, 10, 2);

            DrawTextEx2(u8"TOP 5 GHI BÀN", panelX + 20, panelY + 15, 20, TEXT_LIGHT);

            // Vẽ biểu đồ cột 3D
            int chartY = panelY + 70;
            int chartHeight = panelHeight - 100;
            int barCount = min(5, (int)topScorers.size());

            if (barCount > 0)
            {
                int barSpacing = 15;
                int totalBarWidth = panelWidth - 100;
                int barWidth = (totalBarWidth - (barCount - 1) * barSpacing) / barCount;

                Color barColors[] = {
                    {255, 193, 7, 255},   // Gold
                    {158, 158, 158, 255}, // Silver
                    {205, 127, 50, 255},  // Bronze
                    {52, 152, 219, 255},  // Blue
                    {46, 204, 113, 255}   // Green
                };

                for (int i = 0; i < barCount; i++)
                {
                    int barX = panelX + 50 + i * (barWidth + barSpacing);
                    int goals = topScorers[i].second;
                    int barH = (int)((float)goals / maxGoals * (chartHeight - 80));
                    int barBottomY = chartY + chartHeight - 60;
                    int barTopY = barBottomY - barH;

                    // 3D effect - vẽ mặt nghiêng
                    Color sideColor = barColors[i];
                    sideColor.r = (unsigned char)max(0, (int)sideColor.r - 40);
                    sideColor.g = (unsigned char)max(0, (int)sideColor.g - 40);
                    sideColor.b = (unsigned char)max(0, (int)sideColor.b - 40);

                    // Mặt phải (3D depth)
                    Vector2 rightFace[] = {
                        {(float)(barX + barWidth), (float)barTopY},
                        {(float)(barX + barWidth + 8), (float)(barTopY - 8)},
                        {(float)(barX + barWidth + 8), (float)(barBottomY - 8)},
                        {(float)(barX + barWidth), (float)barBottomY}};
                    DrawTriangleFan(rightFace, 4, sideColor);

                    // Mặt trên (3D top)
                    Color topColor = barColors[i];
                    topColor.r = (unsigned char)min(255, (int)topColor.r + 30);
                    topColor.g = (unsigned char)min(255, (int)topColor.g + 30);
                    topColor.b = (unsigned char)min(255, (int)topColor.b + 30);

                    Vector2 topFace[] = {
                        {(float)barX, (float)barTopY},
                        {(float)(barX + 8), (float)(barTopY - 8)},
                        {(float)(barX + barWidth + 8), (float)(barTopY - 8)},
                        {(float)(barX + barWidth), (float)barTopY}};
                    DrawTriangleFan(topFace, 4, topColor);

                    // Mặt chính với gradient
                    DrawRectangleGradientV(barX, barTopY, barWidth, barH, barColors[i], sideColor);
                    DrawRectangleLinesEx({(float)barX, (float)barTopY, (float)barWidth, (float)barH}, 2, (Color){255, 255, 255, 150});

                    // Badge hạng
                    int badgeY = barTopY - 35;
                    DrawCircle(barX + barWidth / 2 + 2, badgeY + 2, 16, (Color){0, 0, 0, 60});
                    DrawCircle(barX + barWidth / 2, badgeY, 16, barColors[i]);
                    DrawCircle(barX + barWidth / 2, badgeY, 14, (Color){255, 255, 255, 255});
                    string rank = to_string(i + 1);
                    int rankW = MeasureTextEx2(rank, 16);
                    DrawTextEx2(rank, barX + barWidth / 2 - rankW / 2, badgeY - 8, 16, barColors[i]);

                    // Số bàn thắng trên đỉnh cột
                    string goalStr = to_string(goals);
                    int goalW = MeasureTextEx2(goalStr, 18);
                    DrawTextEx2(goalStr, barX + (barWidth - goalW) / 2 + 1, barTopY - 16, 18, (Color){0, 0, 0, 100});
                    DrawTextEx2(goalStr, barX + (barWidth - goalW) / 2, barTopY - 17, 18, TEXT_DARK);

                    // Tên cầu thủ (đầy đủ)
                    string playerName = topScorers[i].first;
                    // Không cắt ngắn tên

                    int nameW = MeasureTextEx2(playerName, 11);
                    int nameX = barX + (barWidth - nameW) / 2;
                    DrawTextEx2(playerName, nameX, barBottomY + 8, 11, TEXT_DARK);
                }
            }
        }

        // Panel phải: Danh hiệu với card 3D
        {
            int panelX = contentX + panelWidth + 20;

            // Shadow
            DrawRectangleRounded({(float)(panelX + 6), (float)(panelY + 6), (float)panelWidth, (float)panelHeight}, 0.1f, 10, (Color){0, 0, 0, 30});

            // Background
            DrawRectangleGradientV(panelX, panelY, panelWidth, panelHeight, (Color){248, 249, 250, 255}, (Color){233, 236, 239, 255});
            DrawRectangleRounded({(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 0.1f, 10, (Color){255, 255, 255, 0});
            // DrawRectangleRoundedLines({(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 0.1f, 10, 2);

            // Header
            DrawRectangleGradientEx({(float)panelX, (float)panelY, (float)panelWidth, 50},
                                    (Color){142, 68, 173, 255},
                                    (Color){155, 89, 182, 255},
                                    (Color){155, 89, 182, 255},
                                    (Color){142, 68, 173, 255});
            // DrawRectangleRoundedLines({(float)panelX, (float)panelY, (float)panelWidth, 50}, 0.1f, 10, 2);

            DrawTextEx2(u8"DANH HIỆU CẦU THỦ", panelX + 20, panelY + 15, 20, TEXT_LIGHT);

            // Scrollable content area
            int contentStartY = panelY + 70;
            int contentAreaHeight = panelHeight - 90;
            int totalContentHeight = playersWithHonors.size() * 75 + 20;

            // Biến scroll riêng cho panel này
            static int honorScrollOffset = 0;

            BeginScissorMode(panelX + 10, contentStartY, panelWidth - 20, contentAreaHeight);

            if (playersWithHonors.empty())
            {
                DrawTextEx2(u8"Chưa có danh hiệu", panelX + 40, contentStartY + 50, 16, (Color){150, 150, 150, 255});
            }
            else
            {
                int cardY = contentStartY - honorScrollOffset;
                Color honorColors[] = {
                    {243, 156, 18, 255}, // Orange
                    {46, 204, 113, 255}, // Green
                    {52, 152, 219, 255}, // Blue
                    {155, 89, 182, 255}, // Purple
                    {231, 76, 60, 255}   // Red
                };

                for (int i = 0; i < min(10, (int)playersWithHonors.size()); i++)
                {
                    int cardX = panelX + 15;
                    int cardW = panelWidth - 30;

                    // Card 3D shadow
                    DrawRectangleRounded({(float)(cardX + 4), (float)(cardY + 4), (float)cardW, 65}, 0.15f, 8, (Color){0, 0, 0, 40});

                    // Card background gradient
                    Color cardColor = honorColors[i % 5];
                    Color darkCard = cardColor;
                    darkCard.r = (unsigned char)max(0, (int)darkCard.r - 30);
                    darkCard.g = (unsigned char)max(0, (int)darkCard.g - 30);
                    darkCard.b = (unsigned char)max(0, (int)darkCard.b - 30);

                    DrawRectangleGradientEx({(float)cardX, (float)cardY, (float)cardW, 65},
                                            (Color){255, 255, 255, 255},
                                            (Color){250, 250, 250, 255},
                                            (Color){245, 245, 245, 255},
                                            (Color){255, 255, 255, 255});
                    // DrawRectangleRoundedLines({(float)cardX, (float)cardY, (float)cardW, 65}, 0.15f, 8, 2);

                    // Medal 3D
                    int medalX = cardX + 20;
                    int medalY = cardY + 32;

                    DrawCircle(medalX + 2, medalY + 2, 20, (Color){0, 0, 0, 60});
                    DrawCircle(medalX, medalY, 20, cardColor);
                    DrawCircle(medalX, medalY, 18, darkCard);
                    DrawCircle(medalX, medalY, 14, cardColor);

                    // Star
                    DrawTextEx2("*", medalX - 5, medalY - 10, 20, TEXT_LIGHT);

                    // Player info (đầy đủ)
                    string playerInfo = playersWithHonors[i].first;
                    // Không cắt ngắn tên
                    DrawTextEx2(playerInfo, cardX + 50, cardY + 12, 12, TEXT_DARK);

                    // Honor/Achievement (đầy đủ)
                    string honor = playersWithHonors[i].second;
                    // Không cắt ngắn danh hiệu

                    // Honor badge
                    int honorW = MeasureTextEx2(honor, 11);
                    DrawRectangleRounded({(float)(cardX + 50), (float)(cardY + 35), (float)(honorW + 20), 22}, 0.3f, 6, cardColor);
                    DrawTextEx2(honor, cardX + 60, cardY + 38, 11, TEXT_LIGHT);

                    cardY += 75;
                }
            }

            EndScissorMode();

            // Scrollbar nếu cần
            if (totalContentHeight > contentAreaHeight)
            {
                float mouseWheel = GetMouseWheelMove();
                if (CheckCollisionPointRec(GetMousePosition(), {(float)panelX, (float)contentStartY, (float)panelWidth, (float)contentAreaHeight}))
                {
                    if (mouseWheel != 0)
                    {
                        honorScrollOffset -= (int)(mouseWheel * 30);
                        honorScrollOffset = max(0, min(honorScrollOffset, totalContentHeight - contentAreaHeight));
                    }
                }

                int scrollbarX = panelX + panelWidth - 15;
                float scrollbarHeight = (float)contentAreaHeight * contentAreaHeight / totalContentHeight;
                float scrollbarY = contentStartY + ((float)honorScrollOffset / (totalContentHeight - contentAreaHeight)) * (contentAreaHeight - scrollbarHeight);

                DrawRectangleRounded({(float)scrollbarX, (float)contentStartY, 8, (float)contentAreaHeight}, 0.5f, 8, (Color){200, 200, 200, 100});
                DrawRectangleRounded({(float)scrollbarX, scrollbarY, 8, scrollbarHeight}, 0.5f, 8, (Color){155, 89, 182, 255});
            }
        }
    }
    void drawEditPlayerPopup()
    {
        if (!showEditPlayer)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 800;
        int popupHeight = 600;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, ACCENT_1);
        DrawTextEx2(u8"CHỈNH SỬA THÔNG TIN CẦU THỦ", popupX + 30, popupY + 20, 22, TEXT_LIGHT);

        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 180), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"HỦY";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        Button saveBtn;
        saveBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        saveBtn.text = u8"LƯU";
        saveBtn.color = {46, 204, 113, 255};
        saveBtn.hoverColor = {39, 174, 96, 255};
        saveBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showEditPlayer = false;
            editInputs.clear(); // Xóa editInputs thay vì inputs
            return;
        }

        // Tạo form nhập liệu nếu chưa có
        if (editInputs.empty())
        {
            editInputs.push_back(InputField({0, 0, 0, 40}, u8"Họ Tên:", 40));
            editInputs.push_back(InputField({0, 0, 0, 40}, u8"Vị Trí:", 20));
            editInputs.push_back(InputField({0, 0, 0, 40}, u8"Ngày Sinh:", 20));
            editInputs.push_back(InputField({0, 0, 0, 40}, "CCCD:", 20));
            editInputs.push_back(InputField({0, 0, 0, 40}, u8"Quê Quán:", 40));
            editInputs.push_back(InputField({0, 0, 0, 40}, "SDT:", 11));
            editInputs.push_back(InputField({0, 0, 0, 40}, u8"Lương:", 20));

            // Load dữ liệu
            editInputs[0].text = editingPlayer.getTen();
            editInputs[1].text = editingPlayer.getViTri();
            editInputs[2].text = editingPlayer.getNgaySinh();
            editInputs[3].text = editingPlayer.getCCCD();
            editInputs[4].text = editingPlayer.getQueQuan();
            editInputs[5].text = editingPlayer.getThongTinLienLac();
            editInputs[6].text = to_string((int)editingPlayer.getLuong());
        }

        // Cập nhật vị trí input fields theo kích thước popup (mỗi frame)
        int fieldWidth = (popupWidth - 100) / 2;
        int leftX = popupX + 40;
        int rightX = popupX + 40 + fieldWidth + 20;

        editInputs[0].rect = {(float)leftX, (float)(popupY + 90), (float)fieldWidth, 40};
        editInputs[1].rect = {(float)rightX, (float)(popupY + 90), (float)fieldWidth, 40};
        editInputs[2].rect = {(float)leftX, (float)(popupY + 170), (float)fieldWidth, 40};
        editInputs[3].rect = {(float)rightX, (float)(popupY + 170), (float)fieldWidth, 40};
        editInputs[4].rect = {(float)leftX, (float)(popupY + 250), (float)fieldWidth, 40};
        editInputs[5].rect = {(float)rightX, (float)(popupY + 250), (float)fieldWidth, 40};
        editInputs[6].rect = {(float)leftX, (float)(popupY + 330), (float)fieldWidth, 40};

        // Vẽ và cập nhật các input
        for (auto &input : editInputs) // Dùng editInputs
        {
            input.draw();
            input.update();
        }

        // Hiển thị đội bóng (không cho sửa)
        DrawTextEx2(u8"Đội Bóng:", popupX + 40, popupY + 410, 16, TEXT_DARK);
        DrawRectangleRounded({(float)(popupX + 40), (float)(popupY + 435), (float)((popupWidth - 80)), 40}, 0.1f, 10, (Color){240, 240, 240, 255});
        DrawTextEx2(editingPlayerTeam, popupX + 55, popupY + 445, 18, ACCENT_1);

        DrawTextEx2(u8"Lưu ý: Lương phải là số", popupX + 40, popupY + 500, 14, (Color){127, 140, 141, 255});

        // Xử lý lưu
        if (saveBtn.isClicked())
        {
            // Bỏ validation, cho phép lưu luôn
            string sSalary = editInputs[6].text;
            double salary = 0;

            if (!sSalary.empty())
            {
                try
                {
                    salary = stod(sSalary);
                }
                catch (...)
                {
                    salary = 0;
                }
            }

            // Cập nhật trực tiếp player hiện tại thay vì tạo mới
            if (editingTeamIndex >= 0 && editingTeamIndex < (int)clb->getTeams().size())
            {
                // Lấy reference đến MyVector teams
                MyVector<Team> &teams = clb->getTeams();
                Team &team = teams[editingTeamIndex];

                // Lấy reference đến MyVector players
                MyVector<Player> &players = team.getPlayers();

                if (editingPlayerIndex >= 0 && editingPlayerIndex < (int)players.size())
                {
                    // Cập nhật trực tiếp từng trường bằng setter
                    Player &player = players[editingPlayerIndex];

                    // Cập nhật các trường từ form
                    player.setTen(editInputs[0].text);
                    player.setViTri(editInputs[1].text);
                    player.setNgaySinh(editInputs[2].text);
                    player.setCCCD(editInputs[3].text);
                    player.setQueQuan(editInputs[4].text);
                    player.setThongTinLienLac(editInputs[5].text);
                    player.setLuong(salary);

                    // Lưu vào JSON
                    saveToJson(*clb);

                    // Cập nhật lại searchResults nếu đang ở tab tìm kiếm
                    if (currentTab == 6 && isSearching)
                    {
                        for (auto &result : searchResults)
                        {
                            if (result.first.getID() == player.getID())
                            {
                                result.first = player;
                                break;
                            }
                        }
                    }

                    // Đóng popup và hiện thông báo
                    showEditPlayer = false;
                    editInputs.clear();
                    showMessage(u8"Đã cập nhật thông tin cầu thủ!");
                }
            }
        }
    }

    void drawEditHLVPopup()
    {
        if (!showEditHLV)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 600;
        int popupHeight = 450;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, COACH_COLOR);
        DrawTextEx2(u8"CHỈNH SỬA THÔNG TIN HLV", popupX + 30, popupY + 20, 22, TEXT_LIGHT);

        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 180), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"HỦY";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        Button saveBtn;
        saveBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        saveBtn.text = u8"LƯU";
        saveBtn.color = {46, 204, 113, 255};
        saveBtn.hoverColor = {39, 174, 96, 255};
        saveBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showEditHLV = false;
            hlvInputs.clear();
            return;
        }

        // Cập nhật vị trí input fields theo kích thước popup
        if (hlvInputs.size() >= 2)
        {
            hlvInputs[0].rect.x = popupX + 50;
            hlvInputs[0].rect.y = popupY + 100;
            hlvInputs[0].rect.width = popupWidth - 100;

            hlvInputs[1].rect.x = popupX + 50;
            hlvInputs[1].rect.y = popupY + 180;
            hlvInputs[1].rect.width = popupWidth - 100;
        }

        // Vẽ và cập nhật các input
        for (auto &input : hlvInputs)
        {
            input.draw();
            input.update();
        }

        DrawTextEx2(u8"Nhập tên đầy đủ của HLV. Để trống nếu chưa có.", popupX + 40, popupY + 400, 14, (Color){127, 140, 141, 255});

        // Xử lý lưu
        if (saveBtn.isClicked())
        {
            if (editingHLVTeamIndex >= 0 && editingHLVTeamIndex < (int)clb->getTeams().size())
            {
                MyVector<Team> &teams = clb->getTeams();
                Team &team = teams[editingHLVTeamIndex];

                team.setHLVTruong(hlvInputs[0].text);
                team.setHLVPho(hlvInputs[1].text);

                saveToJson(*clb);
                showMessage(u8"Đã cập nhật thông tin HLV!");
                showEditHLV = false;
                hlvInputs.clear();
            }
        }
    }

    void drawEditTeamPopup()
    {
        if (!showEditTeam)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 600;
        int popupHeight = 300;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, ACCENT_1);
        DrawTextEx2(u8"ĐỔI TÊN ĐỘI", popupX + 30, popupY + 20, 22, TEXT_LIGHT);

        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 180), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"HỦY";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        Button saveBtn;
        saveBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        saveBtn.text = u8"LƯU";
        saveBtn.color = {46, 204, 113, 255};
        saveBtn.hoverColor = {39, 174, 96, 255};
        saveBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showEditTeam = false;
            teamEditInputs.clear();
            return;
        }

        // Khởi tạo input field nếu chưa có
        if (teamEditInputs.empty() && editingTeamIndex2 >= 0 && editingTeamIndex2 < (int)clb->getTeams().size())
        {
            Team &team = clb->getTeams()[editingTeamIndex2];
            teamEditInputs.push_back(InputField({0, 0, 0, 45}, u8"Tên Đội Mới:"));
            teamEditInputs[0].text = team.getTenDoi();
        }

        // Cập nhật vị trí input field
        if (!teamEditInputs.empty())
        {
            teamEditInputs[0].rect.x = popupX + 50;
            teamEditInputs[0].rect.y = popupY + 120;
            teamEditInputs[0].rect.width = popupWidth - 100;

            teamEditInputs[0].draw();
            teamEditInputs[0].update();
        }

        DrawTextEx2(u8"Nhập tên mới cho đội bóng.", popupX + 40, popupY + 250, 14, (Color){127, 140, 141, 255});

        // Xử lý lưu
        if (saveBtn.isClicked())
        {
            if (!teamEditInputs.empty() && !teamEditInputs[0].text.empty())
            {
                if (editingTeamIndex2 >= 0 && editingTeamIndex2 < (int)clb->getTeams().size())
                {
                    MyVector<Team> &teams = clb->getTeams();
                    Team &team = teams[editingTeamIndex2];

                    team.setTenDoi(teamEditInputs[0].text);
                    saveToJson(*clb);
                    showMessage(u8"Đã đổi tên đội thành công!");
                    showEditTeam = false;
                    teamEditInputs.clear();
                }
            }
            else
            {
                showMessage(u8"Tên đội không được để trống!");
            }
        }
    }

    void drawSearch()
    {
        drawHeader(u8"TÌM KIẾM CẦU THỦ");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        // Chỉ tạo input field một lần duy nhất
        if (inputs.empty())
        {
            inputs.push_back(InputField({(float)contentX, 150, (float)(contentWidth - 450), 45}, u8"Tên Cầu Thủ:"));
        }

        // Cập nhật vị trí nếu màn hình thay đổi kích thước
        if (!inputs.empty())
        {
            inputs[0].rect.x = contentX;
            inputs[0].rect.width = contentWidth - 450;
        }

        inputs[0].draw();
        inputs[0].update();

        // Thêm text hướng dẫn tìm kiếm
        DrawTextEx2(u8"Nhập tên cầu thủ để tìm kiếm (có thể tìm theo tên đầy đủ hoặc một phần)",
                    contentX, 205, 14, (Color){127, 140, 141, 255});

        Button searchBtn;
        searchBtn.rect = {(float)(contentX + contentWidth - 400), 150, 200, 45};
        searchBtn.text = u8"TÌM KIẾM";
        searchBtn.color = ACCENT_1;
        searchBtn.hoverColor = {41, 128, 185, 255};
        searchBtn.draw();

        // Chỉ hiện nút Xóa khi đã có kết quả tìm kiếm
        if (isSearching)
        {
            Button clearBtn;
            clearBtn.rect = {(float)(contentX + contentWidth - 180), 150, 180, 45};
            clearBtn.text = u8"XOÁ TÌM KIẾM";
            clearBtn.color = {149, 165, 166, 255};
            clearBtn.hoverColor = {127, 140, 141, 255};
            clearBtn.draw();

            if (clearBtn.isClicked())
            {
                isSearching = false;
                searchResults.clear();
                inputs[0].text = "";
                scrollOffset = 0;
                showMessage(u8"Đã xoá kết quả tìm kiếm!");
            }
        }

        if (searchBtn.isClicked() && !inputs[0].text.empty())
        {
            isSearching = true;
            searchResults.clear();
            string searchName = inputs[0].text;

            // Xóa khoảng trắng thừa ở đầu và cuối
            searchName.erase(0, searchName.find_first_not_of(" \t\n\r"));
            searchName.erase(searchName.find_last_not_of(" \t\n\r") + 1);

            // Chuyển sang lowercase (chỉ với ASCII)
            transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

            for (auto &team : clb->getTeams())
            {
                for (auto &p : team.getPlayers())
                {
                    string playerName = p.getTen();

                    // Xóa khoảng trắng thừa ở đầu và cuối
                    playerName.erase(0, playerName.find_first_not_of(" \t\n\r"));
                    playerName.erase(playerName.find_last_not_of(" \t\n\r") + 1);

                    transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);

                    if (playerName.find(searchName) != string::npos)
                    {
                        searchResults.push_back({p, team.getTenDoi()});
                    }
                }
            }

            scrollOffset = 0; // Reset scroll khi tìm kiếm mới

            if (searchResults.empty())
            {
                showMessage(u8"Không tìm thấy cầu thủ nào!");
            }
            else
            {
                showMessage(u8"Tìm thấy " + to_string(searchResults.size()) + u8" cầu thủ!");
            }
        }

        // Hiển thị hướng dẫn khi chưa tìm kiếm
        if (!isSearching)
        {
            // Card hướng dẫn
            int guideY = 240;
            int guideHeight = screenHeight - guideY - 50;

            DrawRectangleRounded({(float)contentX, (float)guideY, (float)contentWidth, (float)guideHeight}, 0.1f, 10, CARD_BG);

            // Icon tìm kiếm lớn
            int iconY = guideY + 80;
            DrawCircle(contentX + contentWidth / 2 + 4, iconY + 4, 60, (Color){0, 0, 0, 30});
            DrawCircle(contentX + contentWidth / 2, iconY, 60, ACCENT_1);
            DrawCircle(contentX + contentWidth / 2, iconY, 50, (Color){255, 255, 255, 255});
            DrawCircle(contentX + contentWidth / 2 - 8, iconY - 8, 22, ACCENT_1);
            DrawRectangleRounded({(float)(contentX + contentWidth / 2 + 15), (float)(iconY + 15), 25, 8}, 0.5f, 6, ACCENT_1);

            // Tiêu đề
            string title = u8"TÌM KIẾM CẦU THỦ";
            int titleWidth = MeasureTextEx2(title, 28);
            DrawTextEx2(title, contentX + (contentWidth - titleWidth) / 2, iconY + 100, 28, TEXT_DARK);

            // Hướng dẫn
            int guideTextY = iconY + 160;
            string guide1 = u8"Nhập tên cầu thủ vào ô trên và bấm TÌM KIẾM";
            string guide2 = u8"Bạn có thể tìm kiếm theo tên đầy đủ hoặc một phần của tên";
            string guide3 = "Vi du: 'Nguyen', 'Van', 'Nguyen Van'...";

            int g1Width = MeasureTextEx2(guide1, 16);
            int g2Width = MeasureTextEx2(guide2, 16);
            int g3Width = MeasureTextEx2(guide3, 16);

            DrawTextEx2(guide1, contentX + (contentWidth - g1Width) / 2, guideTextY, 16, (Color){100, 100, 100, 255});
            DrawTextEx2(guide2, contentX + (contentWidth - g2Width) / 2, guideTextY + 30, 16, (Color){100, 100, 100, 255});
            DrawTextEx2(guide3, contentX + (contentWidth - g3Width) / 2, guideTextY + 60, 16, ACCENT_1);

            // Thống kê nhanh
            int statsY = guideTextY + 130;
            int totalPlayers = 0;
            for (auto &team : clb->getTeams())
                totalPlayers += team.getPlayers().size();

            DrawRectangleRounded({(float)(contentX + contentWidth / 2 - 200), (float)statsY, 400, 80}, 0.15f, 10, (Color){240, 248, 255, 255});

            string statsText = u8"Hiện có " + to_string(totalPlayers) + u8" cầu thủ trong " + to_string(clb->getTeams().size()) + u8" đội";
            int statsWidth = MeasureTextEx2(statsText, 18);
            DrawTextEx2(statsText, contentX + (contentWidth - statsWidth) / 2, statsY + 20, 18, TEXT_DARK);

            string tipsText = u8"Sẵn sàng tìm kiếm!";
            int tipsWidth = MeasureTextEx2(tipsText, 16);
            DrawTextEx2(tipsText, contentX + (contentWidth - tipsWidth) / 2, statsY + 48, 16, ACCENT_1);
        }
        // Hiển thị kết quả tìm kiếm
        else if (isSearching && !searchResults.empty())
        {
            DrawTextEx2(u8"KẾT QUẢ TÌM KIẾM (" + to_string(searchResults.size()) + u8" cầu thủ)",
                        contentX, 220, 22, TEXT_DARK);

            int contentHeight = searchResults.size() * 195 + 20;
            int viewHeight = screenHeight - 300;
            int startY = 270;

            BeginScissorMode(contentX, startY, contentWidth, viewHeight);

            int y = startY - scrollOffset;
            int resultIndex = 0;
            for (auto &result : searchResults)
            {
                Player &p = result.first;
                string &teamName = result.second;

                DrawRectangleRounded({(float)contentX, (float)y, (float)contentWidth, 180}, 0.1f, 10, CARD_BG);
                DrawRectangleRounded({(float)contentX, (float)y, (float)contentWidth, 45}, 0.1f, 10, ACCENT_1);

                int badgeX = contentX + 30;
                if (p.getVaiTro() == "DT")
                {
                    DrawRectangleRounded({(float)badgeX, (float)(y + 11), 40, 24}, 0.3f, 8, CAPTAIN_COLOR);
                    DrawTextEx2("DT", badgeX + 8, y + 14, 14, TEXT_LIGHT);
                    badgeX += 50;
                }
                else if (p.getVaiTro() == "DP")
                {
                    DrawRectangleRounded({(float)badgeX, (float)(y + 11), 40, 24}, 0.3f, 8, VICE_CAPTAIN_COLOR);
                    DrawTextEx2("DP", badgeX + 8, y + 14, 14, TEXT_LIGHT);
                    badgeX += 50;
                }

                DrawTextEx2(p.getTen(), badgeX, y + 12, 22, TEXT_LIGHT);

                // Nút CCCD
                Button cccdBtn;
                cccdBtn.rect = {(float)(contentX + contentWidth - 200), (float)(y + 5), 80, 35};
                cccdBtn.text = "CCCD";
                cccdBtn.color = {46, 204, 113, 255};
                cccdBtn.hoverColor = {39, 174, 96, 255};
                cccdBtn.draw();

                if (cccdBtn.isClicked())
                {
                    selectedPlayer = p;
                    selectedPlayerTeam = teamName;
                    showPlayerDetail = true;
                }

                // Nút SỬA
                Button editBtn;
                editBtn.rect = {(float)(contentX + contentWidth - 110), (float)(y + 5), 80, 35};
                editBtn.text = u8"SỬA";
                editBtn.color = {241, 196, 15, 255};
                editBtn.hoverColor = {243, 156, 18, 255};
                editBtn.draw();

                if (editBtn.isClicked())
                {
                    // Tìm team index và player index
                    for (int t = 0; t < (int)clb->getTeams().size(); t++)
                    {
                        auto &team = clb->getTeams()[t];
                        if (team.getTenDoi() == teamName)
                        {
                            auto &players = team.getPlayers();
                            for (int pi = 0; pi < (int)players.size(); pi++)
                            {
                                if (players[pi].getID() == p.getID())
                                {
                                    editingPlayer = p;
                                    editingPlayerTeam = teamName;
                                    editingTeamIndex = t;
                                    editingPlayerIndex = pi;
                                    showEditPlayer = true;
                                    editInputs.clear();
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }

                int infoY = y + 55;
                DrawTextEx2(u8"THÔNG TIN CÁ NHÂN", contentX + 30, infoY, 16, TEXT_DARK);
                infoY += 25;
                DrawTextEx2("Ma: " + p.getID(), contentX + 40, infoY, 14, ACCENT_1);
                DrawTextEx2(u8"Ngày sinh: " + p.getNgaySinh(), contentX + 300, infoY, 14, ACCENT_1);
                infoY += 22;

                DrawTextEx2(u8"Quê quán: " + p.getQueQuan(), contentX + 40, infoY, 14, ACCENT_1);
                DrawTextEx2(u8"Vị trí: " + p.getViTri(), contentX + 300, infoY, 14, ACCENT_1);
                infoY += 30;

                DrawTextEx2(u8"THÀNH TÍCH", contentX + 30, infoY, 16, TEXT_DARK);
                infoY += 25;

                DrawTextEx2(u8"Bàn thắng: " + to_string(p.getBanThang()), contentX + 40, infoY, 14, ACCENT_2);
                DrawTextEx2(u8"Lương: " + formatVND(p.getLuong(), true), contentX + 240, infoY, 14, {46, 204, 113, 255});
                DrawTextEx2(u8"Đội: " + teamName, contentX + 400, infoY, 14, ACCENT_1);

                y += 195;
                resultIndex++;
            }

            EndScissorMode();
            drawScrollbar(contentHeight, viewHeight, startY);
        }
        else if (isSearching && searchResults.empty())
        {
            DrawTextEx2(u8"Không tìm thấy cầu thủ nào!", contentX + 100, 350, 20, ACCENT_2);
        }
    }
    void drawAchievements()
    {
        drawHeader(u8"THÀNH TÍCH CẦU THỦ CÁC NĂM");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        // Player list on the left
        int listWidth = 280;
        int listX = contentX;
        int listY = 160;
        int listHeight = screenHeight - listY - 30;

        // Player details on the right
        int detailX = listX + listWidth + 40;
        int detailWidth = contentWidth - listWidth - 40;

        // Draw player list panel
        DrawRectangle(listX, listY, listWidth, listHeight, {245, 245, 245, 255});
        DrawRectangleLinesEx({(float)listX, (float)listY, (float)listWidth, (float)listHeight}, 1, BORDER_LIGHT);

        DrawTextEx2(u8"DANH SÁCH CẦU THỦ", listX + 20, listY - 30, 16, TEXT_DARK);

        // Draw player list with scroll
        static int achievementScroll = 0;
        int playerItemHeight = 60;
        int visibleItems = (listHeight - 20) / playerItemHeight;
        int totalPlayers = 0;

        for (const auto &team : clb->getTeams())
        {
            totalPlayers += team.getPlayers().size();
        }

        int maxScroll = max(0, totalPlayers - visibleItems);
        float wheel = GetMouseWheelMove();
        if (CheckCollisionPointRec(GetMousePosition(), {(float)listX, (float)listY, (float)listWidth, (float)listHeight}))
        {
            achievementScroll -= wheel * 2;
            achievementScroll = max(0, min(achievementScroll, maxScroll));
        }

        BeginScissorMode(listX + 10, listY + 10, listWidth - 20, listHeight - 20);

        int itemY = listY + 10 - (achievementScroll * playerItemHeight);
        int playerIndex = 0;

        for (const auto &team : clb->getTeams())
        {
            for (const auto &player : team.getPlayers())
            {
                if (itemY + playerItemHeight >= listY && itemY <= listY + listHeight)
                {
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), {(float)(listX + 10), (float)itemY, (float)(listWidth - 20), (float)playerItemHeight});
                    bool isSelected = (selectedPlayerIndex == playerIndex);

                    Color lightGray = {230, 230, 230, 255};
                    Color bgColor = isSelected ? ACCENT_1 : (isHovered ? lightGray : WHITE);
                    Color textColor = isSelected ? WHITE : TEXT_DARK;

                    DrawRectangle(listX + 10, itemY, listWidth - 20, playerItemHeight, bgColor);
                    DrawRectangleLinesEx({(float)(listX + 10), (float)itemY, (float)(listWidth - 20), (float)playerItemHeight}, 1, BORDER_LIGHT);

                    DrawTextEx2(player.getTen(), listX + 20, itemY + 10, 14, textColor);
                    Color secondaryColor = isSelected ? Color{240, 240, 240, 255} : ACCENT_2;
                    string positionText = player.getSoAo() > 0 ? (to_string(player.getSoAo()) + u8" • " + player.getViTri()) : player.getViTri();
                    DrawTextEx2(positionText, listX + 20, itemY + 32, 12, secondaryColor);

                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        selectedPlayerIndex = playerIndex;
                    }
                }

                itemY += playerItemHeight;
                playerIndex++;
            }
        }

        EndScissorMode();

        // Draw selected player details
        if (selectedPlayerIndex >= 0)
        {
            playerIndex = 0;
            for (const auto &team : clb->getTeams())
            {
                for (const auto &player : team.getPlayers())
                {
                    if (playerIndex == selectedPlayerIndex)
                    {
                        int detailY = listY;

                        // Player header
                        Color lightWhite = {240, 240, 240, 255};
                        DrawRectangle(detailX, detailY, detailWidth, 100, ACCENT_1);
                        DrawTextEx2(player.getTen(), detailX + 20, detailY + 15, 22, WHITE);
                        string jerseyInfo = player.getSoAo() > 0 ? (u8"Số áo: " + to_string(player.getSoAo()) + u8" • ") : u8"";
                        DrawTextEx2(jerseyInfo + u8"Vị trí: " + player.getViTri(), detailX + 20, detailY + 50, 14, lightWhite);
                        DrawTextEx2(u8"Đội: " + team.getTenDoi(), detailX + 20, detailY + 75, 14, lightWhite);

                        detailY += 120;

                        // Load achievement data if player changed
                        if (lastLoadedAchievementPlayerIndex != selectedPlayerIndex)
                        {
                            lastLoadedAchievementPlayerIndex = selectedPlayerIndex;
                            tempNgayGiaNhap = player.getNgayGiaNhap();
                            tempDoiBongTruoc = player.getDoiBongTruoc();
                            tempBanThangChoClub = player.getBanThangChoClub();
                            tempDanhHieuTaiClub = player.getDanhHieuTaiClub();
                            tempSoTranThiDau = player.getSoTranThiDau();
                            tempTheVang = player.getTheVang();
                            tempTheDo = player.getTheDo();

                            if (achievementInputs.empty())
                            {
                                achievementInputs.push_back(InputField({0, 0, 300, 40}, "", 20));  // Ngày gia nhập
                                achievementInputs.push_back(InputField({0, 0, 400, 40}, "", 100)); // Đội bóng trước
                                achievementInputs.push_back(InputField({0, 0, 150, 40}, "", 10));  // Bàn thắng
                                achievementInputs.push_back(InputField({0, 0, 150, 40}, "", 10));  // Kiến tạo
                                achievementInputs.push_back(InputField({0, 0, 400, 80}, "", 200)); // Danh hiệu
                            }

                            achievementInputs[0].text = tempNgayGiaNhap;
                            achievementInputs[1].text = tempDoiBongTruoc;
                            achievementInputs[2].text = tempBanThangChoClub > 0 ? to_string(tempBanThangChoClub) : "";
                            achievementInputs[3].text = player.getKienTaoHieuSuat() > 0 ? to_string(player.getKienTaoHieuSuat()) : "";
                            achievementInputs[4].text = tempDanhHieuTaiClub;
                        }

                        // Achievement form
                        int formY = detailY;
                        int labelWidth = 150;
                        int fieldX = detailX + labelWidth + 10;

                        // Ngày gia nhập
                        DrawTextEx2(u8"Ngày gia nhập:", detailX + 20, formY + 10, 16, TEXT_DARK);
                        achievementInputs[0].rect = {(float)fieldX, (float)formY, 300, 40};
                        achievementInputs[0].draw();
                        formY += 55;

                        // Đội bóng trước đây
                        DrawTextEx2(u8"Đội bóng trước:", detailX + 20, formY + 10, 16, TEXT_DARK);
                        achievementInputs[1].rect = {(float)fieldX, (float)formY, 400, 40};
                        achievementInputs[1].draw();
                        formY += 55;

                        // Bàn thắng và Kiến tạo với nút +/-
                        DrawTextEx2(u8"Bàn thắng cho CLB:", detailX + 20, formY + 10, 16, TEXT_DARK);

                        // Buttons for goals
                        Button minusGoalsBtn = {{(float)fieldX, (float)formY, 40, 40}, u8"-", {231, 76, 60, 100}, ACCENT_2};
                        minusGoalsBtn.draw();
                        if (minusGoalsBtn.isClicked() && tempBanThangChoClub > 0)
                        {
                            tempBanThangChoClub--;
                            achievementInputs[2].text = to_string(tempBanThangChoClub);
                        }

                        DrawRectangle(fieldX + 50, formY, 100, 40, WHITE);
                        DrawRectangleLinesEx({(float)(fieldX + 50), (float)formY, 100, 40}, 1, BORDER_LIGHT);
                        DrawTextEx2(to_string(tempBanThangChoClub), fieldX + 85, formY + 10, 20, TEXT_DARK);

                        Button plusGoalsBtn = {{(float)(fieldX + 160), (float)formY, 40, 40}, u8"+", {46, 204, 113, 100}, {46, 204, 113, 255}};
                        plusGoalsBtn.draw();
                        if (plusGoalsBtn.isClicked())
                        {
                            tempBanThangChoClub++;
                            achievementInputs[2].text = to_string(tempBanThangChoClub);
                        }

                        // Kiến tạo - cách xa hơn bên phải
                        DrawTextEx2(u8"Kiến tạo:", fieldX + 250, formY + 10, 16, TEXT_DARK);
                        int kienTaoValue = achievementInputs[3].text.empty() ? 0 : stoi(achievementInputs[3].text);

                        Button minusAssistsBtn = {{(float)(fieldX + 340), (float)formY, 40, 40}, u8"-", {231, 76, 60, 100}, ACCENT_2};
                        minusAssistsBtn.draw();
                        if (minusAssistsBtn.isClicked() && kienTaoValue > 0)
                        {
                            kienTaoValue--;
                            achievementInputs[3].text = to_string(kienTaoValue);
                        }

                        DrawRectangle(fieldX + 390, formY, 100, 40, WHITE);
                        DrawRectangleLinesEx({(float)(fieldX + 390), (float)formY, 100, 40}, 1, BORDER_LIGHT);
                        DrawTextEx2(to_string(kienTaoValue), fieldX + 425, formY + 10, 20, TEXT_DARK);

                        Button plusAssistsBtn = {{(float)(fieldX + 500), (float)formY, 40, 40}, u8"+", {46, 204, 113, 100}, {46, 204, 113, 255}};
                        plusAssistsBtn.draw();
                        if (plusAssistsBtn.isClicked())
                        {
                            kienTaoValue++;
                            achievementInputs[3].text = to_string(kienTaoValue);
                        }

                        formY += 55;

                        // Số trận thi đấu với nút +/-
                        DrawTextEx2(u8"Số trận thi đấu:", detailX + 20, formY + 10, 16, TEXT_DARK);

                        Button minusMatchBtn = {{(float)fieldX, (float)formY, 40, 40}, u8"-", {231, 76, 60, 100}, ACCENT_2};
                        minusMatchBtn.draw();
                        if (minusMatchBtn.isClicked() && tempSoTranThiDau > 0)
                        {
                            tempSoTranThiDau--;
                        }

                        DrawRectangle(fieldX + 50, formY, 100, 40, WHITE);
                        DrawRectangleLinesEx({(float)(fieldX + 50), (float)formY, 100, 40}, 1, BORDER_LIGHT);
                        DrawTextEx2(to_string(tempSoTranThiDau), fieldX + 85, formY + 10, 20, TEXT_DARK);

                        Button plusMatchBtn = {{(float)(fieldX + 160), (float)formY, 40, 40}, u8"+", {46, 204, 113, 100}, {46, 204, 113, 255}};
                        plusMatchBtn.draw();
                        if (plusMatchBtn.isClicked())
                        {
                            tempSoTranThiDau++;
                        }

                        formY += 55;

                        // Thẻ vàng và Thẻ đỏ trên cùng 1 dòng
                        DrawTextEx2(u8"Thẻ vàng:", detailX + 20, formY + 10, 16, TEXT_DARK);

                        Button minusYellowBtn = {{(float)fieldX, (float)formY, 40, 40}, u8"-", {231, 76, 60, 100}, ACCENT_2};
                        minusYellowBtn.draw();
                        if (minusYellowBtn.isClicked() && tempTheVang > 0)
                        {
                            tempTheVang--;
                        }

                        DrawRectangle(fieldX + 50, formY, 100, 40, WHITE);
                        DrawRectangleLinesEx({(float)(fieldX + 50), (float)formY, 100, 40}, 1, BORDER_LIGHT);
                        DrawTextEx2(to_string(tempTheVang), fieldX + 85, formY + 10, 20, {243, 156, 18, 255});

                        Button plusYellowBtn = {{(float)(fieldX + 160), (float)formY, 40, 40}, u8"+", {46, 204, 113, 100}, {46, 204, 113, 255}};
                        plusYellowBtn.draw();
                        if (plusYellowBtn.isClicked())
                        {
                            tempTheVang++;
                        }

                        // Thẻ đỏ bên phải
                        DrawTextEx2(u8"Thẻ đỏ:", fieldX + 250, formY + 10, 16, TEXT_DARK);

                        Button minusRedBtn = {{(float)(fieldX + 340), (float)formY, 40, 40}, u8"-", {231, 76, 60, 100}, ACCENT_2};
                        minusRedBtn.draw();
                        if (minusRedBtn.isClicked() && tempTheDo > 0)
                        {
                            tempTheDo--;
                        }

                        DrawRectangle(fieldX + 390, formY, 100, 40, WHITE);
                        DrawRectangleLinesEx({(float)(fieldX + 390), (float)formY, 100, 40}, 1, BORDER_LIGHT);
                        DrawTextEx2(to_string(tempTheDo), fieldX + 425, formY + 10, 20, ACCENT_2);

                        Button plusRedBtn = {{(float)(fieldX + 500), (float)formY, 40, 40}, u8"+", {46, 204, 113, 100}, {46, 204, 113, 255}};
                        plusRedBtn.draw();
                        if (plusRedBtn.isClicked())
                        {
                            tempTheDo++;
                        }

                        formY += 55;

                        // Danh hiệu tại CLB
                        DrawTextEx2(u8"Danh hiệu tại CLB:", detailX + 20, formY + 10, 16, TEXT_DARK);
                        achievementInputs[4].rect = {(float)fieldX, (float)formY, 400, 80};
                        achievementInputs[4].draw();
                        formY += 95;

                        // Save button
                        Button saveAchievementBtn = {{(float)(detailX + 20), (float)formY, 150, 40}, u8"Lưu thành tích", ACCENT_1, {41, 128, 185, 255}};
                        saveAchievementBtn.draw();

                        if (saveAchievementBtn.isClicked() || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)))
                        {
                            // Sync input fields
                            tempNgayGiaNhap = achievementInputs[0].text;
                            tempDoiBongTruoc = achievementInputs[1].text;
                            tempBanThangChoClub = achievementInputs[2].text.empty() ? 0 : stoi(achievementInputs[2].text);
                            int tempKienTao = achievementInputs[3].text.empty() ? 0 : stoi(achievementInputs[3].text);
                            tempDanhHieuTaiClub = achievementInputs[4].text;

                            // Find and update player
                            int pIdx = 0;
                            for (auto &t : clb->getTeams())
                            {
                                for (auto &p : t.getPlayers())
                                {
                                    if (pIdx == selectedPlayerIndex)
                                    {
                                        p.setNgayGiaNhap(tempNgayGiaNhap);
                                        p.setDoiBongTruoc(tempDoiBongTruoc);
                                        p.setBanThangChoClub(tempBanThangChoClub);
                                        p.setKienTao(tempKienTao);
                                        p.setDanhHieuTaiClub(tempDanhHieuTaiClub);
                                        p.setSoTranThiDau(tempSoTranThiDau);
                                        p.setTheVang(tempTheVang);
                                        p.setTheDo(tempTheDo);

                                        saveToJson(*clb);
                                        showMessage(u8"Đã lưu thành tích cầu thủ!");
                                        goto save_done;
                                    }
                                    pIdx++;
                                }
                            }
                        save_done:;
                        }

                        // Handle input
                        for (auto &input : achievementInputs)
                        {
                            input.update();
                        }

                        return;
                    }
                    playerIndex++;
                }
            }
        }
        else
        {
            // No player selected
            DrawTextEx2(u8"Chọn cầu thủ để xem thành tích", detailX + 100, listY + 150, 18, ACCENT_2);
        }
    }
    void drawHealthPerformance()
    {
        drawHeader(u8"SỨC KHỎE & PHONG ĐỘ CẦU THỦ");
        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        // Initialize health input fields if empty
        if (healthInputs.empty())
        {
            healthInputs.push_back(InputField({0, 0, 250, 40}, "", 20));   // Ngày bắt đầu
            healthInputs.push_back(InputField({0, 0, 250, 40}, "", 20));   // Dự kiến hồi phục
            healthInputs.push_back(InputField({0, 0, 400, 120}, "", 200)); // Ghi chú sức khỏe
            healthInputs.push_back(InputField({0, 0, 200, 40}, "", 10));   // Chiều cao
            healthInputs.push_back(InputField({0, 0, 200, 40}, "", 10));   // Cân nặng
            healthInputs.push_back(InputField({0, 0, 200, 40}, "", 10));   // Tỉ lệ mỡ
            healthInputs.push_back(InputField({0, 0, 400, 120}, "", 200)); // Ghi chú tập luyện
            healthInputs.push_back(InputField({0, 0, 400, 100}, "", 200)); // Chi tiết vi phạm
            healthInputs.push_back(InputField({0, 0, 400, 120}, "", 200)); // Ghi chú tinh thần
        }

        // Sub-tabs cho các mục quản lý
        string subTabs[] = {u8"SỨC KHỎE", u8"THỂ LỰC", u8"HIỆU SUẤT", u8"TẬP LUYỆN", u8"KỶ LUẬT"};
        Color subTabColors[] = {
            {231, 76, 60, 255},  // Đỏ - Sức khỏe
            {46, 204, 113, 255}, // Xanh lá - Thể lực
            {52, 152, 219, 255}, // Xanh dương - Hiệu suất
            {243, 156, 18, 255}, // Vàng - Tập luyện
            {155, 89, 182, 255}  // Tím - Kỷ luật
        };

        int subTabWidth = (contentWidth - 40) / 5;
        for (int i = 0; i < 5; i++)
        {
            int tabX = contentX + i * (subTabWidth + 10);
            Rectangle tabRect = {(float)tabX, 100, (float)subTabWidth, 50};

            bool isHovered = CheckCollisionPointRec(GetMousePosition(), tabRect);
            Color tabColor = (i == healthTab) ? subTabColors[i] : (Color){200, 200, 200, 255};

            if (isHovered && i != healthTab)
            {
                tabColor.r = (unsigned char)min(255, (int)tabColor.r + 30);
                tabColor.g = (unsigned char)min(255, (int)tabColor.g + 30);
                tabColor.b = (unsigned char)min(255, (int)tabColor.b + 30);
            }

            DrawRectangleRounded(tabRect, 0.15f, 10, tabColor);

            int textWidth = MeasureTextEx2(subTabs[i], 16);
            DrawTextEx2(subTabs[i], tabX + (subTabWidth - textWidth) / 2, 118, 16, TEXT_LIGHT);

            if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                healthTab = i;
                scrollOffset = 0;
            }
        }

        // Danh sách cầu thủ
        int listWidth = 300;
        int listX = contentX;
        int listY = 170;
        int listHeight = screenHeight - listY - 20;

        DrawRectangleRounded({(float)listX, (float)listY, (float)listWidth, (float)listHeight}, 0.1f, 10, CARD_BG);
        DrawRectangleRounded({(float)listX, (float)listY, (float)listWidth, 40}, 0.1f, 10, ACCENT_1);
        DrawTextEx2(u8"DANH SÁCH CẦU THỦ", listX + 20, listY + 10, 16, TEXT_LIGHT);

        // Scroll list cầu thủ
        MyVector<pair<Player *, string>> allPlayers;
        for (auto &team : clb->getTeams())
        {
            for (auto &p : team.getPlayers())
            {
                allPlayers.push_back({&p, team.getTenDoi()});
            }
        }

        int playerListContentHeight = allPlayers.size() * 65 + 10;
        int playerListViewHeight = listHeight - 50;
        int playerListStartY = listY + 45;

        BeginScissorMode(listX + 5, playerListStartY, listWidth - 10, playerListViewHeight);

        int py = playerListStartY - scrollOffset;
        for (int i = 0; i < (int)allPlayers.size(); i++)
        {
            auto &p = *allPlayers[i].first;
            auto &teamName = allPlayers[i].second;

            Rectangle playerRect = {(float)(listX + 10), (float)py, (float)(listWidth - 20), 60};
            bool isSelected = (i == selectedHealthPlayerIndex);
            bool isHovered = CheckCollisionPointRec(GetMousePosition(), playerRect);

            Color cardColor = isSelected ? (Color){52, 152, 219, 50} : (isHovered ? (Color){245, 245, 245, 255} : CARD_BG);

            DrawRectangleRounded(playerRect, 0.15f, 8, cardColor);
            if (isSelected)
            {
                // DrawRectangleRoundedLines(playerRect, 0.15f, 8, 3, ACCENT_1);
            }

            DrawTextEx2(p.getTen(), listX + 20, py + 8, 16, TEXT_DARK);
            DrawTextEx2(teamName, listX + 20, py + 32, 13, ACCENT_1);

            if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedHealthPlayerIndex = i;
                selectedHealthTeam = teamName;
                // Load health data from this player
                loadHealthDataFromPlayer(p);
                lastLoadedHealthPlayerIndex = i;
            }

            py += 65;
        }

        EndScissorMode();

        // Scrollbar cho danh sách cầu thủ
        if (playerListContentHeight > playerListViewHeight)
        {
            float mouseWheel = GetMouseWheelMove();
            if (CheckCollisionPointRec(GetMousePosition(), {(float)listX, (float)playerListStartY, (float)listWidth, (float)playerListViewHeight}))
            {
                if (mouseWheel != 0)
                {
                    scrollOffset -= (int)(mouseWheel * 30);
                    scrollOffset = max(0, min(scrollOffset, playerListContentHeight - playerListViewHeight));
                }
            }

            int scrollbarX = listX + listWidth - 15;
            float scrollbarHeight = (float)playerListViewHeight * playerListViewHeight / playerListContentHeight;
            float scrollbarY = playerListStartY + ((float)scrollOffset / (playerListContentHeight - playerListViewHeight)) * (playerListViewHeight - scrollbarHeight);

            DrawRectangle(scrollbarX, playerListStartY, 8, playerListViewHeight, (Color){200, 200, 200, 100});
            DrawRectangleRounded({(float)scrollbarX, scrollbarY, 8, scrollbarHeight}, 0.5f, 8, ACCENT_1);
        }

        // Panel chi tiết bên phải
        int detailX = listX + listWidth + 20;
        int detailWidth = contentWidth - listWidth - 20;
        int detailY = 170;
        int detailHeight = screenHeight - detailY - 20;

        if (selectedHealthPlayerIndex >= 0 && selectedHealthPlayerIndex < (int)allPlayers.size())
        {
            auto &player = *allPlayers[selectedHealthPlayerIndex].first;

            DrawRectangleRounded({(float)detailX, (float)detailY, (float)detailWidth, (float)detailHeight}, 0.1f, 10, CARD_BG);

            // Header với tên cầu thủ
            DrawRectangleRounded({(float)detailX, (float)detailY, (float)detailWidth, 60}, 0.1f, 10, subTabColors[healthTab]);
            DrawTextEx2(player.getTen(), detailX + 20, detailY + 15, 24, TEXT_LIGHT);
            DrawTextEx2(selectedHealthTeam + " | " + player.getViTri(), detailX + 20, detailY + 45, 14, (Color){255, 255, 255, 200});

            int formY = detailY + 80;

            switch (healthTab)
            {
            case 0: // SỨC KHỎE
            {
                DrawTextEx2(u8"TÌNH TRẠNG SỨC KHỎE", detailX + 20, formY, 20, TEXT_DARK);
                formY += 40;

                // Trạng thái sức khỏe
                DrawTextEx2(u8"Trạng thái:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                string healthStatus[] = {u8"Đang khỏe", u8"Đau nhẹ", u8"Chấn thương"};
                string healthStatusValues[] = {u8"Khỏe mạnh", u8"Chấn thương nhẹ", u8"Chấn thương nặng"};
                Color statusColors[] = {{46, 204, 113, 255}, {243, 156, 18, 255}, {231, 76, 60, 255}};

                for (int i = 0; i < 3; i++)
                {
                    Rectangle statusBtn = {(float)(detailX + 30 + i * 150), (float)formY, 140, 40};
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), statusBtn);
                    bool isSelected = (tempTrangThaiSucKhoe == healthStatusValues[i]);
                    Color btnColor = isSelected ? statusColors[i] : (isHovered ? (Color){statusColors[i].r, statusColors[i].g, statusColors[i].b, 180} : (Color){statusColors[i].r, statusColors[i].g, statusColors[i].b, 100});

                    DrawRectangleRounded(statusBtn, 0.15f, 10, btnColor);
                    int textW = MeasureTextEx2(healthStatus[i], 14);
                    DrawTextEx2(healthStatus[i], detailX + 30 + i * 150 + (140 - textW) / 2, formY + 12, 14, TEXT_LIGHT);

                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        tempTrangThaiSucKhoe = healthStatusValues[i];
                    }
                }

                formY += 60;

                // Ngày bắt đầu - Ngày hồi phục
                DrawTextEx2(u8"Thời gian:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                // Ngày bắt đầu
                DrawTextEx2(u8"Ngày bắt đầu:", detailX + 30, formY, 14, (Color){100, 100, 100, 255});
                healthInputs[0].rect = {(float)(detailX + 30), (float)(formY + 20), 250, 40};
                healthInputs[0].label = "";
                healthInputs[0].update();
                if (!healthInputs[0].active)
                {
                    tempNgayBatDauChanThuong = healthInputs[0].text;
                }
                healthInputs[0].draw();

                // Dự kiến hồi phục
                DrawTextEx2(u8"Dự kiến hồi phục:", detailX + 300, formY, 14, (Color){100, 100, 100, 255});
                healthInputs[1].rect = {(float)(detailX + 300), (float)(formY + 20), 250, 40};
                healthInputs[1].label = "";
                healthInputs[1].update();
                if (!healthInputs[1].active)
                {
                    tempDuKienHoiPhuc = healthInputs[1].text;
                }
                healthInputs[1].draw();

                formY += 80;

                // Ghi chú
                DrawTextEx2(u8"Ghi chú:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                healthInputs[2].rect = {(float)(detailX + 30), (float)formY, (float)(detailWidth - 60), 150};
                healthInputs[2].label = "";
                healthInputs[2].update();
                if (!healthInputs[2].active)
                {
                    tempGhiChuSucKhoe = healthInputs[2].text;
                }
                healthInputs[2].draw();

                formY += 170;

                // Nút lưu
                Button saveBtn;
                saveBtn.rect = {(float)(detailX + detailWidth - 150), (float)formY, 120, 45};
                saveBtn.text = u8"LƯU";
                saveBtn.color = {46, 204, 113, 255};
                saveBtn.hoverColor = {39, 174, 96, 255};
                saveBtn.draw();

                if (saveBtn.isClicked() || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)))
                {
                    // Sync data from InputFields first
                    tempNgayBatDauChanThuong = healthInputs[0].text;
                    tempDuKienHoiPhuc = healthInputs[1].text;
                    tempGhiChuSucKhoe = healthInputs[2].text;

                    player.setTrangThaiSucKhoe(tempTrangThaiSucKhoe);
                    player.setNgayBatDauChanThuong(tempNgayBatDauChanThuong);
                    player.setDuKienHoiPhuc(tempDuKienHoiPhuc);
                    player.setGhiChuSucKhoe(tempGhiChuSucKhoe);
                    saveToJson(*clb);
                    showMessage(u8"Đã lưu thông tin sức khỏe!");
                }

                break;
            }

            case 1: // THỂ LỰC
            {
                DrawTextEx2(u8"CHỈ SỐ THỂ LỰC", detailX + 20, formY, 20, TEXT_DARK);
                formY += 40;

                // Mức thể lực
                DrawTextEx2(u8"Mức thể lực hiện tại (1-10):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                // Rating stars 1-10
                for (int i = 1; i <= 10; i++)
                {
                    Rectangle starRect = {(float)(detailX + 30 + (i - 1) * 50), (float)formY, 40, 40};
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), starRect);
                    Color starColor = (i <= tempMucTheLuc) ? (Color){46, 204, 113, 255} : (Color){220, 220, 220, 255};

                    if (isHovered)
                    {
                        starColor.r = (unsigned char)min(255, (int)starColor.r + 30);
                        starColor.g = (unsigned char)min(255, (int)starColor.g + 30);
                    }

                    DrawCircle(detailX + 30 + (i - 1) * 50 + 20, formY + 20, 18, starColor);
                    DrawTextEx2(to_string(i), detailX + 30 + (i - 1) * 50 + 15, formY + 10, 16, TEXT_LIGHT);

                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        tempMucTheLuc = i;
                    }
                }

                formY += 70;

                // Chiều cao - Dropdown
                DrawTextEx2(u8"Chiều cao (cm):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                static bool showChieuCaoDropdown = false;
                static int savedChieuCaoFormY = 0;
                Rectangle chieuCaoBtn = {(float)(detailX + 30), (float)formY, 200, 40};
                bool isChieuCaoHovered = CheckCollisionPointRec(GetMousePosition(), chieuCaoBtn);

                DrawRectangleRounded(chieuCaoBtn, 0.1f, 10, isChieuCaoHovered ? (Color){230, 230, 230, 255} : (Color){240, 240, 240, 255});
                DrawRectangleRoundedLines(chieuCaoBtn, 0.1f, 10, (Color){189, 195, 199, 255});

                char chieuCaoText[20];
                sprintf(chieuCaoText, "%.0f cm", tempChieuCao);
                DrawTextEx2(tempChieuCao > 0 ? chieuCaoText : u8"Chọn chiều cao", detailX + 45, formY + 12, 16, tempChieuCao > 0 ? TEXT_DARK : (Color){150, 150, 150, 255});
                DrawTextEx2("v", detailX + 200, formY + 12, 16, TEXT_DARK);

                if (isChieuCaoHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    showChieuCaoDropdown = !showChieuCaoDropdown;
                }
                savedChieuCaoFormY = formY;

                formY += 60;

                // Cân nặng - Dropdown
                DrawTextEx2(u8"Cân nặng (kg):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                static bool showCanNangDropdown = false;
                static int savedCanNangFormY = 0;
                Rectangle canNangBtn = {(float)(detailX + 30), (float)formY, 200, 40};
                bool isCanNangHovered = CheckCollisionPointRec(GetMousePosition(), canNangBtn);

                DrawRectangleRounded(canNangBtn, 0.1f, 10, isCanNangHovered ? (Color){230, 230, 230, 255} : (Color){240, 240, 240, 255});
                DrawRectangleRoundedLines(canNangBtn, 0.1f, 10, (Color){189, 195, 199, 255});

                char canNangText[20];
                sprintf(canNangText, "%.0f kg", tempCanNang);
                DrawTextEx2(tempCanNang > 0 ? canNangText : u8"Chọn cân nặng", detailX + 45, formY + 12, 16, tempCanNang > 0 ? TEXT_DARK : (Color){150, 150, 150, 255});
                DrawTextEx2("v", detailX + 200, formY + 12, 16, TEXT_DARK);

                if (isCanNangHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    showCanNangDropdown = !showCanNangDropdown;
                }
                savedCanNangFormY = formY;

                formY += 60;

                // Tỷ lệ mỡ - Dropdown
                DrawTextEx2(u8"Tỉ lệ mỡ (%):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;

                static bool showTiLeMoDropdown = false;
                static int savedTiLeMoFormY = 0;
                Rectangle tiLeMoBtn = {(float)(detailX + 30), (float)formY, 200, 40};
                bool isTiLeMoHovered = CheckCollisionPointRec(GetMousePosition(), tiLeMoBtn);

                DrawRectangleRounded(tiLeMoBtn, 0.1f, 10, isTiLeMoHovered ? (Color){230, 230, 230, 255} : (Color){240, 240, 240, 255});
                DrawRectangleRoundedLines(tiLeMoBtn, 0.1f, 10, (Color){189, 195, 199, 255});

                char tiLeMoText[20];
                sprintf(tiLeMoText, "%.1f%%", tempTiLeMo);
                DrawTextEx2(tempTiLeMo > 0 ? tiLeMoText : u8"Chọn tỉ lệ mỡ", detailX + 45, formY + 12, 16, tempTiLeMo > 0 ? TEXT_DARK : (Color){150, 150, 150, 255});
                DrawTextEx2("v", detailX + 200, formY + 12, 16, TEXT_DARK);

                if (isTiLeMoHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    showTiLeMoDropdown = !showTiLeMoDropdown;
                }
                savedTiLeMoFormY = formY;

                formY += 60;

                // Nút lưu
                Button saveBtn;
                saveBtn.rect = {(float)(detailX + detailWidth - 150), (float)formY, 120, 45};
                saveBtn.text = u8"LƯU";
                saveBtn.color = {46, 204, 113, 255};
                saveBtn.hoverColor = {39, 174, 96, 255};
                saveBtn.draw();

                if (saveBtn.isClicked())
                {
                    player.setMucTheLuc(tempMucTheLuc);
                    player.setChieuCao(tempChieuCao);
                    player.setCanNang(tempCanNang);
                    player.setTiLeMo(tempTiLeMo);
                    saveToJson(*clb);
                    showMessage(u8"Đã lưu chỉ số thể lực!");
                }

                // Vẽ dropdown ở cuối để nó nằm trên top
                static int chieuCaoScroll = 0;
                if (showChieuCaoDropdown)
                {
                    int dropdownHeight = 200;
                    int totalItems = 51;
                    int totalHeight = totalItems * 30;

                    Rectangle dropdownRect = {(float)(detailX + 30), (float)(savedChieuCaoFormY + 45), 200, (float)dropdownHeight};
                    DrawRectangleRounded(dropdownRect, 0.1f, 10, CARD_BG);
                    DrawRectangleRoundedLines(dropdownRect, 0.1f, 10, ACCENT_1);

                    if (CheckCollisionPointRec(GetMousePosition(), dropdownRect))
                    {
                        float wheel = GetMouseWheelMove();
                        chieuCaoScroll -= wheel * 30;
                        if (chieuCaoScroll < 0)
                            chieuCaoScroll = 0;
                        if (chieuCaoScroll > totalHeight - dropdownHeight)
                            chieuCaoScroll = totalHeight - dropdownHeight;
                    }

                    BeginScissorMode(detailX + 30, savedChieuCaoFormY + 45, 200, dropdownHeight);

                    for (int i = 150; i <= 200; i += 1)
                    {
                        int idx = (i - 150);
                        Rectangle itemRect = {(float)(detailX + 30), (float)(savedChieuCaoFormY + 45 + idx * 30 - chieuCaoScroll), 200, 30};
                        bool isItemHovered = CheckCollisionPointRec(GetMousePosition(), itemRect);

                        if (isItemHovered)
                        {
                            DrawRectangle(itemRect.x, itemRect.y, itemRect.width, itemRect.height, (Color){52, 152, 219, 50});
                        }

                        char itemText[20];
                        sprintf(itemText, "%d cm", i);
                        DrawTextEx2(itemText, detailX + 45, savedChieuCaoFormY + 45 + idx * 30 - chieuCaoScroll + 5, 16, TEXT_DARK);

                        if (isItemHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            tempChieuCao = i;
                            showChieuCaoDropdown = false;
                            chieuCaoScroll = 0;
                        }
                    }

                    EndScissorMode();
                }

                static int canNangScroll = 0;
                if (showCanNangDropdown)
                {
                    int dropdownHeight = 200;
                    int totalItems = 66;
                    int totalHeight = totalItems * 30;

                    Rectangle dropdownRect = {(float)(detailX + 30), (float)(savedCanNangFormY + 45), 200, (float)dropdownHeight};
                    DrawRectangleRounded(dropdownRect, 0.1f, 10, CARD_BG);
                    DrawRectangleRoundedLines(dropdownRect, 0.1f, 10, ACCENT_1);

                    if (CheckCollisionPointRec(GetMousePosition(), dropdownRect))
                    {
                        float wheel = GetMouseWheelMove();
                        canNangScroll -= wheel * 30;
                        if (canNangScroll < 0)
                            canNangScroll = 0;
                        if (canNangScroll > totalHeight - dropdownHeight)
                            canNangScroll = totalHeight - dropdownHeight;
                    }

                    BeginScissorMode(detailX + 30, savedCanNangFormY + 45, 200, dropdownHeight);

                    for (int i = 45; i <= 110; i += 1)
                    {
                        int idx = (i - 45);
                        Rectangle itemRect = {(float)(detailX + 30), (float)(savedCanNangFormY + 45 + idx * 30 - canNangScroll), 200, 30};
                        bool isItemHovered = CheckCollisionPointRec(GetMousePosition(), itemRect);

                        if (isItemHovered)
                        {
                            DrawRectangle(itemRect.x, itemRect.y, itemRect.width, itemRect.height, (Color){52, 152, 219, 50});
                        }

                        char itemText[20];
                        sprintf(itemText, "%d kg", i);
                        DrawTextEx2(itemText, detailX + 45, savedCanNangFormY + 45 + idx * 30 - canNangScroll + 5, 16, TEXT_DARK);

                        if (isItemHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            tempCanNang = i;
                            showCanNangDropdown = false;
                            canNangScroll = 0;
                        }
                    }

                    EndScissorMode();
                }

                static int tiLeMoScroll = 0;
                if (showTiLeMoDropdown)
                {
                    int dropdownHeight = 200;
                    int totalItems = 31;
                    int totalHeight = totalItems * 30;

                    Rectangle dropdownRect = {(float)(detailX + 30), (float)(savedTiLeMoFormY + 45), 200, (float)dropdownHeight};
                    DrawRectangleRounded(dropdownRect, 0.1f, 10, CARD_BG);
                    DrawRectangleRoundedLines(dropdownRect, 0.1f, 10, ACCENT_1);

                    if (CheckCollisionPointRec(GetMousePosition(), dropdownRect))
                    {
                        float wheel = GetMouseWheelMove();
                        tiLeMoScroll -= wheel * 30;
                        if (tiLeMoScroll < 0)
                            tiLeMoScroll = 0;
                        if (tiLeMoScroll > totalHeight - dropdownHeight)
                            tiLeMoScroll = totalHeight - dropdownHeight;
                    }

                    BeginScissorMode(detailX + 30, savedTiLeMoFormY + 45, 200, dropdownHeight);

                    for (int i = 5; i <= 35; i++)
                    {
                        int idx = i - 5;
                        Rectangle itemRect = {(float)(detailX + 30), (float)(savedTiLeMoFormY + 45 + idx * 30 - tiLeMoScroll), 200, 30};
                        bool isItemHovered = CheckCollisionPointRec(GetMousePosition(), itemRect);

                        if (isItemHovered)
                        {
                            DrawRectangle(itemRect.x, itemRect.y, itemRect.width, itemRect.height, (Color){52, 152, 219, 50});
                        }

                        char itemText[20];
                        sprintf(itemText, "%d%%", i);
                        DrawTextEx2(itemText, detailX + 45, savedTiLeMoFormY + 45 + idx * 30 - tiLeMoScroll + 5, 16, TEXT_DARK);

                        if (isItemHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            tempTiLeMo = i;
                            showTiLeMoDropdown = false;
                            tiLeMoScroll = 0;
                        }
                    }

                    EndScissorMode();
                }

                break;
            }

            case 2: // HIỆU SUẤT
            {
                DrawTextEx2(u8"HIỆU SUẤT THI ĐẤU", detailX + 20, formY, 20, TEXT_DARK);
                formY += 40;

                // Bàn thắng hiệu suất
                DrawTextEx2(u8"Bàn thắng (trong mùa):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                DrawTextEx2(to_string(tempBanThangHieuSuat), detailX + 30, formY, 24, ACCENT_1);
                Rectangle btnMinus1 = {(float)(detailX + 100), (float)formY, 40, 40};
                Rectangle btnPlus1 = {(float)(detailX + 150), (float)formY, 40, 40};
                DrawRectangleRounded(btnMinus1, 0.2f, 8, CheckCollisionPointRec(GetMousePosition(), btnMinus1) ? (Color){231, 76, 60, 255} : (Color){200, 200, 200, 255});
                DrawRectangleRounded(btnPlus1, 0.2f, 8, CheckCollisionPointRec(GetMousePosition(), btnPlus1) ? (Color){46, 204, 113, 255} : (Color){200, 200, 200, 255});
                DrawTextEx2("-", detailX + 115, formY + 8, 24, TEXT_LIGHT);
                DrawTextEx2("+", detailX + 163, formY + 8, 24, TEXT_LIGHT);
                if (CheckCollisionPointRec(GetMousePosition(), btnMinus1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tempBanThangHieuSuat > 0)
                    tempBanThangHieuSuat--;
                if (CheckCollisionPointRec(GetMousePosition(), btnPlus1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    tempBanThangHieuSuat++;

                formY += 60;

                // Kiến tạo
                DrawTextEx2(u8"Kiến tạo:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                DrawTextEx2(to_string(tempKienTao), detailX + 30, formY, 24, ACCENT_1);
                Rectangle btnMinus2 = {(float)(detailX + 100), (float)formY, 40, 40};
                Rectangle btnPlus2 = {(float)(detailX + 150), (float)formY, 40, 40};
                DrawRectangleRounded(btnMinus2, 0.2f, 8, CheckCollisionPointRec(GetMousePosition(), btnMinus2) ? (Color){231, 76, 60, 255} : (Color){200, 200, 200, 255});
                DrawRectangleRounded(btnPlus2, 0.2f, 8, CheckCollisionPointRec(GetMousePosition(), btnPlus2) ? (Color){46, 204, 113, 255} : (Color){200, 200, 200, 255});
                DrawTextEx2("-", detailX + 115, formY + 8, 24, TEXT_LIGHT);
                DrawTextEx2("+", detailX + 163, formY + 8, 24, TEXT_LIGHT);
                if (CheckCollisionPointRec(GetMousePosition(), btnMinus2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tempKienTao > 0)
                    tempKienTao--;
                if (CheckCollisionPointRec(GetMousePosition(), btnPlus2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    tempKienTao++;

                formY += 60;

                // Chuyền đúng (%)
                DrawTextEx2(u8"Chuyền đúng (%):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                DrawTextEx2(to_string(tempChuyenDung) + "%", detailX + 30, formY, 24, ACCENT_1);
                Rectangle btnMinus3 = {(float)(detailX + 100), (float)formY, 40, 40};
                Rectangle btnPlus3 = {(float)(detailX + 150), (float)formY, 40, 40};
                DrawRectangleRounded(btnMinus3, 0.2f, 8, CheckCollisionPointRec(GetMousePosition(), btnMinus3) ? (Color){231, 76, 60, 255} : (Color){200, 200, 200, 255});
                DrawRectangleRounded(btnPlus3, 0.2f, 8, CheckCollisionPointRec(GetMousePosition(), btnPlus3) ? (Color){46, 204, 113, 255} : (Color){200, 200, 200, 255});
                DrawTextEx2("-", detailX + 115, formY + 8, 24, TEXT_LIGHT);
                DrawTextEx2("+", detailX + 163, formY + 8, 24, TEXT_LIGHT);
                if (CheckCollisionPointRec(GetMousePosition(), btnMinus3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tempChuyenDung > 0)
                    tempChuyenDung--;
                if (CheckCollisionPointRec(GetMousePosition(), btnPlus3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tempChuyenDung < 100)
                    tempChuyenDung++;

                formY += 60;

                // Điểm phong độ
                DrawTextEx2(u8"Điểm phong độ tổng hợp (1-10):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                for (int i = 1; i <= 10; i++)
                {
                    Rectangle starRect = {(float)(detailX + 30 + (i - 1) * 50), (float)formY, 40, 40};
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), starRect);
                    Color starColor = (i <= tempDiemPhongDo) ? (Color){243, 156, 18, 255} : (Color){220, 220, 220, 255};
                    if (isHovered)
                    {
                        starColor.r = (unsigned char)min(255, (int)starColor.r + 30);
                        starColor.g = (unsigned char)min(255, (int)starColor.g + 30);
                    }
                    DrawCircle(detailX + 30 + (i - 1) * 50 + 20, formY + 20, 18, starColor);
                    DrawTextEx2(to_string(i), detailX + 30 + (i - 1) * 50 + 15, formY + 10, 16, TEXT_LIGHT);
                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        tempDiemPhongDo = i;
                }

                formY += 70;

                Button saveBtn;
                saveBtn.rect = {(float)(detailX + detailWidth - 150), (float)formY, 120, 45};
                saveBtn.text = u8"LƯU";
                saveBtn.color = {46, 204, 113, 255};
                saveBtn.hoverColor = {39, 174, 96, 255};
                saveBtn.draw();

                if (saveBtn.isClicked())
                {
                    player.setBanThangHieuSuat(tempBanThangHieuSuat);
                    player.setKienTao(tempKienTao);
                    player.setChuyenDung(tempChuyenDung);
                    player.setDiemPhongDo(tempDiemPhongDo);
                    saveToJson(*clb);
                    showMessage(u8"Đã lưu hiệu suất thi đấu!");
                }

                break;
            }

            case 3: // TẬP LUYỆN
            {
                DrawTextEx2(u8"KHỐI LƯỢNG TẬP LUYỆN", detailX + 20, formY, 20, TEXT_DARK);
                formY += 40;

                // Điểm đánh giá buổi tập
                DrawTextEx2(u8"Điểm đánh giá buổi tập (1-10):", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                for (int i = 1; i <= 10; i++)
                {
                    Rectangle scoreRect = {(float)(detailX + 30 + (i - 1) * 50), (float)formY, 40, 40};
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), scoreRect);
                    Color scoreColor = (i <= tempDiemDanhGiaTap) ? (Color){243, 156, 18, 255} : (Color){220, 220, 220, 255};
                    if (isHovered)
                    {
                        scoreColor.r = (unsigned char)min(255, (int)scoreColor.r + 30);
                        scoreColor.g = (unsigned char)min(255, (int)scoreColor.g + 30);
                    }
                    DrawRectangleRounded(scoreRect, 0.15f, 8, scoreColor);
                    DrawTextEx2(to_string(i), detailX + 30 + (i - 1) * 50 + 15, formY + 10, 16, TEXT_LIGHT);
                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        tempDiemDanhGiaTap = i;
                }

                formY += 70;

                // Cường độ tập luyện
                DrawTextEx2(u8"Cường độ tập luyện:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                string intensityLevels[] = {u8"Thấp", u8"Trung bình", u8"Cao"};
                Color intensityColors[] = {{46, 204, 113, 255}, {243, 156, 18, 255}, {231, 76, 60, 255}};
                for (int i = 0; i < 3; i++)
                {
                    Rectangle intensityBtn = {(float)(detailX + 30 + i * 180), (float)formY, 170, 40};
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), intensityBtn);
                    bool isSelected = (tempCuongDoTapLuyen == intensityLevels[i]);
                    Color btnColor = isSelected ? intensityColors[i] : (isHovered ? (Color){intensityColors[i].r, intensityColors[i].g, intensityColors[i].b, 180} : (Color){intensityColors[i].r, intensityColors[i].g, intensityColors[i].b, 100});
                    DrawRectangleRounded(intensityBtn, 0.15f, 10, btnColor);
                    int textW = MeasureTextEx2(intensityLevels[i], 14);
                    DrawTextEx2(intensityLevels[i], detailX + 30 + i * 180 + (170 - textW) / 2, formY + 12, 14, TEXT_LIGHT);
                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        tempCuongDoTapLuyen = intensityLevels[i];
                }

                formY += 70;

                // Ghi chú buổi tập
                DrawTextEx2(u8"Ghi chú buổi tập:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                healthInputs[6].rect = {(float)(detailX + 30), (float)formY, (float)(detailWidth - 60), 120};
                healthInputs[6].label = "";
                healthInputs[6].update();
                if (!healthInputs[6].active)
                    tempGhiChuTapLuyen = healthInputs[6].text;
                healthInputs[6].draw();

                formY += 140;

                Button saveBtn;
                saveBtn.rect = {(float)(detailX + detailWidth - 150), (float)formY, 120, 45};
                saveBtn.text = u8"LƯU";
                saveBtn.color = {46, 204, 113, 255};
                saveBtn.hoverColor = {39, 174, 96, 255};
                saveBtn.draw();

                if (saveBtn.isClicked())
                {
                    player.setDiemDanhGiaTap(tempDiemDanhGiaTap);
                    player.setCuongDoTapLuyen(tempCuongDoTapLuyen);
                    player.setGhiChuTapLuyen(tempGhiChuTapLuyen);
                    saveToJson(*clb);
                    showMessage(u8"Đã lưu khối lượng tập luyện!");
                }

                break;
            }

            case 4: // KỶ LUẬT
            {
                DrawTextEx2(u8"KỶ LUẬT & TINH THẦN", detailX + 20, formY, 20, TEXT_DARK);
                formY += 40;

                // Thái độ
                DrawTextEx2(u8"Thái độ:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                string attitudeTypes[] = {u8"Tốt", u8"Trung bình", u8"Kém"};
                Color attitudeColors[] = {{46, 204, 113, 255}, {243, 156, 18, 255}, {231, 76, 60, 255}};
                for (int i = 0; i < 3; i++)
                {
                    Rectangle attBtn = {(float)(detailX + 30 + i * 180), (float)formY, 170, 40};
                    bool isHovered = CheckCollisionPointRec(GetMousePosition(), attBtn);
                    bool isSelected = (tempThaiDo == attitudeTypes[i]);
                    Color btnColor = isSelected ? attitudeColors[i] : (isHovered ? (Color){attitudeColors[i].r, attitudeColors[i].g, attitudeColors[i].b, 180} : (Color){attitudeColors[i].r, attitudeColors[i].g, attitudeColors[i].b, 100});
                    DrawRectangleRounded(attBtn, 0.15f, 10, btnColor);
                    int textW = MeasureTextEx2(attitudeTypes[i], 14);
                    DrawTextEx2(attitudeTypes[i], detailX + 30 + i * 180 + (170 - textW) / 2, attBtn.y + 12, 14, TEXT_LIGHT);
                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        tempThaiDo = attitudeTypes[i];
                }

                formY += 70;

                // Vi phạm
                DrawTextEx2(u8"Vi phạm:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                Rectangle violationCheckbox = {(float)(detailX + 30), (float)formY, 25, 25};
                bool isHoveredCb = CheckCollisionPointRec(GetMousePosition(), violationCheckbox);
                DrawRectangleRounded(violationCheckbox, 0.2f, 6, tempCoViPham ? ACCENT_2 : (isHoveredCb ? (Color){180, 180, 180, 255} : (Color){200, 200, 200, 255}));
                if (tempCoViPham)
                    DrawTextEx2("X", detailX + 35, formY + 2, 18, TEXT_LIGHT);
                DrawTextEx2(u8"Có vi phạm", detailX + 65, formY + 3, 16, TEXT_DARK);
                if (isHoveredCb && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    tempCoViPham = !tempCoViPham;

                formY += 45;

                // Chi tiết vi phạm (nếu có)
                if (tempCoViPham)
                {
                    DrawTextEx2(u8"Chi tiết vi phạm:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                    formY += 30;
                    healthInputs[7].rect = {(float)(detailX + 30), (float)formY, (float)(detailWidth - 60), 100};
                    healthInputs[7].label = "";
                    healthInputs[7].update();
                    if (!healthInputs[7].active)
                        tempChiTietViPham = healthInputs[7].text;
                    healthInputs[7].draw();
                    formY += 120;
                }

                // Ghi chú
                DrawTextEx2(u8"Ghi chú về tinh thần:", detailX + 30, formY, 16, (Color){100, 100, 100, 255});
                formY += 30;
                healthInputs[8].rect = {(float)(detailX + 30), (float)formY, (float)(detailWidth - 60), 120};
                healthInputs[8].label = "";
                healthInputs[8].update();
                if (!healthInputs[8].active)
                    tempGhiChuTinhThan = healthInputs[8].text;
                healthInputs[8].draw();

                formY += 140;

                Button saveBtn;
                saveBtn.rect = {(float)(detailX + detailWidth - 150), (float)formY, 120, 45};
                saveBtn.text = u8"LƯU";
                saveBtn.color = {46, 204, 113, 255};
                saveBtn.hoverColor = {39, 174, 96, 255};
                saveBtn.draw();

                if (saveBtn.isClicked())
                {
                    player.setThaiDo(tempThaiDo);
                    player.setCoViPham(tempCoViPham);
                    player.setChiTietViPham(tempChiTietViPham);
                    player.setGhiChuTinhThan(tempGhiChuTinhThan);
                    saveToJson(*clb);
                    showMessage(u8"Đã lưu thông tin kỷ luật!");
                }

                break;
            }
            }
        }
        else
        {
            // Chưa chọn cầu thủ nào
            DrawRectangleRounded({(float)detailX, (float)detailY, (float)detailWidth, (float)detailHeight}, 0.1f, 10, CARD_BG);

            int iconY = detailY + detailHeight / 2 - 100;
            DrawCircle(detailX + detailWidth / 2 + 3, iconY + 3, 50, (Color){0, 0, 0, 30});
            DrawCircle(detailX + detailWidth / 2, iconY, 50, subTabColors[healthTab]);
            DrawCircle(detailX + detailWidth / 2, iconY, 42, CARD_BG);
            DrawTextEx2("?", detailX + detailWidth / 2 - 12, iconY - 20, 40, subTabColors[healthTab]);

            string guide = u8"CHỌN CẦU THỦ TỪ DANH SÁCH BÊN TRÁI";
            int guideW = MeasureTextEx2(guide, 20);
            DrawTextEx2(guide, detailX + (detailWidth - guideW) / 2, iconY + 80, 20, TEXT_DARK);

            string subGuide = u8"Để xem và cập nhật thông tin " + string(subTabs[healthTab]);
            int subGuideW = MeasureTextEx2(subGuide, 16);
            DrawTextEx2(subGuide, detailX + (detailWidth - subGuideW) / 2, iconY + 115, 16, (Color){150, 150, 150, 255});
        }
    }
    void run()
    {
        initColors();

        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(screenWidth, screenHeight, "Quan Ly CLB Bong Da");
        SetTargetFPS(60);

        // Load Vietnamese codepoints: Basic Latin + All Vietnamese characters
        int codepointCount = 0;
        int *codepoints = LoadCodepoints("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,;:!?-_()[]{}/'\"<>@#$%&*+=|\\~`ÀÁÂÃÄÅÈÉÊËÌÍÎÏÒÓÔÕÖÙÚÛÜÝàáâãäåèéêëìíîïòóôõöùúûüýÿĂăĐđĨĩŨũƠơƯưẠạẢảẤấẦầẨẩẪẫẬậẮắẰằẲẳẴẵẶặẸẹẺẻẼẽẾếỀềỂểỄễỆệỈỉỊịỌọỎỏỐốỒồỔổỖỗỘộỚớỜờỞởỠỡỢợỤụỦủỨứỪừỬửỮữỰựỲỳỴỵỶỷỸỹ", &codepointCount);
        customFont = LoadFontEx("C:/Windows/Fonts/arial.ttf", 250, codepoints, codepointCount);
        UnloadCodepoints(codepoints);
        SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);

        if (FileExists("assets/players/default.png"))
        {
            defaultPlayerPhoto = LoadTexture("assets/players/default.png");
        }
        else
        {
            Image img = GenImageColor(250, 300, (Color){220, 220, 220, 255});
            defaultPlayerPhoto = LoadTextureFromImage(img);
            UnloadImage(img);
        }

        while (!WindowShouldClose())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            if (IsKeyPressed(KEY_F11))
            {
                ToggleFullscreen();
            }

            BeginDrawing();
            ClearBackground(CONTENT_BG);

            drawSidebar();

            switch (currentTab)
            {
            case 0:
                drawDashboard();
                break;
            case 1:
                drawTeams();
                break;
            case 2:
                drawPlayers();
                break;
            case 3:
                drawHealthPerformance();
                break;
            case 4:
                drawAchievements();
                break;
            case 5:
                drawStatistics();
                break;
            case 6:
                drawSearch();
                break;
            }

            drawMessage();
            drawPlayerDetailPopup();
            drawEditPlayerPopup();
            drawEditHLVPopup();
            drawEditTeamPopup();
            EndDrawing();
        }

        UnloadFont(customFont);
        CloseWindow();
    }
};

int main()
{
    std::cout << "========================================\n";
    std::cout << "CHUONG TRINH KHOI DONG!\n";
    std::cout << "========================================\n\n";

    // KHÔNG TẠO FILE GÌ CẢ - CHỈ LOAD THÔI!

    Club clb;

    std::cout << "Bat dau load JSON...\n";
    loadFromJson(clb);

    std::cout << "So doi sau khi load: " << clb.getTeams().size() << "\n";
    for (const auto &team : clb.getTeams())
    {
        std::cout << "  - " << team.getTenDoi() << "\n";
    }
    std::cout << "\n";

    ClubGUI gui(&clb);
    gui.run();

    std::cout << "\n=== TRUOC KHI LUU ===\n";
    std::cout << "So doi hien tai: " << clb.getTeams().size() << "\n";
    for (const auto &team : clb.getTeams())
    {
        std::cout << "  - " << team.getTenDoi() << "\n";
    }

    saveToJson(clb);

    return 0;
}