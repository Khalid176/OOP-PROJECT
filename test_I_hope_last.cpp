#include <iostream>
using namespace std;
int main()
{
    string name_of_image = "hello.jpg";
    int counter1;
    string outputPath = "edited";
    string type;
    for (counter1 = 0; name_of_image[counter1] != '.'; counter1++)
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
    outputPath = outputPath + "." + type_of_image;
    cout << outputPath << endl;
    int size = 0;

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

    cout << type;

    for (size = 0; outputPath[size] != '\0'; size++)
    {
    }

    char *file_path = new char[size];
    for (int i = 0, j = 0; i < size; i++, j++)
    {
        file_path[j] = outputPath[i];
    }
    file_path[size] = '\0';
    cout << file_path;
    delete[] file_path;
}