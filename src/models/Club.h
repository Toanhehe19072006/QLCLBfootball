#pragma once

#include "../MyVector.h"
#include "Team.h"
#include <iostream>

class Club
{
private:
    MyVector<Team> teams;

public:
    void themTeam(const Team &t)
    {
        Team copy = t; // Explicit copy
        teams.push_back(copy);
    }
    MyVector<Team> &getTeams() { return teams; }
    const MyVector<Team> &getTeams() const { return teams; }

    void xoaCauThuKhoiClub(const std::string &idCauThu)
    {
        for (auto &team : teams)
        {
            team.xoaCauThu(idCauThu);
        }
    }
};
