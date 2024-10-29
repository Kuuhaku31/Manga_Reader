
#include "header.h"

char inputImagePath[]  = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\input.avif";
char outputBinPath[]   = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\bin";
char outputImagePath[] = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\output.png";

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
