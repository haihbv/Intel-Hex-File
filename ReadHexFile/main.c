#include <stdio.h>
#include "library.h"

int main()
{
    FILE *f = fopen("makefile.hex", "r");
    if (!f)
    {
        printf("Failed to open hex file!\n");
        return 1;
    }
    /* Read in Hex file. */
    StatusReadHexFile status = ReadAllLine(f);
    if (status == READ_SUCCESSFULLY)
        printf("File hex da duoc doc va phan tich thanh cong\n");
    else
        printf("Khong the doc hoac phan tich file hex\n");
    /* Close file. */
    fclose(f);

    ConvertHexToBin("makefile.hex", "output.bin");
    return 0;
}
