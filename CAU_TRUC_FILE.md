# CẤU TRÚC DỰ ÁN - ĐÃ CHIA MODULE

## Tổng quan
Dự án đã được chia thành nhiều module để dễ quản lý và bảo trì.

## Cấu trúc file

### 1. **ClubGUI.h** - Header chung (156 dòng)
- Khai báo class `ClubGUI`
- Khai báo struct `Button`, `InputField`  
- Khai báo tất cả các phương thức
- **Vị trí**: `src/ClubGUI.h`

### 2. **JSON_XuLy.cpp** - Xử lý JSON (328 dòng)
- `loadFromJson()` - Đọc dữ liệu từ team.json
- `saveToJson()` - Lưu dữ liệu vào team.json
- `formatVND()` - Format số tiền VND
- **Vị trí**: `src/JSON_XuLy.cpp`

### 3. **ee.cpp** - File chính (3752 dòng) 
**Chứa tất cả implementation:**

#### Phần GUI Components (dòng 381-530)
- Màu sắc, Font
- `Button` struct
- `InputField` struct

#### Phần Class ClubGUI (dòng 531-3718)

**a) Constructor & Core Functions (dòng 531-795)**
- `ClubGUI()` - Khởi tạo
- `~ClubGUI()` - Destructor  
- `showMessage()` - Hiện thông báo
- `drawSidebar()` - Vẽ menu bên trái
- `drawHeader()` - Vẽ tiêu đề
- `drawMessage()` - Vẽ thông báo
- `drawScrollbar()` - Vẽ thanh cuộn

**b) Player Card (dòng 796-897)**
- `drawPlayerCard()` - Vẽ card cầu thủ

**c) Player Detail Popup (dòng 898-1115)**
- `drawPlayerDetailPopup()` - Popup CCCD chi tiết

**d) Teams Management (dòng 1116-1287)**
- `drawTeams()` - Quản lý đội bóng
- Thêm đội, xóa đội

**e) Players Management (dòng 1288-1877)**
- `drawPlayers()` - Quản lý cầu thủ
- Thêm, xóa, sửa cầu thủ
- Chọn đội trưởng/đội phó

**f) Edit Player Popup (dòng 1878-2047)**
- `drawEditPlayerPopup()` - Popup sửa thông tin cầu thủ

**g) Edit Team Popup (dòng 2048-2140)**
- `drawEditTeamPopup()` - Popup đổi tên đội

**h) Edit HLV Popup (dòng 2141-2217)**
- `drawEditHLVPopup()` - Popup sửa HLV

**i) Search Function (dòng 2218-2493)**
- `drawSearch()` - Tìm kiếm cầu thủ
- Hiển thị kết quả

**j) Achievements (dòng 2494-2871)**
- `drawAchievements()` - Quản lý thành tích các năm
- Cập nhật danh hiệu, số trận, thẻ vàng/đỏ

**k) Health Management (dòng 2872-3420)**
- `drawHealth()` - Quản lý sức khỏe
- 4 tab: Tổng quan, Thể lực, Hiệu suất, Tập luyện, Kỷ luật

**l) Statistics (dòng 3421-3635)**
- `drawStats()` - Thống kê tổng quan
- Top cầu thủ, biểu đồ

**m) Main Run Function (dòng 3636-3718)**
- `run()` - Vòng lặp chính
- Xử lý sự kiện, vẽ GUI

### 4. **main()** (dòng 3719-3752)
- Khởi tạo Club
- Gọi `loadFromJson()`  
- Chạy `gui.run()`

## Models (src/models/)
- **Person.h** - Class cha
- **Player.h** - Class cầu thủ (kế thừa Person)
- **Team.h** - Class đội bóng
- **Club.h** - Class câu lạc bộ

## Data Structures (src/)
- **MyVector.h** - Vector tự implement
- **MyMap.h** - Hash map tự implement

## Tính chất OOP
✅ **Encapsulation** - Dữ liệu private, accessor public
✅ **Inheritance** - Player kế thừa Person
✅ **Polymorphism** - Virtual destructor
✅ **Abstraction** - Template class MyVector<T>, MyMap<K,V>

## Cấu trúc dữ liệu tự implement
✅ **MyVector<T>** - Dynamic array với iterator
✅ **MyMap<K,V>** - Hash table với open addressing

## Compile & Run
```bash
make
./ee
```

## Ghi chú
- File ee.cpp được giữ nguyên để đảm bảo code chạy ổn định
- Đã có file JSON_XuLy.cpp tách riêng để demo cấu trúc module
- Có thể dễ dàng tách thêm các phần khác nếu cần trong tương lai
