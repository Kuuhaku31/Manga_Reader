
#include "header.h"

char inputImagePathA[] = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\input1.avif";
char inputImagePathB[] = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\input2.avif";
char outputPath[]      = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\";
char outputImageName[] = "output";

int
main()
{
    // AvifTest();
    // EasyXTest();
    // DecodeAvif(inputImagePath, outputImagePath);
    // CodePngByEasyX(outputBinPath, imageW, imageH, outputImagePath);

    // DecodeAvifToPng(inputImagePath, outputImagePath);
    CombineAvifToPng(inputImagePathA, inputImagePathB, outputPath, outputImageName);

    return 0;
}
