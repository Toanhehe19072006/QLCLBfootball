#include "raylib.h"
#include "rlgl.h"
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
#include "utf8_helper.h"

// ========================================
// TELEX INPUT ENGINE - Built-in Vietnamese typing
// ========================================
// Tắt EVKey/Unikey khi dùng app này!
// Check if character is a Vietnamese vowel (base or with diacritics)
bool isVietnameseVowel(const std::string &ch)
{
    static MyVector<std::string> vowels;
    if (vowels.empty())
    {
        const char *vowelList[] = {
            "a", "A", "à", "À", "á", "Á", "ả", "Ả", "ã", "Ã", "ạ", "Ạ",
            "ă", "Ă", "ằ", "Ằ", "ắ", "Ắ", "ẳ", "Ẳ", "ẵ", "Ẵ", "ặ", "Ặ",
            "â", "Â", "ầ", "Ầ", "ấ", "Ấ", "ẩ", "Ẩ", "ẫ", "Ẫ", "ậ", "Ậ",
            "e", "E", "è", "È", "é", "É", "ẻ", "Ẻ", "ẽ", "Ẽ", "ẹ", "Ẹ",
            "ê", "Ê", "ề", "Ề", "ế", "Ế", "ể", "Ể", "ễ", "Ễ", "ệ", "Ệ",
            "i", "I", "ì", "Ì", "í", "Í", "ỉ", "Ỉ", "ĩ", "Ĩ", "ị", "Ị",
            "o", "O", "ò", "Ò", "ó", "Ó", "ỏ", "Ỏ", "õ", "Õ", "ọ", "Ọ",
            "ô", "Ô", "ồ", "Ồ", "ố", "Ố", "ổ", "Ổ", "ỗ", "Ỗ", "ộ", "Ộ",
            "ơ", "Ơ", "ờ", "Ờ", "ớ", "Ớ", "ở", "Ở", "ỡ", "Ỡ", "ợ", "Ợ",
            "u", "U", "ù", "Ù", "ú", "Ú", "ủ", "Ủ", "ũ", "Ũ", "ụ", "Ụ",
            "ư", "Ư", "ừ", "Ừ", "ứ", "Ứ", "ử", "Ử", "ữ", "Ữ", "ự", "Ự",
            "y", "Y", "ỳ", "Ỳ", "ý", "Ý", "ỷ", "Ỷ", "ỹ", "Ỹ", "ỵ", "Ỵ"};
        for (const char *v : vowelList)
        {
            vowels.push_back(std::string(v));
        }
    }
    for (const auto &v : vowels)
    {
        if (ch == v)
            return true;
    }
    return false;
}

// Add tone to a vowel
std::string addToneToVowel(const std::string &vowel, char tone)
{
    // Map: base vowel -> {s, f, r, x, j} tones
    static MyMap<std::string, MyVector<std::string>> toneMap;
    if (!toneMap.find("a"))
    {
        MyVector<std::string> a_tones;
        a_tones.push_back("á");
        a_tones.push_back("à");
        a_tones.push_back("ả");
        a_tones.push_back("ã");
        a_tones.push_back("ạ");
        toneMap.insert("a", a_tones);

        MyVector<std::string> A_tones;
        A_tones.push_back("Á");
        A_tones.push_back("À");
        A_tones.push_back("Ả");
        A_tones.push_back("Ã");
        A_tones.push_back("Ạ");
        toneMap.insert("A", A_tones);

        MyVector<std::string> aw_tones;
        aw_tones.push_back("ắ");
        aw_tones.push_back("ằ");
        aw_tones.push_back("ẳ");
        aw_tones.push_back("ẵ");
        aw_tones.push_back("ặ");
        toneMap.insert("ă", aw_tones);

        MyVector<std::string> AW_tones;
        AW_tones.push_back("Ắ");
        AW_tones.push_back("Ằ");
        AW_tones.push_back("Ẳ");
        AW_tones.push_back("Ẵ");
        AW_tones.push_back("Ặ");
        toneMap.insert("Ă", AW_tones);

        MyVector<std::string> aa_tones;
        aa_tones.push_back("ấ");
        aa_tones.push_back("ầ");
        aa_tones.push_back("ẩ");
        aa_tones.push_back("ẫ");
        aa_tones.push_back("ậ");
        toneMap.insert("â", aa_tones);

        MyVector<std::string> AA_tones;
        AA_tones.push_back("Ấ");
        AA_tones.push_back("Ầ");
        AA_tones.push_back("Ẩ");
        AA_tones.push_back("Ẫ");
        AA_tones.push_back("Ậ");
        toneMap.insert("Â", AA_tones);

        MyVector<std::string> e_tones;
        e_tones.push_back("é");
        e_tones.push_back("è");
        e_tones.push_back("ẻ");
        e_tones.push_back("ẽ");
        e_tones.push_back("ẹ");
        toneMap.insert("e", e_tones);

        MyVector<std::string> E_tones;
        E_tones.push_back("É");
        E_tones.push_back("È");
        E_tones.push_back("Ẻ");
        E_tones.push_back("Ẽ");
        E_tones.push_back("Ẹ");
        toneMap.insert("E", E_tones);

        MyVector<std::string> ee_tones;
        ee_tones.push_back("ế");
        ee_tones.push_back("ề");
        ee_tones.push_back("ể");
        ee_tones.push_back("ễ");
        ee_tones.push_back("ệ");
        toneMap.insert("ê", ee_tones);

        MyVector<std::string> EE_tones;
        EE_tones.push_back("Ế");
        EE_tones.push_back("Ề");
        EE_tones.push_back("Ể");
        EE_tones.push_back("Ễ");
        EE_tones.push_back("Ệ");
        toneMap.insert("Ê", EE_tones);

        MyVector<std::string> i_tones;
        i_tones.push_back("í");
        i_tones.push_back("ì");
        i_tones.push_back("ỉ");
        i_tones.push_back("ĩ");
        i_tones.push_back("ị");
        toneMap.insert("i", i_tones);

        MyVector<std::string> I_tones;
        I_tones.push_back("Í");
        I_tones.push_back("Ì");
        I_tones.push_back("Ỉ");
        I_tones.push_back("Ĩ");
        I_tones.push_back("Ị");
        toneMap.insert("I", I_tones);

        MyVector<std::string> o_tones;
        o_tones.push_back("ó");
        o_tones.push_back("ò");
        o_tones.push_back("ỏ");
        o_tones.push_back("õ");
        o_tones.push_back("ọ");
        toneMap.insert("o", o_tones);

        MyVector<std::string> O_tones;
        O_tones.push_back("Ó");
        O_tones.push_back("Ò");
        O_tones.push_back("Ỏ");
        O_tones.push_back("Õ");
        O_tones.push_back("Ọ");
        toneMap.insert("O", O_tones);

        MyVector<std::string> oo_tones;
        oo_tones.push_back("ố");
        oo_tones.push_back("ồ");
        oo_tones.push_back("ổ");
        oo_tones.push_back("ỗ");
        oo_tones.push_back("ộ");
        toneMap.insert("ô", oo_tones);

        MyVector<std::string> OO_tones;
        OO_tones.push_back("Ố");
        OO_tones.push_back("Ồ");
        OO_tones.push_back("Ổ");
        OO_tones.push_back("Ỗ");
        OO_tones.push_back("Ộ");
        toneMap.insert("Ô", OO_tones);

        MyVector<std::string> ow_tones;
        ow_tones.push_back("ớ");
        ow_tones.push_back("ờ");
        ow_tones.push_back("ở");
        ow_tones.push_back("ỡ");
        ow_tones.push_back("ợ");
        toneMap.insert("ơ", ow_tones);

        MyVector<std::string> OW_tones;
        OW_tones.push_back("Ớ");
        OW_tones.push_back("Ờ");
        OW_tones.push_back("Ở");
        OW_tones.push_back("Ỡ");
        OW_tones.push_back("Ợ");
        toneMap.insert("Ơ", OW_tones);

        MyVector<std::string> u_tones;
        u_tones.push_back("ú");
        u_tones.push_back("ù");
        u_tones.push_back("ủ");
        u_tones.push_back("ũ");
        u_tones.push_back("ụ");
        toneMap.insert("u", u_tones);

        MyVector<std::string> U_tones;
        U_tones.push_back("Ú");
        U_tones.push_back("Ù");
        U_tones.push_back("Ủ");
        U_tones.push_back("Ũ");
        U_tones.push_back("Ụ");
        toneMap.insert("U", U_tones);

        MyVector<std::string> uw_tones;
        uw_tones.push_back("ứ");
        uw_tones.push_back("ừ");
        uw_tones.push_back("ử");
        uw_tones.push_back("ữ");
        uw_tones.push_back("ự");
        toneMap.insert("ư", uw_tones);

        MyVector<std::string> UW_tones;
        UW_tones.push_back("Ứ");
        UW_tones.push_back("Ừ");
        UW_tones.push_back("Ử");
        UW_tones.push_back("Ữ");
        UW_tones.push_back("Ự");
        toneMap.insert("Ư", UW_tones);

        MyVector<std::string> y_tones;
        y_tones.push_back("ý");
        y_tones.push_back("ỳ");
        y_tones.push_back("ỷ");
        y_tones.push_back("ỹ");
        y_tones.push_back("ỵ");
        toneMap.insert("y", y_tones);

        MyVector<std::string> Y_tones;
        Y_tones.push_back("Ý");
        Y_tones.push_back("Ỳ");
        Y_tones.push_back("Ỷ");
        Y_tones.push_back("Ỹ");
        Y_tones.push_back("Ỵ");
        toneMap.insert("Y", Y_tones);
    }

    int toneIndex = -1;
    if (tone == 's' || tone == 'S')
        toneIndex = 0;
    else if (tone == 'f' || tone == 'F')
        toneIndex = 1;
    else if (tone == 'r' || tone == 'R')
        toneIndex = 2;
    else if (tone == 'x' || tone == 'X')
        toneIndex = 3;
    else if (tone == 'j' || tone == 'J')
        toneIndex = 4;

    if (toneIndex == -1)
        return vowel;

    if (toneMap.find(vowel))
    {
        return toneMap.at(vowel)[toneIndex];
    }
    return vowel;
}

// Get base vowel (remove tone)
std::string getBaseVowel(const std::string &vowel)
{
    static MyMap<std::string, std::string> baseMap;
    if (!baseMap.find("á"))
    {
        // a variants
        baseMap.insert("á", "a");
        baseMap.insert("à", "a");
        baseMap.insert("ả", "a");
        baseMap.insert("ã", "a");
        baseMap.insert("ạ", "a");
        baseMap.insert("Á", "A");
        baseMap.insert("À", "A");
        baseMap.insert("Ả", "A");
        baseMap.insert("Ã", "A");
        baseMap.insert("Ạ", "A");
        // ă variants
        baseMap.insert("ắ", "ă");
        baseMap.insert("ằ", "ă");
        baseMap.insert("ẳ", "ă");
        baseMap.insert("ẵ", "ă");
        baseMap.insert("ặ", "ă");
        baseMap.insert("Ắ", "Ă");
        baseMap.insert("Ằ", "Ă");
        baseMap.insert("Ẳ", "Ă");
        baseMap.insert("Ẵ", "Ă");
        baseMap.insert("Ặ", "Ă");
        // â variants
        baseMap.insert("ấ", "â");
        baseMap.insert("ầ", "â");
        baseMap.insert("ẩ", "â");
        baseMap.insert("ẫ", "â");
        baseMap.insert("ậ", "â");
        baseMap.insert("Ấ", "Â");
        baseMap.insert("Ầ", "Â");
        baseMap.insert("Ẩ", "Â");
        baseMap.insert("Ẫ", "Â");
        baseMap.insert("Ậ", "Â");
        // e variants
        baseMap.insert("é", "e");
        baseMap.insert("è", "e");
        baseMap.insert("ẻ", "e");
        baseMap.insert("ẽ", "e");
        baseMap.insert("ẹ", "e");
        baseMap.insert("É", "E");
        baseMap.insert("È", "E");
        baseMap.insert("Ẻ", "E");
        baseMap.insert("Ẽ", "E");
        baseMap.insert("Ẹ", "E");
        // ê variants
        baseMap.insert("ế", "ê");
        baseMap.insert("ề", "ê");
        baseMap.insert("ể", "ê");
        baseMap.insert("ễ", "ê");
        baseMap.insert("ệ", "ê");
        baseMap.insert("Ế", "Ê");
        baseMap.insert("Ề", "Ê");
        baseMap.insert("Ể", "Ê");
        baseMap.insert("Ễ", "Ê");
        baseMap.insert("Ệ", "Ê");
        // i variants
        baseMap.insert("í", "i");
        baseMap.insert("ì", "i");
        baseMap.insert("ỉ", "i");
        baseMap.insert("ĩ", "i");
        baseMap.insert("ị", "i");
        baseMap.insert("Í", "I");
        baseMap.insert("Ì", "I");
        baseMap.insert("Ỉ", "I");
        baseMap.insert("Ĩ", "I");
        baseMap.insert("Ị", "I");
        // o variants
        baseMap.insert("ó", "o");
        baseMap.insert("ò", "o");
        baseMap.insert("ỏ", "o");
        baseMap.insert("õ", "o");
        baseMap.insert("ọ", "o");
        baseMap.insert("Ó", "O");
        baseMap.insert("Ò", "O");
        baseMap.insert("Ỏ", "O");
        baseMap.insert("Õ", "O");
        baseMap.insert("Ọ", "O");
        // ô variants
        baseMap.insert("ố", "ô");
        baseMap.insert("ồ", "ô");
        baseMap.insert("ổ", "ô");
        baseMap.insert("ỗ", "ô");
        baseMap.insert("ộ", "ô");
        baseMap.insert("Ố", "Ô");
        baseMap.insert("Ồ", "Ô");
        baseMap.insert("Ổ", "Ô");
        baseMap.insert("Ỗ", "Ô");
        baseMap.insert("Ộ", "Ô");
        // ơ variants
        baseMap.insert("ớ", "ơ");
        baseMap.insert("ờ", "ơ");
        baseMap.insert("ở", "ơ");
        baseMap.insert("ỡ", "ơ");
        baseMap.insert("ợ", "ơ");
        baseMap.insert("Ớ", "Ơ");
        baseMap.insert("Ờ", "Ơ");
        baseMap.insert("Ở", "Ơ");
        baseMap.insert("Ỡ", "Ơ");
        baseMap.insert("Ợ", "Ơ");
        // u variants
        baseMap.insert("ú", "u");
        baseMap.insert("ù", "u");
        baseMap.insert("ủ", "u");
        baseMap.insert("ũ", "u");
        baseMap.insert("ụ", "u");
        baseMap.insert("Ú", "U");
        baseMap.insert("Ù", "U");
        baseMap.insert("Ủ", "U");
        baseMap.insert("Ũ", "U");
        baseMap.insert("Ụ", "U");
        // ư variants
        baseMap.insert("ứ", "ư");
        baseMap.insert("ừ", "ư");
        baseMap.insert("ử", "ư");
        baseMap.insert("ữ", "ư");
        baseMap.insert("ự", "ư");
        baseMap.insert("Ứ", "Ư");
        baseMap.insert("Ừ", "Ư");
        baseMap.insert("Ử", "Ư");
        baseMap.insert("Ữ", "Ư");
        baseMap.insert("Ự", "Ư");
        // y variants
        baseMap.insert("ý", "y");
        baseMap.insert("ỳ", "y");
        baseMap.insert("ỷ", "y");
        baseMap.insert("ỹ", "y");
        baseMap.insert("ỵ", "y");
        baseMap.insert("Ý", "Y");
        baseMap.insert("Ỳ", "Y");
        baseMap.insert("Ỷ", "Y");
        baseMap.insert("Ỹ", "Y");
        baseMap.insert("Ỵ", "Y");
    }
    if (baseMap.find(vowel))
        return baseMap.at(vowel);
    return vowel;
}

// Vietnamese diacritic mappings (aa -> â, etc.)
MyMap<std::string, std::string> initDiacriticMap()
{
    MyMap<std::string, std::string> map;
    map.insert("aa", "â");
    map.insert("AA", "Â");
    map.insert("Aa", "Â");
    map.insert("aA", "Â");
    map.insert("aw", "ă");
    map.insert("AW", "Ă");
    map.insert("Aw", "Ă");
    map.insert("aW", "Ă");
    map.insert("ee", "ê");
    map.insert("EE", "Ê");
    map.insert("Ee", "Ê");
    map.insert("eE", "Ê");
    map.insert("oo", "ô");
    map.insert("OO", "Ô");
    map.insert("Oo", "Ô");
    map.insert("oO", "Ô");
    map.insert("ow", "ơ");
    map.insert("OW", "Ơ");
    map.insert("Ow", "Ơ");
    map.insert("oW", "Ơ");
    map.insert("uw", "ư");
    map.insert("UW", "Ư");
    map.insert("Uw", "Ư");
    map.insert("uW", "Ư");
    map.insert("dd", "đ");
    map.insert("DD", "Đ");
    map.insert("Dd", "Đ");
    map.insert("dD", "Đ");
    return map;
}
static MyMap<std::string, std::string> g_diacriticMap = initDiacriticMap();

// Extract UTF-8 characters from string
MyVector<std::string> utf8Chars(const std::string &str)
{
    MyVector<std::string> chars;
    size_t i = 0;
    while (i < str.length())
    {
        int len = 1;
        unsigned char c = str[i];
        if ((c & 0x80) == 0)
            len = 1;
        else if ((c & 0xE0) == 0xC0)
            len = 2;
        else if ((c & 0xF0) == 0xE0)
            len = 3;
        else if ((c & 0xF8) == 0xF0)
            len = 4;
        chars.push_back(str.substr(i, len));
        i += len;
    }
    return chars;
}

// Find the vowel position to add tone (for words like "toán", "thái")
int findVowelForTone(const MyVector<std::string> &chars)
{
    // Find all vowels
    MyVector<int> vowelPositions;
    for (size_t i = 0; i < chars.size(); i++)
    {
        if (isVietnameseVowel(chars[i]))
        {
            vowelPositions.push_back(i);
        }
    }

    if (vowelPositions.empty())
        return -1;
    if (vowelPositions.size() == 1)
        return vowelPositions[0];

    // Vietnamese tone placement rules:

    // SPECIAL RULE: "qu" + vowel -> tone goes on the vowel AFTER "u", not on "u"
    // Example: "qua" -> "quả" (tone on a, not u)
    for (size_t i = 0; i < chars.size(); i++)
    {
        if ((chars[i] == "q" || chars[i] == "Q") && i + 1 < chars.size() &&
            (chars[i + 1] == "u" || chars[i + 1] == "U"))
        {
            // Find the vowel after "qu"
            for (size_t j = i + 2; j < chars.size(); j++)
            {
                if (isVietnameseVowel(chars[j]))
                {
                    return j;
                }
            }
        }
    }

    // SPECIAL RULE: "gi" + vowel -> tone goes on the vowel AFTER "i"
    // Example: "gia" -> "già" (tone on a, not i)
    for (size_t i = 0; i < chars.size(); i++)
    {
        if ((chars[i] == "g" || chars[i] == "G") && i + 1 < chars.size() &&
            (chars[i + 1] == "i" || chars[i + 1] == "I") && i + 2 < chars.size())
        {
            // Check if there's a vowel after "gi"
            for (size_t j = i + 2; j < chars.size(); j++)
            {
                if (isVietnameseVowel(chars[j]))
                {
                    return j;
                }
            }
        }
    }

    // 1. If there's ơ, ư, ê, ô, â, ă -> put tone on that vowel
    // Check for special vowels first
    for (int pos : vowelPositions)
    {
        std::string base = getBaseVowel(chars[pos]);
        if (base == "ơ" || base == "Ơ" || base == "ư" || base == "Ư" ||
            base == "ê" || base == "Ê" || base == "ô" || base == "Ô" ||
            base == "â" || base == "Â" || base == "ă" || base == "Ă")
        {
            return pos;
        }
    }

    // 2. If word ends with consonant -> tone on last vowel
    // Check if ends with consonant
    if (!chars.empty() && !isVietnameseVowel(chars.back()))
    {
        return vowelPositions.back();
    }

    // 3. Otherwise -> tone on second-to-last vowel (or last if only one)
    // Put tone on second-to-last vowel if 2+ vowels, else last
    if (vowelPositions.size() >= 2)
    {
        return vowelPositions[vowelPositions.size() - 2];
    }
    return vowelPositions.back();
}

// Process Telex input - only process the LAST WORD
std::string ProcessTelex(const std::string &text)
{
    if (text.empty())
        return text;

    // Find the last word (separated by space)
    size_t lastSpacePos = text.rfind(' ');
    std::string prefix = "";
    std::string lastWord = text;

    if (lastSpacePos != std::string::npos)
    {
        prefix = text.substr(0, lastSpacePos + 1); // Include the space
        lastWord = text.substr(lastSpacePos + 1);
    }

    if (lastWord.empty())
        return text;

    std::string result = lastWord;

    // First, process diacritics (aa -> â, ow -> ơ, dd -> đ, etc.)
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto it = g_diacriticMap.begin(); it != g_diacriticMap.end(); ++it)
        {
            size_t pos = result.find((*it).key);
            if (pos != std::string::npos)
            {
                result = result.substr(0, pos) + (*it).value + result.substr(pos + (*it).key.length());
                changed = true;
                break;
            }
        }
    }

    // Then, process tone marks (s, f, r, x, j)
    if (result.length() > 0)
    {
        char lastChar = result.back();
        if (lastChar == 's' || lastChar == 'S' ||
            lastChar == 'f' || lastChar == 'F' ||
            lastChar == 'r' || lastChar == 'R' ||
            lastChar == 'x' || lastChar == 'X' ||
            lastChar == 'j' || lastChar == 'J')
        {

            // Remove the tone character
            std::string withoutTone = result.substr(0, result.length() - 1);

            // Get UTF-8 characters
            MyVector<std::string> chars = utf8Chars(withoutTone);

            // Find the correct vowel to add tone (in the last word only)
            int vowelPos = findVowelForTone(chars);

            if (vowelPos >= 0)
            {
                std::string baseVowel = getBaseVowel(chars[vowelPos]);
                std::string tonedVowel = addToneToVowel(baseVowel, lastChar);

                if (tonedVowel != baseVowel)
                {
                    chars[vowelPos] = tonedVowel;
                    result = "";
                    for (const auto &ch : chars)
                    {
                        result += ch;
                    }
                }
            }
        }
    }

    return prefix + result;
}

using json = nlohmann::json;
using namespace std;

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

                            // Load lịch sử trận đấu
                            if (pJson.contains("lichSuTranDau") && pJson["lichSuTranDau"].is_array())
                            {
                                for (const auto &tranJson : pJson["lichSuTranDau"])
                                {
                                    MatchRecord tran;
                                    if (tranJson.contains("ngayThiDau"))
                                        tran.ngayThiDau = tranJson["ngayThiDau"];
                                    if (tranJson.contains("doiDoi"))
                                        tran.doiDoi = tranJson["doiDoi"];
                                    if (tranJson.contains("giaiDau"))
                                        tran.giaiDau = tranJson["giaiDau"];
                                    if (tranJson.contains("banThang"))
                                        tran.banThang = tranJson["banThang"];
                                    if (tranJson.contains("kienTao"))
                                        tran.kienTao = tranJson["kienTao"];
                                    if (tranJson.contains("theVang"))
                                        tran.theVang = tranJson["theVang"];
                                    if (tranJson.contains("theDo"))
                                        tran.theDo = tranJson["theDo"];
                                    if (tranJson.contains("ghiChu"))
                                        tran.ghiChu = tranJson["ghiChu"];
                                    p.addTranDau(tran);
                                }
                            }

                            // Load danh sách giải thưởng cá nhân
                            if (pJson.contains("danhSachGiaiThuong") && pJson["danhSachGiaiThuong"].is_array())
                            {
                                for (const auto &awJson : pJson["danhSachGiaiThuong"])
                                {
                                    Award award;
                                    if (awJson.contains("tenGiai"))
                                        award.tenGiai = awJson["tenGiai"];
                                    if (awJson.contains("nam"))
                                        award.nam = awJson["nam"];
                                    if (awJson.contains("xepHang"))
                                        award.xepHang = awJson["xepHang"];
                                    if (awJson.contains("ghiChu"))
                                        award.ghiChu = awJson["ghiChu"];
                                    p.addGiaiThuong(award);
                                }
                            }
                            // Hỗ trợ chuyển đổi từ format cũ (danhSachDanhHieu) sang format mới
                            else if (pJson.contains("danhSachDanhHieu") && pJson["danhSachDanhHieu"].is_array())
                            {
                                // Chuyển đổi danh hiệu cũ sang giải thưởng mới (mặc định là Top 1, năm 2024)
                                for (const auto &dhJson : pJson["danhSachDanhHieu"])
                                {
                                    if (dhJson.is_string())
                                    {
                                        Award award;
                                        award.tenGiai = dhJson.get<std::string>();
                                        award.nam = 2024;  // Năm mặc định
                                        award.xepHang = 1; // Mặc định Top 1
                                        award.ghiChu = u8"Chuyển đổi từ dữ liệu cũ";
                                        p.addGiaiThuong(award);
                                    }
                                }
                            }

                            // Load thành tích cùng đội bóng
                            if (pJson.contains("thanhTichCungDoi") && pJson["thanhTichCungDoi"].is_array())
                            {
                                for (const auto &ttJson : pJson["thanhTichCungDoi"])
                                {
                                    TeamAchievement tt;
                                    if (ttJson.contains("tenGiai"))
                                        tt.tenGiai = ttJson["tenGiai"];
                                    if (ttJson.contains("nam"))
                                        tt.nam = ttJson["nam"];
                                    if (ttJson.contains("xepHang"))
                                        tt.xepHang = ttJson["xepHang"];
                                    if (ttJson.contains("ghiChu"))
                                        tt.ghiChu = ttJson["ghiChu"];
                                    p.addThanhTichDoi(tt);
                                }
                            }

                            team.themCauThu(p);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << "Error loading player: " << e.what() << "\n";
                        }
                    }
                }

                // Load danh hiệu tập thể của đội
                if (teamJson.contains("danhHieuTapThe") && teamJson["danhHieuTapThe"].is_array())
                {
                    for (const auto &awJson : teamJson["danhHieuTapThe"])
                    {
                        TeamAward award;
                        if (awJson.contains("tenGiai"))
                            award.tenGiai = awJson["tenGiai"];
                        if (awJson.contains("nam"))
                            award.nam = awJson["nam"];
                        if (awJson.contains("xepHang"))
                            award.xepHang = awJson["xepHang"];
                        if (awJson.contains("ghiChu"))
                            award.ghiChu = awJson["ghiChu"];
                        team.addDanhHieuTapThe(award);
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
            // Lưu lịch sử trận đấu
            json lichSuJson = json::array();
            for (const auto &tran : p.getLichSuTranDau())
            {
                lichSuJson.push_back({{"ngayThiDau", tran.ngayThiDau},
                                      {"doiDoi", tran.doiDoi},
                                      {"giaiDau", tran.giaiDau},
                                      {"banThang", tran.banThang},
                                      {"kienTao", tran.kienTao},
                                      {"theVang", tran.theVang},
                                      {"theDo", tran.theDo},
                                      {"ghiChu", tran.ghiChu}});
            }

            // Lưu danh sách giải thưởng cá nhân
            json giaiThuongJson = json::array();
            for (const auto &award : p.getDanhSachGiaiThuong())
            {
                giaiThuongJson.push_back({{"tenGiai", award.tenGiai},
                                          {"nam", award.nam},
                                          {"xepHang", award.xepHang},
                                          {"ghiChu", award.ghiChu}});
            }

            // Lưu thành tích cùng đội bóng
            json thanhTichDoiJson = json::array();
            for (const auto &tt : p.getThanhTichCungDoi())
            {
                thanhTichDoiJson.push_back({{"tenGiai", tt.tenGiai},
                                            {"nam", tt.nam},
                                            {"xepHang", tt.xepHang},
                                            {"ghiChu", tt.ghiChu}});
            }

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
                {"lichSuTranDau", lichSuJson},
                {"danhSachGiaiThuong", giaiThuongJson},
                {"thanhTichCungDoi", thanhTichDoiJson},
                {"sucKhoe", {{"trangThai", p.getTrangThaiSucKhoe()}, {"ngayBatDau", p.getNgayBatDauChanThuong()}, {"duKienHoiPhuc", p.getDuKienHoiPhuc()}, {"ghiChu", p.getGhiChuSucKhoe()}}},
                {"theLuc", {{"mucTheLuc", p.getMucTheLuc()}, {"chieuCao", p.getChieuCao()}, {"canNang", p.getCanNang()}, {"tiLeMo", p.getTiLeMo()}}},
                {"hieuSuat", {{"banThang", p.getBanThangHieuSuat()}, {"kienTao", p.getKienTao()}, {"chuyenDung", p.getChuyenDung()}, {"diemPhongDo", p.getDiemPhongDo()}}},
                {"tapLuyen", {{"diemDanhGia", p.getDiemDanhGiaTap()}, {"cuongDo", p.getCuongDoTapLuyen()}, {"ghiChu", p.getGhiChuTapLuyen()}}},
                {"kyLuat", {{"thaiDo", p.getThaiDo()}, {"coViPham", p.getCoViPham()}, {"chiTietViPham", p.getChiTietViPham()}, {"ghiChuTinhThan", p.getGhiChuTinhThan()}}}};
            teamJson["players"].push_back(pJson);
        }

        // Lưu danh hiệu tập thể của đội
        json danhHieuTapTheJson = json::array();
        for (const auto &award : team.getDanhHieuTapThe())
        {
            danhHieuTapTheJson.push_back({{"tenGiai", award.tenGiai},
                                          {"nam", award.nam},
                                          {"xepHang", award.xepHang},
                                          {"ghiChu", award.ghiChu}});
        }
        teamJson["danhHieuTapThe"] = danhHieuTapTheJson;

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

// Hàm vẽ rectangle bo góc với gradient ngang
void DrawRectangleRoundedGradientH(Rectangle rec, float roundness, int segments, Color left, Color right)
{
    // Vẽ gradient bằng nhiều đường dọc nhỏ
    int steps = (int)rec.width;
    for (int i = 0; i < steps; i++)
    {
        float t = (float)i / (float)steps;
        Color col = {
            (unsigned char)(left.r + (right.r - left.r) * t),
            (unsigned char)(left.g + (right.g - left.g) * t),
            (unsigned char)(left.b + (right.b - left.b) * t),
            (unsigned char)(left.a + (right.a - left.a) * t)};

        float x = rec.x + i;
        float margin = 0;

        // Tính margin cho bo góc
        if (i < rec.height * roundness || i > rec.width - rec.height * roundness)
        {
            float cornerRadius = rec.height * roundness;
            float distFromCorner;
            if (i < cornerRadius)
                distFromCorner = cornerRadius - i;
            else
                distFromCorner = i - (rec.width - cornerRadius);

            if (distFromCorner > 0 && distFromCorner < cornerRadius)
            {
                margin = cornerRadius - sqrt(cornerRadius * cornerRadius - distFromCorner * distFromCorner);
            }
        }

        DrawLineEx({x, rec.y + margin}, {x, rec.y + rec.height - margin}, 1, col);
    }
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
    string compositionText;    // For IME composition
    bool multiline;            // Support multiline input
    string lastClipboard;      // Track clipboard for IME auto-paste
    double lastClipboardCheck; // Time of last clipboard check

    InputField() : rect({0, 0, 0, 0}), label(""), text(""), active(false), maxLength(50), compositionText(""), multiline(false), lastClipboard(""), lastClipboardCheck(0) {}
    InputField(Rectangle r, string l, int max = 50, bool multi = false)
        : rect(r), label(l), text(""), active(false), maxLength(max), compositionText(""), multiline(multi), lastClipboard(""), lastClipboardCheck(0) {}

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
            int key = GetCharPressed();
            while (key > 0)
            {
                // Accept ASCII characters for Telex input
                if (key >= 32 && key <= 126)
                {
                    if (text.length() < (size_t)maxLength * 4)
                    {
                        text += (char)key;
                        // Process Telex after each character
                        text = ProcessTelex(text);
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

            // Handle Ctrl+V paste from clipboard
            if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V))
            {
                const char *clipboardText = GetClipboardText();
                if (clipboardText != NULL)
                {
                    string pasteText = clipboardText;
                    if (text.length() + pasteText.length() < (size_t)maxLength * 4)
                    {
                        text += pasteText;
                    }
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

    // Hệ thống đăng nhập
    bool isLoggedIn;
    bool isAdmin; // true = Admin (full quyền), false = User (chỉ xem)
    string loginUsername;
    string loginPassword;
    string loginError;
    float loginErrorTimer; // Timer để tự động ẩn thông báo lỗi
    int loginActiveField;  // 0 = username, 1 = password

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

    // Popup đổi vai trò đội trưởng/đội phó
    bool showEditRole;
    int editingRoleTeamIndex;
    int roleScrollOffset;

    // Popup xác nhận xóa cầu thủ
    bool showDeleteConfirm;
    string deletePlayerID;
    string deletePlayerName;
    Team *deletePlayerTeam;

    // Popup xác nhận xóa đội bóng
    bool showDeleteTeamConfirm;
    int deleteTeamIndex;
    string deleteTeamName;

    // Popup thống kê chi tiết từng đội
    bool showStatsDetail;
    int statsDetailType; // 0: cầu thủ, 1: bàn thắng, 2: lương
    int statsDetailScrollOffset;
    int statsSelectedYear; // Năm được chọn để lọc thống kê (0 = tất cả)

    // Popup lịch sử trận đấu
    bool showMatchHistory;
    Player *matchHistoryPlayer;
    string matchHistoryTeamName;
    int matchHistoryScrollOffset;

    // Popup thêm trận đấu mới
    bool showAddMatch;
    MyVector<InputField> matchInputs;

    // Popup danh hiệu riêng
    bool showHonorPopup;
    Player *honorPopupPlayer;
    string honorPopupTeamName;

    // Popup danh hiệu tập thể
    bool showTeamAwardPopup;
    Team *teamAwardPopupTeam;
    int teamAwardScrollOffset;

    // Popup giải thưởng cá nhân (Individual Awards)
    bool showIndividualAwardPopup;
    Player *individualAwardPlayer;
    string individualAwardTeamName;

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
                             showEditRole(false), editingRoleTeamIndex(-1), roleScrollOffset(0),
                             showDeleteConfirm(false), deletePlayerID(""), deletePlayerName(""), deletePlayerTeam(nullptr),
                             showDeleteTeamConfirm(false), deleteTeamIndex(-1), deleteTeamName(""),
                             showStatsDetail(false), statsDetailType(0), statsDetailScrollOffset(0), statsSelectedYear(0),
                             showMatchHistory(false), matchHistoryPlayer(nullptr), matchHistoryTeamName(""), matchHistoryScrollOffset(0),
                             showAddMatch(false),
                             showHonorPopup(false), honorPopupPlayer(nullptr), honorPopupTeamName(""),
                             showTeamAwardPopup(false), teamAwardPopupTeam(nullptr), teamAwardScrollOffset(0),
                             showIndividualAwardPopup(false), individualAwardPlayer(nullptr), individualAwardTeamName(""),
                             selectedPlayerIndex(-1), lastLoadedAchievementPlayerIndex(-1),
                             tempNgayGiaNhap(""), tempDoiBongTruoc(""), tempBanThangChoClub(0), tempDanhHieuTaiClub(""), tempSoTranThiDau(0), tempTheVang(0), tempTheDo(0),
                             tempTrangThaiSucKhoe(""), tempNgayBatDauChanThuong(""), tempDuKienHoiPhuc(""), tempGhiChuSucKhoe(""),
                             tempMucTheLuc(0), tempChieuCao(0), tempCanNang(0), tempTiLeMo(0),
                             tempBanThangHieuSuat(0), tempKienTao(0), tempChuyenDung(0), tempDiemPhongDo(0),
                             tempDiemDanhGiaTap(0), tempCuongDoTapLuyen(""), tempGhiChuTapLuyen(""),
                             tempThaiDo(""), tempCoViPham(false), tempChiTietViPham(""), tempGhiChuTinhThan(""),
                             isLoggedIn(false), isAdmin(false), loginUsername(""), loginPassword(""), loginError(""), loginErrorTimer(0), loginActiveField(0)
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

        // Không scroll khi có popup đang mở
        if (!showEditRole && !showEditHLV && !showEditTeam && !showPlayerDetail && !showEditPlayer && !showDeleteConfirm && !showStatsDetail && !showMatchHistory && !showAddMatch && !showHonorPopup && !showTeamAwardPopup)
        {
            float mouseWheel = GetMouseWheelMove();
            if (mouseWheel != 0)
            {
                scrollOffset -= (int)(mouseWheel * 30);
                scrollOffset = max(0, min(scrollOffset, contentHeight - viewHeight));
            }
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

        // Tính bàn thắng từ lịch sử trận đấu để đồng nhất
        int playerGoalsFromHistory = 0;
        for (const auto &match : p.getLichSuTranDau())
        {
            playerGoalsFromHistory += match.banThang;
        }
        string info = u8"Mã: " + p.getMaCauThu() + " | " + teamName + " | " + p.getViTri() +
                      " | BT: " + to_string(playerGoalsFromHistory) +
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

        // Chỉ hiển thị nút XÓA khi là Admin
        if (isAdmin)
        {
            Button deleteBtn;
            deleteBtn.rect = {(float)(x + cardWidth - 100), (float)(y + 10), 80, 40};
            deleteBtn.text = u8"XÓA";
            deleteBtn.color = ACCENT_2;
            deleteBtn.hoverColor = {192, 57, 43, 255};
            deleteBtn.draw();

            if (deleteBtn.isClicked() && team != nullptr)
            {
                showDeleteConfirm = true;
                deletePlayerID = p.getID();
                deletePlayerName = p.getTen();
                deletePlayerTeam = team;
            }
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
        // Tính bàn thắng từ lịch sử trận đấu để đồng nhất
        int cccdGoals = 0;
        for (const auto &match : selectedPlayer.getLichSuTranDau())
        {
            cccdGoals += match.banThang;
        }
        DrawTextEx2(u8"BÀN THẮNG: " + to_string(cccdGoals), rightCol - 50, popupY + popupHeight - 85, 16, ACCENT_2);
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
            // Tính bàn thắng từ lịch sử trận đấu để đồng nhất với tab Thống kê
            int teamGoals = 0;
            for (auto &p : team.getPlayers())
            {
                for (const auto &match : p.getLichSuTranDau())
                {
                    teamGoals += match.banThang;
                }
            }
            string info = u8"Số cầu thủ: " + to_string(team.getPlayers().size()) +
                          u8" | Tổng bàn thắng: " + to_string(teamGoals);
            DrawTextEx2(info, contentX + 40, 205, 18, TEXT_LIGHT);

            string hlvInfo = u8"HLV trưởng: " + (team.getHLVTruong().empty() ? u8"Chưa có" : team.getHLVTruong()) +
                             u8"  |  HLV phó: " + (team.getHLVPho().empty() ? u8"Chưa có" : team.getHLVPho());
            DrawTextEx2(hlvInfo, contentX + 40, 235, 16, (Color){255, 255, 255, 200});

            // Chỉ hiển thị nút SỬA HLV và ĐỔI VAI TRÒ khi là Admin
            if (isAdmin)
            {
                Button editHLVBtn;
                editHLVBtn.rect = {(float)(contentX + contentWidth - 150), 165, 130, 35};
                editHLVBtn.text = u8"SỬA HLV";
                editHLVBtn.color = CAPTAIN_COLOR;
                editHLVBtn.hoverColor = {194, 144, 34, 255};
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

                // Button đổi vai trò đội trưởng/đội phó
                Button editRoleBtn;
                editRoleBtn.rect = {(float)(contentX + contentWidth - 290), 165, 130, 35};
                editRoleBtn.text = u8"ĐỔI VAI TRÒ";
                editRoleBtn.color = {243, 156, 18, 255};
                editRoleBtn.hoverColor = {211, 132, 13, 255};
                editRoleBtn.draw();

                if (editRoleBtn.isClicked())
                {
                    showEditRole = true;
                    editingRoleTeamIndex = selectedTeamIndex;
                    roleScrollOffset = 0;
                }
            } // Đóng ngoặc isAdmin cho phần SỬA HLV và ĐỔI VAI TRÒ

            DrawTextEx2(u8"DANH SÁCH CẦU THỦ", contentX + 20, 310, 20, TEXT_DARK);

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

            return;
        }

        int totalTeams = clb->getTeams().size();
        int totalPlayers = 0, totalGoals = 0;
        for (auto &team : clb->getTeams())
        {
            totalPlayers += team.getPlayers().size();
            // Tính bàn thắng từ lịch sử trận đấu để đồng nhất với tab Thống kê
            for (auto &p : team.getPlayers())
            {
                for (const auto &match : p.getLichSuTranDau())
                {
                    totalGoals += match.banThang;
                }
            }
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
            // Tính bàn thắng từ lịch sử trận đấu để đồng nhất với tab Thống kê
            int teamGoals = 0;
            for (auto &p : team.getPlayers())
            {
                for (const auto &match : p.getLichSuTranDau())
                {
                    teamGoals += match.banThang;
                }
            }
            string info = "So cau thu: " + to_string(team.getPlayers().size()) +
                          " | Bàn thắng: " + to_string(teamGoals);
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

        // Chỉ hiển thị form thêm đội khi là Admin
        if (isAdmin)
        {
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
        } // Đóng ngoặc isAdmin cho phần thêm đội
        else
        {
            // Hiển thị thông báo cho User
            DrawTextEx2(u8"[Chế độ xem - Không có quyền thêm/sửa/xóa]", contentX + 20, 170, 16, (Color){241, 196, 15, 255});
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

            // Chỉ hiển thị nút ĐỔI TÊN và XOÁ ĐỘI khi là Admin
            if (isAdmin)
            {
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
                        // Hiển thị popup xác nhận xóa đội
                        showDeleteTeamConfirm = true;
                        deleteTeamIndex = teamIndex;
                        deleteTeamName = team.getTenDoi();
                    }
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

        // Chỉ hiển thị form thêm cầu thủ khi là Admin
        if (isAdmin)
        {
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
        } // Đóng ngoặc isAdmin cho phần thêm cầu thủ
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
        // Dùng biến member statsSelectedYear thay vì static local
        int &selectedYear = statsSelectedYear;

        int sidebarWidth = max(200, screenWidth / 7);
        int contentX = sidebarWidth + 30;
        int contentWidth = screenWidth - sidebarWidth - 60;

        // Header với năm được chọn
        if (selectedYear == 0)
            drawHeader(u8"THỐNG KÊ THÀNH TÍCH CẦU THỦ - TẤT CẢ");
        else
            drawHeader((u8"THỐNG KÊ THÀNH TÍCH CẦU THỦ - NĂM " + to_string(selectedYear)).c_str());

        // ===== YEAR SELECTOR =====
        int yearSelectorY = 110;
        int btnW = 80;
        int btnH = 32;
        int btnSpacing = 10;

        // Các năm cần hiển thị: 2022, 2023, 2024, 2025, Tất cả
        int years[] = {0, 2022, 2023, 2024, 2025}; // 0 = Tất cả
        const char *yearLabels[] = {u8"Tất cả", "2022", "2023", "2024", "2025"};
        int numYears = 5;

        int totalBtnWidth = numYears * btnW + (numYears - 1) * btnSpacing;
        int startX = contentX + (contentWidth - totalBtnWidth) / 2;

        for (int i = 0; i < numYears; i++)
        {
            int btnX = startX + i * (btnW + btnSpacing);
            Rectangle btnRect = {(float)btnX, (float)yearSelectorY, (float)btnW, (float)btnH};

            bool isSelected = (selectedYear == years[i]);
            bool isHover = CheckCollisionPointRec(GetMousePosition(), btnRect);

            // Background
            if (isSelected)
            {
                DrawRectangleRounded(btnRect, 0.3f, 8, ACCENT_1);
            }
            else if (isHover)
            {
                DrawRectangleRounded(btnRect, 0.3f, 8, (Color){52, 152, 219, 150});
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            }
            else
            {
                DrawRectangleRounded(btnRect, 0.3f, 8, (Color){200, 200, 200, 255});
            }

            // Text
            int labelW = MeasureTextEx2(yearLabels[i], 14);
            Color textColor = isSelected ? TEXT_LIGHT : TEXT_DARK;
            DrawTextEx2(yearLabels[i], btnX + (btnW - labelW) / 2, yearSelectorY + 8, 14, textColor);

            // Click handler
            if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedYear = years[i];
            }
        }

        auto &teams = clb->getTeams();

        if (teams.empty())
        {
            DrawTextEx2(u8"Chưa có dữ liệu thống kê!", contentX + 100, 300, 24, ACCENT_2);
            return;
        }

        // Thu thập dữ liệu (lọc theo năm nếu cần)
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
                // Tính bàn thắng theo năm từ lịch sử trận đấu
                int playerGoalsInYear = 0;

                // Luôn tính từ lịch sử trận đấu
                for (const auto &match : p.getLichSuTranDau())
                {
                    // Trích năm từ ngayThiDau (format: dd/mm/yyyy)
                    int matchYear = 0;
                    if (match.ngayThiDau.length() >= 10)
                    {
                        // Lấy 4 ký tự cuối (năm)
                        try
                        {
                            matchYear = stoi(match.ngayThiDau.substr(6, 4));
                        }
                        catch (...)
                        {
                            matchYear = 0;
                        }
                    }
                    // Nếu selectedYear == 0 (Tất cả) thì lấy hết, còn không thì chỉ lấy năm được chọn
                    if (selectedYear == 0 || matchYear == selectedYear)
                    {
                        playerGoalsInYear += match.banThang;
                    }
                }

                totalGoals += playerGoalsInYear;
                totalSalary += p.getLuong();

                if (playerGoalsInYear > 0 || selectedYear == 0)
                {
                    topScorers.push_back({p.getTen(), playerGoalsInYear});
                }

                // Lấy giải thưởng (lọc theo năm nếu cần)
                const auto &awards = p.getDanhSachGiaiThuong();
                if (!awards.empty())
                {
                    if (selectedYear == 0)
                    {
                        // Tất cả năm - lấy giải mới nhất
                        int newestYear = 0;
                        string bestAward = "";
                        for (const auto &aw : awards)
                        {
                            if (aw.nam >= newestYear)
                            {
                                newestYear = aw.nam;
                                bestAward = aw.tenGiai + u8" (" + to_string(aw.nam) + u8")";
                            }
                        }
                        if (!bestAward.empty())
                            playersWithHonors.push_back({p.getTen(), bestAward});
                    }
                    else
                    {
                        // Lọc giải thưởng theo năm được chọn
                        for (const auto &aw : awards)
                        {
                            if (aw.nam == selectedYear)
                            {
                                playersWithHonors.push_back({p.getTen(), aw.tenGiai});
                            }
                        }
                    }
                }

                if (playerGoalsInYear > maxGoals)
                    maxGoals = playerGoalsInYear;
            }
        }

        sort(topScorers.begin(), topScorers.end(),
             [](const pair<string, int> &a, const pair<string, int> &b)
             { return a.second > b.second; });

        // ===== THẺ THỐNG KÊ TỔNG QUAN 3D =====
        int statsY = 160; // Điều chỉnh Y để chừa chỗ cho year selector
        int cardWidth = (contentWidth - 60) / 3;

        // Card 1: Tổng cầu thủ
        {
            int cardX = contentX;
            Color shadowColor = {0, 0, 0, 40};
            DrawRectangleRounded({(float)(cardX + 8), (float)(statsY + 8), (float)cardWidth, 140}, 0.15f, 10, shadowColor);

            // Gradient background
            Rectangle cardRect = {(float)cardX, (float)statsY, (float)cardWidth, 140};

            // Hiệu ứng hover
            bool isHover = CheckCollisionPointRec(GetMousePosition(), cardRect);
            if (isHover)
            {
                DrawRectangleGradientEx(cardRect,
                                        (Color){72, 172, 239, 255},
                                        (Color){61, 148, 205, 255},
                                        (Color){61, 148, 205, 255},
                                        (Color){72, 172, 239, 255});
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            }
            else
            {
                DrawRectangleGradientEx(cardRect,
                                        (Color){52, 152, 219, 255},
                                        (Color){41, 128, 185, 255},
                                        (Color){41, 128, 185, 255},
                                        (Color){52, 152, 219, 255});
            }

            // Xử lý click
            if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                showStatsDetail = true;
                statsDetailType = 0;
                statsDetailScrollOffset = 0;
            }

            // Icon 3D
            int iconX = cardX + cardWidth / 2;
            int iconY = statsY + 45;
            DrawCircle(iconX + 3, iconY + 3, 28, (Color){0, 0, 0, 60});
            DrawCircle(iconX, iconY, 28, (Color){255, 255, 255, 255});
            DrawCircle(iconX, iconY, 24, ACCENT_1);
            DrawTextEx2("P", iconX - 10, iconY - 12, 28, TEXT_LIGHT);

            // Số liệu với hiệu ứng
            string playerCount = to_string(totalPlayers);
            int numWidth = MeasureTextEx2(playerCount, 36);
            DrawTextEx2(playerCount, cardX + (cardWidth - numWidth) / 2 + 2, statsY + 75, 36, (Color){0, 0, 0, 80});
            DrawTextEx2(playerCount, cardX + (cardWidth - numWidth) / 2, statsY + 73, 36, TEXT_LIGHT);

            int labelWidth = MeasureTextEx2(u8"CẦU THỦ", 16);
            DrawTextEx2(u8"CẦU THỦ", cardX + (cardWidth - labelWidth) / 2, statsY + 115, 16, TEXT_LIGHT);
        }

        // Card 2: Tổng bàn thắng
        {
            int cardX = contentX + cardWidth + 30;
            Color shadowColor = {0, 0, 0, 40};
            DrawRectangleRounded({(float)(cardX + 8), (float)(statsY + 8), (float)cardWidth, 140}, 0.15f, 10, shadowColor);

            Rectangle cardRect = {(float)cardX, (float)statsY, (float)cardWidth, 140};

            // Hiệu ứng hover
            bool isHover = CheckCollisionPointRec(GetMousePosition(), cardRect);
            if (isHover)
            {
                DrawRectangleGradientEx(cardRect,
                                        (Color){251, 96, 80, 255},
                                        (Color){212, 77, 63, 255},
                                        (Color){212, 77, 63, 255},
                                        (Color){251, 96, 80, 255});
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            }
            else
            {
                DrawRectangleGradientEx(cardRect,
                                        (Color){231, 76, 60, 255},
                                        (Color){192, 57, 43, 255},
                                        (Color){192, 57, 43, 255},
                                        (Color){231, 76, 60, 255});
            }

            // Xử lý click
            if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                showStatsDetail = true;
                statsDetailType = 1;
                statsDetailScrollOffset = 0;
            }

            int iconX = cardX + cardWidth / 2;
            int iconY = statsY + 45;
            DrawCircle(iconX + 3, iconY + 3, 28, (Color){0, 0, 0, 60});
            DrawCircle(iconX, iconY, 28, (Color){255, 255, 255, 255});
            DrawCircle(iconX, iconY, 24, ACCENT_2);
            DrawTextEx2("G", iconX - 9, iconY - 12, 28, TEXT_LIGHT);

            string goalCount = to_string(totalGoals);
            int numWidth = MeasureTextEx2(goalCount, 36);
            DrawTextEx2(goalCount, cardX + (cardWidth - numWidth) / 2 + 2, statsY + 75, 36, (Color){0, 0, 0, 80});
            DrawTextEx2(goalCount, cardX + (cardWidth - numWidth) / 2, statsY + 73, 36, TEXT_LIGHT);

            int labelWidth = MeasureTextEx2(u8"BÀN THẮNG", 16);
            DrawTextEx2(u8"BÀN THẮNG", cardX + (cardWidth - labelWidth) / 2, statsY + 115, 16, TEXT_LIGHT);
        }

        // Card 3: Tổng lương
        {
            int cardX = contentX + 2 * (cardWidth + 30);
            Color shadowColor = {0, 0, 0, 40};
            DrawRectangleRounded({(float)(cardX + 8), (float)(statsY + 8), (float)cardWidth, 140}, 0.15f, 10, shadowColor);

            Rectangle cardRect = {(float)cardX, (float)statsY, (float)cardWidth, 140};

            // Hiệu ứng hover
            bool isHover = CheckCollisionPointRec(GetMousePosition(), cardRect);
            if (isHover)
            {
                DrawRectangleGradientEx(cardRect,
                                        (Color){66, 224, 133, 255},
                                        (Color){59, 194, 116, 255},
                                        (Color){59, 194, 116, 255},
                                        (Color){66, 224, 133, 255});
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            }
            else
            {
                DrawRectangleGradientEx(cardRect,
                                        (Color){46, 204, 113, 255},
                                        (Color){39, 174, 96, 255},
                                        (Color){39, 174, 96, 255},
                                        (Color){46, 204, 113, 255});
            }

            // Xử lý click
            if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                showStatsDetail = true;
                statsDetailType = 2;
                statsDetailScrollOffset = 0;
            }

            int iconX = cardX + cardWidth / 2;
            int iconY = statsY + 45;
            DrawCircle(iconX + 3, iconY + 3, 28, (Color){0, 0, 0, 60});
            DrawCircle(iconX, iconY, 28, (Color){255, 255, 255, 255});
            DrawCircle(iconX, iconY, 24, (Color){46, 204, 113, 255});
            DrawTextEx2("$", iconX - 8, iconY - 12, 28, TEXT_LIGHT);

            string salaryText = to_string((int)(totalSalary / 1000)) + "M";
            int numWidth = MeasureTextEx2(salaryText, 36);
            DrawTextEx2(salaryText, cardX + (cardWidth - numWidth) / 2 + 2, statsY + 75, 36, (Color){0, 0, 0, 80});
            DrawTextEx2(salaryText, cardX + (cardWidth - numWidth) / 2, statsY + 73, 36, TEXT_LIGHT);

            int labelWidth = MeasureTextEx2(u8"TỔNG LƯƠNG", 16);
            DrawTextEx2(u8"TỔNG LƯƠNG", cardX + (cardWidth - labelWidth) / 2, statsY + 115, 16, TEXT_LIGHT);
        }

        // ===== PANEL TOP GHI BÀN VỚI BIỂU ĐỒ 3D =====
        int panelY = 330; // Điều chỉnh để chừa chỗ cho year selector
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

            DrawTextEx2(u8"TOP GIẢI THƯỞNG CÁ NHÂN", panelX + 20, panelY + 15, 20, TEXT_LIGHT);

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

                    // Xác định màu dựa trên tên giải thưởng (vàng/bạc/đồng)
                    string honor = playersWithHonors[i].second;
                    string honorLower = honor;
                    // Chuyển về chữ thường để so sánh (xử lý UTF-8 đơn giản)
                    Color cardColor = {243, 156, 18, 255}; // Mặc định màu cam

                    // Kiểm tra tên giải có chứa "vàng", "bạc", "đồng"
                    if (honor.find(u8"vàng") != string::npos || honor.find(u8"Vàng") != string::npos ||
                        honor.find(u8"VÀNG") != string::npos)
                    {
                        cardColor = {255, 193, 7, 255}; // Vàng (Gold)
                    }
                    else if (honor.find(u8"bạc") != string::npos || honor.find(u8"Bạc") != string::npos ||
                             honor.find(u8"BẠC") != string::npos)
                    {
                        cardColor = {158, 158, 158, 255}; // Bạc (Silver)
                    }
                    else if (honor.find(u8"đồng") != string::npos || honor.find(u8"Đồng") != string::npos ||
                             honor.find(u8"ĐỒNG") != string::npos)
                    {
                        cardColor = {205, 127, 50, 255}; // Đồng (Bronze)
                    }

                    Color darkCard = cardColor;
                    darkCard.r = (unsigned char)max(0, (int)darkCard.r - 30);
                    darkCard.g = (unsigned char)max(0, (int)darkCard.g - 30);
                    darkCard.b = (unsigned char)max(0, (int)darkCard.b - 30);

                    DrawRectangleGradientEx({(float)cardX, (float)cardY, (float)cardW, 65},
                                            (Color){255, 255, 255, 255},
                                            (Color){250, 250, 250, 255},
                                            (Color){245, 245, 245, 255},
                                            (Color){255, 255, 255, 255});

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
                    DrawTextEx2(playerInfo, cardX + 50, cardY + 12, 12, TEXT_DARK);

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
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, CAPTAIN_COLOR);
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

    void drawEditRolePopup()
    {
        if (!showEditRole)
            return;

        if (editingRoleTeamIndex < 0 || editingRoleTeamIndex >= (int)clb->getTeams().size())
        {
            showEditRole = false;
            return;
        }

        MyVector<Team> &teams = clb->getTeams();
        Team &team = teams[editingRoleTeamIndex];

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 700;
        int popupHeight = 550;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, CAPTAIN_COLOR);
        DrawTextEx2(u8"ĐỔI VAI TRÒ ĐỘI TRƯỞNG / ĐỘI PHÓ", popupX + 30, popupY + 18, 22, TEXT_LIGHT);

        // Close button
        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"ĐÓNG";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showEditRole = false;
            return;
        }

        // Hiển thị đội trưởng và đội phó hiện tại
        int infoY = popupY + 75;
        DrawTextEx2(u8"Đội hiện tại: " + team.getTenDoi(), popupX + 30, infoY, 18, TEXT_DARK);

        // Tìm đội trưởng và đội phó hiện tại
        string currentDT = u8"Chưa có";
        string currentDP = u8"Chưa có";
        for (const auto &p : team.getPlayers())
        {
            if (p.getVaiTro() == "DT")
                currentDT = p.getTen();
            if (p.getVaiTro() == "DP")
                currentDP = p.getTen();
        }

        infoY += 30;
        DrawRectangleRounded({(float)(popupX + 30), (float)infoY, 35, 22}, 0.3f, 8, CAPTAIN_COLOR);
        DrawTextEx2("DT", popupX + 38, infoY + 3, 14, TEXT_LIGHT);
        DrawTextEx2(u8"Đội trưởng: " + currentDT, popupX + 75, infoY + 2, 16, TEXT_DARK);

        infoY += 30;
        DrawRectangleRounded({(float)(popupX + 30), (float)infoY, 35, 22}, 0.3f, 8, VICE_CAPTAIN_COLOR);
        DrawTextEx2("DP", popupX + 38, infoY + 3, 14, TEXT_LIGHT);
        DrawTextEx2(u8"Đội phó: " + currentDP, popupX + 75, infoY + 2, 16, TEXT_DARK);

        // Danh sách cầu thủ để chọn
        infoY += 45;
        DrawTextEx2(u8"CHỌN CẦU THỦ ĐỂ ĐỔI VAI TRÒ:", popupX + 30, infoY, 16, ACCENT_1);

        int listY = infoY + 30;
        int listHeight = popupHeight - (listY - popupY) - 80;
        int listWidth = popupWidth - 60;

        // Scrollable list
        int contentHeight = team.getPlayers().size() * 50;

        // Handle scroll
        Rectangle listRect = {(float)(popupX + 30), (float)listY, (float)listWidth, (float)listHeight};
        if (CheckCollisionPointRec(GetMousePosition(), listRect))
        {
            float mouseWheel = GetMouseWheelMove();
            if (mouseWheel != 0)
            {
                roleScrollOffset -= (int)(mouseWheel * 30);
                if (roleScrollOffset < 0)
                    roleScrollOffset = 0;
                if (roleScrollOffset > contentHeight - listHeight)
                    roleScrollOffset = max(0, contentHeight - listHeight);
            }
        }

        BeginScissorMode(popupX + 30, listY, listWidth, listHeight);

        int y = listY - roleScrollOffset;
        int playerIdx = 0;
        for (auto &p : team.getPlayers())
        {
            Rectangle rowRect = {(float)(popupX + 30), (float)y, (float)listWidth, 45};
            bool isHovered = CheckCollisionPointRec(GetMousePosition(), rowRect) && y >= listY && y + 45 <= listY + listHeight;

            Color rowColor = isHovered ? (Color){240, 248, 255, 255} : CARD_BG;
            if (p.getVaiTro() == "DT")
                rowColor = (Color){255, 243, 224, 255};
            else if (p.getVaiTro() == "DP")
                rowColor = (Color){232, 245, 233, 255};

            DrawRectangleRounded(rowRect, 0.1f, 8, rowColor);
            DrawRectangleLinesEx(rowRect, 1, BORDER_LIGHT);

            // Tên cầu thủ
            DrawTextEx2(p.getTen(), popupX + 45, y + 12, 16, TEXT_DARK);

            // Vai trò hiện tại
            if (p.getVaiTro() == "DT")
            {
                DrawRectangleRounded({(float)(popupX + 300), (float)(y + 10), 35, 22}, 0.3f, 8, CAPTAIN_COLOR);
                DrawTextEx2("DT", popupX + 308, y + 13, 14, TEXT_LIGHT);
            }
            else if (p.getVaiTro() == "DP")
            {
                DrawRectangleRounded({(float)(popupX + 300), (float)(y + 10), 35, 22}, 0.3f, 8, VICE_CAPTAIN_COLOR);
                DrawTextEx2("DP", popupX + 308, y + 13, 14, TEXT_LIGHT);
            }

            // Buttons để đổi vai trò
            Button setDTBtn;
            setDTBtn.rect = {(float)(popupX + 480), (float)(y + 8), 50, 28};
            setDTBtn.text = u8"DT";
            setDTBtn.color = CAPTAIN_COLOR;
            setDTBtn.hoverColor = {211, 132, 13, 255};

            Button setDPBtn;
            setDPBtn.rect = {(float)(popupX + 540), (float)(y + 8), 50, 28};
            setDPBtn.text = u8"DP";
            setDPBtn.color = VICE_CAPTAIN_COLOR;
            setDPBtn.hoverColor = {39, 174, 96, 255};

            Button clearBtn;
            clearBtn.rect = {(float)(popupX + 600), (float)(y + 8), 50, 28};
            clearBtn.text = u8"Xóa";
            clearBtn.color = {149, 165, 166, 255};
            clearBtn.hoverColor = {127, 140, 141, 255};

            if (y >= listY - 50 && y <= listY + listHeight)
            {
                setDTBtn.draw();
                setDPBtn.draw();
                clearBtn.draw();

                if (setDTBtn.isClicked())
                {
                    // Xóa đội trưởng cũ
                    for (auto &oldP : team.getPlayers())
                    {
                        if (oldP.getVaiTro() == "DT")
                            oldP.setVaiTro("");
                    }
                    // Đặt đội trưởng mới
                    p.setVaiTro("DT");
                    saveToJson(*clb);
                    showMessage(u8"Đã đặt " + p.getTen() + u8" làm Đội trưởng!");
                }

                if (setDPBtn.isClicked())
                {
                    // Xóa đội phó cũ
                    for (auto &oldP : team.getPlayers())
                    {
                        if (oldP.getVaiTro() == "DP")
                            oldP.setVaiTro("");
                    }
                    // Đặt đội phó mới
                    p.setVaiTro("DP");
                    saveToJson(*clb);
                    showMessage(u8"Đã đặt " + p.getTen() + u8" làm Đội phó!");
                }

                if (clearBtn.isClicked())
                {
                    p.setVaiTro("");
                    saveToJson(*clb);
                    showMessage(u8"Đã xóa vai trò của " + p.getTen());
                }
            }

            y += 50;
            playerIdx++;
        }

        EndScissorMode();

        // Scrollbar
        if (contentHeight > listHeight)
        {
            int scrollbarX = popupX + popupWidth - 40;
            float scrollbarHeight = (float)listHeight * listHeight / contentHeight;
            float scrollbarY = listY + ((float)roleScrollOffset / (contentHeight - listHeight)) * (listHeight - scrollbarHeight);

            DrawRectangle(scrollbarX, listY, 8, listHeight, (Color){200, 200, 200, 255});
            DrawRectangleRounded({(float)scrollbarX, scrollbarY, 8, scrollbarHeight}, 0.5f, 10, ACCENT_1);
        }

        // Hướng dẫn
        DrawTextEx2(u8"Mỗi đội chỉ có 1 Đội trưởng và 1 Đội phó. Vai trò cũ sẽ tự động bị xóa.", popupX + 30, popupY + popupHeight - 35, 14, (Color){127, 140, 141, 255});
    }

    void drawDeleteConfirmPopup()
    {
        if (!showDeleteConfirm)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 450;
        int popupHeight = 200;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, ACCENT_2);
        DrawTextEx2(u8"XÁC NHẬN XÓA", popupX + 30, popupY + 18, 22, TEXT_LIGHT);

        // Thông báo xác nhận
        DrawTextEx2(u8"Bạn có chắc chắn muốn xóa cầu thủ:", popupX + 30, popupY + 80, 18, TEXT_DARK);
        DrawTextEx2(deletePlayerName + u8"?", popupX + 30, popupY + 105, 18, ACCENT_1);

        // Nút Hủy
        Button cancelBtn;
        cancelBtn.rect = {(float)(popupX + popupWidth / 2 - 170), (float)(popupY + 145), 150, 40};
        cancelBtn.text = u8"HỦY";
        cancelBtn.color = {149, 165, 166, 255};
        cancelBtn.hoverColor = {127, 140, 141, 255};
        cancelBtn.draw();

        // Nút Xóa
        Button confirmBtn;
        confirmBtn.rect = {(float)(popupX + popupWidth / 2 + 20), (float)(popupY + 145), 150, 40};
        confirmBtn.text = u8"XÓA";
        confirmBtn.color = ACCENT_2;
        confirmBtn.hoverColor = {192, 57, 43, 255};
        confirmBtn.draw();

        if (cancelBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showDeleteConfirm = false;
            deletePlayerID = "";
            deletePlayerName = "";
            deletePlayerTeam = nullptr;
        }

        if (confirmBtn.isClicked())
        {
            if (deletePlayerTeam != nullptr && !deletePlayerID.empty())
            {
                deletePlayerTeam->xoaCauThu(deletePlayerID);
                saveToJson(*clb);
                showMessage(u8"Đã xoá cầu thủ: " + deletePlayerName);
            }
            showDeleteConfirm = false;
            deletePlayerID = "";
            deletePlayerName = "";
            deletePlayerTeam = nullptr;
        }
    }

    void drawDeleteTeamConfirmPopup()
    {
        if (!showDeleteTeamConfirm)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 450;
        int popupHeight = 200;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, ACCENT_2);
        DrawTextEx2(u8"XÁC NHẬN XÓA ĐỘI", popupX + 30, popupY + 18, 22, TEXT_LIGHT);

        // Thông báo xác nhận
        DrawTextEx2(u8"Bạn có chắc chắn muốn xóa đội:", popupX + 30, popupY + 80, 18, TEXT_DARK);
        DrawTextEx2(deleteTeamName + u8"?", popupX + 30, popupY + 105, 18, ACCENT_1);

        // Nút Hủy
        Button cancelBtn;
        cancelBtn.rect = {(float)(popupX + popupWidth / 2 - 170), (float)(popupY + 145), 150, 40};
        cancelBtn.text = u8"HỦY";
        cancelBtn.color = {149, 165, 166, 255};
        cancelBtn.hoverColor = {127, 140, 141, 255};
        cancelBtn.draw();

        // Nút Xóa
        Button confirmBtn;
        confirmBtn.rect = {(float)(popupX + popupWidth / 2 + 20), (float)(popupY + 145), 150, 40};
        confirmBtn.text = u8"XÓA";
        confirmBtn.color = ACCENT_2;
        confirmBtn.hoverColor = {192, 57, 43, 255};
        confirmBtn.draw();

        if (cancelBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showDeleteTeamConfirm = false;
            deleteTeamIndex = -1;
            deleteTeamName = "";
        }

        if (confirmBtn.isClicked())
        {
            if (deleteTeamIndex >= 0 && deleteTeamIndex < (int)clb->getTeams().size())
            {
                clb->getTeams().erase(clb->getTeams().begin() + deleteTeamIndex);
                saveToJson(*clb);
                showMessage(u8"Đã xoá đội: " + deleteTeamName);
            }
            showDeleteTeamConfirm = false;
            deleteTeamIndex = -1;
            deleteTeamName = "";
        }
    }

    void drawStatsDetailPopup()
    {
        if (!showStatsDetail)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 700;
        int popupHeight = 550;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);

        // Tiêu đề và màu dựa trên loại thống kê
        string title;
        Color headerColor;
        string columnHeader;
        string yearSuffix = (statsSelectedYear == 0) ? u8" - TẤT CẢ" : (u8" - NĂM " + to_string(statsSelectedYear));

        if (statsDetailType == 0)
        {
            title = u8"THỐNG KÊ CẦU THỦ TỪNG ĐỘI";
            headerColor = ACCENT_1;
            columnHeader = u8"Số cầu thủ";
        }
        else if (statsDetailType == 1)
        {
            title = u8"BÀN THẮNG TỪNG ĐỘI" + yearSuffix;
            headerColor = ACCENT_2;
            columnHeader = u8"Bàn thắng";
        }
        else
        {
            title = u8"THỐNG KÊ LƯƠNG TỪNG ĐỘI";
            headerColor = (Color){46, 204, 113, 255};
            columnHeader = u8"Tổng lương";
        }

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, headerColor);
        DrawTextEx2(title, popupX + 30, popupY + 18, 22, TEXT_LIGHT);

        // Nút đóng
        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"ĐÓNG";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showStatsDetail = false;
            return;
        }

        // Tiêu đề cột
        int headerY = popupY + 75;
        DrawRectangle(popupX + 20, headerY, popupWidth - 40, 35, (Color){236, 240, 241, 255});
        DrawTextEx2(u8"STT", popupX + 35, headerY + 8, 16, TEXT_DARK);
        DrawTextEx2(u8"Tên đội", popupX + 90, headerY + 8, 16, TEXT_DARK);
        DrawTextEx2(columnHeader, popupX + popupWidth - 180, headerY + 8, 16, TEXT_DARK);

        // Danh sách đội
        int listY = headerY + 45;
        int listHeight = popupHeight - 150;
        int rowHeight = 45;

        MyVector<Team> &teams = clb->getTeams();
        int contentHeight = teams.size() * rowHeight;

        // Xử lý scroll
        Rectangle listRect = {(float)(popupX + 20), (float)listY, (float)(popupWidth - 40), (float)listHeight};
        if (CheckCollisionPointRec(GetMousePosition(), listRect))
        {
            float wheel = GetMouseWheelMove();
            if (wheel != 0)
            {
                statsDetailScrollOffset -= (int)(wheel * 30);
                statsDetailScrollOffset = max(0, min(statsDetailScrollOffset, max(0, contentHeight - listHeight)));
            }
        }

        BeginScissorMode(popupX + 20, listY, popupWidth - 40, listHeight);

        int y = listY - statsDetailScrollOffset;
        int total = 0;

        for (int i = 0; i < (int)teams.size(); i++)
        {
            Team &team = teams[i];

            if (y + rowHeight > listY && y < listY + listHeight)
            {
                // Nền xen kẽ
                Color rowBg = (i % 2 == 0) ? (Color){255, 255, 255, 255} : (Color){245, 248, 250, 255};
                DrawRectangle(popupX + 20, y, popupWidth - 40, rowHeight, rowBg);

                // STT
                DrawTextEx2(to_string(i + 1), popupX + 35, y + 12, 16, TEXT_DARK);

                // Tên đội
                DrawTextEx2(team.getTenDoi(), popupX + 90, y + 12, 16, TEXT_DARK);

                // Giá trị
                string value;
                int numValue = 0;
                if (statsDetailType == 0)
                {
                    numValue = team.getPlayers().size();
                    value = to_string(numValue);
                }
                else if (statsDetailType == 1)
                {
                    // Tính bàn thắng từ lịch sử trận đấu, lọc theo năm
                    numValue = 0;
                    for (const auto &p : team.getPlayers())
                    {
                        for (const auto &match : p.getLichSuTranDau())
                        {
                            int matchYear = 0;
                            if (match.ngayThiDau.length() >= 10)
                            {
                                try
                                {
                                    matchYear = stoi(match.ngayThiDau.substr(6, 4));
                                }
                                catch (...)
                                {
                                    matchYear = 0;
                                }
                            }
                            // Nếu statsSelectedYear == 0 (Tất cả) thì lấy hết, còn không thì lọc theo năm
                            if (statsSelectedYear == 0 || matchYear == statsSelectedYear)
                            {
                                numValue += match.banThang;
                            }
                        }
                    }
                    value = to_string(numValue);
                }
                else
                {
                    double salary = 0;
                    for (auto &p : team.getPlayers())
                    {
                        salary += p.getLuong();
                    }
                    numValue = (int)salary;
                    value = to_string((int)(salary / 1000)) + "M";
                }
                total += numValue;

                DrawTextEx2(value, popupX + popupWidth - 180, y + 12, 16, headerColor);
            }

            y += rowHeight;
        }

        EndScissorMode();

        // Tổng cộng ở cuối
        int footerY = popupY + popupHeight - 50;
        DrawRectangle(popupX + 20, footerY, popupWidth - 40, 35, headerColor);
        DrawTextEx2(u8"TỔNG CỘNG:", popupX + 90, footerY + 8, 16, TEXT_LIGHT);

        string totalStr;
        if (statsDetailType == 2)
        {
            totalStr = to_string(total / 1000) + "M";
        }
        else
        {
            totalStr = to_string(total);
        }
        DrawTextEx2(totalStr, popupX + popupWidth - 180, footerY + 8, 18, TEXT_LIGHT);

        // Scrollbar
        if (contentHeight > listHeight)
        {
            int scrollbarX = popupX + popupWidth - 35;
            float scrollbarHeight = (float)listHeight * listHeight / contentHeight;
            float scrollbarY = listY + ((float)statsDetailScrollOffset / (contentHeight - listHeight)) * (listHeight - scrollbarHeight);

            DrawRectangle(scrollbarX, listY, 8, listHeight, (Color){200, 200, 200, 255});
            DrawRectangleRounded({(float)scrollbarX, scrollbarY, 8, scrollbarHeight}, 0.5f, 10, headerColor);
        }
    }

    void drawMatchHistoryPopup()
    {
        if (!showMatchHistory || matchHistoryPlayer == nullptr)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 920;
        int popupHeight = 550;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, {241, 196, 15, 255});

        string title = u8"LỊCH SỬ TRẬN ĐẤU - " + matchHistoryPlayer->getTen();
        DrawTextEx2(title, popupX + 20, popupY + 16, 20, TEXT_LIGHT);

        // Nút đóng
        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"ĐÓNG";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        // Nút thêm trận đấu - chỉ hiển thị khi là Admin
        if (isAdmin)
        {
            Button addBtn;
            addBtn.rect = {(float)(popupX + popupWidth - 210), (float)(popupY + 10), 110, 40};
            addBtn.text = u8"THÊM";
            addBtn.color = {46, 204, 113, 255};
            addBtn.hoverColor = {39, 174, 96, 255};
            addBtn.draw();

            if (addBtn.isClicked())
            {
                showAddMatch = true;
                matchInputs.clear();
                matchInputs.push_back(InputField({0, 0, 200, 38}, u8"Ngày (dd/mm/yyyy):", 20));
                matchInputs.push_back(InputField({0, 0, 200, 38}, u8"Đối thủ:", 50));
                matchInputs.push_back(InputField({0, 0, 200, 38}, u8"Giải đấu:", 50));
                matchInputs.push_back(InputField({0, 0, 80, 38}, u8"Bàn thắng:", 5));
                matchInputs.push_back(InputField({0, 0, 80, 38}, u8"Kiến tạo:", 5));
                matchInputs.push_back(InputField({0, 0, 80, 38}, u8"Thẻ vàng:", 5));
                matchInputs.push_back(InputField({0, 0, 80, 38}, u8"Thẻ đỏ:", 5));
                matchInputs.push_back(InputField({0, 0, 300, 38}, u8"Ghi chú:", 100));
            }
        }

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showMatchHistory = false;
            matchHistoryPlayer = nullptr;
            return;
        }

        // Thông tin tổng hợp
        int infoY = popupY + 70;
        auto &lichSu = matchHistoryPlayer->getLichSuTranDau();

        string summary = u8"Tổng: " + to_string(lichSu.size()) + u8" trận  |  " +
                         to_string(matchHistoryPlayer->getTongBanThangTuLichSu()) + u8" bàn  |  " +
                         to_string(matchHistoryPlayer->getTongKienTaoTuLichSu()) + u8" kiến tạo  |  " +
                         to_string(matchHistoryPlayer->getTongTheVangTuLichSu()) + u8" thẻ vàng  |  " +
                         to_string(matchHistoryPlayer->getTongTheDoTuLichSu()) + u8" thẻ đỏ";
        DrawTextEx2(summary, popupX + 20, infoY, 18, TEXT_DARK);

        // Tiêu đề cột
        int headerY = infoY + 40;
        DrawRectangle(popupX + 15, headerY, popupWidth - 30, 35, (Color){52, 152, 219, 255});
        DrawTextEx2(u8"Ngày", popupX + 25, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"Đối thủ", popupX + 140, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"Giải đấu", popupX + 320, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"BT", popupX + 510, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"KT", popupX + 570, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"TV", popupX + 630, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"TĐ", popupX + 690, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"Ghi chú", popupX + 750, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"Xóa", popupX + 860, headerY + 8, 16, TEXT_LIGHT);

        // Danh sách trận đấu
        int listY = headerY + 40;
        int listHeight = popupHeight - (listY - popupY) - 20;
        int rowHeight = 45;
        int contentHeight = lichSu.size() * rowHeight;

        // Xử lý scroll
        Rectangle listRect = {(float)(popupX + 15), (float)listY, (float)(popupWidth - 30), (float)listHeight};
        if (CheckCollisionPointRec(GetMousePosition(), listRect))
        {
            float wheel = GetMouseWheelMove();
            if (wheel != 0)
            {
                matchHistoryScrollOffset -= (int)(wheel * 30);
                matchHistoryScrollOffset = max(0, min(matchHistoryScrollOffset, max(0, contentHeight - listHeight)));
            }
        }

        BeginScissorMode(popupX + 15, listY, popupWidth - 30, listHeight);

        int y = listY - matchHistoryScrollOffset;
        int deleteIndex = -1;

        for (int i = 0; i < (int)lichSu.size(); i++)
        {
            const auto &tran = lichSu[i];

            if (y + rowHeight > listY && y < listY + listHeight)
            {
                Color rowBg = (i % 2 == 0) ? (Color){255, 255, 255, 255} : (Color){245, 248, 250, 255};
                DrawRectangle(popupX + 15, y, popupWidth - 30, rowHeight, rowBg);

                DrawTextEx2(tran.ngayThiDau, popupX + 25, y + 12, 16, TEXT_DARK);
                DrawTextEx2(tran.doiDoi, popupX + 140, y + 12, 16, TEXT_DARK);
                DrawTextEx2(tran.giaiDau, popupX + 320, y + 12, 16, TEXT_DARK);
                DrawTextEx2(to_string(tran.banThang), popupX + 515, y + 12, 16, tran.banThang > 0 ? (Color){46, 204, 113, 255} : TEXT_DARK);
                DrawTextEx2(to_string(tran.kienTao), popupX + 575, y + 12, 16, tran.kienTao > 0 ? ACCENT_1 : TEXT_DARK);
                DrawTextEx2(to_string(tran.theVang), popupX + 635, y + 12, 16, tran.theVang > 0 ? CAPTAIN_COLOR : TEXT_DARK);
                DrawTextEx2(to_string(tran.theDo), popupX + 695, y + 12, 16, tran.theDo > 0 ? ACCENT_2 : TEXT_DARK);

                // Cắt ghi chú nếu quá dài
                string ghiChuShort = tran.ghiChu.length() > 12 ? tran.ghiChu.substr(0, 10) + "..." : tran.ghiChu;
                DrawTextEx2(ghiChuShort, popupX + 750, y + 12, 16, TEXT_DARK);

                // Nút xóa - chỉ hiển thị khi là Admin
                if (isAdmin)
                {
                    Rectangle delRect = {(float)(popupX + 850), (float)(y + 7), 55, 32};
                    bool delHover = CheckCollisionPointRec(GetMousePosition(), delRect);
                    DrawRectangleRounded(delRect, 0.3f, 10, delHover ? (Color){192, 57, 43, 255} : ACCENT_2);
                    DrawTextEx2(u8"Xóa", popupX + 862, y + 12, 16, TEXT_LIGHT);

                    if (delHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        deleteIndex = i;
                    }
                }
            }

            y += rowHeight;
        }

        EndScissorMode();

        // Xử lý xóa trận đấu - chỉ khi là Admin
        if (isAdmin && deleteIndex >= 0 && matchHistoryPlayer != nullptr)
        {
            auto &lichSuRef = matchHistoryPlayer->getLichSuTranDau();
            lichSuRef.erase(lichSuRef.begin() + deleteIndex);
            saveToJson(*clb);
            showMessage(u8"Đã xóa trận đấu!");
        }

        // Scrollbar
        if (contentHeight > listHeight)
        {
            int scrollbarX = popupX + popupWidth - 25;
            float scrollbarHeight = (float)listHeight * listHeight / contentHeight;
            float scrollbarY = listY + ((float)matchHistoryScrollOffset / (contentHeight - listHeight)) * (listHeight - scrollbarHeight);

            DrawRectangle(scrollbarX, listY, 8, listHeight, (Color){200, 200, 200, 255});
            DrawRectangleRounded({(float)scrollbarX, scrollbarY, 8, scrollbarHeight}, 0.5f, 10, ACCENT_1);
        }

        // Nếu chưa có trận nào
        if (lichSu.empty())
        {
            DrawTextEx2(u8"Chưa có lịch sử trận đấu. Nhấn 'THÊM' để thêm trận mới.", popupX + 200, listY + 100, 18, (Color){127, 140, 141, 255});
        }
    }

    // ========== POPUP THÀNH TÍCH CÙNG ĐỘI ==========
    void drawHonorPopup()
    {
        if (!showHonorPopup || honorPopupPlayer == nullptr)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 750;
        int popupHeight = 550;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 60}, 0.05f, 10, CAPTAIN_COLOR);

        string title = u8"THÀNH TÍCH CÙNG ĐỘI - " + honorPopupPlayer->getTen();
        DrawTextEx2(title, popupX + 20, popupY + 16, 22, TEXT_LIGHT);

        // Nút đóng
        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 80, 40};
        closeBtn.text = u8"ĐÓNG";
        closeBtn.color = {149, 165, 166, 255};
        closeBtn.hoverColor = {127, 140, 141, 255};
        closeBtn.draw();

        // Nút thêm thành tích - chỉ hiển thị khi là Admin
        static bool showAddAward = false;
        static InputField awardNameInput({0, 0, 400, 38}, "", 100);
        static InputField awardYearInput({0, 0, 150, 38}, "", 4);
        static int selectedRank = 1; // 1=Vô địch, 2=Á quân, 3=Hạng ba

        if (isAdmin)
        {
            Button addBtn;
            addBtn.rect = {(float)(popupX + popupWidth - 200), (float)(popupY + 10), 100, 40};
            addBtn.text = u8"THÊM";
            addBtn.color = {46, 204, 113, 255};
            addBtn.hoverColor = {39, 174, 96, 255};
            addBtn.draw();

            if (addBtn.isClicked())
            {
                showAddAward = true;
                awardNameInput.text = "";
                awardYearInput.text = "2025";
                selectedRank = 1;
            }
        }

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showHonorPopup = false;
            honorPopupPlayer = nullptr;
            return;
        }

        // Lấy danh sách thành tích cùng đội và sắp xếp
        auto &achievements = honorPopupPlayer->getThanhTichCungDoi();
        MyVector<TeamAchievement> sortedAwards;
        for (const auto &award : achievements)
        {
            sortedAwards.push_back(award);
        }

        // Sắp xếp theo năm giảm dần, rồi theo xếp hạng tăng dần
        for (size_t i = 0; i < sortedAwards.size(); i++)
        {
            for (size_t j = i + 1; j < sortedAwards.size(); j++)
            {
                bool swap = false;
                if (sortedAwards[i].nam < sortedAwards[j].nam)
                    swap = true;
                else if (sortedAwards[i].nam == sortedAwards[j].nam && sortedAwards[i].xepHang > sortedAwards[j].xepHang)
                    swap = true;

                if (swap)
                {
                    TeamAchievement temp = sortedAwards[i];
                    sortedAwards[i] = sortedAwards[j];
                    sortedAwards[j] = temp;
                }
            }
        }

        int totalAwards = (int)sortedAwards.size();
        DrawTextEx2(u8"Tổng số thành tích cùng đội: " + to_string(totalAwards), popupX + 20, popupY + 75, 18, TEXT_DARK);

        // Header bảng
        int listY = popupY + 110;
        int listHeight = popupHeight - 130;

        DrawRectangle(popupX + 15, listY, popupWidth - 30, listHeight, (Color){255, 255, 255, 255});
        DrawRectangleLinesEx({(float)(popupX + 15), (float)listY, (float)(popupWidth - 30), (float)listHeight}, 1, BORDER_LIGHT);

        // Header
        int headerY = listY + 5;
        DrawRectangle(popupX + 20, headerY, popupWidth - 40, 35, (Color){52, 152, 219, 255});
        DrawTextEx2(u8"HẠNG", popupX + 35, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"TÊN GIẢI ĐẤU", popupX + 120, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"NĂM", popupX + 450, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"", popupX + 560, headerY + 8, 16, TEXT_LIGHT);

        int rowY = headerY + 45;
        int deleteIndex = -1;

        for (int i = 0; i < (int)sortedAwards.size() && rowY < listY + listHeight - 50; i++)
        {
            const TeamAchievement &award = sortedAwards[i];

            Color rowBg = (i % 2 == 0) ? (Color){255, 250, 230, 255} : (Color){255, 255, 255, 255};
            DrawRectangle(popupX + 20, rowY, popupWidth - 100, 45, rowBg);

            // Badge theo xếp hạng
            string rankText;
            Color rankColor;
            if (award.xepHang == 1)
            {
                rankText = u8"VÔ ĐỊCH";
                rankColor = {255, 193, 7, 255};
            }
            else if (award.xepHang == 2)
            {
                rankText = u8"Á QUÂN";
                rankColor = {158, 158, 158, 255};
            }
            else
            {
                rankText = u8"HẠNG BA";
                rankColor = {205, 127, 50, 255};
            }

            DrawRectangleRounded({(float)(popupX + 30), (float)(rowY + 10), 75, 25}, 0.3f, 6, rankColor);
            int rw = MeasureTextEx2(rankText, 11);
            DrawTextEx2(rankText, popupX + 30 + (75 - rw) / 2, rowY + 14, 11, TEXT_LIGHT);

            DrawTextEx2(award.tenGiai, popupX + 120, rowY + 12, 17, TEXT_DARK);
            DrawTextEx2(to_string(award.nam), popupX + 460, rowY + 12, 17, TEXT_DARK);

            // Nút xóa - chỉ hiển thị khi là Admin
            if (isAdmin)
            {
                Rectangle delRect = {(float)(popupX + popupWidth - 85), (float)(rowY + 7), 60, 30};
                bool delHover = CheckCollisionPointRec(GetMousePosition(), delRect);
                DrawRectangleRounded(delRect, 0.3f, 10, delHover ? (Color){192, 57, 43, 255} : ACCENT_2);
                DrawTextEx2(u8"Xóa", popupX + popupWidth - 72, rowY + 11, 14, TEXT_LIGHT);

                if (delHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    // Tìm index gốc trong vector chưa sắp xếp
                    for (int j = 0; j < (int)achievements.size(); j++)
                    {
                        if (achievements[j].tenGiai == award.tenGiai && achievements[j].nam == award.nam && achievements[j].xepHang == award.xepHang)
                        {
                            deleteIndex = j;
                            break;
                        }
                    }
                }
            }

            rowY += 50;
        }

        // Xử lý xóa - chỉ khi là Admin
        if (isAdmin && deleteIndex >= 0)
        {
            honorPopupPlayer->removeThanhTichDoi(deleteIndex);
            saveToJson(*clb);
            showMessage(u8"Đã xóa thành tích!");
        }

        if (sortedAwards.empty())
        {
            DrawTextEx2(u8"Chưa có thành tích cùng đội.", popupX + 240, listY + 100, 18, (Color){127, 140, 141, 255});
            DrawTextEx2(u8"Nhấn 'THÊM' để thêm thành tích mới.", popupX + 210, listY + 130, 16, (Color){127, 140, 141, 255});
        }

        // Mini popup thêm thành tích
        if (showAddAward)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});

            int awPopupW = 550;
            int awPopupH = 340;
            int awPopupX = (screenWidth - awPopupW) / 2;
            int awPopupY = (screenHeight - awPopupH) / 2;

            DrawRectangleRounded({(float)awPopupX, (float)awPopupY, (float)awPopupW, (float)awPopupH}, 0.05f, 10, CARD_BG);
            DrawRectangleRounded({(float)awPopupX, (float)awPopupY, (float)awPopupW, 50}, 0.05f, 10, CAPTAIN_COLOR);
            DrawTextEx2(u8"THÊM THÀNH TÍCH CÙNG ĐỘI", awPopupX + 20, awPopupY + 14, 20, TEXT_LIGHT);

            // Tên giải đấu
            DrawTextEx2(u8"Tên giải đấu:", awPopupX + 30, awPopupY + 70, 16, TEXT_DARK);
            awardNameInput.rect = {(float)(awPopupX + 30), (float)(awPopupY + 95), 490, 38};
            awardNameInput.update();
            awardNameInput.draw();

            // Năm
            DrawTextEx2(u8"Năm:", awPopupX + 30, awPopupY + 150, 16, TEXT_DARK);
            awardYearInput.rect = {(float)(awPopupX + 30), (float)(awPopupY + 175), 120, 38};
            awardYearInput.update();
            awardYearInput.draw();

            // Xếp hạng (Vô địch, Á quân, Hạng ba)
            DrawTextEx2(u8"Xếp hạng:", awPopupX + 200, awPopupY + 150, 16, TEXT_DARK);

            Rectangle rank1 = {(float)(awPopupX + 200), (float)(awPopupY + 175), 100, 38};
            Rectangle rank2 = {(float)(awPopupX + 310), (float)(awPopupY + 175), 100, 38};
            Rectangle rank3 = {(float)(awPopupX + 420), (float)(awPopupY + 175), 100, 38};

            bool hover1 = CheckCollisionPointRec(GetMousePosition(), rank1);
            bool hover2 = CheckCollisionPointRec(GetMousePosition(), rank2);
            bool hover3 = CheckCollisionPointRec(GetMousePosition(), rank3);

            Color gold = {255, 193, 7, 255};
            Color silver = {158, 158, 158, 255};
            Color bronze = {205, 127, 50, 255};

            DrawRectangleRounded(rank1, 0.2f, 10, selectedRank == 1 ? gold : (hover1 ? (Color){255, 235, 150, 255} : (Color){245, 245, 220, 255}));
            DrawRectangleLinesEx(rank1, 2, selectedRank == 1 ? (Color){218, 165, 32, 255} : (Color){200, 200, 180, 255});
            DrawTextEx2(u8"Vô địch", awPopupX + 220, awPopupY + 184, 16, TEXT_DARK);

            DrawRectangleRounded(rank2, 0.2f, 10, selectedRank == 2 ? silver : (hover2 ? (Color){220, 220, 220, 255} : (Color){245, 245, 245, 255}));
            DrawRectangleLinesEx(rank2, 2, selectedRank == 2 ? (Color){128, 128, 128, 255} : (Color){200, 200, 200, 255});
            DrawTextEx2(u8"Á quân", awPopupX + 330, awPopupY + 184, 16, TEXT_DARK);

            DrawRectangleRounded(rank3, 0.2f, 10, selectedRank == 3 ? bronze : (hover3 ? (Color){220, 180, 140, 255} : (Color){245, 235, 225, 255}));
            DrawRectangleLinesEx(rank3, 2, selectedRank == 3 ? (Color){160, 82, 45, 255} : (Color){200, 180, 160, 255});
            DrawTextEx2(u8"Hạng ba", awPopupX + 438, awPopupY + 184, 16, TEXT_DARK);

            if (hover1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                selectedRank = 1;
            if (hover2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                selectedRank = 2;
            if (hover3 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                selectedRank = 3;

            // Nút Lưu và Hủy
            Button saveAwBtn;
            saveAwBtn.rect = {(float)(awPopupX + awPopupW - 180), (float)(awPopupY + awPopupH - 50), 70, 35};
            saveAwBtn.text = u8"LƯU";
            saveAwBtn.color = {46, 204, 113, 255};
            saveAwBtn.hoverColor = {39, 174, 96, 255};
            saveAwBtn.draw();

            Button cancelAwBtn;
            cancelAwBtn.rect = {(float)(awPopupX + awPopupW - 100), (float)(awPopupY + awPopupH - 50), 70, 35};
            cancelAwBtn.text = u8"HỦY";
            cancelAwBtn.color = {149, 165, 166, 255};
            cancelAwBtn.hoverColor = {127, 140, 141, 255};
            cancelAwBtn.draw();

            if (saveAwBtn.isClicked() && !awardNameInput.text.empty() && !awardYearInput.text.empty())
            {
                int year = 0;
                try
                {
                    year = stoi(awardYearInput.text);
                }
                catch (...)
                {
                    year = 2025;
                }

                // Kiểm tra trùng lặp trong thành tích cùng đội của cầu thủ này
                bool isDuplicate = false;
                for (const auto &existingTT : honorPopupPlayer->getThanhTichCungDoi())
                {
                    if (existingTT.tenGiai == awardNameInput.text &&
                        existingTT.nam == year &&
                        existingTT.xepHang == selectedRank)
                    {
                        isDuplicate = true;
                        break;
                    }
                }

                if (isDuplicate)
                {
                    showMessage(u8"Thành tích này đã được ghi nhận!");
                }
                else
                {
                    TeamAchievement newTT(awardNameInput.text, year, selectedRank);
                    honorPopupPlayer->addThanhTichDoi(newTT);
                    saveToJson(*clb);
                    showMessage(u8"Đã thêm thành tích cùng đội!");
                    showAddAward = false;
                }
            }

            if (cancelAwBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
            {
                showAddAward = false;
            }
        }
    }

    // ========== POPUP GIẢI THƯỞNG CÁ NHÂN (Quả bóng vàng, Vua phá lưới...) ==========
    void drawIndividualAwardPopup()
    {
        if (!showIndividualAwardPopup || individualAwardPlayer == nullptr)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 700;
        int popupHeight = 550;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.03f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 55}, 0.03f, 10, (Color){155, 89, 182, 255}); // Tím

        string title = u8"GIẢI THƯỞNG CÁ NHÂN - " + individualAwardPlayer->getTen();
        DrawTextEx2(title, popupX + 20, popupY + 16, 20, TEXT_LIGHT);

        // Nút đóng
        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 90), (float)(popupY + 10), 70, 35};
        closeBtn.text = u8"ĐÓNG";
        closeBtn.color = ACCENT_2;
        closeBtn.hoverColor = {192, 57, 43, 255};
        closeBtn.draw();

        // Nút thêm giải thưởng - chỉ hiển thị khi là Admin
        static bool showAddIndivAward = false;

        if (isAdmin)
        {
            Button addBtn;
            addBtn.rect = {(float)(popupX + popupWidth - 180), (float)(popupY + 10), 80, 35};
            addBtn.text = u8"THÊM";
            addBtn.color = {46, 204, 113, 255};
            addBtn.hoverColor = {39, 174, 96, 255};
            addBtn.draw();

            if (addBtn.isClicked())
            {
                showAddIndivAward = true;
            }
        }

        if (closeBtn.isClicked() || (!showAddIndivAward && IsKeyPressed(KEY_ESCAPE)))
        {
            showIndividualAwardPopup = false;
            individualAwardPlayer = nullptr;
            return;
        }

        // Lấy danh sách giải thưởng
        auto &awards = individualAwardPlayer->getDanhSachGiaiThuong();

        // Sắp xếp theo năm giảm dần
        MyVector<Award> sortedAwards;
        for (const auto &aw : awards)
            sortedAwards.push_back(aw);
        for (size_t i = 0; i < sortedAwards.size(); i++)
        {
            for (size_t j = i + 1; j < sortedAwards.size(); j++)
            {
                if (sortedAwards[i].nam < sortedAwards[j].nam ||
                    (sortedAwards[i].nam == sortedAwards[j].nam && sortedAwards[i].xepHang > sortedAwards[j].xepHang))
                {
                    Award temp = sortedAwards[i];
                    sortedAwards[i] = sortedAwards[j];
                    sortedAwards[j] = temp;
                }
            }
        }

        int totalAwards = (int)sortedAwards.size();
        DrawTextEx2(u8"Tổng số giải thưởng: " + to_string(totalAwards), popupX + 20, popupY + 75, 18, TEXT_DARK);

        // Header bảng
        int listY = popupY + 110;
        int listHeight = popupHeight - 130;

        DrawRectangle(popupX + 15, listY, popupWidth - 30, listHeight, (Color){255, 255, 255, 255});
        DrawRectangleLinesEx({(float)(popupX + 15), (float)listY, (float)(popupWidth - 30), (float)listHeight}, 1, BORDER_LIGHT);

        // Header
        int headerY = listY + 5;
        DrawRectangle(popupX + 20, headerY, popupWidth - 40, 35, (Color){155, 89, 182, 255});
        DrawTextEx2(u8"TÊN GIẢI THƯỞNG", popupX + 35, headerY + 8, 16, TEXT_LIGHT);
        DrawTextEx2(u8"NĂM", popupX + 520, headerY + 8, 16, TEXT_LIGHT);

        int rowY = headerY + 45;
        int deleteIndex = -1;

        for (int i = 0; i < (int)sortedAwards.size() && rowY < listY + listHeight - 50; i++)
        {
            const Award &award = sortedAwards[i];

            Color rowBg = (i % 2 == 0) ? (Color){248, 240, 255, 255} : (Color){255, 255, 255, 255};
            DrawRectangle(popupX + 20, rowY, popupWidth - 100, 45, rowBg);

            // Hiển thị tên giải thưởng và năm
            DrawTextEx2(award.tenGiai, popupX + 35, rowY + 12, 17, TEXT_DARK);
            DrawTextEx2(to_string(award.nam), popupX + 530, rowY + 12, 17, ACCENT_1);

            // Nút xóa - chỉ hiển thị khi là Admin
            if (isAdmin)
            {
                Rectangle delRect = {(float)(popupX + popupWidth - 85), (float)(rowY + 7), 60, 30};
                bool delHover = CheckCollisionPointRec(GetMousePosition(), delRect);
                DrawRectangleRounded(delRect, 0.3f, 10, delHover ? (Color){192, 57, 43, 255} : ACCENT_2);
                DrawTextEx2(u8"Xóa", popupX + popupWidth - 72, rowY + 11, 14, TEXT_LIGHT);

                if (delHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    // Tìm index gốc trong vector chưa sắp xếp
                    for (int j = 0; j < (int)awards.size(); j++)
                    {
                        if (awards[j].tenGiai == award.tenGiai && awards[j].nam == award.nam && awards[j].xepHang == award.xepHang)
                        {
                            deleteIndex = j;
                            break;
                        }
                    }
                }
            }

            rowY += 50;
        }

        // Xử lý xóa - chỉ khi là Admin
        if (isAdmin && deleteIndex >= 0)
        {
            individualAwardPlayer->removeGiaiThuong(deleteIndex);
            saveToJson(*clb);
            showMessage(u8"Đã xóa giải thưởng!");
        }

        if (sortedAwards.empty())
        {
            DrawTextEx2(u8"Chưa có giải thưởng cá nhân.", popupX + 200, listY + 100, 18, (Color){127, 140, 141, 255});
            DrawTextEx2(u8"Nhấn 'THÊM' để thêm giải thưởng mới.", popupX + 170, listY + 130, 16, (Color){127, 140, 141, 255});
        }

        // Mini popup thêm giải thưởng
        if (showAddIndivAward)
        {
            static InputField indivAwardNameInput;
            static InputField indivAwardYearInput;

            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});

            int awPopupW = 450;
            int awPopupH = 280;
            int awPopupX = (screenWidth - awPopupW) / 2;
            int awPopupY = (screenHeight - awPopupH) / 2;

            DrawRectangleRounded({(float)awPopupX, (float)awPopupY, (float)awPopupW, (float)awPopupH}, 0.05f, 10, CARD_BG);
            DrawRectangleRounded({(float)awPopupX, (float)awPopupY, (float)awPopupW, 50}, 0.05f, 10, (Color){155, 89, 182, 255});
            DrawTextEx2(u8"THÊM GIẢI THƯỞNG CÁ NHÂN", awPopupX + 20, awPopupY + 14, 20, TEXT_LIGHT);

            int inputY = awPopupY + 65;

            // Tên giải thưởng
            DrawTextEx2(u8"Tên giải thưởng:", awPopupX + 30, inputY, 16, TEXT_DARK);
            indivAwardNameInput.rect = {(float)(awPopupX + 30), (float)(inputY + 22), (float)(awPopupW - 60), 38};
            indivAwardNameInput.update();
            indivAwardNameInput.draw();

            // Gợi ý tên giải
            inputY += 75;
            DrawTextEx2(u8"Gợi ý: Quả bóng vàng, Vua phá lưới, Cầu thủ xuất sắc nhất, ...", awPopupX + 30, inputY, 12, (Color){150, 150, 150, 255});

            // Năm
            inputY += 30;
            DrawTextEx2(u8"Năm:", awPopupX + 30, inputY, 16, TEXT_DARK);
            indivAwardYearInput.rect = {(float)(awPopupX + 30), (float)(inputY + 22), 120, 38};
            indivAwardYearInput.update();
            indivAwardYearInput.draw();

            // Nút Hủy và Lưu
            int btnY = awPopupY + awPopupH - 60;

            Button cancelIndivBtn;
            cancelIndivBtn.rect = {(float)(awPopupX + 30), (float)btnY, 180, 42};
            cancelIndivBtn.text = u8"HỦY";
            cancelIndivBtn.color = {149, 165, 166, 255};
            cancelIndivBtn.hoverColor = {127, 140, 141, 255};
            cancelIndivBtn.draw();

            Button saveIndivBtn;
            saveIndivBtn.rect = {(float)(awPopupX + awPopupW - 210), (float)btnY, 180, 42};
            saveIndivBtn.text = u8"LƯU";
            saveIndivBtn.color = {46, 204, 113, 255};
            saveIndivBtn.hoverColor = {39, 174, 96, 255};
            saveIndivBtn.draw();

            if (saveIndivBtn.isClicked() && !indivAwardNameInput.text.empty() && !indivAwardYearInput.text.empty())
            {
                int year = 0;
                try
                {
                    year = stoi(indivAwardYearInput.text);
                }
                catch (...)
                {
                    year = 2024;
                }

                // Kiểm tra trùng lặp (chỉ theo tên giải và năm)
                bool isDuplicate = false;
                for (const auto &existingAw : individualAwardPlayer->getDanhSachGiaiThuong())
                {
                    if (existingAw.tenGiai == indivAwardNameInput.text && existingAw.nam == year)
                    {
                        isDuplicate = true;
                        break;
                    }
                }

                if (isDuplicate)
                {
                    showMessage(u8"Giải thưởng này đã được ghi nhận!");
                }
                else
                {
                    Award newAward(indivAwardNameInput.text, year, 1); // mặc định xếp hạng = 1
                    individualAwardPlayer->addGiaiThuong(newAward);
                    saveToJson(*clb);
                    showMessage(u8"Đã thêm giải thưởng cá nhân!");
                    showAddIndivAward = false;
                    // Reset input
                    indivAwardNameInput.text = "";
                    indivAwardYearInput.text = "2024";
                }
            }

            if (cancelIndivBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
            {
                showAddIndivAward = false;
            }
        }
    }

    // ========== POPUP DANH HIỆU TẬP THỂ ==========
    void drawTeamAwardPopup()
    {
        if (!showTeamAwardPopup)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});

        int popupWidth = 900;
        int popupHeight = 600;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.03f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 50}, 0.03f, 10, (Color){155, 89, 182, 255}); // Tím

        DrawTextEx2(u8"DANH HIỆU TẬP THỂ CÁC ĐỘI", popupX + 30, popupY + 13, 22, TEXT_LIGHT);

        // Nút đóng
        Button closeBtn;
        closeBtn.rect = {(float)(popupX + popupWidth - 100), (float)(popupY + 8), 80, 34};
        closeBtn.text = u8"ĐÓNG";
        closeBtn.color = ACCENT_2;
        closeBtn.hoverColor = {192, 57, 43, 255};
        closeBtn.draw();

        if (closeBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showTeamAwardPopup = false;
            return;
        }

        // Hiển thị danh hiệu tập thể theo từng đội
        int contentY = popupY + 70;
        int contentHeight = popupHeight - 90;

        // Tính tổng chiều cao content
        int totalContentHeight = 0;
        for (auto &team : clb->getTeams())
        {
            totalContentHeight += 50;                                                     // Header của đội
            totalContentHeight += max(1, (int)team.getDanhHieuTapThe().size()) * 45 + 60; // Danh hiệu + nút thêm
        }

        // Xử lý scroll
        float wheel = GetMouseWheelMove();
        if (CheckCollisionPointRec(GetMousePosition(), {(float)popupX, (float)contentY, (float)popupWidth, (float)contentHeight}))
        {
            teamAwardScrollOffset -= (int)(wheel * 30);
            teamAwardScrollOffset = max(0, min(teamAwardScrollOffset, max(0, totalContentHeight - contentHeight)));
        }

        BeginScissorMode(popupX + 10, contentY, popupWidth - 20, contentHeight);

        int yPos = contentY - teamAwardScrollOffset;

        static bool showAddTeamAward = false;
        static Team *addAwardTeam = nullptr;
        static InputField teamAwardNameInput;
        static InputField teamAwardYearInput;
        static int selectedTeamRank = 1;

        for (auto &team : clb->getTeams())
        {
            // Header của đội
            DrawRectangleRounded({(float)(popupX + 20), (float)yPos, (float)(popupWidth - 40), 40}, 0.1f, 8, (Color){155, 89, 182, 255});
            DrawTextEx2(team.getTenDoi(), popupX + 35, yPos + 10, 18, TEXT_LIGHT);

            // Nút thêm danh hiệu cho đội - chỉ hiển thị khi là Admin
            if (isAdmin)
            {
                Rectangle addBtnRect = {(float)(popupX + popupWidth - 140), (float)(yPos + 5), 100, 30};
                bool isAddHover = CheckCollisionPointRec(GetMousePosition(), addBtnRect);
                DrawRectangleRounded(addBtnRect, 0.3f, 8, isAddHover ? Color{46, 204, 113, 255} : Color{39, 174, 96, 255});
                int addTextW = MeasureTextEx2(u8"THÊM", 14);
                DrawTextEx2(u8"THÊM", popupX + popupWidth - 140 + (100 - addTextW) / 2, yPos + 12, 14, TEXT_LIGHT);

                if (isAddHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    showAddTeamAward = true;
                    addAwardTeam = &team;
                    teamAwardNameInput = InputField();
                    teamAwardNameInput.text = "";
                    teamAwardYearInput = InputField();
                    teamAwardYearInput.text = "2024";
                    selectedTeamRank = 1;
                }
            }

            yPos += 50;

            // Danh sách danh hiệu
            auto &awards = team.getDanhHieuTapThe();
            if (awards.empty())
            {
                DrawTextEx2(u8"Chưa có danh hiệu tập thể", popupX + 40, yPos + 10, 14, (Color){150, 150, 150, 255});
                yPos += 45;
            }
            else
            {
                // Sắp xếp theo năm giảm dần
                MyVector<int> indices;
                for (int i = 0; i < (int)awards.size(); i++)
                    indices.push_back(i);
                for (size_t i = 0; i < indices.size(); i++)
                {
                    for (size_t j = i + 1; j < indices.size(); j++)
                    {
                        if (awards[indices[i]].nam < awards[indices[j]].nam)
                        {
                            int tmp = indices[i];
                            indices[i] = indices[j];
                            indices[j] = tmp;
                        }
                    }
                }

                for (int idx : indices)
                {
                    const auto &aw = awards[idx];
                    int rowY = yPos;

                    // Nền hàng
                    DrawRectangle(popupX + 30, rowY, popupWidth - 60, 40, (Color){248, 249, 250, 255});

                    // Hạng
                    string rankText;
                    Color rankColor;
                    if (aw.xepHang == 1)
                    {
                        rankText = u8"VÔ ĐỊCH";
                        rankColor = {255, 193, 7, 255};
                    }
                    else if (aw.xepHang == 2)
                    {
                        rankText = u8"Á QUÂN";
                        rankColor = {158, 158, 158, 255};
                    }
                    else
                    {
                        rankText = u8"HẠNG BA";
                        rankColor = {205, 127, 50, 255};
                    }

                    DrawRectangleRounded({(float)(popupX + 40), (float)(rowY + 8), 80, 24}, 0.3f, 6, rankColor);
                    int rankW = MeasureTextEx2(rankText, 12);
                    DrawTextEx2(rankText, popupX + 40 + (80 - rankW) / 2, rowY + 12, 12, TEXT_LIGHT);

                    // Tên giải
                    DrawTextEx2(aw.tenGiai, popupX + 140, rowY + 12, 16, TEXT_DARK);

                    // Năm
                    DrawTextEx2(to_string(aw.nam), popupX + popupWidth - 180, rowY + 12, 16, ACCENT_1);

                    // Nút xóa - chỉ hiển thị khi là Admin
                    if (isAdmin)
                    {
                        Rectangle delRect = {(float)(popupX + popupWidth - 100), (float)(rowY + 8), 50, 24};
                        bool isDelHover = CheckCollisionPointRec(GetMousePosition(), delRect);
                        DrawRectangleRounded(delRect, 0.3f, 6, isDelHover ? ACCENT_2 : Color{200, 200, 200, 255});
                        int xW = MeasureTextEx2("X", 14);
                        DrawTextEx2("X", popupX + popupWidth - 100 + (50 - xW) / 2, rowY + 11, 14, isDelHover ? TEXT_LIGHT : TEXT_DARK);

                        if (isDelHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            team.removeDanhHieuTapThe(idx);
                            saveToJson(*clb);
                            showMessage(u8"Đã xóa danh hiệu tập thể!");
                        }
                    }

                    yPos += 45;
                }
            }

            yPos += 15; // Khoảng cách giữa các đội
        }

        EndScissorMode();

        // Scrollbar
        if (totalContentHeight > contentHeight)
        {
            int scrollbarX = popupX + popupWidth - 15;
            float scrollbarHeight = (float)contentHeight * contentHeight / totalContentHeight;
            float scrollbarY = contentY + ((float)teamAwardScrollOffset / (totalContentHeight - contentHeight)) * (contentHeight - scrollbarHeight);

            DrawRectangleRounded({(float)scrollbarX, (float)contentY, 8, (float)contentHeight}, 0.5f, 8, (Color){200, 200, 200, 100});
            DrawRectangleRounded({(float)scrollbarX, scrollbarY, 8, scrollbarHeight}, 0.5f, 8, (Color){155, 89, 182, 255});
        }

        // Popup thêm danh hiệu tập thể
        if (showAddTeamAward && addAwardTeam != nullptr)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});

            int addPopupW = 450;
            int addPopupH = 380;
            int addPopupX = (screenWidth - addPopupW) / 2;
            int addPopupY = (screenHeight - addPopupH) / 2;

            DrawRectangleRounded({(float)addPopupX, (float)addPopupY, (float)addPopupW, (float)addPopupH}, 0.05f, 10, CARD_BG);
            DrawRectangleRounded({(float)addPopupX, (float)addPopupY, (float)addPopupW, 50}, 0.05f, 10, (Color){46, 204, 113, 255});
            DrawTextEx2((u8"THÊM DANH HIỆU - " + addAwardTeam->getTenDoi()), addPopupX + 20, addPopupY + 13, 18, TEXT_LIGHT);

            int inputY = addPopupY + 65;

            // Input tên giải
            DrawTextEx2(u8"Tên giải đấu:", addPopupX + 20, inputY, 16, TEXT_DARK);
            teamAwardNameInput.rect = {(float)(addPopupX + 20), (float)(inputY + 22), (float)(addPopupW - 40), 35};
            teamAwardNameInput.update();
            teamAwardNameInput.draw();
            inputY += 70;

            // Input năm
            DrawTextEx2(u8"Năm:", addPopupX + 20, inputY, 16, TEXT_DARK);
            teamAwardYearInput.rect = {(float)(addPopupX + 20), (float)(inputY + 22), 120, 35};
            teamAwardYearInput.update();
            teamAwardYearInput.draw();
            inputY += 70;

            // Chọn xếp hạng
            DrawTextEx2(u8"Xếp hạng:", addPopupX + 20, inputY, 16, TEXT_DARK);
            inputY += 28;

            // 3 nút: Vô địch, Á quân, Hạng ba
            struct RankOption
            {
                int rank;
                const char *label;
                Color color;
            };
            RankOption rankOptions[] = {
                {1, u8"Vô địch", {255, 193, 7, 255}},
                {2, u8"Á quân", {158, 158, 158, 255}},
                {3, u8"Hạng ba", {205, 127, 50, 255}}};

            int btnX = addPopupX + 20;
            for (int i = 0; i < 3; i++)
            {
                Rectangle btnRect = {(float)btnX, (float)inputY, 125, 35};
                bool isSel = (selectedTeamRank == rankOptions[i].rank);
                bool isHov = CheckCollisionPointRec(GetMousePosition(), btnRect);

                Color bgCol = isSel ? rankOptions[i].color : (isHov ? (Color){230, 230, 230, 255} : (Color){245, 245, 245, 255});
                DrawRectangleRounded(btnRect, 0.2f, 8, bgCol);
                DrawRectangleLinesEx(btnRect, 2, rankOptions[i].color);

                int lblW = MeasureTextEx2(rankOptions[i].label, 14);
                DrawTextEx2(rankOptions[i].label, btnX + (125 - lblW) / 2, inputY + 10, 14, isSel ? TEXT_LIGHT : TEXT_DARK);

                if (isHov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    selectedTeamRank = rankOptions[i].rank;
                }

                btnX += 135;
            }

            // Nút Hủy và Lưu - đặt cách nút xếp hạng 60px
            int btnY = addPopupY + addPopupH - 60;

            Button cancelTAwBtn;
            cancelTAwBtn.rect = {(float)(addPopupX + 30), (float)btnY, 180, 42};
            cancelTAwBtn.text = u8"HỦY";
            cancelTAwBtn.color = {149, 165, 166, 255};
            cancelTAwBtn.hoverColor = {127, 140, 141, 255};
            cancelTAwBtn.draw();

            Button saveTAwBtn;
            saveTAwBtn.rect = {(float)(addPopupX + addPopupW - 210), (float)btnY, 180, 42};
            saveTAwBtn.text = u8"LƯU";
            saveTAwBtn.color = {46, 204, 113, 255};
            saveTAwBtn.hoverColor = {39, 174, 96, 255};
            saveTAwBtn.draw();

            if (saveTAwBtn.isClicked())
            {
                if (teamAwardNameInput.text.empty())
                {
                    showMessage(u8"Vui lòng nhập tên giải đấu!");
                }
                else
                {
                    int year = 2024;
                    try
                    {
                        year = stoi(teamAwardYearInput.text);
                    }
                    catch (...)
                    {
                    }

                    // Kiểm tra trùng lặp
                    bool isDup = false;
                    for (const auto &aw : addAwardTeam->getDanhHieuTapThe())
                    {
                        if (aw.tenGiai == teamAwardNameInput.text && aw.nam == year && aw.xepHang == selectedTeamRank)
                        {
                            isDup = true;
                            break;
                        }
                    }

                    if (isDup)
                    {
                        showMessage(u8"Danh hiệu này đã tồn tại!");
                    }
                    else
                    {
                        TeamAward newAward(teamAwardNameInput.text, year, selectedTeamRank);
                        addAwardTeam->addDanhHieuTapThe(newAward);
                        saveToJson(*clb);
                        showMessage(u8"Đã thêm danh hiệu tập thể!");
                        showAddTeamAward = false;
                        addAwardTeam = nullptr;
                    }
                }
            }

            if (cancelTAwBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
            {
                showAddTeamAward = false;
                addAwardTeam = nullptr;
            }
        }
    }

    void drawAddMatchPopup()
    {
        if (!showAddMatch || matchHistoryPlayer == nullptr)
            return;

        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 200});

        int popupWidth = 500;
        int popupHeight = 430;
        int popupX = (screenWidth - popupWidth) / 2;
        int popupY = (screenHeight - popupHeight) / 2;

        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight}, 0.05f, 10, CARD_BG);
        DrawRectangleRounded({(float)popupX, (float)popupY, (float)popupWidth, 50}, 0.05f, 10, (Color){46, 204, 113, 255});
        DrawTextEx2(u8"THÊM TRẬN ĐẤU MỚI", popupX + 20, popupY + 13, 20, TEXT_LIGHT);

        // Nút Hủy
        Button cancelBtn;
        cancelBtn.rect = {(float)(popupX + popupWidth / 2 - 170), (float)(popupY + popupHeight - 55), 150, 40};
        cancelBtn.text = u8"HỦY";
        cancelBtn.color = {149, 165, 166, 255};
        cancelBtn.hoverColor = {127, 140, 141, 255};
        cancelBtn.draw();

        // Nút Lưu
        Button saveBtn;
        saveBtn.rect = {(float)(popupX + popupWidth / 2 + 20), (float)(popupY + popupHeight - 55), 150, 40};
        saveBtn.text = u8"LƯU";
        saveBtn.color = {46, 204, 113, 255};
        saveBtn.hoverColor = {39, 174, 96, 255};
        saveBtn.draw();

        if (cancelBtn.isClicked() || IsKeyPressed(KEY_ESCAPE))
        {
            showAddMatch = false;
            matchInputs.clear();
            return;
        }

        // Vẽ các input
        int inputY = popupY + 70;
        int inputSpacing = 52;
        int labelX = popupX + 20;
        int inputX = popupX + 150;

        // Cập nhật vị trí và vẽ từng input (8 fields - bỏ danh hiệu)
        if (matchInputs.size() >= 8)
        {
            // Xóa label của InputField để tự vẽ label riêng
            for (int i = 0; i < 8; i++)
                matchInputs[i].label = "";

            // Ngày
            DrawTextEx2(u8"Ngày thi đấu:", labelX, inputY + 10, 17, TEXT_DARK);
            matchInputs[0].rect = {(float)inputX, (float)inputY, 200, 38};
            matchInputs[0].draw();
            matchInputs[0].update();
            inputY += inputSpacing;

            // Đối thủ
            DrawTextEx2(u8"Đội đối thủ:", labelX, inputY + 10, 17, TEXT_DARK);
            matchInputs[1].rect = {(float)inputX, (float)inputY, 320, 38};
            matchInputs[1].draw();
            matchInputs[1].update();
            inputY += inputSpacing;

            // Giải đấu
            DrawTextEx2(u8"Giải đấu:", labelX, inputY + 10, 17, TEXT_DARK);
            matchInputs[2].rect = {(float)inputX, (float)inputY, 320, 38};
            matchInputs[2].draw();
            matchInputs[2].update();
            inputY += inputSpacing;

            // Bàn thắng và Kiến tạo trên cùng hàng
            DrawTextEx2(u8"Bàn thắng:", labelX, inputY + 10, 17, TEXT_DARK);
            matchInputs[3].rect = {(float)inputX, (float)inputY, 80, 38};
            matchInputs[3].draw();
            matchInputs[3].update();

            DrawTextEx2(u8"Kiến tạo:", popupX + 260, inputY + 10, 17, TEXT_DARK);
            matchInputs[4].rect = {(float)(popupX + 350), (float)inputY, 80, 38};
            matchInputs[4].draw();
            matchInputs[4].update();
            inputY += inputSpacing;

            // Thẻ vàng và Thẻ đỏ trên cùng hàng
            DrawTextEx2(u8"Thẻ vàng:", labelX, inputY + 10, 17, TEXT_DARK);
            matchInputs[5].rect = {(float)inputX, (float)inputY, 80, 38};
            matchInputs[5].draw();
            matchInputs[5].update();

            DrawTextEx2(u8"Thẻ đỏ:", popupX + 260, inputY + 10, 17, TEXT_DARK);
            matchInputs[6].rect = {(float)(popupX + 350), (float)inputY, 80, 38};
            matchInputs[6].draw();
            matchInputs[6].update();
            inputY += inputSpacing;

            // Ghi chú
            DrawTextEx2(u8"Ghi chú:", labelX, inputY + 10, 17, TEXT_DARK);
            matchInputs[7].rect = {(float)inputX, (float)inputY, 320, 38};
            matchInputs[7].draw();
            matchInputs[7].update();
        }

        if (saveBtn.isClicked())
        {
            // Tạo trận đấu mới (bỏ danh hiệu)
            MatchRecord newMatch;
            newMatch.ngayThiDau = matchInputs[0].text;
            newMatch.doiDoi = matchInputs[1].text;
            newMatch.giaiDau = matchInputs[2].text;

            try
            {
                newMatch.banThang = matchInputs[3].text.empty() ? 0 : stoi(matchInputs[3].text);
            }
            catch (...)
            {
                newMatch.banThang = 0;
            }
            try
            {
                newMatch.kienTao = matchInputs[4].text.empty() ? 0 : stoi(matchInputs[4].text);
            }
            catch (...)
            {
                newMatch.kienTao = 0;
            }
            try
            {
                newMatch.theVang = matchInputs[5].text.empty() ? 0 : stoi(matchInputs[5].text);
            }
            catch (...)
            {
                newMatch.theVang = 0;
            }
            try
            {
                newMatch.theDo = matchInputs[6].text.empty() ? 0 : stoi(matchInputs[6].text);
            }
            catch (...)
            {
                newMatch.theDo = 0;
            }

            newMatch.ghiChu = matchInputs[7].text;

            matchHistoryPlayer->addTranDau(newMatch);
            saveToJson(*clb);

            showAddMatch = false;
            matchInputs.clear();
            showMessage(u8"Đã thêm trận đấu mới!");
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

                // Nút SỬA - chỉ hiển thị khi là Admin
                if (isAdmin)
                {
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
                } // Đóng ngoặc isAdmin cho nút SỬA

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

        // Nút DANH HIỆU TẬP THỂ ở góc trên phải
        Button teamAwardBtn;
        teamAwardBtn.rect = {(float)(screenWidth - 250), 110, 220, 35};
        teamAwardBtn.text = u8"DANH HIỆU CHUNG";
        teamAwardBtn.color = {155, 89, 182, 255}; // Tím (khác với màu vàng của giải cá nhân)
        teamAwardBtn.hoverColor = {142, 68, 173, 255};
        teamAwardBtn.draw();

        if (teamAwardBtn.isClicked())
        {
            showTeamAwardPopup = true;
            teamAwardScrollOffset = 0;
        }

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
                    string positionText = player.getSoAo() > 0 ? (to_string(player.getSoAo()) + u8" | " + player.getViTri()) : player.getViTri();
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
            for (auto &team : clb->getTeams())
            {
                for (auto &player : team.getPlayers())
                {
                    if (playerIndex == selectedPlayerIndex)
                    {
                        int detailY = listY;

                        // Player header
                        Color lightWhite = {240, 240, 240, 255};
                        DrawRectangle(detailX, detailY, detailWidth, 100, ACCENT_1);
                        DrawTextEx2(player.getTen(), detailX + 20, detailY + 15, 22, WHITE);
                        string jerseyInfo = player.getSoAo() > 0 ? (u8"Số áo: " + to_string(player.getSoAo()) + u8" | ") : u8"";
                        DrawTextEx2(jerseyInfo + u8"Vị trí: " + player.getViTri(), detailX + 20, detailY + 50, 14, lightWhite);
                        DrawTextEx2(u8"Đội: " + team.getTenDoi(), detailX + 20, detailY + 75, 14, lightWhite);

                        // 2 Nút: Lịch sử đá bóng, Giải thưởng
                        // Nút xem lịch sử trận đấu
                        Button historyBtn;
                        historyBtn.rect = {(float)(detailX + detailWidth - 350), (float)(detailY + 58), 170, 32};
                        historyBtn.text = u8"LỊCH SỬ ĐÁ BÓNG";
                        historyBtn.color = {241, 196, 15, 255}; // Vàng
                        historyBtn.hoverColor = {243, 156, 18, 255};
                        historyBtn.draw();

                        // Nút xem giải thưởng cá nhân (Quả bóng vàng, Vua phá lưới...)
                        Button indivAwardBtn;
                        indivAwardBtn.rect = {(float)(detailX + detailWidth - 170), (float)(detailY + 58), 150, 32};
                        indivAwardBtn.text = u8"GIẢI THƯỞNG";
                        indivAwardBtn.color = {155, 89, 182, 255}; // Tím (khác màu để phân biệt)
                        indivAwardBtn.hoverColor = {142, 68, 173, 255};
                        indivAwardBtn.draw();

                        if (historyBtn.isClicked())
                        {
                            showMatchHistory = true;
                            matchHistoryPlayer = &player;
                            matchHistoryTeamName = team.getTenDoi();
                            matchHistoryScrollOffset = 0;
                        }

                        if (indivAwardBtn.isClicked())
                        {
                            showIndividualAwardPopup = true;
                            individualAwardPlayer = &player;
                            individualAwardTeamName = team.getTenDoi();
                        }

                        detailY += 120;

                        // Tính tổng từ lịch sử trận đấu
                        int tongTran = player.getSoTranTuLichSu();
                        int tongBanThang = player.getTongBanThangTuLichSu();
                        int tongKienTao = player.getTongKienTaoTuLichSu();
                        int tongTheVang = player.getTongTheVangTuLichSu();
                        int tongTheDo = player.getTongTheDoTuLichSu();

                        // Lấy top 3 giải thưởng gần nhất
                        string giaiThuongText = "";
                        const auto &awards = player.getDanhSachGiaiThuong();
                        int soGiai = (int)awards.size();

                        // Sắp xếp giải thưởng theo năm giảm dần
                        MyVector<Award> sortedAwards;
                        for (const auto &aw : awards)
                            sortedAwards.push_back(aw);
                        for (size_t i = 0; i < sortedAwards.size(); i++)
                        {
                            for (size_t j = i + 1; j < sortedAwards.size(); j++)
                            {
                                if (sortedAwards[i].nam < sortedAwards[j].nam)
                                {
                                    Award temp = sortedAwards[i];
                                    sortedAwards[i] = sortedAwards[j];
                                    sortedAwards[j] = temp;
                                }
                            }
                        }

                        for (int i = 0; i < min(3, soGiai); i++)
                        {
                            if (!giaiThuongText.empty())
                                giaiThuongText += ", ";
                            giaiThuongText += sortedAwards[i].tenGiai + u8" (" + to_string(sortedAwards[i].nam) + u8")";
                        }
                        if (soGiai > 3)
                            giaiThuongText += u8" (+" + to_string(soGiai - 3) + u8" khác)";

                        // Thống kê từ lịch sử (chỉ hiển thị)
                        int formY = detailY;
                        DrawRectangle(detailX + 10, formY, detailWidth - 20, 200, (Color){245, 248, 250, 255});
                        DrawRectangleLinesEx({(float)(detailX + 10), (float)formY, (float)(detailWidth - 20), 200}, 1, BORDER_LIGHT);

                        DrawTextEx2(u8"THỐNG KÊ TỪ LỊCH SỬ TRẬN ĐẤU", detailX + 20, formY + 12, 18, ACCENT_1);
                        formY += 50;

                        // Row 1: Số trận | Bàn thắng
                        DrawTextEx2(u8"Số trận thi đấu:", detailX + 30, formY + 5, 18, TEXT_DARK);
                        DrawTextEx2(to_string(tongTran), detailX + 200, formY + 5, 24, (Color){46, 204, 113, 255});

                        DrawTextEx2(u8"Bàn thắng:", detailX + 300, formY + 5, 18, TEXT_DARK);
                        DrawTextEx2(to_string(tongBanThang), detailX + 420, formY + 5, 24, (Color){46, 204, 113, 255});
                        formY += 45;

                        // Row 2: Kiến tạo | Thẻ vàng | Thẻ đỏ
                        DrawTextEx2(u8"Kiến tạo:", detailX + 30, formY + 5, 18, TEXT_DARK);
                        DrawTextEx2(to_string(tongKienTao), detailX + 130, formY + 5, 24, ACCENT_1);

                        DrawTextEx2(u8"Thẻ vàng:", detailX + 220, formY + 5, 18, TEXT_DARK);
                        DrawTextEx2(to_string(tongTheVang), detailX + 330, formY + 5, 24, CAPTAIN_COLOR);

                        DrawTextEx2(u8"Thẻ đỏ:", detailX + 400, formY + 5, 18, TEXT_DARK);
                        DrawTextEx2(to_string(tongTheDo), detailX + 480, formY + 5, 24, ACCENT_2);
                        formY += 50;

                        // Giải thưởng
                        DrawTextEx2(u8"Giải thưởng:", detailX + 30, formY + 5, 18, TEXT_DARK);
                        string giaiThuongDisplay = giaiThuongText.empty() ? u8"(Chưa có)" : giaiThuongText;
                        if (giaiThuongDisplay.length() > 80)
                            giaiThuongDisplay = giaiThuongDisplay.substr(0, 77) + "...";
                        DrawTextEx2(giaiThuongDisplay, detailX + 160, formY + 5, 17, TEXT_DARK);

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

                // Nút lưu - chỉ hiển thị khi là Admin
                if (isAdmin)
                {
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
                } // Đóng ngoặc isAdmin

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

                // Nút lưu - chỉ hiển thị khi là Admin
                if (isAdmin)
                {
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
                    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isAdmin)
                        tempDiemPhongDo = i;
                }

                formY += 70;

                if (isAdmin)
                {
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

                if (isAdmin)
                {
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

                if (isAdmin)
                {
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

    void drawLoginScreen()
    {
        // Animated background với gradient đẹp hơn
        static float animTime = 0;
        animTime += GetFrameTime();

        // Gradient background với hiệu ứng chuyển màu
        Color topColor = {25, 42, 86, 255};     // Xanh đậm
        Color bottomColor = {40, 60, 100, 255}; // Xanh nhạt hơn
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, topColor, bottomColor);

        // Vẽ các hình tròn trang trí với hiệu ứng động
        for (int i = 0; i < 8; i++)
        {
            float offset = sin(animTime * 0.5f + i * 0.8f) * 20;
            int cx = (screenWidth / 8) * i + 50;
            int cy = screenHeight / 2 + (int)offset;
            DrawCircle(cx, cy - 200, 80 + i * 15, (Color){255, 255, 255, 8});
            DrawCircle(cx + 30, cy + 250, 60 + i * 10, (Color){255, 255, 255, 5});
        }

        // Vẽ các đường chéo trang trí
        for (int i = 0; i < 5; i++)
        {
            DrawLineEx({(float)(i * 300 - 100), (float)screenHeight},
                       {(float)(i * 300 + 200), 0}, 1, (Color){255, 255, 255, 15});
        }

        // Login box với shadow đẹp hơn
        int boxWidth = 480;
        int boxHeight = 520;
        int boxX = (screenWidth - boxWidth) / 2;
        int boxY = (screenHeight - boxHeight) / 2;

        // Multiple shadow layers cho depth effect
        DrawRectangleRounded({(float)(boxX + 15), (float)(boxY + 15), (float)boxWidth, (float)boxHeight}, 0.08f, 10, (Color){0, 0, 0, 30});
        DrawRectangleRounded({(float)(boxX + 8), (float)(boxY + 8), (float)boxWidth, (float)boxHeight}, 0.08f, 10, (Color){0, 0, 0, 40});

        // Main box với gradient nhẹ
        DrawRectangleRounded({(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 0.08f, 10, (Color){255, 255, 255, 250});

        // Header với gradient đẹp
        DrawRectangleRoundedGradientH({(float)boxX, (float)boxY, (float)boxWidth, 90}, 0.08f, 10,
                                      (Color){52, 152, 219, 255}, (Color){41, 128, 185, 255});

        // Logo/Icon container với hiệu ứng glow
        int iconCenterX = boxX + boxWidth / 2;
        int iconCenterY = boxY + 140;

        // Glow effect
        DrawCircle(iconCenterX, iconCenterY, 58, (Color){52, 152, 219, 40});
        DrawCircle(iconCenterX, iconCenterY, 52, (Color){52, 152, 219, 60});
        // Icon background
        DrawCircle(iconCenterX, iconCenterY, 45, (Color){52, 152, 219, 255});
        DrawCircleLines(iconCenterX, iconCenterY, 45, (Color){41, 128, 185, 255});

        // Icon bóng đá đơn giản
        DrawCircle(iconCenterX, iconCenterY, 25, WHITE);
        DrawCircleLines(iconCenterX, iconCenterY, 25, (Color){52, 152, 219, 255});
        // Vẽ pattern bóng đá
        DrawCircle(iconCenterX, iconCenterY - 8, 8, (Color){52, 152, 219, 255});
        DrawCircle(iconCenterX - 12, iconCenterY + 8, 6, (Color){52, 152, 219, 255});
        DrawCircle(iconCenterX + 12, iconCenterY + 8, 6, (Color){52, 152, 219, 255});

        // Title với shadow
        string title = u8"QUẢN LÝ CÂU LẠC BỘ";
        int titleW = MeasureTextEx2(title, 26);
        DrawTextEx2(title, boxX + (boxWidth - titleW) / 2 + 1, boxY + 26, 26, (Color){0, 0, 0, 50});
        DrawTextEx2(title, boxX + (boxWidth - titleW) / 2, boxY + 25, 26, TEXT_LIGHT);

        string subtitle = u8"BÓNG ĐÁ VIỆT NAM";
        int subW = MeasureTextEx2(subtitle, 16);
        DrawTextEx2(subtitle, boxX + (boxWidth - subW) / 2, boxY + 58, 16, (Color){255, 255, 255, 200});

        int fieldY = boxY + 210;

        // Username field với icon
        DrawTextEx2(u8"Tên đăng nhập", boxX + 50, fieldY, 15, (Color){100, 100, 100, 255});
        fieldY += 28;

        Rectangle usernameRect = {(float)(boxX + 50), (float)fieldY, (float)(boxWidth - 100), 50};
        bool usernameHover = CheckCollisionPointRec(GetMousePosition(), usernameRect);
        Color usernameBg = loginActiveField == 0 ? (Color){240, 248, 255, 255} : (usernameHover ? (Color){248, 250, 252, 255} : (Color){250, 250, 250, 255});
        Color usernameBorder = loginActiveField == 0 ? (Color){52, 152, 219, 255} : (usernameHover ? (Color){150, 180, 200, 255} : (Color){220, 220, 220, 255});

        DrawRectangleRounded(usernameRect, 0.15f, 10, usernameBg);
        DrawRectangleRoundedLinesEx(usernameRect, 0.15f, 10, loginActiveField == 0 ? 2 : 1, usernameBorder);

        // Icon user
        DrawCircle(boxX + 75, fieldY + 25, 12, (Color){52, 152, 219, loginActiveField == 0 ? (unsigned char)255 : (unsigned char)150});
        DrawTextEx2("U", boxX + 68, fieldY + 15, 16, TEXT_LIGHT);

        // Input text
        string displayUsername = loginUsername.empty() ? u8"Nhập tên đăng nhập..." : loginUsername;
        Color usernameTextColor = loginUsername.empty() ? (Color){180, 180, 180, 255} : TEXT_DARK;
        DrawTextEx2(displayUsername, boxX + 100, fieldY + 15, 18, usernameTextColor);

        // Cursor nhấp nháy
        if (loginActiveField == 0 && ((int)(animTime * 2) % 2 == 0))
        {
            int cursorX = boxX + 100 + MeasureTextEx2(loginUsername, 18);
            DrawRectangle(cursorX, fieldY + 12, 2, 26, ACCENT_1);
        }

        if (CheckCollisionPointRec(GetMousePosition(), usernameRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            loginActiveField = 0;
        }

        fieldY += 70;

        // Password field với icon
        DrawTextEx2(u8"Mật khẩu", boxX + 50, fieldY, 15, (Color){100, 100, 100, 255});
        fieldY += 28;

        Rectangle passwordRect = {(float)(boxX + 50), (float)fieldY, (float)(boxWidth - 100), 50};
        bool passwordHover = CheckCollisionPointRec(GetMousePosition(), passwordRect);
        Color passwordBg = loginActiveField == 1 ? (Color){240, 248, 255, 255} : (passwordHover ? (Color){248, 250, 252, 255} : (Color){250, 250, 250, 255});
        Color passwordBorder = loginActiveField == 1 ? (Color){52, 152, 219, 255} : (passwordHover ? (Color){150, 180, 200, 255} : (Color){220, 220, 220, 255});

        DrawRectangleRounded(passwordRect, 0.15f, 10, passwordBg);
        DrawRectangleRoundedLinesEx(passwordRect, 0.15f, 10, loginActiveField == 1 ? 2 : 1, passwordBorder);

        // Icon lock
        DrawRectangle(boxX + 68, fieldY + 18, 14, 12, (Color){52, 152, 219, loginActiveField == 1 ? (unsigned char)255 : (unsigned char)150});
        DrawRectangleRounded({(float)(boxX + 66), (float)(fieldY + 10), 18, 14}, 0.5f, 8, (Color){0, 0, 0, 0});
        DrawRectangleRoundedLinesEx({(float)(boxX + 68), (float)(fieldY + 8), 14, 12}, 0.5f, 8, 2, (Color){52, 152, 219, loginActiveField == 1 ? (unsigned char)255 : (unsigned char)150});

        // Masked password hoặc placeholder
        string maskedPassword = "";
        for (size_t i = 0; i < loginPassword.length(); i++)
            maskedPassword += ".";
        string displayPassword = loginPassword.empty() ? u8"Nhập mật khẩu..." : maskedPassword;
        Color passwordTextColor = loginPassword.empty() ? (Color){180, 180, 180, 255} : TEXT_DARK;
        DrawTextEx2(displayPassword, boxX + 100, fieldY + 15, 18, passwordTextColor);

        // Cursor nhấp nháy cho password
        if (loginActiveField == 1 && ((int)(animTime * 2) % 2 == 0))
        {
            int cursorX = boxX + 100 + MeasureTextEx2(maskedPassword, 18);
            DrawRectangle(cursorX, fieldY + 12, 2, 26, ACCENT_1);
        }

        if (CheckCollisionPointRec(GetMousePosition(), passwordRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            loginActiveField = 1;
        }

        // Handle keyboard input
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 126)
            {
                if (loginActiveField == 0 && loginUsername.length() < 20)
                {
                    loginUsername += (char)key;
                }
                else if (loginActiveField == 1 && loginPassword.length() < 20)
                {
                    loginPassword += (char)key;
                }
            }
            key = GetCharPressed();
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            if (loginActiveField == 0 && !loginUsername.empty())
            {
                loginUsername.pop_back();
            }
            else if (loginActiveField == 1 && !loginPassword.empty())
            {
                loginPassword.pop_back();
            }
        }

        // Tab to switch fields
        if (IsKeyPressed(KEY_TAB))
        {
            loginActiveField = (loginActiveField + 1) % 2;
        }

        fieldY += 60;

        // Error message hiển thị giữa mật khẩu và nút đăng nhập
        if (!loginError.empty() && loginErrorTimer > 0)
        {
            loginErrorTimer -= GetFrameTime();
            if (loginErrorTimer <= 0)
            {
                loginError = "";
                loginErrorTimer = 0;
            }
            else
            {
                // Tính alpha dựa trên thời gian còn lại (fade out trong 0.3s cuối)
                float alpha = loginErrorTimer < 0.3f ? (loginErrorTimer / 0.3f) : 1.0f;
                int errW = MeasureTextEx2(loginError, 14);
                // Error background với fade - nền đỏ nổi bật
                DrawRectangleRounded({(float)(boxX + (boxWidth - errW) / 2 - 15), (float)(fieldY), (float)(errW + 30), 28}, 0.4f, 8, (Color){231, 76, 60, (unsigned char)(230 * alpha)});
                DrawTextEx2(loginError, boxX + (boxWidth - errW) / 2, fieldY + 6, 14, (Color){255, 255, 255, (unsigned char)(255 * alpha)});
            }
        }

        fieldY += 25;

        // Login button với hiệu ứng đẹp
        Rectangle loginBtnRect = {(float)(boxX + 50), (float)fieldY, (float)(boxWidth - 100), 55};
        bool loginBtnHover = CheckCollisionPointRec(GetMousePosition(), loginBtnRect);

        // Button gradient
        Color btnColor1 = loginBtnHover ? (Color){41, 128, 185, 255} : (Color){52, 152, 219, 255};
        Color btnColor2 = loginBtnHover ? (Color){35, 110, 160, 255} : (Color){41, 128, 185, 255};

        DrawRectangleRoundedGradientH(loginBtnRect, 0.2f, 10, btnColor1, btnColor2);

        string btnText = u8"ĐĂNG NHẬP";
        int btnTextW = MeasureTextEx2(btnText, 20);
        DrawTextEx2(btnText, boxX + (boxWidth - btnTextW) / 2 + 1, fieldY + 17, 20, (Color){0, 0, 0, 50});
        DrawTextEx2(btnText, boxX + (boxWidth - btnTextW) / 2, fieldY + 16, 20, TEXT_LIGHT);

        // Handle login
        bool loginClicked = loginBtnHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (loginClicked || IsKeyPressed(KEY_ENTER))
        {
            // Admin account
            if (loginUsername == "admin" && loginPassword == "admin123")
            {
                isLoggedIn = true;
                isAdmin = true;
                loginError = "";
            }
            // User account
            else if (loginUsername == "user" && loginPassword == "user123")
            {
                isLoggedIn = true;
                isAdmin = false;
                loginError = "";
            }
            else
            {
                loginError = u8"Sai tên đăng nhập hoặc mật khẩu!";
                loginErrorTimer = 1.5f; // Hiển thị trong 1.5 giây
            }
        }

        // Footer - Info text với thiết kế đẹp hơn
        int footerY = boxY + boxHeight - 45;
        DrawLine(boxX + 50, footerY, boxX + boxWidth - 50, footerY, (Color){230, 230, 230, 255});

        // Admin info
        DrawCircle(boxX + 68, footerY + 15, 8, (Color){46, 204, 113, 255});
        DrawTextEx2(u8"Admin: admin / admin123", boxX + 85, footerY + 7, 13, (Color){100, 100, 100, 255});

        // User info
        DrawCircle(boxX + 68, footerY + 35, 8, (Color){52, 152, 219, 255});
        DrawTextEx2(u8"User: user / user123", boxX + 85, footerY + 27, 13, (Color){100, 100, 100, 255});

        // Copyright
        string copyright = u8"© 2024 Football Club Manager";
        int cpW = MeasureTextEx2(copyright, 12);
        DrawTextEx2(copyright, (screenWidth - cpW) / 2, screenHeight - 30, 12, (Color){255, 255, 255, 100});
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

            if (!isLoggedIn)
            {
                // Hiển thị màn hình đăng nhập
                drawLoginScreen();
            }
            else
            {
                // Hiển thị giao diện chính
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
                drawEditRolePopup();
                drawDeleteConfirmPopup();
                drawDeleteTeamConfirmPopup();
                drawStatsDetailPopup();
                drawMatchHistoryPopup();
                drawAddMatchPopup();
                drawHonorPopup();
                drawIndividualAwardPopup();
                drawTeamAwardPopup();
                drawEditTeamPopup();

                // Hiển thị role ở góc trên phải
                string roleText = isAdmin ? u8"[ADMIN]" : u8"[USER - Chỉ xem]";
                Color roleColor = isAdmin ? (Color){46, 204, 113, 255} : (Color){241, 196, 15, 255};
                int roleW = MeasureTextEx2(roleText, 16);
                DrawTextEx2(roleText, screenWidth - roleW - 20, 10, 16, roleColor);

                // Nút đăng xuất
                Button logoutBtn;
                logoutBtn.rect = {(float)(screenWidth - 100), 35, 80, 30};
                logoutBtn.text = u8"Đăng xuất";
                logoutBtn.color = {149, 165, 166, 255};
                logoutBtn.hoverColor = {127, 140, 141, 255};
                logoutBtn.draw();

                if (logoutBtn.isClicked())
                {
                    isLoggedIn = false;
                    isAdmin = false;
                    loginUsername = "";
                    loginPassword = "";
                    loginError = "";
                }
            }

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