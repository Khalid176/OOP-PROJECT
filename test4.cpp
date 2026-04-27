#include <iostream>
#include <fstream>
using namespace std;
bool F_M_APPEND_SESSION(string cnic, string timestamp, string filterpipeline, string image_name)
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
int main()
{
    F_M_APPEND_SESSION("123", "23-10-2006", "GD>BA>30", "hello.png");
}