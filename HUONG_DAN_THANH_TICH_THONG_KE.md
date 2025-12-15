# HƯỚNG DẪN SỬ DỤNG TAB THÀNH TÍCH & THỐNG KÊ

## 📊 TAB THỐNG KÊ (Statistics)

### 1. Tổng quan

Tab Thống kê hiển thị các số liệu tổng hợp của câu lạc bộ, bao gồm:

- **Tổng số cầu thủ**
- **Tổng bàn thắng** (tính từ lịch sử đá bóng)
- **Tổng lương**

### 2. Lọc theo năm

Có 5 nút lọc ở phía trên:

- **Tất cả**: Hiển thị tổng hợp tất cả các năm
- **2022**: Chỉ hiển thị số liệu năm 2022
- **2023**: Chỉ hiển thị số liệu năm 2023
- **2024**: Chỉ hiển thị số liệu năm 2024
- **2025**: Chỉ hiển thị số liệu năm 2025

### 3. Cách tính bàn thắng

**Bàn thắng được tính tự động từ LỊCH SỬ ĐÁ BÓNG:**

- Hệ thống đọc ngày thi đấu (format: dd/mm/yyyy)
- Trích xuất năm từ ngày
- Tổng hợp số bàn thắng (BT) theo năm được chọn

**Ví dụ:**

```
Trận 19/07/2023 - BT: 2
Trận 10/09/2023 - BT: 1
Trận 15/03/2024 - BT: 3

→ Chọn "2023": Bàn thắng = 3 (2+1)
→ Chọn "2024": Bàn thắng = 3
→ Chọn "Tất cả": Bàn thắng = 6 (2+1+3)
```

### 4. TOP GHI BÀN

Panel bên trái hiển thị danh sách cầu thủ ghi nhiều bàn nhất:

- Sắp xếp theo số bàn thắng giảm dần
- Có biểu đồ thanh ngang trực quan
- Lọc theo năm được chọn

### 5. TOP GIẢI THƯỞNG CÁ NHÂN

Panel bên phải hiển thị các cầu thủ có giải thưởng:

- Quả bóng vàng, Quả bóng bạc, Quả bóng đồng
- Vua phá lưới
- Cầu thủ xuất sắc nhất
- v.v...

Màu sắc badge giải thưởng:

- 🟡 **Vàng**: Giải có chữ "vàng" trong tên
- ⚪ **Bạc**: Giải có chữ "bạc" trong tên
- 🟤 **Đồng**: Giải có chữ "đồng" trong tên
- 🟠 **Cam**: Các giải khác

---

## 🏆 TAB THÀNH TÍCH (Achievements)

### 1. Giao diện chính

- **Danh sách cầu thủ** ở bên trái
- **Chi tiết cầu thủ** ở bên phải khi chọn

### 2. Các nút chức năng

#### 📝 DANH HIỆU CHUNG (góc trên phải - màu tím)

Quản lý danh hiệu tập thể của các đội:

- V-League, Cup Quốc gia, AFC Champions League...
- Thêm/xóa danh hiệu cho từng đội

#### 📜 LỊCH SỬ ĐÁ BÓNG (màu vàng)

Quản lý lịch sử trận đấu của từng cầu thủ:

- **Ngày**: Ngày thi đấu (dd/mm/yyyy)
- **Đối thủ**: Tên đội đối thủ
- **Giải đấu**: Tên giải/cup
- **BT**: Số bàn thắng
- **KT**: Số kiến tạo
- **TV**: Thẻ vàng (0 hoặc 1)
- **TĐ**: Thẻ đỏ (0 hoặc 1)
- **Ghi chú**: Ghi chú thêm

⚠️ **QUAN TRỌNG**: Dữ liệu từ đây được dùng để tính thống kê!

#### 🏅 GIẢI THƯỞNG (màu tím)

Quản lý giải thưởng cá nhân của cầu thủ:

- **Tên giải thưởng**: VD: "Quả bóng Vàng", "Vua phá lưới"
- **Năm**: Năm nhận giải

**Tip**: Thêm "Vàng/Bạc/Đồng" vào tên giải để hiển thị màu tương ứng trong thống kê.

### 3. Thông tin hiển thị cho mỗi cầu thủ

#### Thống kê từ lịch sử trận đấu:

- Số trận thi đấu
- Bàn thắng
- Kiến tạo
- Thẻ vàng
- Thẻ đỏ

#### Giải thưởng:

- Hiển thị top 3 giải thưởng gần nhất

---

## 📝 QUY TRÌNH NHẬP LIỆU

### Bước 1: Thêm lịch sử đá bóng

1. Vào tab **Thành tích**
2. Chọn cầu thủ từ danh sách
3. Click **LỊCH SỬ ĐÁ BÓNG**
4. Click **THÊM** để thêm trận mới
5. Điền thông tin: Ngày, Đối thủ, Giải đấu, BT, KT, TV, TĐ
6. Click **LƯU**

### Bước 2: Thêm giải thưởng (nếu có)

1. Click **GIẢI THƯỞNG**
2. Click **THÊM**
3. Điền: Tên giải thưởng, Năm
4. Click **LƯU**

### Bước 3: Xem thống kê

1. Vào tab **Thống kê**
2. Chọn năm muốn xem hoặc "Tất cả"
3. Xem số liệu tổng hợp và top ghi bàn

---

## ⚠️ LƯU Ý QUAN TRỌNG

1. **Format ngày**: Phải nhập đúng format **dd/mm/yyyy** (VD: 15/12/2025)
2. **Năm lọc**: Chỉ lọc được các năm 2022-2025. Trận đấu năm khác chỉ hiển thị khi chọn "Tất cả"
3. **Tự động lưu**: Dữ liệu được lưu tự động vào file JSON
4. **Backup**: Nên backup file `team.json` thường xuyên

---

## 🔧 XỬ LÝ SỰ CỐ

### Bàn thắng hiển thị 0?

- Kiểm tra đã nhập lịch sử đá bóng chưa
- Kiểm tra format ngày đúng dd/mm/yyyy chưa
- Kiểm tra năm lọc có phù hợp không

### Giải thưởng không hiển thị?

- Kiểm tra đã thêm giải thưởng cho cầu thủ chưa
- Kiểm tra năm giải thưởng có khớp với năm lọc không

---

_Cập nhật: 15/12/2025_
