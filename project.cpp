#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
    bool save(string outputPath, F_M_READ_WRITE &FM_RW);
    friend class FilterSession;
    Image(string file_name, int height, int width)
    {
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
    void display_Ascii()
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
    Image(Image &other)
    {
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
        return image;
    }
    bool F_M_WRITE_IMAGE(Image *image, string outputPath)
    {

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
        for (counter = 0; outputPath[counter] != '.'; counter++)
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
        char *file_path = new char[size + 7];
        for (int i = 0, j = 0; i < size; i++)
        {
            file_path[j] = outputPath[i];

            j++;

            if (outputPath[i + 1] == '.')
            {

                file_path[j] = 'E';

                j++;

                file_path[j] = 'D';

                j++;

                file_path[j] = 'I';

                j++;

                file_path[j] = 'T';

                j++;

                file_path[j] = 'E';

                j++;

                file_path[j] = 'D';

                j++;
            }
        }

        file_path[size + 6] = '\0';
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
    virtual ~Filter() {}

protected:
    bool avaliable;
};
class GrayScale : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Invert : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Brightness_Adjust : public Filter
{

    int increment = 67;

public:
    void increment_setter(int set)
    {
        increment = set;
    }
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Contrast_Stretch : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Red_Channel_Only : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Green_Channel_Only : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Blue_Channel_Only : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Box_Blur : public Filter
{
public:
    bool apply(Image *IImage)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Flip_Horizontal : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};
class Flip_Vertical : public Filter
{
public:
    bool apply(Image *image)
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
    bool is_avaliable()
    {
        return avaliable;
    }
};

class FilterSession
{
    string customer_cnic;
    string timestamp;
    Image *image = nullptr;
    vector<Filter *> filters;
    int counter;

public:
    FilterSession(string customer_cnic, string timestamp, Image *image)
    {
        this->customer_cnic = customer_cnic;
        this->timestamp = timestamp;
        this->image = image;
        counter = 0;
    }
    bool add_filter(Filter *filter)
    {
        filters.push_back(filter);
        counter++;
        return true;
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
                    counter2++;
                }
                else
                {
                    cout << "An error has occoured";
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
        delete image;
        filters.clear();
    }
};
class F_M_Customers;
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
    bool virtual login(string cnic, string password, Customer *customer) = 0;
};

// Customer strting ====================================

class Customer : public user
{

    bool is_blocked;

public:
    friend class F_M_Customers;
    Customer(string name, string phone, string city, string password, string Gender, string cnic)
    {
        this->name = name;
        this->phone = phone;
        this->cnic = cnic;
        this->Gender = Gender;
        this->city = city;
        this->password = password;
        is_blocked = false;
    }
    bool is_blocked_getter()
    {
        return is_blocked;
    }
    bool login(string e_cnic, string e_password, Customer *customer); // forward declaration
    bool display_history()
    {
        //  would be done in F_M_Sessions
    }
};
class F_M_Customers
{
public:
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
            cout << "Error: Could not open the file!" << endl;
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
                    if (customer->is_blocked != 1)
                    {
                        file.close();
                    }
                    else
                    {
                        cout << "User has been blocked by the admin \n ";
                        file.close();
                    }
                }
            }
        }
        file.close();
        return customer;
    }
    bool F_M_Save(Customer *customer)
    {
        ofstream file("Customers.txt", ios::app);
        if (!file.is_open())
        {
            cout << "UNABLE TO OPEN THE FILE " << endl;
        }
        else
        {
            file << customer->cnic << "|" << customer->password << "|" << customer->name << "|" << customer->Gender << "|" << customer->phone << "|" << customer->city << "|" << 0 << endl;
        }
        file.close();
    }

    int F_M_Customer_Counter()
    {
        ifstream file("Customers.txt");
        if (!file.is_open())
        {
            cout << "UNABLE TO OPEN THE FILE " << endl;
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
    bool F_M_Delete(string cnic)
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
                cout << "ENter";
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
    }
    bool F_M_Toogle(string cnic)
    {
        
    }
};
bool Customer::login(string e_cnic, string e_password, Customer *customer)
{
    F_M_Customers F_M_C;
    if (F_M_C.F_M_Load("Customers.txt", customer, e_cnic, e_password))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int main()
{
}
