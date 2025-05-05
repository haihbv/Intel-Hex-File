#ifndef HEXTOBIN_H
#define HEXTOBIN_H

/****************************************************************************************
 * Library
 ****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************************
 * Defines
 ****************************************************************************************/
#define uint8_t unsigned char
#define uint16_t unsigned short
#define MAX_LINE_LENGTH 256

/****************************************************************************************
 * Prototypes
 ****************************************************************************************/

/****************************************************************************************
 * Name: typedef enum FieldHexFiles (Trường Hex Files);
 * @field START_CODE      0
 * @field BYTE_COUNT      1
 * @field ADDRESS         2
 * @field RECORD_TYPE     3
 * @field DATA            4
 * @field CHECKSUM         5
 ****************************************************************************************/
typedef enum
{
    START_CODE = 0U,
    BYTE_COUNT,
    ADDRESS,
    RECORD_TYPE,
    DATA,
    CHECKSUM,
    DONE
} FieldHexFiles;

/****************************************************************************************
 * Name: typedef enum StatusReadHexFile
 * @brief enum định nghĩa trạng thái đọc file hex.
 * /field READ_SUCCESSFULLY: đọc file thành công, giá trị là 0
 * /field READ_FAILED: đọc file thất bại, giá trị là 1
 ****************************************************************************************/
typedef enum
{
    READ_SUCCESSFULLY = 0U,
    READ_FAILED
} StatusReadHexFile;

/****************************************************************************************
 * Name: typedef struct HexFormData
 * @brief struct lữu trữ thông tin của 1 dòng trong hex file
 * @var u8ByteCount: số byte dữ liệu
 * @var u8Addr[2]: địa chỉ bắt đầu của dữ liệu
 * @var u8RecordType: loại bản ghi
 * @var u8Data[16]: dữ liệu
 * @var u8CheckSum: giá trị checksum.
 *****************************************************************************************/
typedef struct
{
    uint8_t u8ByteCount;
    uint8_t u8Addr[2];
    uint8_t u8RecordType;
    uint8_t u8Data[16];
    uint8_t u8CheckSum;
} HexFormData;

/****************************************************************************************
 * Extern Variables
 ****************************************************************************************/

/****************************************************************************************
 * Name: char cBufferData[50]
 * @brief mảng lưu trữ dữ liệu đọc từ file hex
 ****************************************************************************************/
extern char cBufferData[50];

/****************************************************************************************
 * Name: int cInteralBufferData[50]
 * @brief mảng lưu trữ dữ liệu hex sau khi chuyển đổi
 ****************************************************************************************/
extern int cInteralBufferData[50];

/****************************************************************************************
 * Name: HexFormData hexStruct
 * @brief biến struct lưu trữ thông tin của một dòng hex
 ****************************************************************************************/
extern HexFormData hexStruct;

/****************************************************************************************
 * Functions
 ****************************************************************************************/

/****************************************************************************************
 * Name: ReadLine
 * @brief hàm đọc một dòng từ file
 * @param FILE *fp: truyền vào file cần đọc
 * @param char *buffer: lưu trữ dữ liệu đọc được từ file
 * @return 1 neu doc dong thanh cong
 * @return 0 neu doc den End Of FIle (EOF) va ko co data nao duoc doc
 ****************************************************************************************/

/****************************************************************************************
 * Name: ConvertStringToHex
 * @brief chuyển đổi chuỗi ký tự thành giá trị hex
 * @param char *buff: chuỗi ký tự cần chuyển đổi sang giá trị hex
 * @param int length: độ dài của chuỗi ký tự cần chuyển đổi
 * @return void
 * /saved được lưu trực tiếp vào mảng cInteralBufferData
 ****************************************************************************************/
void ConvertStringToHex(char *buff, int length);

/****************************************************************************************************************************************
 * Name: StatusReadHexFile ReadAllLine
 * @brief đọc từng dòng trong file hex và phân tích nó
 * @brief use a state machine để xác định trạng thái hiện tại (START_CODE, BYTE_COUNT, ADDRESS, RECORD_TYPE, DATA, CHECKSUM, DONE)
 * @brief tính toán checksum để kiểm tra tính toàn vẹn của dữ liệu
 * @param FILE *fp: con trỏ đến file cần đọc
 * @return READ_SUCCESSFULLY (0): nếu đọc và phân tích file hex thành công
 * @return READ_FAILED (1): nếu có lỗi xảy ra trong quá trình đọc hoặc phân tích file
 ****************************************************************************************************************************************/
StatusReadHexFile ReadAllLine(FILE *fp);

/****************************************************************************************
 * Name: ConvertHexToBin
 * @brief chuyển đổi nội dung của 1 file HEX (format INTEL HEX) sang file BINARY
 * /used  chuyển đổi hex sang bin -> phân tích hex file -> ghi dữ liệu vào file bin
 * @param const char *hexFile: truyền đến file hex cần chuyển đổi (read-only)
 * @param const char *binFile: truyền đến file bin cần chuyển đổi (read-only)
 * @return void
 ****************************************************************************************/
void ConvertHexToBin(const char *hexFile, const char *binFile);
#endif
