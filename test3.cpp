#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
bool F_M_Delete(string cnic)
{
    // int count = 5;
    // ifstream file_real("Customers.txt");
    // ofstream file_temp("Temp.txt");
    // if (!(file_real.is_open() && file_temp.is_open()))
    // {
    //     cout << "UNABLE TO OpEN THE FILE " << endl;
    //     return 0;
    // }
    // else
    // {
    //     string data;
    //     int k = 0;

    //     while (getline(file_real, data))
    //     {
    //         int i = 0;

    //         string picked_password;
    //         string picked_cnic;
    //         string picked_name;
    //         string picked_Gender;
    //         string picked_phone;
    //         string picked_city;
    //         bool picked_is_blocked;

    //         for (; data[i] != '|'; i++)
    //         {
    //             picked_cnic = picked_cnic + data[i];
    //         }
    //         i++;

    //         for (; data[i] != '|'; i++)
    //         {
    //             picked_password += data[i];
    //         }

    //         i++;
    //         for (; data[i] != '|'; i++)
    //         {
    //             picked_name += data[i];
    //         }

    //         i++;
    //         for (; data[i] != '|'; i++)
    //         {
    //             picked_Gender += data[i];
    //         }

    //         i++;
    //         for (; data[i] != '|'; i++)
    //         {
    //             picked_phone += data[i];
    //         }

    //         i++;
    //         for (; data[i] != '|'; i++)
    //         {
    //             picked_city += data[i];
    //         }

    //         i++;
    //         if (picked_cnic == cnic)
    //         {
    //             for (; data[i] != '\0'; i++)
    //             {
    //                 picked_is_blocked = !(data[i] == '1');
    //             }
    //         }
    //         file_temp << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;
    //     }
    //     file_real.close();
    //     file_temp.close();
    //     ofstream file_real("Customers.txt");
    //     ifstream file_temp("Temp.txt");
    //     if (!(file_real.is_open() && file_temp.is_open()))
    //     {
    //         cout<<"UNABLE TO OPEN FILES";
    //     }
    //     else
    //     {

    //         while (getline(file_temp, data))
    //         {
    //             int i = 0;
    //             string picked_password;
    //             string picked_cnic;
    //             string picked_name;
    //             string picked_Gender;
    //             string picked_phone;
    //             string picked_city;
    //             bool picked_is_blocked;

    //             for (; data[i] != '|'; i++)
    //             {
    //                 picked_cnic = picked_cnic + data[i];
    //             }
    //             i++;

    //                 for (; data[i] != '|'; i++)
    //                 {
    //                     picked_password += data[i];
    //                 }
    //                 i++;
    //                 for (; data[i] != '|'; i++)
    //                 {
    //                     picked_name += data[i];
    //                 }

    //                 i++;
    //                 for (; data[i] != '|'; i++)
    //                 {
    //                     picked_Gender += data[i];
    //                 }

    //                 i++;
    //                 for (; data[i] != '|'; i++)
    //                 {
    //                     picked_phone += data[i];
    //                 }

    //                 i++;
    //                 for (; data[i] != '|'; i++)
    //                 {
    //                     picked_city += data[i];
    //                 }

    //                 i++;
    //                 for (; data[i] != '\0'; i++)
    //                 {
    //                     picked_is_blocked = (data[i] == '1');
    //                 }
    //                 file_real << picked_cnic << "|" << picked_password << "|" << picked_name << "|" << picked_Gender << "|" << picked_phone << "|" << picked_city << "|" << picked_is_blocked << endl;

    //         }
    //         file_real.close();
    //         file_temp.close();
    //         return 1;
    //     }

    ifstream file_real2("Sessions.txt");
    ofstream file_temp2("Temp2.txt");
    if (!(file_real2.is_open() && file_temp2.is_open()))
    {
        cout << "UNABLE TO OpEN THE FILE " << endl;
        return 0;
    }
    else
    {
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

int main()
{
    F_M_Delete("123");
}