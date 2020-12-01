#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
using namespace std;

/// <summary>
/// BMP fileinfo structure https://ru.wikipedia.org/wiki/BMP#/media/%D0%A4%D0%B0%D0%B9%D0%BB:BMPfileFormat.png
/// </summary>
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
    unsigned int headsize;
    unsigned int width;
    unsigned int height;
    BmpHeader() : fileSize(0), reserved1(0), reserved2(0), dataOffset(0), width(0), height(0) {}
};

class SpriteBMP {
public: 
    int width;
    int height;
    unsigned char*** raw_bytes;
    BmpHeader hdr;
    string name_of_file;

    SpriteBMP(string name_of_file) : width(0), height(0) {
        const char* INPUT_FILENAME = name_of_file.c_str();
        this->name_of_file = name_of_file;
        ifstream fin(INPUT_FILENAME, ios::binary);
        if (fin.good())
        {
            BmpSignature sig;
            if (!fin)
                return;
            fin.seekg(0, ios::beg);
            fin.read((char*)&sig, sizeof(sig));
            fin.read((char*)&hdr, sizeof(hdr));

            width = hdr.width;
            height = hdr.height;
            raw_bytes = new unsigned char** [width];
            for (int i = 0; i < width; i++)
            {
                raw_bytes[i] = new unsigned char* [height];
                for (int j = 0; j < height; j++)
                {
                    raw_bytes[i][j] = new unsigned char[3];
                    for (int k = 0; k < 3; k++)
                    {
                        raw_bytes[i][j][k] = ' ';
                    }
                }
            }
            fin.seekg(hdr.dataOffset, ios::beg);
            unsigned char trash_can;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    fin >> this->raw_bytes[x][y][0];
                    fin >> this->raw_bytes[x][y][1];
                    fin >> this->raw_bytes[x][y][2];
                }
                if ((width * 3) % 4 != 0)
                {
                    for (int i = 0; i < (width * 3) % 4 + 1; i++)
                    {
                        fin >> trash_can;
                    }
                }
            }
        }
        fin.close();
    }
};

void DrawSpritePixels(SpriteBMP & sprite) {
    HWND myconsole = GetConsoleWindow();
    HDC mydc = GetDC(myconsole);
    for (int i = 0; i < sprite.width; i++)
    {
        for (int j = 0; j < sprite.height; j++)
        {
            SetPixel(mydc, i, sprite.height - j, RGB(sprite.raw_bytes[i][j][2], sprite.raw_bytes[i][j][1], sprite.raw_bytes[i][j][0]));
        }
    }
}

int main()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,10 });
    SpriteBMP * s = new SpriteBMP("ban hammer4.bmp");
    DrawSpritePixels(*s);
    cin.get();
    //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,10 });
}