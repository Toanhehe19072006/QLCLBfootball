#pragma once

#include <string>

class Person
{
protected:
    std::string id, ten, ngaySinh, queQuan, thongTinLienLac;

public:
    Person() {}
    Person(const std::string &id, const std::string &ten, const std::string &ns, const std::string &qq, const std::string &ttll)
        : id(id), ten(ten), ngaySinh(ns), queQuan(qq), thongTinLienLac(ttll) {}
    virtual ~Person() {}

    std::string getID() const { return id; }
    std::string getTen() const { return ten; }
    std::string getNgaySinh() const { return ngaySinh; }
    std::string getQueQuan() const { return queQuan; }
    std::string getThongTinLienLac() const { return thongTinLienLac; }
};
