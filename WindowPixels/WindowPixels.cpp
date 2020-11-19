#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;

//path to the file
const char* INPUT_FILENAME = "ban hammer.bmp";

//simple header signature struct
struct BmpSignature
{
    unsigned char data[2];
    BmpSignature() { data[0] = data[1] = 0; }
};
//simple header additional info struct
struct BmpHeader
{
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataOffset;
    BmpHeader() : fileSize(0), reserved1(0), reserved2(0), dataOffset(0) { }
};
/// <summary>
/// Function for filling structures of header info of .bmp file.
/// </summary>
/// <param name="fin">Address of the ifstream thread, who reads file.</param>
/// <param name="sig">Address of the Signature of the file structure</param>
/// <param name="header">Address of the Header of the file structure</param>
void ReadHeader(ifstream& fin, BmpSignature& sig, BmpHeader& header)
{
    //if we in the file
    if (!fin)
        return;
    //go to start of the bytes
    fin.seekg(0, ios::beg);
    //read and write to signature structure bytes
    fin.read((char*)&sig, sizeof(sig));
    //read and write to header structure bytes
    fin.read((char*)&header, sizeof(header));
}

int main()
{
    //get a console handle
    HWND myconsole = GetConsoleWindow();
    //get a handle to device context
    HDC mydc = GetDC(myconsole);
    //create a thread and send the path to the file, read binary
    ifstream fin(INPUT_FILENAME, ios::binary);
    //create signature structure
    BmpSignature sig;
    //create header structure
    BmpHeader hdr;
    //call structure filling function
    ReadHeader(fin, sig, hdr);
    //move to the end of header information
    fin.seekg(hdr.dataOffset, 0);
    //create 3d array for pixels
    char *** pixels = new char**[100];
    for (int i = 0; i < 100; i++)
    {
        pixels[i] = new char*[100];
        for (int j = 0; j < 100; j++)
        {
            pixels[i][j] = new char[3];
        }
    }
    //make every single pixel white
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

    //create variables for the position of the current pixel 
    int x = 0, y = 0;
    //initialize bytes for each color of the pixels
    unsigned char r;
    unsigned char g;
    unsigned char b;
    //go through file reading bytes
    //cycle ends where the next byte is existing
    while (fin >> r) {
        //write to red value into array
        pixels[x][y][0] = r; 
        //move 1 byte ahead
        fin >> g;
        //write to green value into array
        pixels[x][y][1] = g; 
        fin >> b;
        //write to blue value into array
        pixels[x][y][2] = b; 
        //move by 1 horizontally
        x++;
        //if we reach last pixel in a row
        if (x > 99)
        {
            //reset position in the line
            x = 0;
            //move to the next line by 1 vertically
            y++;
        }
    }
    //go through 3d array of pixels
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            //create array of pixel info for readability of the next line
            char * p = pixels[i][j];
            //use SetPixel with:
            //hendler for drawing
            //cords inside picture x = i, y = 100 - j (y is upside down)
            //RGB components from temporary array "p"
            SetPixel(mydc, i, 100 - j, RGB((short)p[0], (short)p[1], (short)p[2]));
        }
    }
    //move console output handle for sake of not removing our picture with end program info
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,10 });
}