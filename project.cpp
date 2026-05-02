#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
using namespace std;
class Pixel
{
    int Red;
    int Green;
    int Blue;

public:
    Pixel(int Red, int Green, int Blue)
    {
        this->Red = Red;
        this->Green = Green;
        this->Blue = Blue;
    }
    Pixel()
    {
        Red = 0;
        Green = 0;
        Blue = 0;
    }
    int getter_Red()
    {
        return Red;
    }
    int getter_Green()
    {
        return Green;
    }
    int getter_Blue()
    {
        return Blue;
    }
    void setter_Red(int Red)
    {
        this->Red = Red;
    }
    void setter_Green(int Green)
    {
        this->Green = Green;
    }
    void setter_Blue(int Blue)
    {
        this->Blue = Blue;
    }
    static int clamp_unity(int value);
    Pixel operator+(const Pixel &Pix)
    {
        Pixel P;
        P.Red = clamp_unity(this->Red + Pix.Red);
        P.Blue = clamp_unity(this->Blue + Pix.Blue);
        P.Green = clamp_unity(this->Green + Pix.Green);

        return P;
    }
    friend ostream &operator<<(ostream &out, const Pixel &p)
    {
        out << "Red : " << p.Red << " , ";
        out << "Blue : " << p.Blue << " , ";
        out << "Green : " << p.Green;
        return out;
    }
};
int Pixel::clamp_unity(int value)
{
    if ((value <= 255) && (value >= 0))
    {
        return value;
    }
    else if (value > 255)
    {
        return 255;
    }
    else if (value < 0)
    {
        return 0;
    }
    return value;
}

class F_M_READ_WRITE; // Forward declaration of the class F_M_READ_WRITE to be used in the saveable interface

class saveable
{
public:
    bool virtual save(string outputPath, F_M_READ_WRITE &FM_RW) = 0;
};

class displayable
{
public:
    void virtual display_Ascii() = 0;
};

class F_M_READ_WRITE; // Forward declaration of the class F_M_READ_WRITE to be used in the Image class
class FilterSession;  // Forward declaration of the class FilterSession to be used in the Image class

class Image : public displayable, public saveable
{
    int height;
    int width;
    string file_name;
    Pixel **Grid;

public:
    string file_name_getter()
    {
        return file_name;
    }
    Image &operator=(const Image &other)
    {
        cout << "\033[34m" << "Image assignment operator called" << "\033[0m" << endl; // For submission only
        if (this == &other)
        {
            return *this;
        }
        this->file_name = other.file_name;
        int old_height = this->height;
        this->height = other.height;
        this->width = other.width;

        if (this->Grid != nullptr)
        {
            for (int i = 0; i < old_height; i++)
            {
                delete[] Grid[i];
            }
            delete[] Grid;
        }

        this->Grid = new Pixel *[this->height];
        for (int i = 0; i < height; i++)
        {
            this->Grid[i] = new Pixel[this->width];
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                this->Grid[i][j] = other.Grid[i][j];
            }
        }
        return *this;
    }
    bool save(string outputPath, F_M_READ_WRITE &FM_RW);
    friend class FilterSession;
    Image(string file_name, int height, int width)
    {
        cout << "\033[34m" << "Image object created with data" << "\033[0m" << endl; // For submission only
        this->file_name = file_name;
        this->height = height;
        this->width = width;
        Grid = new Pixel *[height];
        for (int i = 0; i < height; i++)
        {
            Grid[i] = new Pixel[width];
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                Grid[i][j] = Pixel(0, 0, 0);
            }
        }
    }
    Image()
    {
        cout << "\033[34m" << "Image object created without data " << "\033[0m" << endl; // For submission only

        file_name = "NO AVALIABLE";
        height = 0;
        width = 0;
        Grid = nullptr;
    }
    int getter_height()
    {
        return height;
    }
    int getter_width()
    {
        return width;
    }
    void width_setter(int width)
    {
        this->width = width;
    }
    void height_setter(int height)
    {
        this->height = height;
    }
    Pixel &at(int row, int column)
    {
        return Grid[row][column];
    }
    Pixel **GridGetter()
    {
        return Grid;
    }
    void display_Ascii_FUll()
    {
        for (int j = 0; j < height; j++)
        {

            for (int i = 0; i < width; i++)
            {

                int brightness = ((Grid[j][i].getter_Red()) + (Grid[j][i].getter_Blue()) + (Grid[j][i].getter_Green())) / 3;

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

            cout << "\n";
        }
    }
    void display_Ascii_HALF()
    {

        int rowStep = (height / 20) > 0 ? (height / 20) : 1;
        int columnStep = (width / 40) > 0 ? (width / 40) : 1;
        int counter = 0; // counter to keep track of the number of pixels printed in the current row
        for (int j = 0; j < height; j++)
        {
            bool printedThisRow = 0;

            for (int i = 0; i < width; i++)
            {

                int brightness = ((Grid[j][i].getter_Red()) + (Grid[j][i].getter_Blue()) + (Grid[j][i].getter_Green())) / 3;
                if ((i % columnStep == 0) && (j % rowStep == 0))
                {
                    printedThisRow = true;
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
                counter++;
            }
            // if ((width * height) % (j > 0 ? j : 1) == 0)
            // {
            //     cout << "\n";
            // }
            if ((j % rowStep == 0) && (printedThisRow))
            {
                cout << "\n";
            }
        }
    }
    void display_Ascii()
    {
        char ahhh_Aik_or_choice = '\0';

        cout << "Which Type of Ascii you want to produce : " << endl;
        cout << "1. Display Full Ascii \n";
        cout << "2. Display scaled down Ascii \n";
        do
        {
            cout << "Enter Your choice : ";
            cin >> ahhh_Aik_or_choice;
            if (ahhh_Aik_or_choice != '1' && ahhh_Aik_or_choice != '2')
            {
                cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
            }

        } while (ahhh_Aik_or_choice != '1' && ahhh_Aik_or_choice != '2');
        if (ahhh_Aik_or_choice == '1')
        {
            display_Ascii_FUll();
        }
        else if (ahhh_Aik_or_choice == '2')
        {
            display_Ascii_HALF();
        }
    }
    Image(Image &other)
    {
        this->file_name = other.file_name;
        this->height = other.height;
        this->width = other.width;

        this->Grid = new Pixel *[this->height];
        for (int i = 0; i < height; i++)
        {
            this->Grid[i] = new Pixel[this->width];
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                this->Grid[i][j] = other.Grid[i][j];
            }
        }
    }
    ~Image()
    {
        cout << "\033[34m" << "Image object Destroyed " << "\033[0m" << endl; // For submission only
        if (Grid != nullptr)
        {
            for (int i = 0; i < height; i++)
            {
                delete[] Grid[i];
            }
            delete[] Grid;
            Grid = nullptr;
        }
    }
};

class F_M_READ_WRITE
{
    int height;
    int width;
    int channels;
    unsigned char *data = nullptr;
    string type;

public:
    Image *F_M_READ_IMAGE(string name_of_file)
    {

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
        int counter = 0;
        for (counter = 0; file_name[counter] != '.' && file_name[counter] != '\0'; counter++)
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
        data = stbi_load(file_name, &width, &height, &channels, 3); // would assign pixel values on the unsigned char array
        if (data == nullptr)
        {
            cout << "Failed to load image!" << endl;
            delete[] file_name;
            return nullptr;
        }
        Image *image = new Image(name_of_file, height, width);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int Red = data[3 * (j * width + i) + 0];
                int Green = data[3 * (j * width + i) + 1];
                int Blue = data[3 * (j * width + i) + 2];
                image->GridGetter()[j][i].setter_Red(Red);
                image->GridGetter()[j][i].setter_Green(Green);
                image->GridGetter()[j][i].setter_Blue(Blue);
            }
        }
        delete[] file_name;
        stbi_image_free(data);
        return image;
    }
    bool F_M_WRITE_IMAGE(Image *image, string outputPath)
    {
        int counter1 = 0;
        string name_of_image = image->file_name_getter();
        for (counter1 = 0; name_of_image[counter1] != '.' && name_of_image[counter1] != '\0'; counter1++)
        {
        }
        string type_of_image;
        if ((name_of_image[counter1 + 1] == 'j' && name_of_image[counter1 + 2] == 'p' && name_of_image[counter1 + 3] == 'g') && name_of_image[counter1 + 4] == '\0')
        {
            type_of_image = "jpg";
        }
        else if ((name_of_image[counter1 + 1] == 'p' && name_of_image[counter1 + 2] == 'n' && name_of_image[counter1 + 3] == 'g') && name_of_image[counter1 + 4] == '\0')
        {
            type_of_image = "png";
        }
        else
        {
            cout << "\033[33m" << "Invalid TYPE OF IMAGE FAILED TO WRITE " << "\033[0m" << endl;
            return false;
        }
        outputPath = outputPath + "." + type_of_image;
        int size = 0;
        int width = image->getter_width();
        int height = image->getter_height();

        unsigned char *output_data = new unsigned char[width * height * 3];

        for (size = 0; outputPath[size] != '\0'; size++)
        {
        }
        for (size = 0; outputPath[size] != '\0'; size++)
        {
        }
        int counter = 0;
        for (counter = 0; outputPath[counter] != '.' && outputPath[counter] != '\0'; counter++)
        {
        }
        if ((outputPath[counter + 1] == 'j' && outputPath[counter + 2] == 'p' && outputPath[counter + 3] == 'g') && outputPath[counter + 4] == '\0')
        {
            type = "jpg";
        }
        else if ((outputPath[counter + 1] == 'p' && outputPath[counter + 2] == 'n' && outputPath[counter + 3] == 'g') && outputPath[counter + 4] == '\0')
        {
            type = "png";
        }
        char *file_path = new char[size + 1];
        for (int i = 0, j = 0; i < size; i++, j++)
        {
            file_path[j] = outputPath[i];
        }
        file_path[size] = '\0';

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int index = 3 * (j * width + i);
                output_data[index + 0] = image->GridGetter()[j][i].getter_Red();
                output_data[index + 1] = image->GridGetter()[j][i].getter_Green();
                output_data[index + 2] = image->GridGetter()[j][i].getter_Blue();
            }
        }

        if (type == "jpg")
        {
            bool check = (stbi_write_jpg(file_path, width, height, 3, output_data, 100));
            delete[] file_path;
            delete[] output_data;
            return check;
        }
        else if (type == "png")
        {
            bool check = (stbi_write_png(file_path, width, height, 3, output_data, width * 3));
            delete[] file_path;
            delete[] output_data;
            return check;
        }
        else
        {
            delete[] file_path;
            delete[] output_data;
            return false;
        }
    }

    int F_M_READ_HEIGHT()
    {
        return height;
    }
    int F_M_READ_WIDTH()
    {
        return width;
    }
};

bool Image::save(string outputPath, F_M_READ_WRITE &FM_RW)
{

    bool check = 0;

    check = FM_RW.F_M_WRITE_IMAGE(this, outputPath);
    return check;
}

class Filter
{

public:
    bool virtual apply(Image *image) = 0;
    bool virtual is_avaliable() = 0;
    void virtual set_avaliable(bool avaliable) = 0;
    virtual ~Filter() {}
    string name_getter()
    {
        return name;
    }
    string ID_getter()
    {
        return ID;
    }

protected:
    bool avaliable = true;
    string name;
    string ID;
};
class GrayScale : public Filter
{

public:
    GrayScale()
    {
        name = "GrayScale";
        ID = "01";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

                    int average = (Red + Green + Blue) / 3;
                    Red = average;
                    Green = average;
                    Blue = average;
                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Invert : public Filter
{
public:
    Invert()
    {
        name = "Invert";
        ID = "02";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

                    Red = image->GridGetter()[j][i].clamp_unity(255 - Red);
                    Green = image->GridGetter()[j][i].clamp_unity(255 - Green);
                    Blue = image->GridGetter()[j][i].clamp_unity(255 - Blue);

                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Brightness_Adjust : public Filter
{

    float increment;

public:
    Brightness_Adjust()
    {
        name = "Brightness_Adjust";
        ID = "03";
        this->increment = 0.0;
    }
    void increment_setter(float set)
    {
        increment = set;
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

                    Red = image->GridGetter()[j][i].clamp_unity(Red + increment);
                    Green = image->GridGetter()[j][i].clamp_unity(Green + increment);
                    Blue = image->GridGetter()[j][i].clamp_unity(Blue + increment);

                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;

            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
    int increment_getter()
    {
        return increment;
    }
};
class Contrast_Stretch : public Filter
{
public:
    Contrast_Stretch()
    {
        name = "Contrast_Stretch";
        ID = "04";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

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
                    if (max != min)
                    {

                        Red = image->GridGetter()[j][i].clamp_unity((float)(Red - min) / (max - min) * 255);
                        Green = image->GridGetter()[j][i].clamp_unity((float)(Green - min) / (max - min) * 255);
                        Blue = image->GridGetter()[j][i].clamp_unity((float)(Blue - min) / (max - min) * 255);

                        image->GridGetter()[j][i].setter_Red(Red);
                        image->GridGetter()[j][i].setter_Green(Green);
                        image->GridGetter()[j][i].setter_Blue(Blue);
                    }
                    else
                    {
                        image->GridGetter()[j][i].setter_Red(Red);
                        image->GridGetter()[j][i].setter_Green(Green);
                        image->GridGetter()[j][i].setter_Blue(Blue);
                    }
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Red_Channel_Only : public Filter
{
public:
    Red_Channel_Only()
    {
        name = "Red_Channel_Only";
        ID = "05";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

                    Green = 0;
                    Blue = 0;

                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Green_Channel_Only : public Filter
{
public:
    Green_Channel_Only()
    {
        name = "Green_Channel_Only";
        ID = "06";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

                    Red = 0;
                    Blue = 0;

                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Blue_Channel_Only : public Filter
{
public:
    Blue_Channel_Only()
    {
        name = "Blue_Channel_Only";
        ID = "07";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_width(); i++)
            {
                for (int j = 0; j < image->getter_height(); j++)
                {
                    int Red = image->GridGetter()[j][i].getter_Red();
                    int Green = image->GridGetter()[j][i].getter_Green();
                    int Blue = image->GridGetter()[j][i].getter_Blue();

                    Green = 0;
                    Red = 0;

                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Box_Blur : public Filter
{
public:
    Box_Blur()
    {
        name = "Box_Blur";
        ID = "08";
    }
    bool apply(Image *IImage)
    {
        if (avaliable == 1)
        {
            Image *image = new Image(*IImage);
            int count = 1;
            for (int i = 0; i < image->getter_height(); i++)
            {
                for (int j = 0; j < image->getter_width(); j++)
                {
                    count = 1;
                    int Red = image->GridGetter()[i][j].getter_Red();
                    int Green = image->GridGetter()[i][j].getter_Green();
                    int Blue = image->GridGetter()[i][j].getter_Blue();
                    if (((i + 1) < image->getter_height()) && ((i + 1) >= 0))
                    {
                        Red = Red + image->GridGetter()[i + 1][j].getter_Red();
                        Blue = Blue + image->GridGetter()[i + 1][j].getter_Blue();
                        Green = Green + image->GridGetter()[i + 1][j].getter_Green();
                        count++;
                    }
                    if (((i - 1) < image->getter_height()) && ((i - 1) >= 0))
                    {
                        Red = Red + image->GridGetter()[i - 1][j].getter_Red();
                        Blue = Blue + image->GridGetter()[i - 1][j].getter_Blue();
                        Green = Green + image->GridGetter()[i - 1][j].getter_Green();
                        count++;
                    }
                    if (((j - 1) < image->getter_width()) && ((j - 1) >= 0))
                    {
                        Red = Red + image->GridGetter()[i][j - 1].getter_Red();
                        Blue = Blue + image->GridGetter()[i][j - 1].getter_Blue();
                        Green = Green + image->GridGetter()[i][j - 1].getter_Green();
                        count++;
                    }
                    if (((j + 1) < image->getter_width()) && ((j + 1) >= 0))
                    {
                        Red = Red + image->GridGetter()[i][j + 1].getter_Red();
                        Blue = Blue + image->GridGetter()[i][j + 1].getter_Blue();
                        Green = Green + image->GridGetter()[i][j + 1].getter_Green();
                        count++;
                    }
                    if ((((j + 1) < image->getter_width()) && ((j + 1) >= 0)) && (((i + 1) < image->getter_height()) && ((i + 1) >= 0)))
                    {
                        Red = Red + image->GridGetter()[i + 1][j + 1].getter_Red();
                        Blue = Blue + image->GridGetter()[i + 1][j + 1].getter_Blue();
                        Green = Green + image->GridGetter()[i + 1][j + 1].getter_Green();
                        count++;
                    }
                    if ((((j - 1) < image->getter_width()) && ((j - 1) >= 0)) && (((i - 1) < image->getter_height()) && ((i - 1) >= 0)))
                    {
                        Red = Red + image->GridGetter()[i - 1][j - 1].getter_Red();
                        Blue = Blue + image->GridGetter()[i - 1][j - 1].getter_Blue();
                        Green = Green + image->GridGetter()[i - 1][j - 1].getter_Green();
                        count++;
                    }
                    if ((((j + 1) < image->getter_width()) && ((j + 1) >= 0)) && (((i - 1) < image->getter_height()) && ((i - 1) >= 0)))
                    {
                        Red = Red + image->GridGetter()[i - 1][j + 1].getter_Red();
                        Blue = Blue + image->GridGetter()[i - 1][j + 1].getter_Blue();
                        Green = Green + image->GridGetter()[i - 1][j + 1].getter_Green();
                        count++;
                    }
                    if ((((j - 1) < image->getter_width()) && ((j - 1) >= 0)) && (((i + 1) < image->getter_height()) && ((i + 1) >= 0)))
                    {
                        Red = Red + image->GridGetter()[i + 1][j - 1].getter_Red();
                        Blue = Blue + image->GridGetter()[i + 1][j - 1].getter_Blue();
                        Green = Green + image->GridGetter()[i + 1][j - 1].getter_Green();
                        count++;
                    }

                    Red = IImage->GridGetter()[i][j].clamp_unity(Red / count);
                    Green = IImage->GridGetter()[i][j].clamp_unity(Green / count);
                    Blue = IImage->GridGetter()[i][j].clamp_unity(Blue / count);

                    IImage->GridGetter()[i][j].setter_Red(Red);
                    IImage->GridGetter()[i][j].setter_Green(Green);
                    IImage->GridGetter()[i][j].setter_Blue(Blue);
                }
            }

            delete image;
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Flip_Horizontal : public Filter
{
public:
    Flip_Horizontal()
    {
        name = "Flip_Horizontal";
        ID = "09";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int i = 0; i < image->getter_height(); i++)
            {
                for (int j = 0; j < image->getter_width() / 2; j++)
                {
                    int Red = image->GridGetter()[i][image->getter_width() - 1 - j].getter_Red();
                    int Green = image->GridGetter()[i][image->getter_width() - 1 - j].getter_Green();
                    int Blue = image->GridGetter()[i][image->getter_width() - 1 - j].getter_Blue();

                    image->GridGetter()[i][image->getter_width() - 1 - j].setter_Red(image->GridGetter()[i][j].getter_Red());
                    image->GridGetter()[i][image->getter_width() - 1 - j].setter_Green(image->GridGetter()[i][j].getter_Green());
                    image->GridGetter()[i][image->getter_width() - 1 - j].setter_Blue(image->GridGetter()[i][j].getter_Blue());

                    image->GridGetter()[i][j].setter_Red(Red);
                    image->GridGetter()[i][j].setter_Green(Green);
                    image->GridGetter()[i][j].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};
class Flip_Vertical : public Filter
{
public:
    Flip_Vertical()
    {
        name = "Flip_Vertical";
        ID = "10";
    }
    bool apply(Image *image)
    {
        if (avaliable == 1)
        {
            for (int j = 0; j < image->getter_height() / 2; j++)
            {
                for (int i = 0; i < image->getter_width(); i++)
                {
                    int Red = image->GridGetter()[image->getter_height() - 1 - j][i].getter_Red();
                    int Green = image->GridGetter()[image->getter_height() - 1 - j][i].getter_Green();
                    int Blue = image->GridGetter()[image->getter_height() - 1 - j][i].getter_Blue();

                    image->GridGetter()[image->getter_height() - 1 - j][i].setter_Red(image->GridGetter()[j][i].getter_Red());
                    image->GridGetter()[image->getter_height() - 1 - j][i].setter_Green(image->GridGetter()[j][i].getter_Green());
                    image->GridGetter()[image->getter_height() - 1 - j][i].setter_Blue(image->GridGetter()[j][i].getter_Blue());

                    image->GridGetter()[j][i].setter_Red(Red);
                    image->GridGetter()[j][i].setter_Green(Green);
                    image->GridGetter()[j][i].setter_Blue(Blue);
                }
            }
            return true;
        }
        else
        {
            cout << "\033[31m" << "Sorry this filter cannot be used it is blocked by the admin " << "\033[0m" << endl;
            return false;
        }
    }
    bool is_avaliable()
    {
        return avaliable;
    }
    void set_avaliable(bool avaliable)
    {
        this->avaliable = avaliable;
    }
};

class FilterSession
{
    string customer_cnic;
    string timestamp;
    Image *image = nullptr;
    vector<Filter *> filters;
    int counter;
    string what_happened_helper_in_append;

public:
    void clear_pipeline()
    {
        filters.clear();
        counter = 0;
        what_happened_helper_in_append = "";
    }
    string getter()
    {
        return what_happened_helper_in_append;
    }
    void setter(string k)
    {
        what_happened_helper_in_append = k;
    }
    FilterSession(string customer_cnic, string timestamp, Image *image)
    {
        this->customer_cnic = customer_cnic;
        this->timestamp = timestamp;
        this->image = image;
        counter = 0;
        what_happened_helper_in_append = "";
    }
    FilterSession &add_filter(Filter *filter, bool &is_done)
    {
        if (filter->is_avaliable() == 1)
        {

            filters.push_back(filter);
            counter++;
            is_done = 1;
            return *this;
        }
        else
        {
            is_done = 0;
            cout << "\033[31m" << "Sorry " << filter->name_getter() << " cannot be added to the pipeline. It is currently blocked by the admin " << "\033[0m" << endl;
            cout << "\033[33m" << "For further information contact admin" << "\033[0m" << endl;
            return *this;
        }
    }
    bool apply_pipeline()
    {
        int counter2 = 0;

        for (int i = 0; i < counter; i++)
        {
            if (filters.at(i)->is_avaliable())
            {
                if (filters.at(i)->apply(image))
                {

                    char choice = '\0';
                up1:
                    cout << filters.at(i)->name_getter() << " has been applied " << endl;
                    do
                    {
                        cout << "Do you wish to preview the image in ascii in this state (Y/N): ";
                        cin >> choice;
                        if (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
                    if (choice == 'y' || choice == 'Y')
                    {
                        image->display_Ascii();
                    }
                    if (choice != 'y' && choice != 'Y' && choice != 'N' && choice != 'n')
                    {
                        cout << "\033[33m" << "Invalid choice" << "\033[0m" << endl;
                        goto up1;
                    }

                    counter2++;
                }
                else
                {
                    cout << "\033[31m" << "An error has occoured" << "\033[0m";
                    return false;
                }
            }
        }

        return true;
    }
    bool save_result(string output_path, F_M_READ_WRITE &fm_R_W)
    {
        return (image->save(output_path, fm_R_W));
    }
    void display_ascii_preview()
    {
        image->display_Ascii();
    }
    ~FilterSession()
    {
        filters.clear();
    }
};
class F_M_Customers;
class Customer;
class user
{
    friend class F_M_Customers;

protected:
    string name;
    string cnic;
    string phone;
    string city;
    string Gender;
    string password;

public:
    user()
    {
        cout << "\033[34m" << "User Object is created" << "\033[0m" << endl; // For submission only
    }
    virtual ~user()
    {
        cout << "\033[34m" << "User Object is destroyed" << "\033[0m" << endl; // For submission only
    }
    bool virtual login(string cnic, string password) = 0;
};

// Customer strting ====================================

class Customer : public user
{

    bool is_blocked;

public:
    friend class F_M_Customers;
    string customer_name_getter()
    {
        return name;
    }
    string customer_CNIC_getter()
    {
        return cnic;
    }
    Customer()
    {
        name = "";
        phone = "";
        city = "";
        password = "";
        Gender = "";
        cnic = "";
        is_blocked = false;
        cout << "\033[34m" << "Customer object created" << "\033[0m" << endl; // For submission only
    }
    Customer(string name, string phone, string city, string password, string Gender, string cnic)
    {
        this->name = name;
        this->phone = phone;
        this->cnic = cnic;
        this->Gender = Gender;
        this->city = city;
        this->password = password;
        is_blocked = false;
        cout << "\033[34m" << "Customer object created with data" << "\033[0m" << endl; // For submission only
    }
    bool is_blocked_getter()
    {
        return is_blocked;
    }
    bool login(string e_cnic, string e_password); // forward declaration
    void display()
    {
        cout << "Name : " << name << endl;
        cout << "Phone Number : " << phone << endl;
        cout << "Gender : " << Gender << endl;
        cout << "City : " << city << endl;
        cout << "Password : " << password << endl;
    }
    ~Customer()
    {
        cout << "Customer object destroyed" << endl;
    }
};
class F_M_Customers
{
public:
    void F_M_Display__All()
    {
        ifstream file_real("Customers.txt");
        if (!(file_real.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
        }
        else
        {
            string data;
            int k = 0;

            while (getline(file_real, data))
            {
                int i = 0;

                string picked_password;
                string picked_cnic;
                string picked_name;
                string picked_Gender;
                string picked_phone;
                string picked_city;
                bool picked_is_blocked;

                for (; data[i] != '|'; i++)
                {
                    picked_cnic = picked_cnic + data[i];
                }
                i++;

                for (; data[i] != '|'; i++)
                {
                    picked_password += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_name += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_Gender += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_phone += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_city += data[i];
                }

                i++;
                for (; data[i] != '\0'; i++)
                {
                    picked_is_blocked = (data[i] == '1');
                }

                cout << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
            }
            file_real.close();
        }
    }
    Customer *F_M_Load(string filemane, Customer *customer, string cnic, string password)
    {
        int cnic_size;
        int password_size;

        for (password_size = 0; password[password_size] != '\0'; password_size++)
        {
        }
        for (cnic_size = 0; cnic[cnic_size] != '\0'; cnic_size++)
        {
        }

        ifstream file("Customers.txt");
        if (!file.is_open())
        {
            cout << "\033[31m" << "Error: Could not open the file!" << "\033[0m" << endl;
            return nullptr;
        }
        string data;
        while (getline(file, data))
        {
            string Picked_password;
            string Picked_cnic;
            string Picked_name;
            string Picked_Gender;
            string Picked_phone;
            string Picked_city;
            bool Picked_is_blocked;

            int i = 0;
            for (i = 0; data[i] != '|'; i++)
            {
                Picked_cnic += data[i];
            }

            i++;
            for (; data[i] != '|'; i++)
            {
                Picked_password += data[i];
            }

            i++;
            int Picked_password_size = Picked_password.length();
            int Picked_cnic_size = Picked_cnic.length();

            if (Picked_cnic_size != cnic_size || Picked_password_size != password_size)
            {
                continue;
            }
            else
            {
                if ((Picked_password == password) && (Picked_cnic == cnic))
                {
                    for (; data[i] != '|'; i++)
                    {
                        Picked_name += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_Gender += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_phone += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_city += data[i];
                    }

                    i++;
                    for (; data[i] != '\0'; i++)
                    {
                        Picked_is_blocked = (data[i] == '1');
                    }
                    customer->name = Picked_name;
                    customer->phone = Picked_phone;
                    customer->Gender = Picked_Gender;
                    customer->city = Picked_city;
                    customer->is_blocked = Picked_is_blocked;
                    customer->cnic = Picked_cnic;
                    customer->password = Picked_password;
                    if (customer->is_blocked != 1)
                    {
                        file.close();
                        return customer;
                    }
                    else
                    {
                        cout << "User has been blocked by the admin \n ";
                        file.close();
                        return nullptr;
                    }
                }
            }
        }
        file.close();
        return nullptr;
    }
    int F_M_Save(Customer *customer)
    {
        ofstream file("Customers.txt", ios::app);
        if (!file.is_open())
        {
            cout << "UNABLE TO OPEN THE FILE " << endl;
            return 0;
        }
        else
        {
            file << customer->cnic << "|" << customer->password << "|" << customer->name << "|" << customer->Gender << "|" << customer->phone << "|" << customer->city << "|" << 0 << endl;
            file.close();
            return 1;
        }
    }

    int F_M_Customer_Counter()
    {
        ifstream file("Customers.txt");
        if (!file.is_open())
        {
            cout << "UNABLE TO OPEN THE FILE " << endl;
            return 0;
        }
        else
        {
            int counter = 0;
            string line;
            while (getline(file, line))
            {
                counter++;
            }
            file.close();
            return counter;
        }
    }
    int F_M_Delete(string cnic)
    {
        int count = F_M_Customer_Counter();
        ifstream file_real("Customers.txt");
        ofstream file_temp("Temp.txt");
        if (!(file_real.is_open() && file_temp.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
            return 0;
        }
        else
        {
            string data;
            int k = 0;

            while (getline(file_real, data))
            {
                int i = 0;

                string picked_password;
                string picked_cnic;
                string picked_name;
                string picked_Gender;
                string picked_phone;
                string picked_city;
                bool picked_is_blocked;

                for (; data[i] != '|'; i++)
                {
                    picked_cnic = picked_cnic + data[i];
                }
                i++;

                if (picked_cnic != cnic)
                {

                    for (; data[i] != '|'; i++)
                    {
                        picked_password += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_name += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_Gender += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_phone += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_city += data[i];
                    }

                    i++;
                    for (; data[i] != '\0'; i++)
                    {
                        picked_is_blocked = (data[i] == '1');
                    }

                    file_temp << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
                }
            }
            file_real.close();
            file_temp.close();
            ofstream file_real("Customers.txt");
            ifstream file_temp("Temp.txt");
            if (!(file_real.is_open() && file_temp.is_open()))
            {
                return 0;
            }
            else
            {
                while (getline(file_temp, data))
                {
                    int i = 0;
                    string picked_password;
                    string picked_cnic;
                    string picked_name;
                    string picked_Gender;
                    string picked_phone;
                    string picked_city;
                    bool picked_is_blocked;

                    for (; data[i] != '|'; i++)
                    {
                        picked_cnic = picked_cnic + data[i];
                    }
                    i++;

                    if (picked_cnic != cnic)
                    {
                        for (; data[i] != '|'; i++)
                        {
                            picked_password += data[i];
                        }
                        i++;
                        for (; data[i] != '|'; i++)
                        {
                            picked_name += data[i];
                        }

                        i++;
                        for (; data[i] != '|'; i++)
                        {
                            picked_Gender += data[i];
                        }

                        i++;
                        for (; data[i] != '|'; i++)
                        {
                            picked_phone += data[i];
                        }

                        i++;
                        for (; data[i] != '|'; i++)
                        {
                            picked_city += data[i];
                        }

                        i++;
                        for (; data[i] != '\0'; i++)
                        {
                            picked_is_blocked = (data[i] == '1');
                        }
                        file_real << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
                    }
                }
                file_real.close();
                file_temp.close();
            }

            ifstream file_real2("Sessions.txt");
            ofstream file_temp2("Temp2.txt");
            if (!(file_real2.is_open() && file_temp2.is_open()))
            {
                cout << "UNABLE TO OpEN THE FILE second last" << endl;
                return 0;
            }
            else
            {

                string data2;
                if (!(file_real2.is_open() && file_temp2.is_open()))
                {
                    cout << "UNABLE TO OpEN THE FILE second last" << endl;
                    return 0;
                }
                else
                {
                    while (getline(file_real2, data2))
                    {
                        int i = 0;
                        string picked_cnic, picked_Timestamp, picked_FiltersAplied, picked_OutputFile;
                        for (; data2[i] != '|'; i++)
                        {
                            picked_cnic = picked_cnic + data2[i];
                        }
                        i++;
                        if (picked_cnic != cnic)
                        {
                            for (; data2[i] != '|'; i++)
                            {
                                picked_Timestamp += data2[i];
                            }

                            i++;
                            for (; data2[i] != '|'; i++)
                            {
                                picked_FiltersAplied += data2[i];
                            }

                            i++;
                            for (; data2[i] != '\0'; i++)
                            {
                                picked_OutputFile += data2[i];
                            }

                            i++;

                            file_temp2 << picked_cnic << "|" << picked_Timestamp << "|" << picked_FiltersAplied << "|" << picked_OutputFile << endl;
                        }
                    }

                    file_real2.close();
                    file_temp2.close();
                    ofstream file_real2("Sessions.txt");
                    ifstream file_temp2("Temp2.txt");

                    if (!(file_real2.is_open() && file_temp2.is_open()))
                    {
                        cout << "UNABLE TO OPEN THE FILE last" << endl;
                        return 0;
                    }
                    else
                    {
                        while (getline(file_temp2, data2))
                        {
                            file_real2 << data2 << endl;
                        }
                    }

                    file_real2.close();
                    file_temp2.close();
                    return 1;
                }
            }
        }
        return 0;
    }
    int F_M_Toogle(string cnic)
    {
        ifstream file_real("Customers.txt");
        ofstream file_temp("Temp.txt");
        if (!(file_real.is_open() && file_temp.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
            return 0;
        }
        else
        {
            string data;
            int k = 0;

            while (getline(file_real, data))
            {
                int i = 0;

                string picked_password;
                string picked_cnic;
                string picked_name;
                string picked_Gender;
                string picked_phone;
                string picked_city;
                bool picked_is_blocked;

                for (; data[i] != '|'; i++)
                {
                    picked_cnic = picked_cnic + data[i];
                }
                i++;

                for (; data[i] != '|'; i++)
                {
                    picked_password += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_name += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_Gender += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_phone += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_city += data[i];
                }

                i++;
                if (picked_cnic == cnic)
                {
                    for (; data[i] != '\0'; i++)
                    {
                        picked_is_blocked = !(data[i] == '1');
                    }
                }
                else
                {
                    for (; data[i] != '\0'; i++)
                    {
                        picked_is_blocked = (data[i] == '1');
                    }
                }
                file_temp << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
                if ((picked_is_blocked == 1) && (picked_cnic == cnic))
                {
                    ofstream file_block("Blocked_Cnics.txt", ios::app);

                    file_block << picked_cnic << endl;
                    file_block.close();
                }
                else if ((picked_cnic == cnic) && (picked_is_blocked == 0))
                {
                    ifstream file_blocked("Blocked_Cnics.txt");
                    ofstream file_blocked_temp("Blocked_Cnics_temp.txt");
                    string fata;
                    while (getline(file_blocked, fata))
                    {
                        if (cnic != fata)
                        {
                            file_blocked_temp << fata << endl;
                        }
                    }
                    file_blocked.close();
                    file_blocked_temp.close();

                    ofstream file_blocked2("Blocked_Cnics.txt");
                    ifstream file_blocked_temp2("Blocked_Cnics_temp.txt");
                    string fata2;

                    while (getline(file_blocked_temp2, fata2))
                    {
                        file_blocked2 << fata2 << endl;
                    }
                    file_blocked2.close();
                    file_blocked_temp2.close();
                }
            }
            file_real.close();
            file_temp.close();
            ofstream file_real("Customers.txt");
            ifstream file_temp("Temp.txt");
            if (!(file_real.is_open() && file_temp.is_open()))
            {
                cout << "UNABLE TO OPEN FILES";
                return 0;
            }
            else
            {

                while (getline(file_temp, data))
                {
                    int i = 0;
                    string picked_password;
                    string picked_cnic;
                    string picked_name;
                    string picked_Gender;
                    string picked_phone;
                    string picked_city;
                    bool picked_is_blocked;

                    for (; data[i] != '|'; i++)
                    {
                        picked_cnic = picked_cnic + data[i];
                    }
                    i++;

                    for (; data[i] != '|'; i++)
                    {
                        picked_password += data[i];
                    }
                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_name += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_Gender += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_phone += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        picked_city += data[i];
                    }

                    i++;
                    for (; data[i] != '\0'; i++)
                    {
                        picked_is_blocked = (data[i] == '1');
                    }
                    file_real << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
                }
                file_real.close();
                file_temp.close();
                return 1;
            }
        }
        return 0;
    }
    Customer *F_M_SEARCH_cnic(string cnic, Customer *customer)
    {
        int cnic_size;
        int password_size;

        for (cnic_size = 0; cnic[cnic_size] != '\0'; cnic_size++)
        {
        }

        ifstream file("Customers.txt");
        if (!file.is_open())
        {
            cout << "\033[31m" << "Error: Could not open the file!" << "\033[0m" << endl;
            return 0;
        }
        string data;
        while (getline(file, data))
        {
            string Picked_password;
            string Picked_cnic;
            string Picked_name;
            string Picked_Gender;
            string Picked_phone;
            string Picked_city;
            bool Picked_is_blocked;

            int i = 0;
            for (i = 0; data[i] != '|'; i++)
            {
                Picked_cnic += data[i];
            }

            i++;

            int Picked_cnic_size = Picked_cnic.length();

            if (Picked_cnic_size != cnic_size)
            {
                continue;
            }
            else
            {

                if ((Picked_cnic == cnic))
                {
                    for (; data[i] != '|'; i++)
                    {
                        Picked_password += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_name += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_Gender += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_phone += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_city += data[i];
                    }

                    i++;
                    for (; data[i] != '\0'; i++)
                    {
                        Picked_is_blocked = (data[i] == '1');
                    }
                    customer->cnic = Picked_cnic;
                    customer->name = Picked_name;
                    customer->phone = Picked_phone;
                    customer->Gender = Picked_Gender;
                    customer->city = Picked_city;
                    customer->password = Picked_password;
                    customer->is_blocked = Picked_is_blocked;
                    if (customer->is_blocked != 1)
                    {
                        file.close();
                        return customer;
                    }
                    else
                    {
                        cout << "User has been blocked by the admin \n ";
                        file.close();
                        return customer;
                    }
                }
            }
        }
        file.close();
        return nullptr;
    }
    Customer *F_M_SEARCH_name(string name, Customer *customer)
    {
        int name_size;

        for (name_size = 0; name[name_size] != '\0'; name_size++)
        {
        }

        ifstream file("Customers.txt");
        if (!file.is_open())
        {
            cout << "\033[31m" << "Error: Could not open the file!" << "\033[0m" << endl;
            return 0;
        }
        string data;
        while (getline(file, data))
        {
            string Picked_password;
            string Picked_cnic;
            string Picked_name;
            string Picked_Gender;
            string Picked_phone;
            string Picked_city;
            bool Picked_is_blocked;

            int i = 0;
            for (i = 0; data[i] != '|'; i++)
            {
                Picked_cnic += data[i];
            }

            i++;
            for (; data[i] != '|'; i++)
            {
                Picked_password += data[i];
            }

            i++;
            for (; data[i] != '|'; i++)
            {
                Picked_name += data[i];
            }
            i++;
            int Picked_name_size = Picked_name.length();

            if (Picked_name_size != name_size)
            {
                continue;
            }
            else
            {

                if ((Picked_name == name))
                {

                    for (; data[i] != '|'; i++)
                    {
                        Picked_Gender += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_phone += data[i];
                    }

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_city += data[i];
                    }

                    i++;
                    for (; data[i] != '\0'; i++)
                    {
                        Picked_is_blocked = (data[i] == '1');
                    }
                    customer->name = Picked_name;
                    customer->phone = Picked_phone;
                    customer->Gender = Picked_Gender;
                    customer->city = Picked_city;
                    customer->password = Picked_password;
                    customer->is_blocked = Picked_is_blocked;
                    customer->cnic = Picked_cnic; 
                    if (customer->is_blocked != 1)
                    {
                        file.close();
                        return customer;
                    }
                    else
                    {
                        cout << "User has been blocked by the admin \n ";
                        file.close();
                        return customer;
                    }
                }
            }
        }
        file.close();
        return nullptr;
    }
};
class F_M_SESSIONS
{
public:
    int F_M_Sessions_For_one_user(string cnic)
    {
        int count = 0;
        ifstream file("Sessions.txt");
        if (!file.is_open())
        {
            cout << "\033[31m" << "Error: Could not open the file!" << "\033[0m" << endl;
            return 0;
        }
        string data;
        while (getline(file, data))
        {
            int i = 0;
            string picked_cnic;
            for (; data[i] != '|'; i++)
            {
                picked_cnic += data[i];
            }
            if (picked_cnic == cnic)
            {
                count++;
            }
        }
        file.close();
        return count;
    }
    void F_M_LOAD_FOR_SPECIFIC_USER(string cnic)
    {
        ifstream file_real2("Sessions.txt");
        // ofstream file_temp2("Temp2.txt");
        if (!(file_real2.is_open() /*&& file_temp2.is_open()*/))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
        }
        else
        {
            string data2;
            if (!(file_real2.is_open() /*&& file_temp2.is_open()*/))
            {
                cout << "UNABLE TO OpEN THE FILE " << endl;
            }
            else
            {
                while (getline(file_real2, data2))
                {
                    int i = 0;
                    string picked_cnic, picked_Timestamp, picked_FiltersAplied, picked_OutputFile;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_cnic = picked_cnic + data2[i];
                    }
                    i++;
                    if (picked_cnic == cnic)
                    {
                        for (; data2[i] != '|'; i++)
                        {
                            picked_Timestamp += data2[i];
                        }

                        i++;
                        for (; data2[i] != '|'; i++)
                        {
                            picked_FiltersAplied += data2[i];
                        }

                        i++;
                        for (; data2[i] != '\0'; i++)
                        {
                            picked_OutputFile += data2[i];
                        }

                        i++;

                        cout << picked_cnic << "|" << picked_Timestamp << "|" << picked_FiltersAplied << "|" << picked_OutputFile << endl;
                    }
                }

                file_real2.close();
            }
        }
    }
    int F_M_APPEND_SESSION(string cnic, string filterpipeline, string timestamp, string image_name)
    {
        ofstream file("Sessions.txt", ios::app);
        string data;

        if (!file.is_open())
        {
            cout << "UNABLE TO OPEN FILE ";
            return 0;
        }
        else
        {
            file << cnic << "|" << timestamp << "|" << filterpipeline << "|" << image_name << endl;
            return 1;
        }
    }
    int F_M_DELETE_SESSIONS_FOR_SPECIFIC_USER(string cnic)
    {
        ifstream file_real2("Sessions.txt");
        ofstream file_temp2("Temp2.txt");
        if (!(file_real2.is_open() && file_temp2.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
            return 0;
        }
        else
        {
            string data2;
            if (!(file_real2.is_open() && file_temp2.is_open()))
            {
                return 0;
            }
            else
            {
                while (getline(file_real2, data2))
                {
                    int i = 0;
                    string picked_cnic, picked_Timestamp, picked_FiltersAplied, picked_OutputFile;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_cnic = picked_cnic + data2[i];
                    }
                    i++;
                    if (picked_cnic != cnic)
                    {
                        for (; data2[i] != '|'; i++)
                        {
                            picked_Timestamp += data2[i];
                        }

                        i++;
                        for (; data2[i] != '|'; i++)
                        {
                            picked_FiltersAplied += data2[i];
                        }

                        i++;
                        for (; data2[i] != '\0'; i++)
                        {
                            picked_OutputFile += data2[i];
                        }

                        i++;

                        file_temp2 << picked_cnic << "|" << picked_Timestamp << "|" << picked_FiltersAplied << "|" << picked_OutputFile << endl;
                    }
                }

                file_real2.close();
                file_temp2.close();
                ofstream file_real2("Sessions.txt");
                ifstream file_temp2("Temp2.txt");

                if (!(file_real2.is_open() && file_temp2.is_open()))
                {
                    cout << "UNABLE TO OPEN THE FILE last" << endl;
                    return 0;
                }
                else
                {
                    while (getline(file_temp2, data2))
                    {
                        file_real2 << data2 << endl;
                    }
                }

                file_real2.close();
                file_temp2.close();
                return 1;
            }
        }
    }
};
class F_M_filter_catalog
{
public:
    int F_M_TOGGLE_AVAILABILITY(string filter_id)
    {
        ifstream file_real2("Catalog.txt");
        ofstream file_temp2("Temp3.txt");
        if (!(file_real2.is_open() && file_temp2.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
            return 0;
        }
        else
        {
            string data2;
            if (!(file_real2.is_open() && file_temp2.is_open()))
            {
                return 0;
            }
            else
            {
                while (getline(file_real2, data2))
                {

                    int i = 0;
                    string picked_filter_id, picked_filter_name, picked_Category;
                    bool picked_IsEnabled;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_id = picked_filter_id + data2[i];
                    }
                    i++;

                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_name += data2[i];
                    }

                    i++;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_Category += data2[i];
                    }

                    i++;
                    if (picked_filter_id == filter_id)
                    {

                        for (; data2[i] != '\0'; i++)
                        {

                            picked_IsEnabled = !(data2[i] == '1');
                        }
                    }
                    else
                    {

                        for (; data2[i] != '\0'; i++)
                        {
                            picked_IsEnabled = (data2[i] == '1');
                        }
                    }

                    file_temp2 << picked_filter_id << "|" << picked_filter_name << "|" << picked_Category << "|" << picked_IsEnabled << endl;
                }
            }

            file_real2.close();
            file_temp2.close();
            ofstream file_real2("Catalog.txt");
            ifstream file_temp2("Temp3.txt");

            if (!(file_real2.is_open() && file_temp2.is_open()))
            {
                cout << "UNABLE TO OPEN THE FILE last" << endl;
                return 0;
            }
            else
            {
                while (getline(file_temp2, data2))
                {
                    file_real2 << data2 << endl;
                }
            }

            file_real2.close();
            file_temp2.close();
            return 1;
        }
    }
    int F_M_Load(bool *array, Filter *filters[])
    {

        ifstream file_real2("Catalog.txt");

        if (!(file_real2.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
            return 0;
        }
        else
        {
            int counter = 0;
            string data2;
            if (!(file_real2.is_open()))
            {
                return 0;
            }
            else
            {
                while (getline(file_real2, data2))
                {

                    int i = 0;
                    string picked_filter_id, picked_filter_name, picked_Category;
                    bool picked_IsEnabled;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_id = picked_filter_id + data2[i];
                    }
                    i++;

                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_name += data2[i];
                    }

                    i++;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_Category += data2[i];
                    }

                    i++;

                    for (; data2[i] != '\0'; i++)
                    {

                        picked_IsEnabled = (data2[i] == '1');
                    }

                    i++;
                    array[counter] = picked_IsEnabled;
                    counter++;
                }
            }

            file_real2.close();

            for (int i = 0; i < 10; i++)
            {
                bool temp = array[i];
                filters[i]->set_avaliable(temp);
            }
            return 1;
        }
    }
    void display()
    {
        ifstream file_real2("Catalog.txt");

        if (!(file_real2.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
        }
        else
        {
            string data2;
            if (!(file_real2.is_open()))
            {
                cout << "UNABLE TO OpEN THE FILE " << endl;
            }
            else
            {
                while (getline(file_real2, data2))
                {

                    int i = 0;
                    string picked_filter_id, picked_filter_name, picked_Category;
                    bool picked_IsEnabled;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_id = picked_filter_id + data2[i];
                    }
                    i++;

                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_name += data2[i];
                    }

                    i++;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_Category += data2[i];
                    }

                    i++;

                    for (; data2[i] != '\0'; i++)
                    {

                        picked_IsEnabled = (data2[i] == '1');
                    }

                    i++;
                    cout << picked_filter_id << "|" << picked_filter_name << "|" << picked_Category << "|" << picked_IsEnabled << endl;
                }
            }

            file_real2.close();
        }
    }
    void display_customer()
    {
        ifstream file_real2("Catalog.txt");

        if (!(file_real2.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
        }
        else
        {
            string data2;
            if (!(file_real2.is_open()))
            {
                cout << "UNABLE TO OpEN THE FILE " << endl;
            }
            else
            {
                while (getline(file_real2, data2))
                {

                    int i = 0;
                    string picked_filter_id, picked_filter_name, picked_Category;
                    bool picked_IsEnabled;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_id = picked_filter_id + data2[i];
                    }
                    i++;

                    for (; data2[i] != '|'; i++)
                    {
                        picked_filter_name += data2[i];
                    }

                    i++;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_Category += data2[i];
                    }

                    i++;

                    for (; data2[i] != '\0'; i++)
                    {

                        picked_IsEnabled = (data2[i] == '1');
                    }

                    i++;
                    cout << picked_filter_id << "|" << picked_filter_name << endl;
                }
            }

            file_real2.close();
        }
    }
};
class Admin : public user
{
public:
    Admin()
    {
        cout << "\033[34m" << "Admin object created" << "\033[0m" << endl; // For submission only
    }
    ~Admin()
    {
        cout << "\033[34m" << "Admin object destroyed" << "\033[0m" << endl; // For submission only
    }
    bool login(string cnic, string e_password)
    {
        if (cnic == "6767678696969" && e_password == "khalid@2020")
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    bool toggle_block(string cnic)
    {
        F_M_Customers F_M_C;

        return (F_M_C.F_M_Toogle(cnic));
    }
    bool toggle_filter(string filter_ID)
    {
        F_M_filter_catalog F_M_F_C;
        return (F_M_F_C.F_M_TOGGLE_AVAILABILITY(filter_ID));
    }
    bool delete_account(string cnic)
    {
        F_M_Customers F_M_C;
        return (F_M_C.F_M_Delete(cnic));
    }
    void view_sessions_All()
    {
        ifstream file_real2("Sessions.txt");
        // ofstream file_temp2("Temp2.txt");
        if (!(file_real2.is_open() /*&& file_temp2.is_open()*/))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
        }
        else
        {
            string data2;
            if (!(file_real2.is_open() /*&& file_temp2.is_open()*/))
            {
                cout << "UNABLE TO OpEN THE FILE " << endl;
            }
            else
            {
                while (getline(file_real2, data2))
                {
                    int i = 0;
                    string picked_cnic, picked_Timestamp, picked_FiltersAplied, picked_OutputFile;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_cnic = picked_cnic + data2[i];
                    }
                    i++;

                    for (; data2[i] != '|'; i++)
                    {
                        picked_Timestamp += data2[i];
                    }

                    i++;
                    for (; data2[i] != '|'; i++)
                    {
                        picked_FiltersAplied += data2[i];
                    }

                    i++;
                    for (; data2[i] != '\0'; i++)
                    {
                        picked_OutputFile += data2[i];
                    }

                    i++;

                    cout << picked_cnic << "|" << picked_Timestamp << "|" << picked_FiltersAplied << "|" << picked_OutputFile << endl;
                }

                file_real2.close();
            }
        }
    }
    void view_sessions_single_user(string cnic)
    {
        F_M_SESSIONS F_M_S;
        F_M_S.F_M_LOAD_FOR_SPECIFIC_USER(cnic);
    }
    void Display_blocked()
    {
        ifstream file_real("Customers.txt");

        if (!(file_real.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
        }
        else
        {
            int count = 1;
            string data;
            while (getline(file_real, data))
            {
                int i = 0;

                string picked_password;
                string picked_cnic;
                string picked_name;
                string picked_Gender;
                string picked_phone;
                string picked_city;
                bool picked_is_blocked;

                for (; data[i] != '|'; i++)
                {
                    picked_cnic = picked_cnic + data[i];
                }
                i++;

                for (; data[i] != '|'; i++)
                {
                    picked_password += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_name += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_Gender += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_phone += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_city += data[i];
                }

                i++;

                for (; data[i] != '\0'; i++)
                {
                    picked_is_blocked = (data[i] == '1');
                }
                if (picked_is_blocked == 1)
                {
                    cout << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
                }
            }
            file_real.close();
        }
    }
};
bool Customer::login(string e_cnic, string e_password)
{
    F_M_Customers F_M_C;
    if (F_M_C.F_M_Load("Customers.txt", this, e_cnic, e_password))
    {

        return 1;
    }
    else
    {
        return 0;
    }
}
class Vailidations
{
public:
    bool Password_Check(string password)
    {
        int check = 0;
        int check2 = 0;
        int check3 = 0;

        if (password.length() == 9)
        {
            check++;
        }
        for (int i = 0; password[i] != '\0'; i++)
        {
            if ((password[i] >= 48) && (password[i] <= 57))
            {
                check2++;
            }
            else if ((password[i] >= 65) && (password[i] <= 90))
            {
                check3++;
            }
        }
        if (check == 1)
        {
            if (check2 >= 1)
            {
                if (check3 >= 1)
                {
                    return 1;
                }
            }
        }
        return 0;
    }

    int cnic_check(string cnic)
    {
        if (cnic.length() != 13)
        {
            return -2;
        }
        for (int i = 0; i < cnic.length(); i++)
        {
            if (cnic[i] < '0' || cnic[i] > '9')
                return -2;
        }
        ifstream file_real("Customers.txt");

        if (!(file_real.is_open()))
        {
            cout << "UNABLE TO OpEN THE FILE " << endl;
            return 0;
        }
        else
        {
            int count = 1;
            string data;
            while (getline(file_real, data))
            {
                int i = 0;

                string picked_password;
                string picked_cnic;
                string picked_name;
                string picked_Gender;
                string picked_phone;
                string picked_city;
                bool picked_is_blocked;

                for (; data[i] != '|'; i++)
                {
                    picked_cnic = picked_cnic + data[i];
                }
                i++;

                for (; data[i] != '|'; i++)
                {
                    picked_password += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_name += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_Gender += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_phone += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    picked_city += data[i];
                }

                i++;

                for (; data[i] != '\0'; i++)
                {
                    picked_is_blocked = (data[i] == '1');
                }
                if (picked_cnic == cnic && picked_is_blocked == 0)
                {
                    file_real.close();

                    return 1; // found in cnic_general_list
                }
                else if (picked_cnic == cnic && picked_is_blocked == 1)
                {
                    file_real.close();

                    return 2; // found in blocked
                }
            }
            file_real.close();
        }
        ifstream file_blocked("Blocked_Cnics.txt");
        if (!file_blocked.is_open())
        {
            cout << "UNABLE TO OPEN FILE ";
            return 0;
        }
        else
        {
            string data;
            while (getline(file_blocked, data))
            {
                if (data == cnic)
                {
                    file_blocked.close();
                    return 2; // found in blocked
                }
            }
            file_blocked.close();
        }
        return -1;
    }
};
string timestamp_maker()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    string timestamp = to_string(1900 + ltm->tm_year) + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + to_string(ltm->tm_min) + to_string(ltm->tm_sec);
    return timestamp;
}
int main()
{

    bool image_loaded = 0;
    F_M_Customers F_M_C;
    F_M_SESSIONS F_M_S;
    F_M_filter_catalog F_M_F_C;
    Vailidations validations;
    Admin admin;
    F_M_READ_WRITE F_M_R_W;

    GrayScale grayScale;
    Invert invert;
    Brightness_Adjust brightness_Adjust;
    Contrast_Stretch contrast_Stretch;
    Red_Channel_Only red_channel_only;
    Green_Channel_Only green_channel_only;
    Blue_Channel_Only blue_channel_only;
    Box_Blur box_Blur;
    Flip_Horizontal flip_Horizontal;
    Flip_Vertical flip_Vertical;

    Filter *filters[10] = {&grayScale, &invert, &brightness_Adjust, &contrast_Stretch, &red_channel_only, &green_channel_only, &blue_channel_only, &box_Blur, &flip_Horizontal, &flip_Vertical};
    bool availability[10];
    try
    {
        F_M_F_C.F_M_Load(availability, filters);
    }
    catch (exception &e)
    {
        cout << "\033[31m" << "Error loading catalog: " << e.what() << "\033[0m" << endl;
    }

mainmenu:
    for (int i = 0; i < 44; i++)
    {
        cout << "=";
    }
    cout << endl;
    cout << "||";
    cout << setw(40) << left << " WELCOME TO IMAGE FILTER STUDIO" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " PLEASE SELECT AN OPTION : " << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 1. ADMIN LOGIN" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 2. CUSTOMER LOGIN" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 3. CUSTOMER SIGNUP" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 4. EXIT" << setw(45) << left << "||" << endl;
    for (int i = 0; i < 44; i++)
    {
        cout << "=";
    }
    cout << endl;
    char choice1 = '\0';
    do
    {
        cout << "Please Enter Your choice : ";
        cin >> choice1;
        if (choice1 != '1' && choice1 != '2' && choice1 != '3' && choice1 != '4')
        {
            cout << "\033[33m" << "Invalid choice " << "\033[0m" << endl;
        }

    } while (choice1 != '1' && choice1 != '2' && choice1 != '3' && choice1 != '4');

    if (choice1 == '1')
    {
        int tries = 0; // checks how many login attemps have been made
        string cnic;
        string password;
        bool logged_in = 0;
        while (tries < 3)
        {
            cout << "\033[31m" << "NOTE : You have only " << 3 - tries << " attempts to login after which you will be redirected to main menu " << "\033[0m" << endl;

            cout << "Enter Admin CNIC: ";
            cin >> cnic;
            cout << "Enter Admin Password: ";
            cin >> password;
            tries++;
            if (admin.login(cnic, password))
            {
                logged_in = 1;
                break;
            }
            else
            {
                cout << "\033[33m" << "Invalid CNIC or Password. Please try again." << "\033[0m" << endl;
            }
        }

        if (logged_in)
        {
        admin_menu:
            for (int i = 0; i < 44; i++)
            {
                cout << "=";
            }
            cout << endl;
            cout << "||";
            cout << setw(40) << left << " ADMIN PANEL : IMAGE FILTER STUDIO " << setw(40) << left << "||" << endl;
            cout << "||";
            cout << setw(40) << left << " PLEASE SELECT AN OPTION :  " << setw(40) << left << "||" << endl;
            cout << "||";
            cout << setw(40) << left << " 1. Manage Filter Catalog " << setw(40) << left << "||" << endl;
            cout << "||";
            cout << setw(40) << left << " 2. Manage Customers " << setw(40) << left << "||" << endl;
            cout << "||";
            cout << setw(40) << left << " 3. View Sessions " << setw(40) << left << "||" << endl;
            cout << "||";
            cout << setw(40) << left << " 4. Logout  " << setw(45) << left << "||" << endl;
            for (int i = 0; i < 44; i++)
            {
                cout << "=";
            }
            cout << endl;
            char choice2 = '\0';
        again:
            cout << "Please Enter Your choice : ";
            cin >> choice2;
            if (choice2 != '1' && choice2 != '2' && choice2 != '3' && choice2 != '4')
            {
                cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
                goto again;
            }

            if (choice2 == '1')
            {
            filter_Catalog_menu:
                char choice3 = '\0';

                for (int i = 0; i < 44; i++)
                {
                    cout << "=";
                }
                cout << endl;
                cout << "||";
                cout << setw(40) << left << " MANAGE FILTER CATALOG " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " PLEASE SELECT AN OPTION :  " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 1. Toggle Filter Availability " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 2. View Filter Catalog " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 3. Back to Admin Menu  " << setw(45) << left << "||" << endl;
                for (int i = 0; i < 44; i++)
                {
                    cout << "=";
                }
                cout << endl;
                do
                {
                    cout << "Enter Your Choice : ";
                    cin >> choice3;
                    if (choice3 != '1' && choice3 != '2' && choice3 != '3')
                    {
                        cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
                    }
                } while (choice3 != '1' && choice3 != '2' && choice3 != '3');
                if (choice3 == '1')
                {
                Filter_Toggle:
                    string filter_id;
                    cout << "FOLLOWING IS THE FILTER CATALOG : " << endl;
                    F_M_F_C.display();
                    do
                    {
                        cout << "Enter Filter ID to toggle availability : ";
                        cin >> filter_id;
                        if (filter_id != "01" && filter_id != "02" && filter_id != "03" && filter_id != "04" && filter_id != "05" && filter_id != "06" && filter_id != "07" && filter_id != "08" && filter_id != "09" && filter_id != "10")
                        {
                            cout << "\033[33m" << "Invalid Filter ID Please Try Again " << "\033[0m" << endl;
                        }

                    } while (filter_id != "01" && filter_id != "02" && filter_id != "03" && filter_id != "04" && filter_id != "05" && filter_id != "06" && filter_id != "07" && filter_id != "08" && filter_id != "09" && filter_id != "10");

                    if (admin.toggle_filter(filter_id))
                    {
                        char choice6 = 0;
                        cout << "\033[32m" << "Filter Availability Toggled Successfully " << "\033[0m" << endl;
                        try
                        {
                            F_M_F_C.F_M_Load(availability, filters);
                        }
                        catch (exception &e)
                        {
                            cout << "\033[31m" << "Error loading catalog: " << e.what() << "\033[0m" << endl;
                        }
                        do
                        {
                            cout << "Do you wish to toggle avalibility of another filter (Y/N) : ";
                            cin >> choice6;
                            if (choice6 != 'y' && choice6 != 'Y' && choice6 != 'n' && choice6 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice6 != 'y' && choice6 != 'Y' && choice6 != 'n' && choice6 != 'N');
                        if (choice6 == 'y' || choice6 == 'Y')
                        {
                            goto Filter_Toggle;
                        }
                        else if (choice6 == 'n' || choice6 == 'N')
                        {
                            char choice7;
                            do
                            {
                                cout << "Do you wish to go back to filter catalog menu (Y/N): ";
                                cin >> choice7;
                                if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                            if (choice7 == 'y' || choice7 == 'Y')
                            {
                                goto filter_Catalog_menu;
                            }
                            else if (choice7 == 'n' || choice7 == 'N')
                            {
                                char choice8;
                                do
                                {
                                    cout << "Do you wish to go back to admin menu (Y/N) : ";

                                    cin >> choice8;
                                    if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');
                                if (choice8 == 'y' || choice8 == 'Y')
                                {
                                    goto admin_menu;
                                }
                                else if (choice8 == 'n' || choice8 == 'N')
                                {
                                    char choice9;
                                    do
                                    {
                                        cout << "Do you wish to go back to main menu (Y/N) : ";

                                        cin >> choice9;
                                        if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                        {
                                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                        }
                                    } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');
                                    if (choice9 == 'y' || choice9 == 'Y')
                                    {
                                        goto mainmenu;
                                    }
                                    else if (choice9 == 'n' || choice9 == 'N')
                                    {
                                        char choice10;

                                        do
                                        {
                                            cout << "Do you wish to exit the program (Y/N) : ";

                                            cin >> choice10;
                                            if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                            {
                                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                            }
                                        } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                        if (choice10 == 'y' || choice10 == 'Y')
                                        {
                                            exit(0);
                                        }
                                        else if (choice10 == 'n' || choice10 == 'N')
                                        {
                                            cout << " Select one of the following options : " << endl;
                                            cout << "1. Go back to main menu (You will be logged out) " << endl;
                                            cout << "2. Go back to admin menu " << endl;
                                            cout << "3. Go back to filter catalog menu " << endl;
                                            char choice11;
                                            do
                                            {
                                                cout << "Enter your choice : ";
                                                cin >> choice11;
                                                if (choice11 == '1')
                                                {
                                                    goto mainmenu;
                                                }
                                                else if (choice11 == '2')
                                                {
                                                    goto admin_menu;
                                                }
                                                else if (choice11 == '3')
                                                {
                                                    goto filter_Catalog_menu;
                                                }
                                                if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                                {
                                                    cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                                }
                                            } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        cout << "\033[31m" << "An error occurred while toggling filter availability " << "\033[0m" << endl;
                    }
                }
                else if (choice3 == '2')
                {

                    cout << "FILTER CATALOG IS AS FOLLOWS : " << endl;
                    F_M_F_C.display();

                    char choice13;
                    do
                    {
                        cout << "Do you wish to go back to filter catalog menu (Y/N): ";
                        cin >> choice13;
                        if (choice13 != 'y' && choice13 != 'Y' && choice13 != 'n' && choice13 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice13 != 'y' && choice13 != 'Y' && choice13 != 'n' && choice13 != 'N');
                    if (choice13 == 'y' || choice13 == 'Y')
                    {
                        goto filter_Catalog_menu;
                    }
                    else if (choice13 == 'n' || choice13 == 'N')
                    {
                        char choice12;
                        do
                        {
                            cout << "Do you wish to go back to admin menu (Y/N) : ";

                            cin >> choice12;
                            if (choice12 != 'y' && choice12 != 'Y' && choice12 != 'n' && choice12 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }

                        } while (choice12 != 'y' && choice12 != 'Y' && choice12 != 'n' && choice12 != 'N');
                        if (choice12 == 'y' || choice12 == 'Y')
                        {
                            goto admin_menu;
                        }
                        else if (choice12 == 'n' || choice12 == 'N')
                        {
                            char choice14;
                            do
                            {
                                cout << "Do you wish to go back to main menu (Y/N) : ";

                                cin >> choice14;
                                if (choice14 != 'y' && choice14 != 'Y' && choice14 != 'n' && choice14 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice14 != 'y' && choice14 != 'Y' && choice14 != 'n' && choice14 != 'N');
                            if (choice14 == 'y' || choice14 == 'Y')
                            {
                                goto mainmenu;
                            }
                            else if (choice14 == 'n' || choice14 == 'N')
                            {
                                char choice15;
                                do
                                {
                                    cout << "Do you wish to exit the program (Y/N) : ";

                                    cin >> choice15;
                                    if (choice15 != 'y' && choice15 != 'Y' && choice15 != 'n' && choice15 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice15 != 'y' && choice15 != 'Y' && choice15 != 'n' && choice15 != 'N');
                                if (choice15 == 'y' || choice15 == 'Y')
                                {
                                    exit(0);
                                }
                                else if (choice15 == 'n' || choice15 == 'N')
                                {
                                    cout << " Select one of the following options : " << endl;
                                    cout << "1. Go back to main menu (You will be logged out) " << endl;
                                    cout << "2. Go back to admin menu " << endl;
                                    cout << "3. Go back to filter catalog menu " << endl;
                                    char choice16;
                                    do
                                    {
                                        cout << "Enter your choice : ";
                                        cin >> choice16;
                                        if (choice16 == '1')
                                        {
                                            goto mainmenu;
                                        }
                                        else if (choice16 == '2')
                                        {
                                            goto admin_menu;
                                        }
                                        else if (choice16 == '3')
                                        {
                                            goto filter_Catalog_menu;
                                        }

                                        if (choice16 != '1' && choice16 != '2' && choice16 != '3')
                                        {
                                            cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                        }
                                    } while (choice16 != '1' && choice16 != '2' && choice16 != '3');
                                }
                            }
                        }
                    }
                }
                else if (choice3 == '3')
                {
                    goto admin_menu;
                }
            }
            if (choice2 == '2')
            {
            manage_customers_menu:
                char choice4 = '\0';
                for (int i = 0; i < 44; i++)
                {
                    cout << "=";
                }
                cout << endl;
                cout << "||";
                cout << setw(40) << left << " MANAGE CUSTOMERS " << setw(40) << left << "||" << endl;
                cout << "||";

                cout << setw(40) << left << " PLEASE SELECT AN OPTION :  " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 1. Toggle Customer Block Status " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 2. View Blocked Customers " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 3. Search Account " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 4. Delete Account " << setw(40) << left << "||" << endl;
                cout << "||";

                cout << setw(40) << left << " 5. Back to Admin Menu  " << setw(45) << left << "||" << endl;
                for (int i = 0; i < 44; i++)
                {
                    cout << "=";
                }
                cout << endl;
                do
                {
                    cout << "Enter Your Choice : ";
                    cin >> choice4;
                    if (choice4 != '1' && choice4 != '2' && choice4 != '3' && choice4 != '4' && choice4 != '5')
                    {
                        cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
                    }
                } while (choice4 != '1' && choice4 != '2' && choice4 != '3' && choice4 != '4' && choice4 != '5');

                if (choice4 == '3')
                {
                    string temp;
                    char choicek;
                search_Kind:
                    do
                    {
                        for (int i = 0; i < 44; i++)
                        {
                            cout << "=";
                        }
                        cout << endl;
                        cout << "||";
                        cout << setw(40) << left << " AVALIABLE SEARCH TYPES " << setw(40) << left << "||" << endl;
                        cout << "||";

                        cout << setw(40) << left << " PLEASE SELECT AN OPTION :  " << setw(40) << left << "||" << endl;
                        cout << "||";
                        cout << setw(40) << left << " 1. Search By Name " << setw(40) << left << "||" << endl;
                        cout << "||";
                        cout << setw(40) << left << " 2. Search By CNIC " << setw(40) << left << "||" << endl;
                        for (int i = 0; i < 44; i++)
                        {
                            cout << "=";
                        }
                        cout << endl;
                        cout << "Enter Your Choice : ";
                        cin >> choicek;
                        if (choicek != '1' && choicek != '2')
                        {
                            cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
                        }
                    } while (choicek != '1' && choicek != '2');
                    if (choicek == '2')
                    {
                        cout << "Enter the CNIC of the user You wish to Search : ";
                        cin >> temp;
                        Customer c;
                        if (F_M_C.F_M_SEARCH_cnic(temp, &c) != nullptr)
                        {
                            c.display();
                        }
                        else
                        {
                            cout << " UNABLE TO FIND USER " << endl;
                        }
                    }
                    else if (choicek == '1')
                    {
                        cout << "Enter the Name of the user You wish to Search : ";
                        cin.ignore();
                        getline(cin, temp);
                        Customer c;
                        if (F_M_C.F_M_SEARCH_name(temp, &c) != nullptr)
                        {
                            c.display();
                        }
                        else
                        {
                            cout << " UNABLE TO FIND USER " << endl;
                        }
                    }

                    char cop;
                    do
                    {
                        cout << "Do You wish to search for another user (Y/N): ";
                        cin >> cop;
                        if (cop != 'y' && cop != 'Y' && cop != 'n' && cop != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (cop != 'y' && cop != 'Y' && cop != 'n' && cop != 'N');
                    if (cop == 'y' || cop == 'Y')
                    {
                        goto search_Kind;
                    }
                    else if (cop == 'n' || cop == 'N')
                    {
                        char choice7;
                        do
                        {
                            cout << "Do you wish to go back to manage customers menu (Y/N): ";
                            cin >> choice7;
                            if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                        if (choice7 == 'y' || choice7 == 'Y')
                        {
                            goto manage_customers_menu;
                        }
                        else if (choice7 == 'n' || choice7 == 'N')
                        {
                            char choice8;
                            do
                            {
                                cout << "Do you wish to go back to admin menu (Y/N) : ";

                                cin >> choice8;
                                if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');
                            if (choice8 == 'y' || choice8 == 'Y')
                            {
                                goto admin_menu;
                            }
                            else if (choice8 == 'n' || choice8 == 'N')
                            {
                                char choice9;
                                do
                                {
                                    cout << "Do you wish to go back to main menu (Y/N) : ";

                                    cin >> choice9;
                                    if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');
                                if (choice9 == 'y' || choice9 == 'Y')
                                {
                                    goto mainmenu;
                                }
                                else if (choice9 == 'n' || choice9 == 'N')
                                {
                                    char choice10;
                                    do
                                    {
                                        cout << "Do you wish to exit the program (Y/N) : ";

                                        cin >> choice10;
                                        if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                        {
                                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                        }
                                    } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                    if (choice10 == 'y' || choice10 == 'Y')
                                    {
                                        exit(0);
                                    }
                                    else if (choice10 == 'n' || choice10 == 'N')
                                    {
                                        cout << " Select one of the following options : " << endl;
                                        cout << "1. Go back to main menu (You will be logged out) " << endl;
                                        cout << "2. Go back to admin menu " << endl;
                                        cout << "3. Go back to manage customers menu " << endl;
                                        char choice11;
                                        do
                                        {
                                            cout << "Enter your choice : ";
                                            cin >> choice11;
                                            if (choice11 == '1')
                                            {
                                                goto mainmenu;
                                            }
                                            else if (choice11 == '2')
                                            {
                                                goto admin_menu;
                                            }
                                            else if (choice11 == '3')
                                            {
                                                goto manage_customers_menu;
                                            }
                                            if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                            {
                                                cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                            }
                                        } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                    }
                                }
                            }
                        }
                    }
                }

                else if (choice4 == '4')
                {
                    string temp;
                    F_M_C.F_M_Display__All();
                del:
                    cout << "Enter the CNIC of the user You wish to delete : ";
                    cin >> temp;

                    Customer c;
                    if (F_M_C.F_M_SEARCH_cnic(temp, &c) != nullptr)
                    {
                        try
                        {
                            admin.delete_account(temp);
                            cout << "\033[32m" << "Account Deleted Successfully" << "\033[0m" << endl;
                        }
                        catch (exception &e)
                        {
                            cout << "\033[31m" << "Error deleting account: " << e.what() << "\033[0m" << endl;
                        }
                    }
                    else
                    {
                        cout << "\033[33m" << " Invalid CNIC " << "\033[0m" << endl;
                        goto del;
                    }
                    char cop;
                    do
                    {
                        cout << "Do You wish to Delete for another user (Y/N): ";
                        cin >> cop;
                        if (cop != 'y' && cop != 'Y' && cop != 'n' && cop != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (cop != 'y' && cop != 'Y' && cop != 'n' && cop != 'N');
                    if (cop == 'y' || cop == 'Y')
                    {
                        goto del;
                    }
                    else if (cop == 'n' || cop == 'N')
                    {
                        char choice7;
                        do
                        {
                            cout << "Do you wish to go back to manage customers menu (Y/N): ";
                            cin >> choice7;
                            if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                        if (choice7 == 'y' || choice7 == 'Y')
                        {
                            goto manage_customers_menu;
                        }
                        else if (choice7 == 'n' || choice7 == 'N')
                        {
                            char choice8;
                            do
                            {
                                cout << "Do you wish to go back to admin menu (Y/N) : ";

                                cin >> choice8;
                                if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');
                            if (choice8 == 'y' || choice8 == 'Y')
                            {
                                goto admin_menu;
                            }
                            else if (choice8 == 'n' || choice8 == 'N')
                            {

                                char choice9;
                                do
                                {
                                    cout << "Do you wish to go back to main menu (Y/N) : ";

                                    cin >> choice9;
                                    if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');
                                if (choice9 == 'y' || choice9 == 'Y')
                                {
                                    goto mainmenu;
                                }
                                else if (choice9 == 'n' || choice9 == 'N')
                                {
                                    char choice10;
                                    do
                                    {
                                        cout << "Do you wish to exit the program (Y/N) : ";

                                        cin >> choice10;
                                        if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                        {
                                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                        }
                                    } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                    if (choice10 == 'y' || choice10 == 'Y')
                                    {
                                        exit(0);
                                    }
                                    else if (choice10 == 'n' || choice10 == 'N')
                                    {
                                        cout << " Select one of the following options : " << endl;
                                        cout << "1. Go back to main menu (You will be logged out) " << endl;
                                        cout << "2. Go back to admin menu " << endl;
                                        cout << "3. Go back to manage customers menu " << endl;
                                        char choice11;
                                        do
                                        {
                                            cout << "Enter your choice : ";
                                            cin >> choice11;
                                            if (choice11 == '1')
                                            {
                                                goto mainmenu;
                                            }
                                            else if (choice11 == '2')
                                            {
                                                goto admin_menu;
                                            }
                                            else if (choice11 == '3')
                                            {
                                                goto manage_customers_menu;
                                            }
                                            if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                            {
                                                cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                            }
                                        } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                    }
                                }
                            }
                        }
                    }
                }
                else if (choice4 == '1')
                {
                Customer_Toggle:
                    string cnic;
                    cout << "Following are the Customers : ";
                    F_M_C.F_M_Display__All();
                    cout << "Enter CNIC of Customer to toggle block status : ";
                    cin >> cnic;
                    try
                    {
                        if (admin.toggle_block(cnic))
                        {
                            cout << "\033[32m" << "Customer Block Status Toggled Successfully " << "\033[0m" << endl;

                            char choice6 = '\0';

                            do
                            {
                                cout << "Do you wish to toggle block status of another customer (Y/N) : ";
                                cin >> choice6;
                                if (choice6 != 'y' && choice6 != 'Y' && choice6 != 'n' && choice6 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice6 != 'y' && choice6 != 'Y' && choice6 != 'n' && choice6 != 'N');
                            if (choice6 == 'y' || choice6 == 'Y')
                            {
                                goto Customer_Toggle;
                            }
                            else if (choice6 == 'n' || choice6 == 'N')
                            {
                                char choice7;
                                do
                                {
                                    cout << "Do you wish to go back to manage customers menu (Y/N): ";
                                    cin >> choice7;
                                    if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                                if (choice7 == 'y' || choice7 == 'Y')
                                {
                                    goto manage_customers_menu;
                                }
                                else if (choice7 == 'n' || choice7 == 'N')
                                {
                                    char choice8;
                                    do
                                    {
                                        cout << "Do you wish to go back to admin menu (Y/N) : ";

                                        cin >> choice8;
                                        if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                                        {
                                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                        }
                                    } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');
                                    if (choice8 == 'y' || choice8 == 'Y')
                                    {
                                        goto admin_menu;
                                    }
                                    else if (choice8 == 'n' || choice8 == 'N')
                                    {
                                        char choice9;
                                        do
                                        {
                                            cout << "Do you wish to go back to main menu (Y/N) : ";

                                            cin >> choice9;
                                            if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                            {
                                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                            }
                                        } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');
                                        if (choice9 == 'y' || choice9 == 'Y')
                                        {
                                            goto mainmenu;
                                        }
                                        else if (choice9 == 'n' || choice9 == 'N')
                                        {
                                            char choice10;
                                            do
                                            {
                                                cout << "Do you wish to exit the program (Y/N) : ";
                                                cin >> choice10;
                                                if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                                {
                                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                                }
                                            } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                            if (choice10 == 'y' || choice10 == 'Y')
                                            {
                                                exit(0);
                                            }
                                            else if (choice10 == 'n' || choice10 == 'N')
                                            {
                                                cout << " Select one of the following options : " << endl;
                                                cout << "1. Go back to main menu (You will be logged out) " << endl;
                                                cout << "2. Go back to admin menu " << endl;
                                                cout << "3. Go back to manage customers menu " << endl;
                                                char choice11;
                                                do
                                                {
                                                    cout << "Enter your choice : ";
                                                    cin >> choice11;
                                                    if (choice11 == '1')
                                                    {
                                                        goto mainmenu;
                                                    }
                                                    else if (choice11 == '2')
                                                    {
                                                        goto admin_menu;
                                                    }
                                                    else if (choice11 == '3')
                                                    {
                                                        goto manage_customers_menu;
                                                    }
                                                    if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                                    {
                                                        cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                                    }
                                                } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            cout << "\033[31m" << "An error occurred while toggling customer block status " << "\033[0m" << endl;
                        }
                    }
                    catch (exception &e)
                    {
                        cout << "\033[31m" << "Error toggling block: " << e.what() << "\033[0m" << endl;
                    }
                }
                else if (choice4 == '2')
                {
                    cout << "BLOCKED CUSTOMERS ARE AS FOLLOWS : " << endl;
                    admin.Display_blocked();
                    char choice7;
                    do
                    {
                        cout << "Do you wish to go back to manage customers menu (Y/N): ";
                        cin >> choice7;
                        if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                    if (choice7 == 'y' || choice7 == 'Y')
                    {
                        goto manage_customers_menu;
                    }
                    else if (choice7 == 'n' || choice7 == 'N')
                    {
                        char choice8;
                        do
                        {

                            cout << "Do you wish to go back to admin menu (Y/N) : ";

                            cin >> choice8;
                            if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');
                        if (choice8 == 'y' || choice8 == 'Y')
                        {
                            goto admin_menu;
                        }
                        else if (choice8 == 'n' || choice8 == 'N')
                        {
                            char choice9;
                            do
                            {
                                cout << "Do you wish to go back to main menu (Y/N) : ";

                                cin >> choice9;
                                if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');

                            if (choice9 == 'y' || choice9 == 'Y')
                            {
                                goto mainmenu;
                            }
                            else if (choice9 == 'n' || choice9 == 'N')
                            {
                                char choice10;
                                do
                                {
                                    cout << "Do you wish to exit the program (Y/N) : ";

                                    cin >> choice10;
                                    if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                if (choice10 == 'y' || choice10 == 'Y')
                                {
                                    exit(0);
                                }
                                else if (choice10 == 'n' || choice10 == 'N')
                                {
                                    cout << " Select one of the following options : " << endl;
                                    cout << "1. Go back to main menu (You will be logged out) " << endl;
                                    cout << "2. Go back to admin menu " << endl;
                                    cout << "3. Go back to manage customers menu " << endl;
                                    char choice11;
                                    do
                                    {
                                        cout << "Enter your choice : ";
                                        cin >> choice11;
                                        if (choice11 == '1')
                                        {
                                            goto mainmenu;
                                        }
                                        else if (choice11 == '2')
                                        {
                                            goto admin_menu;
                                        }
                                        else if (choice11 == '3')
                                        {
                                            goto manage_customers_menu;
                                        }
                                        if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                        {
                                            cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                        }
                                    } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                }
                            }
                        }
                    }
                }
                else if (choice4 == '5')
                {
                    goto admin_menu;
                }
            }
            else if (choice2 == '3')
            {
            view_sessions_menu:
                char choice5 = '\0';

                for (int i = 0; i < 45; i++)
                {
                    cout << "=";
                }
                cout << endl;
                cout << "||";
                cout << setw(40) << left << " VIEW SESSIONS " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " PLEASE SELECT AN OPTION :  " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 1. View All Sessions " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 2. View Sessions of Specific Customer " << setw(40) << left << "||" << endl;
                cout << "||";
                cout << setw(40) << left << " 3. Back to Admin Menu  " << setw(45) << left << "||" << endl;
                for (int i = 0; i < 45; i++)
                {
                    cout << "=";
                }
                cout << endl;
                do
                {
                    cout << "Enter Your Choice : ";
                    cin >> choice5;
                    if (choice5 != '1' && choice5 != '2' && choice5 != '3')
                    {
                        cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
                    }
                } while (choice5 != '1' && choice5 != '2' && choice5 != '3');

                if (choice5 == '1')
                {
                    cout << "Following are all the sessions : " << endl;
                    admin.view_sessions_All();
                    char choice7;
                    do
                    {
                        cout << "Do you wish to go back to view sessions menu (Y/N): ";
                        cin >> choice7;
                        if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                    if (choice7 == 'y' || choice7 == 'Y')
                    {
                        goto view_sessions_menu;
                    }
                    else if (choice7 == 'n' || choice7 == 'N')
                    {
                        char choice8;
                        do
                        {
                            cout << "Do you wish to go back to admin menu (Y/N) : ";

                            cin >> choice8;
                            if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');

                        if (choice8 == 'y' || choice8 == 'Y')
                        {
                            goto admin_menu;
                        }
                        else if (choice8 == 'n' || choice8 == 'N')
                        {
                            char choice9;
                            do
                            {
                                cout << "Do you wish to go back to main menu (Y/N) : ";

                                cin >> choice9;
                                if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');
                            if (choice9 == 'y' || choice9 == 'Y')
                            {
                                goto mainmenu;
                            }
                            else if (choice9 == 'n' || choice9 == 'N')
                            {
                                char choice10;
                                do
                                {
                                    cout << "Do you wish to exit the program (Y/N) : ";

                                    cin >> choice10;
                                    if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                if (choice10 == 'y' || choice10 == 'Y')
                                {
                                    exit(0);
                                }
                                else if (choice10 == 'n' || choice10 == 'N')
                                {
                                    cout << " Select one of the following options : " << endl;
                                    cout << "1. Go back to main menu (You will be logged out) " << endl;
                                    cout << "2. Go back to admin menu " << endl;
                                    cout << "3. Go back to view sessions menu " << endl;
                                    char choice11;
                                    do
                                    {
                                        cout << "Enter your choice : ";
                                        cin >> choice11;
                                        if (choice11 == '1')
                                        {
                                            goto mainmenu;
                                        }
                                        else if (choice11 == '2')
                                        {
                                            goto admin_menu;
                                        }
                                        else if (choice11 == '3')
                                        {
                                            goto view_sessions_menu;
                                        }
                                        if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                        {
                                            cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                        }
                                    } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                }
                            }
                        }
                    }
                }
                else if (choice5 == '2')
                {
                    string cnic;
                    cout << "Enter CNIC of Customer to view sessions : ";
                    cin >> cnic;
                    cout << "Following are the sessions of customer with CNIC : " << cnic << " : " << endl;
                    admin.view_sessions_single_user(cnic);
                    char choice7;
                    do
                    {
                        cout << "Do you wish to go back to view sessions menu (Y/N): ";
                        cin >> choice7;
                        if (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice7 != 'y' && choice7 != 'Y' && choice7 != 'n' && choice7 != 'N');
                    if (choice7 == 'y' || choice7 == 'Y')
                    {
                        goto view_sessions_menu;
                    }
                    else if (choice7 == 'n' || choice7 == 'N')
                    {
                        char choice8;
                        do
                        {
                            cout << "Do you wish to go back to admin menu (Y/N) : ";

                            cin >> choice8;
                            if (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice8 != 'y' && choice8 != 'Y' && choice8 != 'n' && choice8 != 'N');

                        if (choice8 == 'y' || choice8 == 'Y')
                        {
                            goto admin_menu;
                        }
                        else if (choice8 == 'n' || choice8 == 'N')
                        {
                            char choice9;
                            do
                            {
                                cout << "Do you wish to go back to main menu (Y/N) : ";

                                cin >> choice9;

                                if (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N')
                                {
                                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                }
                            } while (choice9 != 'y' && choice9 != 'Y' && choice9 != 'n' && choice9 != 'N');
                            if (choice9 == 'y' || choice9 == 'Y')
                            {
                                goto mainmenu;
                            }
                            else if (choice9 == 'n' || choice9 == 'N')
                            {
                                char choice10;
                                do
                                {
                                    cout << "Do you wish to exit the program (Y/N) : ";

                                    cin >> choice10;
                                    if (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N')
                                    {
                                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                                    }
                                } while (choice10 != 'y' && choice10 != 'Y' && choice10 != 'n' && choice10 != 'N');
                                if (choice10 == 'y' || choice10 == 'Y')
                                {
                                    exit(0);
                                }
                                else if (choice10 == 'n' || choice10 == 'N')
                                {
                                    cout << " Select one of the following options : " << endl;
                                    cout << "1. Go back to main menu (You will be logged out) " << endl;
                                    cout << "2. Go back to admin menu " << endl;
                                    cout << "3. Go back to view sessions menu " << endl;
                                    char choice11;
                                    do
                                    {
                                        cout << "Enter your choice : ";
                                        cin >> choice11;
                                        if (choice11 == '1')
                                        {
                                            goto mainmenu;
                                        }
                                        else if (choice11 == '2')
                                        {
                                            goto admin_menu;
                                        }
                                        else if (choice11 == '3')
                                        {
                                            goto view_sessions_menu;
                                        }
                                        if (choice11 != '1' && choice11 != '2' && choice11 != '3')
                                        {
                                            cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                        }
                                    } while (choice11 != '1' && choice11 != '2' && choice11 != '3');
                                }
                            }
                        }
                    }
                }
                else if (choice5 == '3')
                {
                    goto admin_menu;
                }
            }
            else if (choice2 == '4')
            {
                cout << "Logging out and going back to main menu " << endl;
                goto mainmenu;
            }
        }
        else
        {
            cout << "\033[31m" << " Too many failed login attempts Going back to main menu " << "\033[0m" << endl;
            goto mainmenu;
        }
    }
    if (choice1 == '2')
    {
        Image image;
        image_loaded = false;
        Customer customer;
        cout << "Customer Login Selected " << endl;
        string cnic, password;
        int login_attempts = 0;

        while (login_attempts < 3)
        {
            cout << "\033[31m" << "NOTE : You have only " << 3 - login_attempts << " attempts to login after which you will be redirected to main menu " << "\033[0m" << endl;

            cout << "Enter CNIC : ";
            cin >> cnic;
            cout << "Enter Password : ";
            cin >> password;
            try
            {
                if (customer.login(cnic, password))
                {
                    break;
                }
                else
                {
                    cout << "\033[33m" << "Invalid CNIC or Password " << "\033[0m" << endl;
                    login_attempts++;
                }
            }
            catch (exception &e)
            {
                cout << "\033[31m" << "Login error: " << e.what() << "\033[0m" << endl;
                login_attempts++;
            }
        }
        if (login_attempts == 3)
        {
            cout << " Too many failed login attempts Going back to main menu " << endl;
            goto mainmenu;
        }
        FilterSession filterSession(customer.customer_CNIC_getter(), timestamp_maker(), &image);
    customer_menu:
        for (int i = 0; i < 46; i++)
        {
            cout << "=";
        }
        cout << endl;

        cout << "|| " << left << setw(41) << (" WELCOME, " + customer.customer_name_getter()) << "||" << endl;
        cout << "|| " << left << setw(41) << (" Sessions completed : " + to_string(F_M_S.F_M_Sessions_For_one_user(customer.customer_CNIC_getter()))) << "||" << endl;
        for (int i = 0; i < 46; i++)
        {
            cout << "=";
        }
        cout << endl;
        cout << "|| " << left << setw(41) << " PLEASE SELECT AN OPTION : " << "||" << endl;
        cout << "|| " << left << setw(41) << " 1. Browse Filter Catalog" << "||" << endl;
        cout << "|| " << left << setw(41) << " 2. Load Image" << "||" << endl;
        cout << "|| " << left << setw(41) << " 3. Build Filter Pipeline" << "||" << endl;
        cout << "|| " << left << setw(41) << " 4. Apply Pipeline & Save Result" << "||" << endl;
        cout << "|| " << left << setw(41) << " 5. View My Session History" << "||" << endl;
        cout << "|| " << left << setw(41) << " 6. Logout" << "||" << endl;

        for (int i = 0; i < 46; i++)
        {
            cout << "=";
        }
        cout << endl;
        char choice2 = '\0';
        do
        {
            cout << "Please Enter Your choice : ";
            cin >> choice2;
            if (choice2 != '1' && choice2 != '2' && choice2 != '3' && choice2 != '4' && choice2 != '5' && choice2 != '6')
            {
                cout << "\033[33m" << "Invalid Choice Please Try Again " << "\033[0m" << endl;
            }

        } while (choice2 != '1' && choice2 != '2' && choice2 != '3' && choice2 != '4' && choice2 != '5' && choice2 != '6');
        if ((choice2 == '3' || choice2 == '4') && image_loaded == false)
        {
            cout << "\033[31m" << "Unable to proceed please load an image first " << "\033[0m" << endl;
            goto customer_menu;
        }

        if (choice2 == '1')
        {
            cout << "FILTER CATALOG IS AS FOLLOWS : " << endl;
            F_M_F_C.display_customer();

            char choice3;
            do
            {
                cout << "Do you wish to go back to customer menu (Y/N): ";
                cin >> choice3;
                if (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N');
            if (choice3 == 'y' || choice3 == 'Y')
            {
                goto customer_menu;
            }
            else if (choice3 == 'n' || choice3 == 'N')
            {
                char choice4;
                do
                {
                    cout << "Do you wish to go back to main menu (Y/N) : ";

                    cin >> choice4;
                    if (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N')
                    {
                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                    }
                } while (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N');
                if (choice4 == 'y' || choice4 == 'Y')
                {
                    goto mainmenu;
                }
                else if (choice4 == 'n' || choice4 == 'N')
                {
                    char choice5;
                    do
                    {
                        cout << "Do you wish to exit the program (Y/N) : ";

                        cin >> choice5;
                        if (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N');
                    if (choice5 == 'y' || choice5 == 'Y')
                    {
                        exit(0);
                    }
                    else if (choice5 == 'n' || choice5 == 'N')
                    {
                        cout << " Select one of the following options : " << endl;
                        cout << "1. Go back to main menu (You will be logged out) " << endl;
                        cout << "2. Go back to customer menu " << endl;

                        char choice6;
                        do
                        {
                            cout << "Enter your choice : ";
                            cin >> choice6;
                            if (choice6 == '1')
                            {
                                goto mainmenu;
                            }
                            else if (choice6 == '2')
                            {
                                goto customer_menu;
                            }
                            if (choice6 != '1' && choice6 != '2')
                            {
                                cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                            }
                        } while (choice6 != '1' && choice6 != '2');
                    }
                }
            }
        }
        if (choice2 == '2')
        {
            string image_path;
            char load_choice;
            for (int i = 0; i < 46; i++)
            {
                cout << "=";
            }
            cout << endl;

            cout << "|| " << left << setw(41) << "Load Image " << "||" << endl;

            for (int i = 0; i < 46; i++)
            {
                cout << "=";
            }
            cout << endl;
            cout << "|| " << left << setw(41) << " PLEASE SELECT AN OPTION : " << "||" << endl;
            cout << "|| " << left << setw(41) << "1. Load from JPG/PNG file" << "||" << endl;
            cout << "|| " << left << setw(41) << "2. Generate test pattern " << "||" << endl;

            for (int i = 0; i < 46; i++)
            {
                cout << "=";
            }
            cout << endl;

            do
            {
                cout << "Enter Your Choice: ";
                cin >> load_choice;
            } while (load_choice != '1' && load_choice != '2');

            if (load_choice == '1')
            {
                cout << "Enter the path of the image you want to load: ";
                cin >> image_path;
                Image *image_temp = nullptr;
                try
                {
                    image_temp = F_M_R_W.F_M_READ_IMAGE(image_path);
                }
                catch (exception &e)
                {
                    cout << "\033[31m" << "Error reading image: " << e.what() << "\033[0m" << endl;
                    goto customer_menu;
                }
                if (image_temp == nullptr)
                {

                    goto customer_menu;
                }
                image_loaded = 1;
                image = *image_temp;
                delete image_temp;

                cout << "\033[32m" << "Image Loaded Successfully " << "\033[0m" << endl;
            }
            else if (load_choice == '2')
            {

                Image *image_temp = new Image("test_pattern.png", 100, 100);
                for (int i = 0; i < 100; i++)
                {
                    for (int j = 0; j < 100; j++)
                    {

                        if ((i / 10 + j / 10) % 2 == 0)
                        {
                            image_temp->GridGetter()[i][j].setter_Red(255);
                            image_temp->GridGetter()[i][j].setter_Green(0);
                            image_temp->GridGetter()[i][j].setter_Blue(0);
                        }
                        else
                        {
                            image_temp->GridGetter()[i][j].setter_Red(0);
                            image_temp->GridGetter()[i][j].setter_Green(0);
                            image_temp->GridGetter()[i][j].setter_Blue(255);
                        }
                    }
                }
                image_loaded = 1;
                image = *image_temp;
                delete image_temp;
                cout << "\033[32m" << "Test Pattern Generated Successfully (100x100)" << "\033[0m" << endl;
            }
            char special_choice;
            do
            {
                cout << "Do you wish to view the image brightness in ascii (Y/N) : ";
                cin >> special_choice;
                if (special_choice != 'y' && special_choice != 'Y' && special_choice != 'n' && special_choice != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (special_choice != 'y' && special_choice != 'Y' && special_choice != 'n' && special_choice != 'N');
            if ((special_choice == 'y' || special_choice == 'Y'))
            {
                image.display_Ascii();
            }
            if (special_choice == 'n' || special_choice == 'N' || special_choice == 'y' || special_choice == 'Y')
            {
                char choice3;
                do
                {
                    cout << "Do you wish to go back to customer menu (Y/N): ";
                    cin >> choice3;
                    if (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N')
                    {
                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                    }
                } while (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N');
                if (choice3 == 'y' || choice3 == 'Y')
                {
                    goto customer_menu;
                }
                else if (choice3 == 'n' || choice3 == 'N')
                {
                    char choice4;
                    do
                    {
                        cout << "Do you wish to go back to main menu (Y/N) : ";

                        cin >> choice4;
                        if (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N');
                    if (choice4 == 'y' || choice4 == 'Y')
                    {
                        goto mainmenu;
                    }
                    else if (choice4 == 'n' || choice4 == 'N')
                    {
                        char choice5;
                        do
                        {
                            cout << "Do you wish to exit the program (Y/N) : ";

                            cin >> choice5;
                            if (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N')
                            {
                                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                            }
                        } while (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N');
                        if (choice5 == 'y' || choice5 == 'Y')
                        {
                            exit(0);
                        }
                        else if (choice5 == 'n' || choice5 == 'N')
                        {
                            cout << " Select one of the following options : " << endl;
                            cout << "1. Go back to main menu (You will be logged out) " << endl;
                            cout << "2. Go back to customer menu " << endl;

                            char choice6;
                            do
                            {
                                cout << "Enter your choice : ";
                                cin >> choice6;
                                if (choice6 == '1')
                                {
                                    goto mainmenu;
                                }
                                else if (choice6 == '2')
                                {
                                    goto customer_menu;
                                }
                                if (choice6 != '1' && choice6 != '2')
                                {
                                    cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                                }
                            } while (choice6 != '1' && choice6 != '2');
                        }
                    }
                }
            }
        }
        if (choice2 == '3')
        {
            string filter_id;
            int increment;
            F_M_F_C.display_customer();
            do
            {
            filter_pipeline:
                cout << "Enter the ID of the filter you want to add to pipeline : ";
                cin >> filter_id;

                if (filter_id != "01" && filter_id != "02" && filter_id != "03" && filter_id != "04" && filter_id != "05" && filter_id != "06" && filter_id != "07" && filter_id != "08" && filter_id != "09" && filter_id != "10")
                {
                    cout << "\033[33m" << "Invalid Filter ID Please try again " << "\033[0m" << endl;
                }
                if (filter_id == "03")
                {
                up:
                    cout << "Enter the increment value for brightness adjustment filter : ";
                    cin >> increment;
                    if (increment < -255 || increment > 255)
                    {
                        cout << "\033[31m" << "Invalid increment value Please try again (Recommended value is between -255 to 255) " << "\033[0m" << endl;
                        goto up;
                    }
                    else
                    {
                        brightness_Adjust.increment_setter(increment);
                    }
                }

            } while (filter_id != "01" && filter_id != "02" && filter_id != "03" && filter_id != "04" && filter_id != "05" && filter_id != "06" && filter_id != "07" && filter_id != "08" && filter_id != "09" && filter_id != "10");
            if (filter_id == "01")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + grayScale.name_getter() + ">";
                filterSession.add_filter(&grayScale, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Grayscale Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "02")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + invert.name_getter() + ">";
                filterSession.add_filter(&invert, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Invert Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "03")
            {
                int temp = brightness_Adjust.increment_getter();
                bool is_done = 0;
                string k = "";
                if (temp > 0)
                {
                    k = filterSession.getter() + brightness_Adjust.name_getter() + "+" + to_string(increment) + ">";
                }
                else
                {
                    k = filterSession.getter() + brightness_Adjust.name_getter() + to_string(increment) + ">";
                }
                filterSession.add_filter(&brightness_Adjust, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Brightness Adjustment Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "04")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + contrast_Stretch.name_getter() + ">";
                filterSession.add_filter(&contrast_Stretch, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Contrast Adjustment Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "05")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + red_channel_only.name_getter() + ">";
                filterSession.add_filter(&red_channel_only, is_done).setter(k);
                if (is_done == 1)
                {

                    cout << "\033[32m" << "Red Channel Only Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "06")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + green_channel_only.name_getter() + ">";
                filterSession.add_filter(&green_channel_only, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Green Channel Only Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "07")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + blue_channel_only.name_getter() + ">";
                filterSession.add_filter(&blue_channel_only, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Blue Channel Only Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "08")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + box_Blur.name_getter() + ">";
                filterSession.add_filter(&box_Blur, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Box Blur Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "09")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + flip_Horizontal.name_getter() + ">";
                filterSession.add_filter(&flip_Horizontal, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Flip Horizontal Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }
            else if (filter_id == "10")
            {
                bool is_done = 0;
                string k = "";
                k = filterSession.getter() + flip_Vertical.name_getter() + ">";
                filterSession.add_filter(&flip_Vertical, is_done).setter(k);
                if (is_done == 1)
                {
                    cout << "\033[32m" << "Flip Vertical Filter Added to Pipeline Successfully " << "\033[0m" << endl;
                }
            }

            char once_more;
            do
            {
                cout << "Do you wish to add another filter to pipeline (Y/N) : ";
                cin >> once_more;
                if (once_more != 'y' && once_more != 'Y' && once_more != 'n' && once_more != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (once_more != 'y' && once_more != 'Y' && once_more != 'n' && once_more != 'N');
            if (once_more == 'y' || once_more == 'Y')
            {
                goto filter_pipeline;
            }
            char choice3;
            do
            {
                cout << "Do you wish to go back to customer menu (Y/N): ";
                cin >> choice3;
                if (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N');
            if (choice3 == 'y' || choice3 == 'Y')
            {
                goto customer_menu;
            }
            else if (choice3 == 'n' || choice3 == 'N')
            {
                char choice4;
                do
                {
                    cout << "Do you wish to go back to main menu (Y/N) : ";

                    cin >> choice4;
                    if (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N')
                    {
                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                    }
                } while (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N');
                if (choice4 == 'y' || choice4 == 'Y')
                {
                    goto mainmenu;
                }
                else if (choice4 == 'n' || choice4 == 'N')
                {
                    char choice5;
                    do
                    {
                        cout << "Do you wish to exit the program (Y/N) : ";

                        cin >> choice5;

                        if (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N');

                    if (choice5 == 'y' || choice5 == 'Y')
                    {
                        exit(0);
                    }
                    else if (choice5 == 'n' || choice5 == 'N')
                    {
                        cout << " Select one of the following options : " << endl;
                        cout << "1. Go back to main menu (You will be logged out) " << endl;
                        cout << "2. Go back to customer menu " << endl;

                        char choice6;
                        do
                        {
                            cout << "Enter your choice : ";
                            cin >> choice6;
                            if (choice6 == '1')
                            {
                                goto mainmenu;
                            }
                            else if (choice6 == '2')
                            {
                                goto customer_menu;
                            }
                            if (choice6 != '1' && choice6 != '2')
                            {
                                cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                            }
                        } while (choice6 != '1' && choice6 != '2');
                    }
                }
            }
        }
        if (choice2 == '4')
        {
            if (filterSession.apply_pipeline())
            {
                cout << "\033[32m" << "Filters Applied Successfully " << "\033[0m" << endl;
                string save_path;
                string timestamp = timestamp_maker();
                char choice_last_Alhamdulillah = 0;
                for (int i = 0; i < 46; i++)
                {
                    cout << "=";
                }
                cout << endl;

                cout << "|| " << left << setw(41) << "Load Image " << "||" << endl;

                for (int i = 0; i < 46; i++)
                {
                    cout << "=";
                }
                cout << endl;
                cout << "|| " << left << setw(41) << "Do you wish to give a name of your choosing or do you wish it to be autogenerated :  " << "||" << endl;
                cout << "|| " << left << setw(41) << "1. Autogenerated " << "||" << endl;
                cout << "|| " << left << setw(41) << "2. Give name yourself " << endl;

                for (int i = 0; i < 46; i++)
                {
                    cout << "=";
                }
                cout << endl;

                do
                {
                    cout << "Your choice : ";
                    cin >> choice_last_Alhamdulillah;
                    if (choice_last_Alhamdulillah != '1' && choice_last_Alhamdulillah != '2')
                    {

                        cout << "\033[33m" << "Invalid input. Please enter 1 or 2." << "\033[0m" << endl;
                    }
                } while (choice_last_Alhamdulillah != '1' && choice_last_Alhamdulillah != '2');

                if (choice_last_Alhamdulillah == '1')
                {
                    save_path = customer.customer_CNIC_getter() + "_" + timestamp;
                }
                else if (choice_last_Alhamdulillah == '2')
                {
                    cout << "Enter the name to save the resulting image : ";
                    cin >> save_path;
                }
                try
                {
                    if (F_M_R_W.F_M_WRITE_IMAGE(&image, save_path))
                    {
                        cout << "\033[32m" << "Image Saved Successfully at " << save_path << " " << "\033[0m" << endl;
                        try
                        {
                            F_M_S.F_M_APPEND_SESSION(customer.customer_CNIC_getter(), filterSession.getter(), timestamp, image.file_name_getter());
                        }
                        catch (exception &e)
                        {
                            cout << "\033[31m" << "Error saving session: " << e.what() << "\033[0m" << endl;
                        }
                        filterSession.clear_pipeline();
                    }
                    else
                    {
                        cout << "\033[31m" << "An error occurred while saving the image " << "\033[0m" << endl;
                    }
                }
                catch (exception &e)
                {
                    cout << "\033[31m" << "Error reading image: " << e.what() << "\033[0m" << endl;
                    goto customer_menu;
                }
            }
            else
            {
                cout << "\033[31m" << "An error occurred while applying filters " << "\033[0m" << endl;
            }
            char choice3;
            do
            {
                cout << "Do you wish to go back to customer menu (Y/N): ";
                cin >> choice3;
                if (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N');
            if (choice3 == 'y' || choice3 == 'Y')
            {
                goto customer_menu;
            }
            else if (choice3 == 'n' || choice3 == 'N')
            {
                char choice4;
                do
                {
                    cout << "Do you wish to go back to main menu (Y/N) : ";

                    cin >> choice4;
                    if (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N')
                    {
                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                    }
                } while (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N');

                if (choice4 == 'y' || choice4 == 'Y')
                {
                    goto mainmenu;
                }
                else if (choice4 == 'n' || choice4 == 'N')
                {
                    char choice5;
                    do
                    {
                        cout << "Do you wish to exit the program (Y/N) : ";

                        cin >> choice5;
                        if (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N');
                    if (choice5 == 'y' || choice5 == 'Y')
                    {
                        exit(0);
                    }
                    else if (choice5 == 'n' || choice5 == 'N')
                    {
                        cout << " Select one of the following options : " << endl;
                        cout << "1. Go back to main menu (You will be logged out) " << endl;
                        cout << "2. Go back to customer menu " << endl;

                        char choice6;
                        do
                        {
                            cout << "Enter your choice : ";
                            cin >> choice6;
                            if (choice6 == '1')
                            {
                                goto mainmenu;
                            }
                            else if (choice6 == '2')
                            {
                                goto customer_menu;
                            }
                            if (choice6 != '1' && choice6 != '2')
                            {
                                cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                            }
                        } while (choice6 != '1' && choice6 != '2');
                    }
                }
            }
        }
        if (choice2 == '5')
        {
            cout << "Your Session History is as follows : " << endl;
            F_M_S.F_M_LOAD_FOR_SPECIFIC_USER(customer.customer_CNIC_getter());
            char choice3;
            do
            {
                cout << "Do you wish to go back to customer menu (Y/N): ";
                cin >> choice3;
                if (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N');
            if (choice3 == 'y' || choice3 == 'Y')
            {
                goto customer_menu;
            }
            else if (choice3 == 'n' || choice3 == 'N')
            {
                char choice4;
                do
                {
                    cout << "Do you wish to go back to main menu (Y/N) : ";

                    cin >> choice4;
                    if (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N')
                    {
                        cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                    }
                } while (choice4 != 'y' && choice4 != 'Y' && choice4 != 'n' && choice4 != 'N');
                if (choice4 == 'y' || choice4 == 'Y')
                {
                    goto mainmenu;
                }
                else if (choice4 == 'n' || choice4 == 'N')
                {
                    char choice5;
                    do
                    {
                        cout << "Do you wish to exit the program (Y/N) : ";

                        cin >> choice5;
                        if (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N')
                        {
                            cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                        }
                    } while (choice5 != 'y' && choice5 != 'Y' && choice5 != 'n' && choice5 != 'N');
                    if (choice5 == 'y' || choice5 == 'Y')
                    {
                        exit(0);
                    }
                    else if (choice5 == 'n' || choice5 == 'N')
                    {
                        cout << " Select one of the following options : " << endl;
                        cout << "1. Go back to main menu (You will be logged out) " << endl;
                        cout << "2. Go back to customer menu " << endl;

                        char choice6;
                        do
                        {
                            cout << "Enter your choice : ";
                            cin >> choice6;
                            if (choice6 == '1')
                            {
                                goto mainmenu;
                            }
                            else if (choice6 == '2')
                            {
                                goto customer_menu;
                            }
                            if (choice6 != '1' && choice6 != '2')
                            {
                                cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                            }
                        } while (choice6 != '1' && choice6 != '2');
                    }
                }
            }
        }
        if (choice2 == '6')
        {
            cout << "Logging out" << endl;
            goto mainmenu;
        }
    }
    if (choice1 == '3')
    {
        Vailidations v;
        string name, cnic, password, gender, city, phone_number, C_password;

        cout << "Customer Signup Selected " << endl;
        cout << "Enter Name : ";
        cin.ignore();
        getline(cin, name);

    CCNICC:
        cout << "Enter CNIC : ";
        cin >> cnic;
        int cnic_result = v.cnic_check(cnic);
        if (cnic_result == -2)
        {
            cout << "CNIC must be exactly 13 digits" << endl;
            goto CCNICC;
        }
        if (cnic_result == 1 || cnic_result == 2)
        {
            cout << "CNIC already exists or is blocked" << endl;
            goto CCNICC;
        }
    Password:
        cout << "Enter Password : ";
        cin >> password;

        if (!v.Password_Check(password))
        {
            cout << "\033[31m" << "Invalid Password Format Please try again " << "\033[0m" << endl;
            goto Password;
        }
        cout << "Confirm Password : ";
        cin >> C_password;
        if (C_password != password)
        {
            cout << "\033[31m" << " Confirm Password and Password Didnot match try Again " << "\033[0m";
            goto Password;
        }
        cout << "Enter Gender : ";
        cin >> gender;
        cout << "Enter City : ";
        cin >> city;
        cout << "Enter Phone Number : ";
        cin >> phone_number;

        Customer customer(name, phone_number, city, password, gender, cnic);
        try
        {
            F_M_C.F_M_Save(&customer);
        }
        catch (exception &e)
        {
            cout << "\033[31m" << "Error saving customer: " << e.what() << "\033[0m" << endl;
        }

        cout << "\033[32m" << "Signup Successful You can now login with your CNIC and Password " << "\033[0m" << endl;

        char choice2;
        do
        {
            cout << "Do you wish to go back to main menu (Y/N): ";
            cin >> choice2;
            if (choice2 != 'y' && choice2 != 'Y' && choice2 != 'n' && choice2 != 'N')
            {
                cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
            }
        } while (choice2 != 'y' && choice2 != 'Y' && choice2 != 'n' && choice2 != 'N');
        if (choice2 == 'y' || choice2 == 'Y')
        {
            goto mainmenu;
        }
        else if (choice2 == 'n' || choice2 == 'N')
        {
            char choice3;
            do
            {
                cout << "Do you wish to exit the program (Y/N) : ";

                cin >> choice3;
                if (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N')
                {
                    cout << "\033[33m" << "Invalid input. Please enter Y or N." << "\033[0m" << endl;
                }
            } while (choice3 != 'y' && choice3 != 'Y' && choice3 != 'n' && choice3 != 'N');
            if (choice3 == 'y' || choice3 == 'Y')
            {
                exit(0);
            }
            else if (choice3 == 'n' || choice3 == 'N')
            {
                cout << " Select one of the following options : " << endl;
                cout << "1. Go back to main menu (You will be logged out) " << endl;
                cout << "2. Exit Program " << endl;

                char choice4;
                do
                {
                    cout << "Enter your choice : ";
                    cin >> choice4;
                    if (choice4 == '1')
                    {
                        goto mainmenu;
                    }
                    else if (choice4 == '2')
                    {
                        exit(0);
                    }
                    if (choice4 != '1' && choice4 != '2')
                    {
                        cout << "\033[33m" << "Invalid choice Please try again " << "\033[0m" << endl;
                    }
                } while (choice4 != '1' && choice4 != '2');
            }
        }
    }
    if (choice1 == '4')
    {
        cout << " Exiting the Program :) " << endl;
        cout << " THANKS FOR USING OUT IMAGE FILTER STUDIO " << endl;
        exit(0);
    }
}
