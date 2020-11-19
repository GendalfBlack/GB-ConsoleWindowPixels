#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;

const char* INPUT_FILENAME = "ban hammer.bmp";

struct BmpSignature
{
    unsigned char data[2];
    BmpSignature() { data[0] = data[1] = 0; }
};
struct BmpHeader
{
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataOffset;
    BmpHeader() : fileSize(0), reserved1(0), reserved2(0), dataOffset(0) { }
};
void ReadHeader(ifstream& fin, BmpSignature& sig, BmpHeader& header)
{
    if (!fin)
        return;

    fin.seekg(0, ios::beg);

    fin.read((char*)&sig, sizeof(sig));
    fin.read((char*)&header, sizeof(header));
}

int main()
{
    //Get a console handle
    HWND myconsole = GetConsoleWindow();
    //Get a handle to device context
    HDC mydc = GetDC(myconsole);

    ifstream fin(INPUT_FILENAME, ios::binary);
    BmpSignature sig;
    BmpHeader hdr;
    ReadHeader(fin, sig, hdr);

    fin.seekg(hdr.dataOffset, 0);
    
    char *** pixels = new char**[100];
    for (int i = 0; i < 100; i++)
    {
        pixels[i] = new char*[100];
        for (int j = 0; j < 100; j++)
        {
            pixels[i][j] = new char[3];
        }
    }
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                pixels[i][j][k] = (char)255;
            }
        }
    }
    int x = 0, y = 0;
    int count = 0;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    while (fin >> r) {
        pixels[x][y][0] = r;
        fin >> g;
        pixels[x][y][1] = g;
        fin >> b;
        pixels[x][y][2] = b;

        x++;
        if (x > 99)
        {
            x = 0;
            y++;
        }
    }
    system("cls");
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            char * p = pixels[i][j];
            SetPixel(mydc, i, 100 - j, RGB((short)p[0], (short)p[1], (short)p[2]));
        }
    }
    
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,10 });
}