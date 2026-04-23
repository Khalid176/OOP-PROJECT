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
    string name_of_file= "test.jpg";
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

    file_name[size + 1] = '\0';
    cout<<file_name<<endl;
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
    int coordinate_X_max = width * 3;
    int coordinate_Y_max = height;
    for (int i = 0; i < coordinate_X_max * coordinate_Y_max; i++, i++, i++)
    {
        Red = data[3 * (coordinate_Y * width + coordinate_X) + 0 + i];
        Green = data[3 * (coordinate_Y * width + coordinate_X) + 1 + i];
        Blue = data[3 * (coordinate_Y * width + coordinate_X) + 2 + i];
        if (filter == 1)
        {
            Red = Red / 3;
            Green = Green / 3;
            Blue = Blue / 3;
        }
        else if (filter == 2)
        {
            Red=255-Red; 
            Green=255-Green; 
            Blue=255-Blue;
        }
        else if(filter == 3)
        {
            Red = Red + 67;
            Blue = Blue + 67;
            Green = Green + 67;
            // clamping is still required ++++++++++++++++++++++++++++++++++++++++++++
        }
        else if(filter == 4)
        {
            int min = Red , max = Red;
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
            Red = (Red - min) / (max - min) * 255;
            Green = (Green - min) / (max - min) * 255;
            Blue = (Blue - min) / (max - min) * 255;
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
        
    }
                                      
    stbi_image_free(data);
}