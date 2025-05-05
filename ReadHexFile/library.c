#include "library.h"
char cBufferData[50];
int cInteralBufferData[50];
HexFormData hexStruct;
/****************************************************************************************
 * Static functions, Static Variables
 ****************************************************************************************/

/****************************************************************************************
 * Name: ReadLine
 * @brief hàm đọc một dòng từ file
 * @param FILE *fp: truyền vào file cần đọc
 * @param char *buffer: lưu trữ dữ liệu đọc được từ file
 * @return 1 neu doc dong thanh cong
 * @return 0 neu doc den End Of FIle (EOF) va ko co data nao duoc doc
 ****************************************************************************************/
static char ReadLine(FILE *fp, char *pBufferData)
{
    int u32Count = 0;
    char cDataLine;

    while ((cDataLine = fgetc(fp)) != EOF && cDataLine != '\n')
    {
        pBufferData[u32Count++] = cDataLine;
    }
    pBufferData[u32Count] = '\0';

    return (cDataLine == EOF && u32Count == 0) ? 0 : 1;
}

/****************************************************************************************
 * Name: static uint8_t HexCharToByte
 * @brief chuyển đổi một ký tự hex thành giá trị số tương ứng
 * @param char c: 1 ký tự đại diện cho Hex (0->9, A->F, a->f)
 * @return
 ****************************************************************************************/
static uint8_t HexCharToByte(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 0;
}

/****************************************************************************************
 * Name: static uint8_t HexStringToByte
 * @brief chuyển đổi một chuỗi hex 2 ký tự thành giá trị byte (8 bit) tương ứng
 * @param char *hex: chuỗi ký tự đại diện cho 1 byte hex (2 ký tự).
 * @return
 ****************************************************************************************/
static uint8_t HexStringToByte(char *hex)
{
    return (HexCharToByte(hex[0]) * 16) + HexCharToByte(hex[1]);
}

/****************************************************************************************
 * Name: static uint16_t HexStringToWord
 * @brief chuyển đổi một chuỗi hex 4 ký tự thành giá trị word (16 bit) tương ứng.
 * @param char *hex: Chuỗi ký tc đại diện cho một word hex (4 ký tự).
 * @return
 ****************************************************************************************/
static uint16_t HexStringToWord(char *hex)
{
    return (HexStringToByte(hex) * 256) + HexStringToByte(hex + 2);
}

/****************************************************************************************
 * Global functions
 ****************************************************************************************/

void ConvertStringToHex(char *buff, int length)
{
    int tempBuffer[100], i, j = 0;

    for (i = 0; i < length; i++)
    {
        if (buff[i] >= '0' && buff[i] <= '9')
        {
            tempBuffer[i] = buff[i] - '0';
        }
        else if (buff[i] >= 'A' && buff[i] <= 'F')
        {
            tempBuffer[i] = buff[i] - 'A' + 10;
        }
        else if (buff[i] >= 'a' && buff[i] <= 'f')
        {
            tempBuffer[i] = buff[i] - 'a' + 10;
        }
        else
        {
            return;
        }

        if (i % 2 != 0)
        {
            cInteralBufferData[j] = (tempBuffer[i - 1] * 16) + tempBuffer[i];
            j++;
        }
    }
}
StatusReadHexFile ReadAllLine(FILE *fp)
{
    uint8_t u8CaculChecksum;
    FieldHexFiles State;
    int u32Count, u32DataCount;
    int cntLine = 0;

    while (ReadLine(fp, cBufferData))
    {
        for (int i = 0; i < sizeof(hexStruct.u8Data); i++)
        {
            hexStruct.u8Data[i] = 0;
        }
        cntLine++;
        State = START_CODE;

        for (u32Count = 0; u32Count < strlen(cBufferData); u32Count++)
        {
            switch (State)
            {
            case START_CODE:
                if (cBufferData[u32Count] != ':')
                {
                    return READ_FAILED;
                }
                else
                {
                    State = BYTE_COUNT;
                }
                break;

            case BYTE_COUNT:
                ConvertStringToHex(&cBufferData[u32Count], 2);
                hexStruct.u8ByteCount = cInteralBufferData[0];
                u32Count += 1;
                State = ADDRESS;
                break;

            case ADDRESS:
                ConvertStringToHex(&cBufferData[u32Count], 4);
                hexStruct.u8Addr[0] = cInteralBufferData[0];
                hexStruct.u8Addr[1] = cInteralBufferData[1];
                u32Count += 3;
                State = RECORD_TYPE;
                break;

            case RECORD_TYPE:
                ConvertStringToHex(&cBufferData[u32Count], 2);
                hexStruct.u8RecordType = cInteralBufferData[0];
                u32Count += 1;
                State = DATA;
                break;

            case DATA:
                ConvertStringToHex(&cBufferData[u32Count], hexStruct.u8ByteCount * 2);
                for (u32DataCount = 0; u32DataCount < hexStruct.u8ByteCount; u32DataCount++)
                {
                    hexStruct.u8Data[u32DataCount] = cInteralBufferData[u32DataCount];
                }
                u32Count += (hexStruct.u8ByteCount * 2) - 1;
                State = CHECKSUM;
                break;

            case CHECKSUM:
                ConvertStringToHex(&cBufferData[u32Count], 2);
                hexStruct.u8CheckSum = cInteralBufferData[0];
                State = DONE;
                break;

            case DONE:
                u8CaculChecksum = hexStruct.u8ByteCount + hexStruct.u8Addr[0] + hexStruct.u8Addr[1] + hexStruct.u8RecordType;
                for (u32DataCount = 0; u32DataCount < hexStruct.u8ByteCount; u32DataCount++)
                {
                    u8CaculChecksum += hexStruct.u8Data[u32DataCount];
                }
                u8CaculChecksum = ~u8CaculChecksum + 1U;

                if (u8CaculChecksum == hexStruct.u8CheckSum)
                {
                    if (hexStruct.u8RecordType == 0x01)
                    {
                        return READ_SUCCESSFULLY;
                    }
                }
                else
                {
                    return READ_FAILED;
                }
                break;

            default:
                return READ_FAILED;
            }
        }
    }
    return READ_SUCCESSFULLY;
}
void ConvertHexToBin(const char *hexFile, const char *binFile)
{
    FILE *fpHex = fopen(hexFile, "r");
    FILE *fpBin = fopen(binFile, "wb");

    if (!fpHex || !fpBin)
    {
        printf("Failed to open files!\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fpHex))
    {
        if (line[0] != ':')
            continue;

        uint8_t byteCount = HexStringToByte(&line[1]);
        uint16_t address = HexStringToWord(&line[3]);
        uint8_t recordType = HexStringToByte(&line[7]);

        if (recordType == 0x00)
        {
            for (int i = 0; i < byteCount; i++)
            {
                uint8_t data = HexStringToByte(&line[9 + (i * 2)]);
                fwrite(&data, sizeof(uint8_t), 1, fpBin);
            }
        }
        else if (recordType == 0x01)
        {
            break;
        }
    }

    fclose(fpHex);
    fclose(fpBin);
    printf("Chuyen doi Hex sang Binary thanh cong\n");
}
