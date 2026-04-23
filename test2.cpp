#include <iostream>
using namespace std;
int main() 
{
    string name_of_file = "khalid.cpp";
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
    cout<<file_name;
}