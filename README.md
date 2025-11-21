# ⚽ Quản Lý Câu Lạc Bộ Bóng Đá (QLCLBfootball)

Phần mềm quản lý câu lạc bộ bóng đá chuyên nghiệp được xây dựng bằng **C++** và thư viện đồ họa **raylib 5.5**.

## ✨ Tính năng chính

### 📋 Quản lý cơ bản
- **Đội bóng**: Thêm, sửa, xóa thông tin đội bóng và huấn luyện viên
- **Cầu thủ**: Quản lý hồ sơ đầy đủ (CCCD, địa chỉ, lương, vị trí, vai trò)
- **Vai trò**: Phân quyền đội trưởng/đội phó

### 🏥 Sức khỏe & Thể lực
- Theo dõi tình trạng sức khỏe
- Quản lý chấn thương và dự kiến hồi phục
- Đánh giá thể lực (chiều cao, cân nặng, tỉ lệ mỡ)

### 🏆 Thành tích & Thống kê
- **Thành tích career**: Số trận, bàn thắng cho CLB, kiến tạo
- **Thẻ phạt**: Theo dõi thẻ vàng, thẻ đỏ
- **Hiệu suất theo năm**: Bàn thắng, kiến tạo, chuyền dứt điểm
- **Điểm phong độ**: Đánh giá hiệu suất cầu thủ

### 🔍 Tiện ích
- Tìm kiếm cầu thủ nhanh
- Kiểm tra trùng lặp mã đội/cầu thủ
- Giao diện tiếng Việt đầy đủ
- Hỗ trợ UTF-8 và font tiếng Việt

## 🛠️ Yêu cầu hệ thống

- **OS**: Windows 10/11
- **Compiler**: MinGW-w64 (GCC) với hỗ trợ C++14
- **Library**: raylib 5.5
- **Tools**: VS Code (khuyến nghị)

## 📦 Cài đặt

### 1. Cài đặt raylib và w64devkit
```bash
# Tải raylib từ: https://www.raylib.com/
# Giải nén vào: C:/raylib/
# Cấu trúc:
# C:/raylib/
#   ├── raylib/        (thư viện)
#   └── w64devkit/     (compiler)
```

### 2. Clone repository
```bash
git clone https://github.com/Toanhehe19072006/QLCLBfootball.git
cd QLCLBfootball
```

### 3. Biên dịch
```bash
# Debug build
mingw32-make RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=ee OBJS=src/*.cpp BUILD_MODE=DEBUG

# Release build
mingw32-make RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=ee OBJS=src/*.cpp
```

### 4. Chạy chương trình
```bash
./ee.exe
```

## 📁 Cấu trúc dự án

```
QLCLBfootball/
├── src/                    # Mã nguồn
│   ├── ee.cpp             # File chính (3600+ dòng)
│   ├── models/            # Các class model
│   │   ├── Club.h        # Quản lý câu lạc bộ
│   │   ├── Team.h        # Quản lý đội bóng
│   │   ├── Player.h      # Quản lý cầu thủ
│   │   └── Person.h      # Class cơ sở
│   ├── MyVector.h         # Custom vector container
│   ├── MyMap.h            # Custom map container
│   ├── json.hpp           # JSON parser (nlohmann)
│   └── utf8_helper.h      # Hỗ trợ UTF-8
├── assets/
│   ├── fonts/             # Font tiếng Việt
│   └── players/           # Ảnh cầu thủ
├── team.json              # Dữ liệu đội bóng
├── Makefile               # Build script
└── README.md
```

## ⌨️ Phím tắt

| Phím | Chức năng |
|------|-----------|
| `F11` | Bật/tắt toàn màn hình |
| `Chuột giữa` | Đổi vai trò cầu thủ (đội trưởng/đội phó) |
| `Ctrl + S` | Lưu nhanh trong các form |
| `ESC` | Đóng popup/hủy thao tác |

## 🎨 Giao diện

Giao diện được thiết kế với 7 tab chính:
1. **TỔNG QUAN**: Dashboard hiển thị thống kê tổng thể
2. **ĐỘI BÓNG**: Danh sách đội bóng và quản lý HLV
3. **CẦU THỦ**: Thêm, sửa, xóa cầu thủ
4. **SỨC KHỎE**: Theo dõi tình trạng sức khỏe
5. **THÀNH TÍCH**: Quản lý thành tích career
6. **THỐNG KÊ**: Hiệu suất theo năm
7. **TÌM KIẾM**: Tìm kiếm cầu thủ

## 💾 Định dạng dữ liệu

Dữ liệu được lưu trong file `team.json` với cấu trúc:
```json
{
  "teams": [
    {
      "id": "VN01",
      "name": "Đội tuyển QGVN",
      "hlvTruong": "Trần Công Minh",
      "hlvPho": "Lê Huy Khoa",
      "players": [...]
    }
  ]
}
```

## 🔧 Tính năng kỹ thuật

- **Input fields**: Hỗ trợ multiline, UTF-8, validation
- **Duplicate check**: Kiểm tra trùng mã (case-insensitive)
- **Scroll view**: Danh sách cuộn mượt mà
- **Responsive**: Tự động điều chỉnh khi thay đổi kích thước cửa sổ
- **Color coding**: Màu sắc phân biệt vai trò, trạng thái

## 👨‍💻 Tác giả

**Nguyễn Thái Toàn** (Toanhehe19072006)

## 📄 License

MIT License - Tự do sử dụng và chỉnh sửa

---

⭐ Nếu thấy hữu ích, hãy star repo này nhé!
