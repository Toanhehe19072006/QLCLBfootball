# HƯỚNG DẪN SỬ DỤNG HỆ THỐNG GIẢI THƯỞNG CÁ NHÂN

## 1. Cách thêm giải thưởng trong ứng dụng

### Bước 1: Vào tab "Thành Tích"
- Chọn cầu thủ muốn thêm giải thưởng
- Nhấn nút **"GIẢI THƯỞNG CÁ NHÂN"** (màu vàng)

### Bước 2: Nhấn nút "THÊM"
- Nhập **Tên giải thưởng**: VD "Quả bóng vàng", "Vua phá lưới", "Cầu thủ xuất sắc nhất"
- Nhập **Năm**: VD 2024, 2025
- Chọn **Xếp hạng**: Top 1 (Vàng), Top 2 (Bạc), hoặc Top 3 (Đồng)

### Bước 3: Nhấn "LƯU"
- Hệ thống sẽ kiểm tra trùng lặp
- **LƯU Ý**: Mỗi giải thưởng với cùng tên, cùng năm, cùng xếp hạng chỉ được trao cho 1 cầu thủ trong toàn CLB

### Ví dụ giải thưởng phổ biến:
- **Quả bóng vàng** (Top 1, 2, 3)
- **Quả bóng bạc** (Top 1, 2, 3)
- **Quả bóng đồng** (Top 1, 2, 3)
- **Vua phá lưới** (Top 1, 2, 3)
- **Cầu thủ xuất sắc nhất tháng**
- **Cầu thủ triển vọng**
- **Thủ môn xuất sắc nhất**

---

## 2. Cách nhập giải thưởng qua file JSON

### Format JSON mới (từ phiên bản hiện tại):

```json
{
  "teams": [
    {
      "teamName": "Đội A",
      "players": [
        {
          "id": "P001",
          "name": "Nguyễn Văn A",
          "playerID": "CT001",
          "position": "Tiền đạo",
          
          "danhSachGiaiThuong": [
            {
              "tenGiai": "Quả bóng vàng",
              "nam": 2024,
              "xepHang": 1,
              "ghiChu": "Giải thưởng cao nhất"
            },
            {
              "tenGiai": "Vua phá lưới",
              "nam": 2024,
              "xepHang": 2,
              "ghiChu": ""
            },
            {
              "tenGiai": "Cầu thủ xuất sắc nhất tháng 12",
              "nam": 2023,
              "xepHang": 1,
              "ghiChu": ""
            }
          ]
        }
      ]
    }
  ]
}
```

### Giải thích các trường:

- **tenGiai** (string): Tên giải thưởng
- **nam** (number): Năm nhận giải (VD: 2024, 2025)
- **xepHang** (number): Xếp hạng
  - `1` = Top 1 / Vàng
  - `2` = Top 2 / Bạc
  - `3` = Top 3 / Đồng
- **ghiChu** (string): Ghi chú thêm (có thể để trống "")

---

## 3. Chuyển đổi từ dữ liệu cũ

### Format cũ (tự động chuyển đổi):
```json
"danhSachDanhHieu": [
  "Quả bóng vàng 2024",
  "Vua phá lưới"
]
```

### Sẽ được chuyển thành:
```json
"danhSachGiaiThuong": [
  {
    "tenGiai": "Quả bóng vàng 2024",
    "nam": 2024,
    "xepHang": 1,
    "ghiChu": "Chuyển đổi từ dữ liệu cũ"
  },
  {
    "tenGiai": "Vua phá lưới",
    "nam": 2024,
    "xepHang": 1,
    "ghiChu": "Chuyển đổi từ dữ liệu cũ"
  }
]
```

**LƯU Ý**: Sau khi chương trình load và tự động chuyển đổi, hãy vào từng cầu thủ để sửa lại năm và xếp hạng cho chính xác.

---

## 4. Quy tắc unique (không trùng lặp)

### Quy tắc:
Trong cùng 1 năm, không có 2 cầu thủ nhận cùng 1 giải thưởng với cùng xếp hạng.

### Ví dụ hợp lệ:
✅ Cầu thủ A: "Quả bóng vàng" - Năm 2024 - Top 1
✅ Cầu thủ B: "Quả bóng vàng" - Năm 2024 - Top 2
✅ Cầu thủ C: "Quả bóng vàng" - Năm 2025 - Top 1

### Ví dụ KHÔNG hợp lệ:
❌ Cầu thủ A: "Quả bóng vàng" - Năm 2024 - Top 1
❌ Cầu thủ B: "Quả bóng vàng" - Năm 2024 - Top 1  ← BỊ TỪ CHỐI

---

## 5. Hiển thị trong ứng dụng

### Tab "Trang chủ" - Panel "Top Giải Thưởng Cá Nhân":
- Hiển thị giải thưởng cao nhất (ưu tiên Top 1 mới nhất)
- Format: "Tên giải Vàng/Bạc/Đồng (năm)"

### Tab "Thành tích" - Chi tiết cầu thủ:
- Hiển thị top 3 giải thưởng gần nhất
- Có thể xem đầy đủ bằng cách nhấn nút "GIẢI THƯỞNG CÁ NHÂN"

### Popup "Top Giải Thưởng Cá Nhân":
- Hiển thị tất cả giải thưởng
- Tự động sắp xếp: Năm mới nhất → Xếp hạng cao nhất
- Màu sắc: Vàng (1), Bạc (2), Đồng (3)

---

## 6. Ví dụ đầy đủ

```json
{
  "clubName": "Manchester United",
  "teams": [
    {
      "teamName": "Đội 1",
      "players": [
        {
          "id": "P001",
          "name": "Cristiano Ronaldo",
          "playerID": "CR7",
          "position": "Tiền đạo",
          "soAo": 7,
          
          "danhSachGiaiThuong": [
            {
              "tenGiai": "Quả bóng vàng",
              "nam": 2024,
              "xepHang": 1,
              "ghiChu": "Giải thưởng cá nhân cao nhất"
            },
            {
              "tenGiai": "Vua phá lưới",
              "nam": 2024,
              "xepHang": 1,
              "ghiChu": "45 bàn thắng"
            },
            {
              "tenGiai": "Cầu thủ xuất sắc nhất năm",
              "nam": 2023,
              "xepHang": 2,
              "ghiChu": ""
            }
          ]
        },
        {
          "id": "P002",
          "name": "Lionel Messi",
          "playerID": "LM10",
          "position": "Tiền vệ tấn công",
          "soAo": 10,
          
          "danhSachGiaiThuong": [
            {
              "tenGiai": "Quả bóng vàng",
              "nam": 2024,
              "xepHang": 2,
              "ghiChu": "Á quân"
            },
            {
              "tenGiai": "Quả bóng vàng",
              "nam": 2023,
              "xepHang": 1,
              "ghiChu": "Vô địch World Cup"
            }
          ]
        }
      ]
    }
  ]
}
```

---

## 7. Lỗi thường gặp

### Lỗi: "Giải thưởng này đã được trao cho [Tên cầu thủ]"
**Nguyên nhân**: Đã có cầu thủ khác nhận giải này với cùng năm và xếp hạng
**Giải pháp**: Đổi xếp hạng (1→2→3) hoặc đổi năm

### Lỗi: Không hiển thị giải thưởng
**Nguyên nhân**: 
- Sai format JSON (thiếu dấu phay, ngoặc)
- Trường `xepHang` không phải số 1, 2, hoặc 3
**Giải pháp**: Kiểm tra lại cú pháp JSON

### Lỗi: Giải thưởng bị mất sau khi khởi động lại
**Nguyên nhân**: Chưa lưu file (chương trình tự động lưu khi thay đổi)
**Giải pháp**: Kiểm tra file `team.json` đã được cập nhật chưa

---

## 8. Tips & Tricks

### Đặt tên giải thưởng chuẩn:
- Dùng tên đầy đủ: "Quả bóng vàng" thay vì "QBV"
- Tránh ký tự đặc biệt: @, #, $, %
- Có thể dùng tiếng Việt có dấu

### Quản lý năm:
- Dùng năm thực tế khi trao giải
- Có thể tạo giải cho nhiều năm khác nhau

### Xếp hạng:
- Top 1 = Vàng = Giải nhất
- Top 2 = Bạc = Giải nhì  
- Top 3 = Đồng = Giải ba

### Ghi chú:
- Có thể ghi thêm thông tin như: số bàn thắng, số kiến tạo, lý do nhận giải
- Không bắt buộc, có thể để trống

---

**Phiên bản**: 2.0 - Hệ thống giải thưởng cá nhân
**Cập nhật**: 06/12/2025
