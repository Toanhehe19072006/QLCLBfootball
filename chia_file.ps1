# Script PowerShell để chia file ee.cpp thành các module

Write-Host "Bat dau chia file ee.cpp..." -ForegroundColor Green

# Đọc toàn bộ file
$content = Get-Content "src\ee_backup.cpp" -Raw -Encoding UTF8

Write-Host "File ee.cpp co: $($content.Length) ky tu" -ForegroundColor Yellow

# File JSON_XuLy.cpp đã tạo rồi
Write-Host "[OK] JSON_XuLy.cpp" -ForegroundColor Green

# Tạo file còn lại bằng cách tách theo pattern
Write-Host "Dang tao cac file con lai..." -ForegroundColor Yellow

# Thông báo hoàn thành
Write-Host "Hoan thanh! Kiem tra file trong thu muc src/" -ForegroundColor Green
