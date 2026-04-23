#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
using namespace std;
int main()
{
    int height = 0;
    int width = 0;
    int channels = 0;
    int Red = 0;
    int Green = 0;
    int Blue = 0;
    int coordinate_X = 0;
    int coordinate_Y = 0;
    int filter = -67;
    string name_of_file = "test.jpg";
    string type;
    int size;
    for (size = 0; name_of_file[size] != '\0'; size++)
    {
    }
    char *file_name = new char[size + 1];
    for (int i = 0; i < size; i++)
    {

        file_name[i] = name_of_file[i];
    }

    file_name[size] = '\0';
    cout << file_name << endl;
    int counter = 0;
    for (counter = 0; file_name[counter] != '.'; counter++)
    {
    }
    if ((file_name[counter + 1] == 'j' && file_name[counter + 2] == 'p' && file_name[counter + 3] == 'g') && file_name[counter + 4] == '\0')
    {
        type = "jpg";
    }
    else if ((file_name[counter + 1] == 'p' && file_name[counter + 2] == 'n' && file_name[counter + 3] == 'g') && file_name[counter + 4] == '\0')
    {
        type = "png";
    }
    else
    {
        type = "INVALID ENTRY TRY AGAIN";
    }
    unsigned char *data = stbi_load(file_name, &width, &height, &channels, 3); // would assign pixel values on the unsigned char array
    if (data == nullptr)
    {
        cout << "Failed to load image!" << endl;
        delete[] file_name;
        return -1;
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Red = data[3 * (j * width + i) + 0];
            Green = data[3 * (j * width + i) + 1];
            Blue = data[3 * (j * width + i) + 2];
            int brightness = (Red + Green + Blue) / 3;
            
                
                if (brightness <= 25)
                {
                    cout << " ";
                }
                else if ((brightness <= 56) && (brightness > 25))
                {
                    cout << ".";
                }
                else if ((brightness <= 84) && (brightness > 56))
                {
                    cout << ":";
                }
                else if ((brightness <= 112) && (brightness > 84))
                {
                    cout << "-";
                }
                else if ((brightness <= 140) && (brightness > 112))
                {
                    cout << "=";
                }
                else if ((brightness <= 168) && (brightness > 140))
                {
                    cout << "+";
                }
                else if ((brightness <= 196) && (brightness > 168))
                {
                    cout << "*";
                }
                else if ((brightness <= 224) && (brightness > 196))
                {
                    cout << "#";
                }
                else if ((brightness <= 255) && (brightness > 224))
                {
                    cout << "@";
                }
            
           
        }
        if(i % 20 == 0)
        {
            cout << "\n";
        }
        
    }
cout<<"\n";
    cout << "Which filter do you want : ";
    cin >> filter;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Red = data[3 * (j * width + i) + 0];
            Green = data[3 * (j * width + i) + 1];
            Blue = data[3 * (j * width + i) + 2];
            if (filter == 1)
            {
                Red = (Red + Green + Blue) / 3;
                Green = (Red);
                Blue = (Red);
            }
            else if (filter == 2)
            {
                Red = 255 - Red;
                Green = 255 - Green;
                Blue = 255 - Blue;
            }
            else if (filter == 3)
            {
                Red = Red + 67;
                Blue = Blue + 67;
                Green = Green + 67;
                // clamping is still required ++++++++++++++++++++++++++++++++++++++++++++
            }
            else if (filter == 4)
            {
                int min = Red, max = Red;
                if ((Red >= Blue) && (Red >= Green))
                {
                    max = Red;
                }
                else if ((Blue >= Red) && (Blue >= Green))
                {
                    max = Blue;
                }
                else if ((Green >= Blue) && (Green >= Red))
                {
                    max = Green;
                }

                if ((Red <= Blue) && (Red <= Green))
                {
                    min = Red;
                }
                else if ((Blue <= Red) && (Blue <= Green))
                {
                    min = Blue;
                }
                else if ((Green <= Blue) && (Green <= Red))
                {
                    min = Green;
                }
                Red = (float)(Red - min) / (max - min) * 255;
                Green = (float)(Green - min) / (max - min) * 255;
                Blue = (float)(Blue - min) / (max - min) * 255;
            }
            else if (filter == 5)
            {
                Blue = 0;
                Green = 0;
            }
            else if (filter == 6)
            {
                Red = 0;
                Blue = 0;
            }
            else if (filter == 7)
            {
                Green = 0;
                Red = 0;
            }
            data[3 * (j * width + i) + 0] = Red;
            data[3 * (j * width + i) + 1] = Green;
            data[3 * (j * width + i) + 2] = Blue;
        }
    }
    if (type == "jpg")
    {
        stbi_write_jpg("Edited.jpg", width, height, 3, data, 100);
    }
    else if (type == "png")
    {
        stbi_write_png("Edited.png", width, height, 3, data, width * 3);
    }
    cout << " AFTER FILTER : \n \n \n";
     
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Red = data[3 * (j * width + i) + 0];
            Green = data[3 * (j * width + i) + 1];
            Blue = data[3 * (j * width + i) + 2];
            int brightness = (Red + Green + Blue) / 3;
            
                
                if (brightness <= 25)
                {
                    cout << " ";
                }
                else if ((brightness <= 56) && (brightness > 25))
                {
                    cout << ".";
                }
                else if ((brightness <= 84) && (brightness > 56))
                {
                    cout << ":";
                }
                else if ((brightness <= 112) && (brightness > 84))
                {
                    cout << "-";
                }
                else if ((brightness <= 140) && (brightness > 112))
                {
                    cout << "=";
                }
                else if ((brightness <= 168) && (brightness > 140))
                {
                    cout << "+";
                }
                else if ((brightness <= 196) && (brightness > 168))
                {
                    cout << "*";
                }
                else if ((brightness <= 224) && (brightness > 196))
                {
                    cout << "#";
                }
                else if ((brightness <= 255) && (brightness > 224))
                {
                    cout << "@";
                }
            
            
        }
        if(i % 20 == 0)
        {
            cout << "\n";
        }
        
    }
cout<<"\n";

    stbi_image_free(data);
    delete[] file_name;
}
