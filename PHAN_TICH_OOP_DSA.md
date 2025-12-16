# PHÂN TÍCH BỐ CỤC VÀ TÍNH CHẤT OOP & DSA

## Dự án: Quản Lý Câu Lạc Bộ Bóng Đá

---

## 📁 I. BỐ CỤC DỰ ÁN

### 1. Cấu trúc thư mục

```
project/
├── src/
│   ├── ClubGUI.cpp          # File chính - Giao diện người dùng (6300+ dòng)
│   ├── MyVector.h           # Cấu trúc dữ liệu Vector tự cài đặt
│   ├── MyMap.h              # Cấu trúc dữ liệu Map (Hash Table) tự cài đặt
│   ├── ime_helper.h         # Hỗ trợ nhập tiếng Việt Telex
│   ├── json.hpp             # Thư viện nlohmann JSON
│   └── models/
│       ├── Person.h         # Class cơ sở Person
│       ├── Player.h         # Class Player kế thừa Person
│       ├── Team.h           # Class Team quản lý cầu thủ
│       └── Club.h           # Class Club quản lý các đội
├── team.json                # File dữ liệu JSON
├── assets/                  # Tài nguyên (fonts, images)
└── Makefile                 # File build
```

### 2. Sơ đồ quan hệ các Class

```
                    ┌─────────────┐
                    │   Person    │ (Base Class)
                    └──────┬──────┘
                           │ Inheritance
                           ▼
                    ┌─────────────┐
                    │   Player    │
                    └──────┬──────┘
                           │ Composition
                           ▼
    ┌──────────────────────┴──────────────────────┐
    │                                             │
    ▼                                             ▼
┌─────────────┐                           ┌─────────────┐
│ MatchRecord │ (Struct)                  │    Award    │ (Struct)
└─────────────┘                           └─────────────┘

                    ┌─────────────┐
                    │    Team     │
                    └──────┬──────┘
                           │ Aggregation (MyVector<Player>)
                           │
                           ▼
                    ┌─────────────┐
                    │    Club     │
                    └─────────────┘
                           │ Aggregation (MyVector<Team>)
```

---

## 🎯 II. TÍNH CHẤT OOP (Object-Oriented Programming)

### 1. ENCAPSULATION (Đóng gói)

#### Định nghĩa:

Đóng gói là việc che giấu dữ liệu bên trong đối tượng và chỉ cho phép truy cập thông qua các phương thức công khai.

#### Ví dụ trong dự án:

**a) Class Person (Person.h)**

```cpp
class Person
{
protected:
    // Dữ liệu được bảo vệ - chỉ truy cập qua getter/setter
    std::string id, ten, ngaySinh, queQuan, thongTinLienLac;

public:
    // Getter - Đọc dữ liệu
    std::string getID() const { return id; }
    std::string getTen() const { return ten; }
    std::string getNgaySinh() const { return ngaySinh; }

    // Setter - Ghi dữ liệu
    void setTen(const std::string &t) { ten = t; }
    void setNgaySinh(const std::string &ns) { ngaySinh = ns; }
};
```

**b) Class Player (Player.h)**

```cpp
class Player : public Person
{
protected:
    // Dữ liệu riêng tư của cầu thủ
    std::string maCauThu, viTri;
    int banThang, soAo;
    double mucLuong;

    // Dữ liệu sức khỏe
    std::string trangThaiSucKhoe;
    int mucTheLuc;

    // Lịch sử trận đấu - dùng MyVector (không dùng std::vector)
    MyVector<MatchRecord> lichSuTranDau;

public:
    // Encapsulation: Truy cập qua getter/setter
    int getSoAo() const { return soAo; }
    void setSoAo(int sa) { soAo = sa; }

    double getLuong() const { return mucLuong; }
    void setLuong(double l) { mucLuong = l; }

    // Encapsulation cho collection - dùng MyVector
    MyVector<MatchRecord>& getLichSuTranDau() { return lichSuTranDau; }
    void addTranDau(const MatchRecord& tran) { lichSuTranDau.push_back(tran); }
};
```

**Lợi ích:**

- Bảo vệ dữ liệu khỏi truy cập trực tiếp
- Kiểm soát validation khi set giá trị
- Dễ dàng thay đổi cài đặt bên trong mà không ảnh hưởng code bên ngoài

---

### 2. INHERITANCE (Kế thừa)

#### Định nghĩa:

Kế thừa cho phép một class con (derived) thừa hưởng các thuộc tính và phương thức từ class cha (base).

#### Ví dụ trong dự án:

**Player kế thừa từ Person:**

```cpp
// Base class (Lớp cơ sở)
class Person
{
protected:
    std::string id, ten, ngaySinh, queQuan, thongTinLienLac;

public:
    Person() {}
    Person(const std::string &id, const std::string &ten, ...);
    virtual ~Person() {}  // Virtual destructor cho đa hình

    std::string getID() const { return id; }
    std::string getTen() const { return ten; }
};

// Derived class (Lớp dẫn xuất)
class Player : public Person  // Kế thừa public
{
protected:
    // Thuộc tính riêng của Player
    std::string maCauThu, viTri;
    int banThang, soAo;
    double mucLuong;

public:
    Player() : Person("", "", "", "", "") { ... }  // Gọi constructor cha

    Player(const std::string &id, const std::string &ten, ...)
        : Person(id, ten, ns, qq, ttll),  // Khởi tạo base class
          maCauThu(ma), viTri(vt), ... { }

    // Player có thể dùng các method của Person
    // VD: player.getTen() - kế thừa từ Person
};
```

**Minh họa quan hệ kế thừa:**

```
        Person
           │
    ┌──────┴──────┐
    │  protected  │
    │  - id       │
    │  - ten      │
    │  - ngaySinh │
    ├─────────────┤
    │   public    │
    │  + getID()  │
    │  + getTen() │
    └──────┬──────┘
           │ is-a
           ▼
        Player
           │
    ┌──────┴──────┐
    │  protected  │
    │  - maCauThu │ (riêng)
    │  - viTri    │ (riêng)
    │  - soAo     │ (riêng)
    │  + id       │ (kế thừa)
    │  + ten      │ (kế thừa)
    ├─────────────┤
    │   public    │
    │  + getSoAo()│ (riêng)
    │  + getID()  │ (kế thừa)
    │  + getTen() │ (kế thừa)
    └─────────────┘
```

**Lợi ích:**

- Tái sử dụng code (Person có thể dùng cho HLV, Nhân viên...)
- Tổ chức code theo hierarchy hợp lý
- Dễ mở rộng thêm loại Person khác

---

### 3. POLYMORPHISM (Đa hình)

#### Định nghĩa:

Đa hình cho phép một method có thể có nhiều hình thức khác nhau tùy thuộc vào đối tượng gọi nó.

#### Ví dụ trong dự án:

**a) Virtual Destructor:**

```cpp
class Person
{
public:
    virtual ~Person() {}  // Virtual destructor - đảm bảo gọi đúng destructor
};

class Player : public Person
{
public:
    ~Player() = default;  // Destructor của Player
};

// Khi delete qua pointer Person*, vẫn gọi đúng ~Player()
Person* p = new Player();
delete p;  // Gọi ~Player() nhờ virtual
```

**b) Function Overloading (Nạp chồng hàm):**

```cpp
// Trong MyVector.h - Nhiều phiên bản operator[]
T &operator[](size_t index) { return data[index]; }
const T &operator[](size_t index) const { return data[index]; }

// Nhiều phiên bản at()
T &at(size_t index);
const T &at(size_t index) const;

// Nhiều constructor
Player();
Player(const std::string &id, const std::string &ten, ...);
```

**c) Operator Overloading (Nạp chồng toán tử):**

```cpp
// Trong MyVector::iterator
iterator &operator++() { ++ptr; return *this; }      // ++it
iterator operator++(int) { iterator tmp = *this; ++ptr; return tmp; }  // it++
iterator operator+(ptrdiff_t n) const { return iterator(ptr + n); }
ptrdiff_t operator-(const iterator &other) const { return ptr - other.ptr; }
T &operator*() { return *ptr; }
bool operator==(const iterator &other) const { return ptr == other.ptr; }
```

---

### 4. ABSTRACTION (Trừu tượng)

#### Định nghĩa:

Trừu tượng hóa là việc ẩn đi chi tiết cài đặt phức tạp, chỉ hiển thị interface đơn giản cho người dùng.

#### Ví dụ trong dự án:

**a) Class MyVector - Ẩn chi tiết quản lý bộ nhớ:**

```cpp
template <typename T>
class MyVector
{
private:
    T *data;           // Chi tiết cài đặt - người dùng không cần biết
    size_t size_;
    size_t capacity_;

    // Hàm nội bộ - người dùng không thấy
    void reserve_internal(size_t newCapacity) { ... }

public:
    // Interface đơn giản cho người dùng
    void push_back(const T &value);   // Thêm phần tử
    void pop_back();                  // Xóa phần tử cuối
    T &at(size_t index);              // Truy cập phần tử
    size_t size() const;              // Lấy kích thước
    bool empty() const;               // Kiểm tra rỗng
};

// Người dùng chỉ cần biết:
MyVector<Player> players;
players.push_back(player1);
players.push_back(player2);
int count = players.size();
```

**b) Class MyMap - Ẩn chi tiết Hash Table:**

```cpp
template <typename Key, typename Value>
class MyMap
{
private:
    MyVector<InternalPair> buckets;  // Chi tiết cài đặt

    size_t hash(const Key &key) const { ... }  // Hàm băm - ẩn
    void rehash() { ... }                       // Mở rộng - ẩn

public:
    // Interface đơn giản
    void insert(const Key &key, const Value &value);
    bool find(const Key &key) const;
    Value &at(const Key &key);
    Value &operator[](const Key &key);
};

// Người dùng chỉ cần biết:
MyMap<string, int> scoreMap;
scoreMap.insert("player1", 10);
int score = scoreMap["player1"];
```

---

## 📊 III. CẤU TRÚC DỮ LIỆU & GIẢI THUẬT (DSA)

### 1. MYVECTOR - Dynamic Array (Mảng động)

#### Đặc điểm:

| Thuộc tính      | Giá trị                |
| --------------- | ---------------------- |
| Kiểu            | Template class         |
| Bộ nhớ          | Liên tục (contiguous)  |
| Tự động mở rộng | Có (doubling strategy) |
| Random access   | O(1)                   |

#### Cấu trúc:

```cpp
template <typename T>
class MyVector
{
private:
    T *data;           // Con trỏ đến mảng
    size_t size_;      // Số phần tử hiện có
    size_t capacity_;  // Dung lượng tối đa
};
```

#### Độ phức tạp các thao tác:

| Thao tác               | Độ phức tạp    | Ghi chú             |
| ---------------------- | -------------- | ------------------- |
| `push_back()`          | O(1) amortized | O(n) khi resize     |
| `pop_back()`           | O(1)           |                     |
| `at(i)` / `operator[]` | O(1)           | Random access       |
| `front()` / `back()`   | O(1)           |                     |
| `erase(pos)`           | O(n)           | Dịch chuyển phần tử |
| `size()`               | O(1)           |                     |
| `empty()`              | O(1)           |                     |
| `clear()`              | O(1)           | Chỉ reset size      |

#### Chiến lược mở rộng (Doubling Strategy):

```cpp
void push_back(const T &value)
{
    if (size_ == capacity_)
    {
        // Nhân đôi capacity khi đầy
        size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve_internal(newCapacity);
    }
    data[size_++] = value;
}
```

**Tại sao nhân đôi?**

- Giảm số lần realloc
- Amortized O(1) cho push_back
- Tổng chi phí cho n phần tử: O(n) thay vì O(n²)

#### Iterator Implementation:

```cpp
class iterator
{
private:
    T *ptr;  // Con trỏ đến phần tử

public:
    // Random Access Iterator - hỗ trợ đầy đủ các toán tử
    iterator &operator++();           // ++it
    iterator operator++(int);         // it++
    iterator &operator--();           // --it
    iterator operator+(ptrdiff_t n);  // it + n
    ptrdiff_t operator-(const iterator &other);  // it1 - it2
    T &operator*();                   // *it
    bool operator==(const iterator &other);
    bool operator<(const iterator &other);
};
```

#### Sử dụng trong dự án:

```cpp
// Danh sách đội trong CLB
MyVector<Team> teams;

// Danh sách cầu thủ trong đội
MyVector<Player> cauThu;

// Danh sách giải thưởng
MyVector<TeamAward> danhHieuTapThe;
```

---

### 2. MYMAP - Hash Table (Bảng băm)

#### Đặc điểm:

| Thuộc tính  | Giá trị                          |
| ----------- | -------------------------------- |
| Kiểu        | Template class với Key-Value     |
| Phương pháp | Open Addressing (Linear Probing) |
| Load Factor | 0.75                             |
| Hàm băm     | Polynomial Rolling Hash          |

#### Cấu trúc:

```cpp
template <typename Key, typename Value>
class MyMap
{
private:
    struct InternalPair
    {
        Key key;
        Value value;
        bool used;  // Đánh dấu slot đã dùng
    };

    MyVector<InternalPair> buckets;  // Mảng các bucket
    size_t count_;                    // Số phần tử
};
```

#### Hàm băm (Hash Function):

```cpp
size_t hash(const Key &key) const
{
    // Polynomial Rolling Hash cho string
    size_t h = 0;
    const char *str = key.c_str();
    while (*str)
    {
        h = h * 31 + *str++;  // h = h*31 + char
    }
    return h % buckets.size();
}
```

**Tại sao dùng 31?**

- Số nguyên tố → phân bố đều
- 31 = 32 - 1 = 2⁵ - 1 → tối ưu với shift: `31*i = (i<<5) - i`

#### Xử lý xung đột (Collision Resolution):

```cpp
// Linear Probing - tìm slot trống tiếp theo
size_t findBucket(const Key &key) const
{
    size_t idx = hash(key);
    size_t start = idx;

    while (buckets[idx].used && buckets[idx].key != key)
    {
        idx = (idx + 1) % buckets.size();  // Dịch sang slot tiếp theo
        if (idx == start)
            return buckets.size();  // Đã quét hết
    }
    return idx;
}
```

#### Rehashing (Mở rộng bảng):

```cpp
void rehash()
{
    MyVector<InternalPair> oldBuckets = buckets;
    buckets.clear();
    buckets.resize(oldBuckets.size() * 2);  // Nhân đôi kích thước

    // Re-insert tất cả phần tử
    for (const auto &pair : oldBuckets)
    {
        if (pair.used)
        {
            // Tính lại vị trí với hash mới
            size_t idx = hash(pair.key);
            while (buckets[idx].used)
            {
                idx = (idx + 1) % buckets.size();
            }
            buckets[idx] = pair;
        }
    }
}
```

#### Độ phức tạp:

| Thao tác     | Average | Worst Case |
| ------------ | ------- | ---------- |
| `insert()`   | O(1)    | O(n)       |
| `find()`     | O(1)    | O(n)       |
| `at()`       | O(1)    | O(n)       |
| `operator[]` | O(1)    | O(n)       |

---

### 3. CÁC GIẢI THUẬT SỬ DỤNG

#### a) Thuật toán sắp xếp (Sorting):

**Sử dụng std::sort với custom comparator:**

```cpp
// Sắp xếp top ghi bàn giảm dần
sort(topScorers.begin(), topScorers.end(),
     [](const pair<string, int> &a, const pair<string, int> &b)
     { return a.second > b.second; });

// Sắp xếp giải thưởng theo năm giảm dần
for (size_t i = 0; i < sortedAwards.size(); i++) {
    for (size_t j = i + 1; j < sortedAwards.size(); j++) {
        if (sortedAwards[i].nam < sortedAwards[j].nam) {
            swap(sortedAwards[i], sortedAwards[j]);
        }
    }
}
```

#### b) Thuật toán tìm kiếm (Searching):

**Tìm kiếm tuyến tính với điều kiện:**

```cpp
// Tìm cầu thủ theo ID
auto it = std::find_if(cauThu.begin(), cauThu.end(),
    [&](const Player &x) { return x.getID() == p.getID(); });

// Tìm kiếm text trong danh sách
for (auto &player : team.getPlayers())
{
    string nameUpper = toUpperCase(player.getTen());
    if (nameUpper.find(searchUpper) != string::npos)
    {
        filteredPlayers.push_back(&player);
    }
}
```

#### c) Thuật toán tổng hợp (Aggregation):

**Tính tổng bàn thắng từ lịch sử:**

```cpp
int getTongBanThangTuLichSu() const {
    int tong = 0;
    for (const auto& tran : lichSuTranDau)
        tong += tran.banThang;
    return tong;
}

// Tổng hợp theo năm
for (const auto& match : p.getLichSuTranDau()) {
    int matchYear = stoi(match.ngayThiDau.substr(6, 4));
    if (selectedYear == 0 || matchYear == selectedYear) {
        playerGoalsInYear += match.banThang;
    }
}
```

#### d) Xử lý chuỗi (String Processing):

**Chuyển đổi ký tự tiếng Việt (Telex Input) - Sử dụng MyMap tự cài đặt:**

```cpp
// Ánh xạ dấu tiếng Việt - dùng MyMap (không dùng std::map)
MyMap<std::string, std::string> initDiacriticMap()
{
    MyMap<std::string, std::string> map;
    map.insert("aa", "â");  // a + a = â
    map.insert("aw", "ă");  // a + w = ă
    map.insert("ee", "ê");  // e + e = ê
    map.insert("oo", "ô");  // o + o = ô
    map.insert("ow", "ơ");  // o + w = ơ
    map.insert("uw", "ư");  // u + w = ư
    map.insert("dd", "đ");  // d + d = đ
    return map;
}

// Ánh xạ tone marks - dùng MyMap<string, MyVector<string>>
static MyMap<std::string, MyVector<std::string>> toneMap;
// toneMap["a"] = {"á", "à", "ả", "ã", "ạ"};  // s, f, r, x, j
// toneMap["ă"] = {"ắ", "ằ", "ẳ", "ẵ", "ặ"};
// ...
```

---

## 📈 IV. THỐNG KÊ CODE

### 1. Số lượng Class/Struct:

| Loại   | Tên             | Dòng code | Mô tả                    |
| ------ | --------------- | --------- | ------------------------ |
| Class  | Person          | ~27       | Lớp cơ sở người          |
| Class  | Player          | ~255      | Cầu thủ (kế thừa Person) |
| Class  | Team            | ~120      | Đội bóng                 |
| Class  | Club            | ~30       | Câu lạc bộ               |
| Class  | MyVector        | ~303      | Vector tự cài đặt        |
| Class  | MyMap           | ~270      | Hash Map tự cài đặt      |
| Class  | ClubGUI         | ~6300     | Giao diện chính          |
| Struct | MatchRecord     | ~20       | Thông tin trận đấu       |
| Struct | Award           | ~15       | Giải thưởng cá nhân      |
| Struct | TeamAward       | ~15       | Giải thưởng đội          |
| Struct | TeamAchievement | ~15       | Thành tích cùng đội      |
| Struct | Button          | ~50       | Nút bấm GUI              |

### 2. Tổng quan:

| Metric                | Giá trị |
| --------------------- | ------- |
| Tổng số dòng code     | ~7,500+ |
| Số file header (.h)   | 7       |
| Số file source (.cpp) | 1       |
| Số class              | 7       |
| Số struct             | 5       |
| Số template class     | 2       |

### 3. Tính năng OOP sử dụng:

| Tính năng            | Có/Không | Vị trí                                   |
| -------------------- | -------- | ---------------------------------------- |
| Encapsulation        | ✅       | Tất cả class                             |
| Inheritance          | ✅       | Player : Person                          |
| Polymorphism         | ✅       | Virtual destructor, Operator overloading |
| Abstraction          | ✅       | MyVector, MyMap                          |
| Template             | ✅       | MyVector<T>, MyMap<K,V>                  |
| Copy Constructor     | ✅       | MyVector, Team                           |
| Move Semantics       | ✅       | MyVector                                 |
| Operator Overloading | ✅       | MyVector::iterator                       |

### 4. DSA sử dụng:

| Cấu trúc/Giải thuật | Vị trí      | Mục đích            |
| ------------------- | ----------- | ------------------- |
| Dynamic Array       | MyVector.h  | Lưu trữ danh sách   |
| Hash Table          | MyMap.h     | Tra cứu nhanh       |
| Linear Search       | ClubGUI.cpp | Tìm kiếm cầu thủ    |
| Bubble Sort         | ClubGUI.cpp | Sắp xếp giải thưởng |
| std::sort           | ClubGUI.cpp | Sắp xếp top ghi bàn |
| Iterator Pattern    | MyVector.h  | Duyệt collection    |

---

## 🎨 V. SƠ ĐỒ KIẾN TRÚC

### Class Diagram:

```
┌─────────────────────────────────────────────────────────────────┐
│                           ClubGUI                                │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ - clb: Club*                                                │ │
│  │ - currentTab: int                                           │ │
│  │ - showPlayerDetail: bool                                    │ │
│  │ - customFont: Font                                          │ │
│  ├─────────────────────────────────────────────────────────────┤ │
│  │ + run()                                                     │ │
│  │ + drawSidebar()                                             │ │
│  │ + drawPlayers()                                             │ │
│  │ + drawStatistics()                                          │ │
│  │ + drawAchievements()                                        │ │
│  │ + saveToJson()                                              │ │
│  │ + loadFromJson()                                            │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ uses
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                            Club                                  │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ - teams: MyVector<Team>                                     │ │
│  ├─────────────────────────────────────────────────────────────┤ │
│  │ + themTeam(Team)                                            │ │
│  │ + getTeams(): MyVector<Team>&                               │ │
│  │ + xoaCauThuKhoiClub(id)                                     │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ contains
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                            Team                                  │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ - idDoi, tenDoi: string                                     │ │
│  │ - hlvTruong, hlvPho: string                                 │ │
│  │ - cauThu: MyVector<Player>                                  │ │
│  │ - danhHieuTapThe: MyVector<TeamAward>                       │ │
│  ├─────────────────────────────────────────────────────────────┤ │
│  │ + themCauThu(Player)                                        │ │
│  │ + xoaCauThu(id)                                             │ │
│  │ + tongBanThang(): int                                       │ │
│  │ + capNhatDoiTruong(id)                                      │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ contains
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                          Person                                  │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ # id, ten, ngaySinh: string                                 │ │
│  │ # queQuan, thongTinLienLac: string                          │ │
│  ├─────────────────────────────────────────────────────────────┤ │
│  │ + getID(), getTen(), ...                                    │ │
│  │ + setTen(), setNgaySinh(), ...                              │ │
│  │ + virtual ~Person()                                         │ │
│  └─────────────────────────────────────────────────────────────┘ │
└──────────────────────────────┬──────────────────────────────────┘
                               │
                               │ inherits
                               ▼
┌─────────────────────────────────────────────────────────────────┐
│                          Player                                  │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ # maCauThu, viTri: string                                   │ │
│  │ # banThang, soAo: int                                       │ │
│  │ # mucLuong: double                                          │ │
│  │ # lichSuTranDau: MyVector<MatchRecord>                      │ │
│  │ # danhSachGiaiThuong: MyVector<Award>                       │ │
│  ├─────────────────────────────────────────────────────────────┤ │
│  │ + getSoAo(), setSoAo()                                      │ │
│  │ + getLuong(), setLuong()                                    │ │
│  │ + addTranDau(), getTongBanThangTuLichSu()                   │ │
│  │ + addGiaiThuong(), removeGiaiThuong()                       │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📝 VI. KẾT LUẬN

### Điểm mạnh:

1. **Áp dụng đầy đủ 4 tính chất OOP**: Encapsulation, Inheritance, Polymorphism, Abstraction
2. **Tự cài đặt DSA cơ bản**: MyVector và MyMap không dùng STL
3. **Code có cấu trúc rõ ràng**: Phân chia thành các class/file hợp lý
4. **Sử dụng Template**: Tăng tính tái sử dụng code
5. **Iterator Pattern**: Hỗ trợ range-based for loop

### Có thể cải thiện:

1. Tách ClubGUI.cpp thành nhiều file nhỏ hơn
2. Thêm interface/abstract class cho các loại Person khác
3. Implement thêm các DSA như Tree, Graph cho tính năng nâng cao
4. Thêm unit test cho các class DSA

---

_Tài liệu phân tích - Cập nhật: 16/12/2025_
