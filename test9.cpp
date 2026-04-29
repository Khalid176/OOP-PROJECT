#include <iostream>
#include <fstream>
using namespace std;
void test()
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
                picked_is_blocked = !(data[i] == '1');
            }
            if (picked_is_blocked == 1)
            {
                cout << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
            }
        }
        file_real.close();
    }
}
int main()
{
    test();
}