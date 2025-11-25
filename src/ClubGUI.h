#pragma once

#include "raylib.h"
#include <string>
#include <utility>
#include "MyVector.h"
#include "MyMap.h"
#include "models/Club.h"
#include "models/Team.h"
#include "models/Player.h"

using namespace std;

// Forward declarations
struct Button;
struct InputField;

// Button struct
struct Button
{
    Rectangle rect;
    string text;
    Color color, hoverColor;

    void draw();
    bool isHovered(Vector2 mousePos);
    bool isClicked();
};

// InputField struct
struct InputField
{
    Rectangle rect;
    string label, text;
    bool active;
    int maxLength;
    string compositionText;
    bool multiline;

    InputField();
    InputField(Rectangle r, string l, int max = 50, bool multi = false);
    void draw();
    void update();
};

// Main GUI Class
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
    
    bool showEditPlayer;
    Player editingPlayer;
    string editingPlayerTeam;
    int editingTeamIndex;
    int editingPlayerIndex;
    MyVector<InputField> editInputs;

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

    int lastLoadedHealthPlayerIndex;
    int selectedPlayerIndex;
    int lastLoadedAchievementPlayerIndex;
    MyVector<InputField> achievementInputs;

    // Temporary variables
    string tempNgayGiaNhap, tempDoiBongTruoc, tempDanhHieuTaiClub;
    int tempBanThangChoClub, tempSoTranThiDau, tempTheVang, tempTheDo;
    string tempTrangThaiSucKhoe, tempNgayBatDauChanThuong, tempDuKienHoiPhuc, tempGhiChuSucKhoe;
    int tempMucTheLuc, tempBanThangHieuSuat, tempKienTao, tempChuyenDung, tempDiemPhongDo, tempDiemDanhGiaTap;
    double tempChieuCao, tempCanNang, tempTiLeMo;
    string tempCuongDoTapLuyen, tempGhiChuTapLuyen, tempThaiDo, tempChiTietViPham, tempGhiChuTinhThan;
    bool tempCoViPham;

public:
    ClubGUI(Club *clubPtr);
    ~ClubGUI();
    Texture2D defaultPlayerPhoto();
    void run();

    // Core functions (GUI_CoBan.cpp)
    void showMessage(const string &msg);
    void drawSidebar();
    void drawHeader(string title);
    void drawMessage();
    void drawScrollbar(int contentHeight, int viewHeight, int startY);

    // Teams functions (GUI_DoiBong.cpp)
    void drawTeams();
    void drawEditTeamPopup();

    // Players functions (GUI_CauThu.cpp)
    void drawPlayers();
    void drawPlayerCard(Player &p, const string &teamName, int x, int y, int cardWidth, Team *team);

    // Health functions (GUI_SucKhoe.cpp)
    void drawHealth();

    // Achievements functions (GUI_ThanhTich.cpp)
    void drawAchievements();

    // Stats functions (GUI_ThongKe.cpp)
    void drawStats();

    // Search functions (GUI_TimKiem.cpp)
    void drawSearch();

    // Popup functions (GUI_Popup.cpp)
    void drawPlayerDetailPopup();
    void drawEditPlayerPopup();
    void drawEditHLVPopup();
};

// Utility functions (JSON_XuLy.cpp)
void loadFromJson(Club &clb);
void saveToJson(const Club &clb);
string formatVND(double value, bool valueIsInK = true);
