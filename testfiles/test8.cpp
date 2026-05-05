#include <iostream>
#include <fstream>
using namespace std;
void test()
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
int main()
{
    test();
}