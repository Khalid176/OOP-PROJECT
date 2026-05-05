#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int test(bool * array)
{
    
    ifstream file_real2("Catalog.txt");
    ofstream file_temp2("Temp3.txt");
    if (!(file_real2.is_open() && file_temp2.is_open()))
    {
        cout << "UNABLE TO OpEN THE FILE " << endl;
        return -1;
    }
    else
    {
        int counter = 0;
        string data2;
        if (!(file_real2.is_open() && file_temp2.is_open()))
        {
            return -1;
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
                counter ++;
            }
        }

        file_real2.close();

        return 1;
    }
    return 0;
}

int main()
{
    bool * array = new bool [10];
    test( array );
    for (int i = 0; i < 10; i++)
    {
        cout<<array[i]<<endl;
    }

}