#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
bool F_M_Delete(string cnic)
    {
        int count = 5;
        ifstream file_real("Customers.txt");
        ofstream file_temp("Temp.txt");
        if (!(file_real.is_open() && file_temp.is_open()))
        {
            cout << "UNABLE TO OPEN THE FILE " << endl;
            return 0;
        }
        else
        {
            string data;
            int k = 0;

            while (getline(file_real, data))
            {
                int i = 0;

                string Picked_password;
                string Picked_cnic;
                string Picked_name;
                string Picked_Gender;
                string Picked_phone;
                string Picked_city;
                bool Picked_is_blocked;

                for (; data[i] != '|'; i++)
                {
                    Picked_cnic = Picked_cnic + data[i];
                }
                i++;
                cout<<"Cnic picked"<<endl;
                if (Picked_cnic != cnic)
                {
                    for (; data[i] != '|'; i++)
                    {
                        Picked_name += data[i];
                    }
                cout<<"name picked"<<endl;

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_Gender += data[i];
                    }
                cout<<"Gender picked"<<endl;

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_phone += data[i];
                    }
                cout<<"Phone picked"<<endl;

                    i++;
                    for (; data[i] != '|'; i++)
                    {
                        Picked_city += data[i];
                    }
                cout<<"City picked"<<endl;

                    i++;
                    for (; data[i] != '\0'; i++)
                    {
                        Picked_is_blocked = (data[i] == '1');
                    }
                cout<<"block picked"<<endl;

                    file_temp << Picked_cnic << "|" << Picked_password << "|" << Picked_name << "|" << Picked_Gender << "|" << Picked_phone << "|" << Picked_city << "|" << Picked_is_blocked << endl;
                }
                cout<<++k<<endl<<endl;
                
            }
            file_real.close();
            file_temp.close();
            cout<<"P1"<<endl;
            ofstream file_real("Customers.txt");
            ifstream file_temp("Temp.txt");
            if (!(file_real.is_open() && file_temp.is_open()))
            {
                cout<<"Unable to open file";
            }
            else
            {
                while (getline(file_temp, data))
                {
                    file_real << data << endl;
                }
            }
            return 1;
        }
    }
int main() 
{
    F_M_Delete("123");
}