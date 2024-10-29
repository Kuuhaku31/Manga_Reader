
#include "header.h"

char inputImagePath[]  = "D:\\Project\\FishTank\\Manga_Reader\\avifto\\images\\input.avif";
char outputBinPath[]   = "D:\\Project\\FishTank\\Manga_Reader\\avifto\\images\\bin";
char outputImagePath[] = "D:\\Project\\FishTank\\Manga_Reader\\avifto\\images\\output.png";

int imageW = 1354;
int imageH = 1920;

int
main()
{
    // AvifTest();
    // EasyXTest();
    // DecodeAvif(inputImagePath, outputImagePath);
    // CodePngByEasyX(outputBinPath, imageW, imageH, outputImagePath);

    DecodeAvifToPng(inputImagePath, outputImagePath);

    return 0;
}
