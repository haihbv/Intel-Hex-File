# Intel-Hex-File

Tìm hiểu cấu trúc file HEX và cách áp dụng trong lập trình Vi điều khiển.  
Định dạng file HEX được sử dụng rộng rãi trong việc nạp chương trình cho vi điều khiển.  

---

## Các loại file nhị phân cơ bản

### 1. Định dạng .BIN
- Đây là định dạng đơn giản nhất.
- Không chứa thông tin bổ sung hay relocation.
- Các lệnh được tải trực tiếp đến các địa chỉ cố định trong bộ nhớ.

### 2. Định dạng .ELF (Executable and Linkable Format)
- Chứa thông tin về các symbol, relocation, và hỗ trợ việc nạp chương trình vào các địa chỉ tùy ý.
- ELF thường được sử dụng trong quá trình debug.

### 3. Định dạng .HEX
- Định dạng phổ biến, đặc biệt với các dòng vi điều khiển 8-bit.

---

## Tại sao cần phải biết cấu trúc file HEX?

Trong các ứng dụng Bootloader, việc nạp code không thể phụ thuộc vào các công cụ IDE như thường lệ (Keil C, STM32Cube Programmer).  
Thay vào đó, cần:
- Nạp code qua giao thức không dây (Wi-Fi, Bluetooth) hoặc có dây (UART, SPI, I2C).
- Phân tích dữ liệu từ file HEX và ghi trực tiếp vào bộ nhớ FLASH của vi điều khiển.
- Xử lý các lỗi liên quan đến checksum.

---

## Cấu trúc file HEX

| Thành phần      | Kích thước   | Ý nghĩa                                                                 |
|------------------|--------------|-------------------------------------------------------------------------|
| **Start Code**   | 1 ký tự       | Luôn bắt đầu bằng dấu 2 chấm (`:`).                                     |
| **Byte Count**   | 1 byte (2 số) | Số byte dữ liệu (data) trên dòng hiện tại.                             |
| **Address**      | 2 byte (4 số) | Địa chỉ offset của dữ liệu. Địa chỉ thực tế = Base Address + Address.   |
| **Record Type**  | 1 byte        | Xác định loại dữ liệu.                                                  |
| **Data**         | N byte        | Dữ liệu được nạp vào bộ nhớ FLASH.                                     |
| **Checksum**     | 1 byte        | Giá trị dùng để kiểm tra tính chính xác của dòng dữ liệu.              |

---

## Các loại Record Type thường gặp

- **00**: Dữ liệu thông thường.  
- **01**: Kết thúc file HEX.  
- **02**: Địa chỉ cơ sở (Base Address) cho các đoạn dữ liệu.  
- **04**: Địa chỉ mở rộng (Extended Linear Address).  
- **05**: Địa chỉ khởi động (Start Linear Address).  

---

## Phân tích một dòng Hex File

Ví dụ: `:0300300002337A1E`

| Thành phần      | Giá trị  | Ý nghĩa                              |
|------------------|----------|--------------------------------------|
| **Start Code**   | `:`      | Bắt đầu dòng dữ liệu.                |
| **Byte Count**   | `03`     | 3 byte dữ liệu.                      |
| **Address**      | `0030`   | Offset 0x0030.                      |
| **Record Type**  | `00`     | Dữ liệu thông thường.                |
| **Data**         | `02337A` | Dữ liệu cần nạp vào bộ nhớ.          |
| **Checksum**     | `1E`     | Giá trị kiểm tra tính chính xác.     |

### Tính toán Checksum:
1. Tổng tất cả các byte (ngoại trừ checksum):  
   `03 + 00 + 30 + 00 + 02 + 33 + 7A = E2`.
2. Đảo bit của `E2`: `1D`.
3. Cộng thêm 1: `1E`.

---

## Tài liệu tham khảo
- [Intel HEX - Wikipedia](https://en.wikipedia.org/wiki/Intel_HEX)