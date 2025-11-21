#pragma once

#include <string>
#include <algorithm>
#include "../MyVector.h"
#include "Player.h"

class Team
{
private:
    std::string idDoi, tenDoi;
    std::string hlvTruong, hlvPho;
    MyVector<Player> cauThu;

public:
    Team() : hlvTruong(""), hlvPho("") {}
    Team(const std::string &id, const std::string &ten) : idDoi(id), tenDoi(ten), hlvTruong(""), hlvPho("") {}

    Team(const Team &other) : idDoi(other.idDoi), tenDoi(other.tenDoi), hlvTruong(other.hlvTruong), hlvPho(other.hlvPho), cauThu(other.cauThu) {}

    Team &operator=(const Team &other)
    {
        if (this != &other)
        {
            idDoi = other.idDoi;
            tenDoi = other.tenDoi;
            hlvTruong = other.hlvTruong;
            hlvPho = other.hlvPho;
            cauThu = other.cauThu;
        }
        return *this;
    }

    ~Team() = default;

    std::string getTenDoi() const { return tenDoi; }
    std::string getIDDoi() const { return idDoi; }
    std::string getHLVTruong() const { return hlvTruong; }
    std::string getHLVPho() const { return hlvPho; }
    void setTenDoi(const std::string &ten) { tenDoi = ten; }
    void setHLVTruong(const std::string &ten) { hlvTruong = ten; }
    void setHLVPho(const std::string &ten) { hlvPho = ten; }
    MyVector<Player> &getPlayers() { return cauThu; }
    const MyVector<Player> &getPlayers() const { return cauThu; }

    void themCauThu(const Player &p)
    {
        auto it = std::find_if(cauThu.begin(), cauThu.end(),
                               [&](const Player &x)
                               { return x.getID() == p.getID(); });
        if (it != cauThu.end())
            return;
        cauThu.push_back(p);
    }

    void xoaCauThu(const std::string &id)
    {
        auto it = cauThu.begin();
        while (it != cauThu.end())
        {
            if (it->getID() == id)
            {
                it = cauThu.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    int tongBanThang() const
    {
        int tong = 0;
        for (auto &p : cauThu)
            tong += p.getBanThang();
        return tong;
    }

    void capNhatDoiTruong(const std::string &idCauThu)
    {
        for (auto &p : cauThu)
        {
            if (p.getID() == idCauThu)
                p.setVaiTro("DT");
            else if (p.getVaiTro() == "DT")
                p.setVaiTro("");
        }
    }

    void capNhatDoiPho(const std::string &idCauThu)
    {
        for (auto &p : cauThu)
        {
            if (p.getID() == idCauThu)
                p.setVaiTro("DP");
            else if (p.getVaiTro() == "DP")
                p.setVaiTro("");
        }
    }
};
