#include <iostream>
using namespace std;
int main()
{
    string outputPath = "HELLO.png";
    int size = 0;
    for (size = 0; outputPath[size] != '\0'; size++)
    {
    }
    char *file_path = new char[size + 7];
    for (int i = 0, j = 0; i < size; i++)
    {
        file_path[j] = outputPath[i];
        cout << file_path[j] << endl;
        j++;

        if (outputPath[i + 1] == '.')
        {
            cout << "Mission start";
            file_path[j] = 'E';
            cout << file_path[j] << endl;

            j++;

            file_path[j] = 'D';
            cout << file_path[j] << endl;

            j++;

            file_path[j] = 'I';
            cout << file_path[j] << endl;

            j++;

            file_path[j] = 'T';
            cout << file_path[j] << endl;

            j++;

            file_path[j] = 'E';
            cout << file_path[j] << endl;

            j++;

            file_path[j] = 'D';
            cout << file_path[j] << endl;

            j++;
        }
    }

    file_path[size+6] = '\0';
    cout << file_path;
}