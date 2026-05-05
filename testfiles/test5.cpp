#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int test(string filter_id)
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
                if (picked_filter_id == filter_id)
                {
                    cout<<"Using if "<<picked_filter_id<<endl;
                    
                    for (; data2[i] != '\0'; i++)
                    {
                        
                        picked_IsEnabled = !(data2[i] == '1');
                    } 

                    i++;
                }
                else
                {
                    cout<<"Using else "<<picked_filter_id<<endl;
                    for (; data2[i] != '\0'; i++)
                    {
                        picked_IsEnabled = (data2[i] == '1');
                    }

                    i++;
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
            return -1;
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


int main()
{
    test("05");
}