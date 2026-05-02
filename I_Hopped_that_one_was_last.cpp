#include <iostream>
#include <fstream>
using namespace std;
class Customer
{
public:
    string name;
    string cnic;
    string phone;
    string city;
    string Gender;
    string password;
    bool is_blocked;
};
Customer *F_M_SEARCH_name(string name, Customer *customer)
{
    int name_size;
    int password_size;

    for (name_size = 0; name[name_size] != '\0'; name_size++)
    {
    }

    ifstream file("Customers.txt");
    if (!file.is_open())
    {
        cout << "\033[31m" << "Error: Could not open the file!" << "\033[0m" << endl;
        return 0;
    }
    string data;
    while (getline(file, data))
    {
        string Picked_password;
        string Picked_cnic;
        string Picked_name;
        string Picked_Gender;
        string Picked_phone;
        string Picked_city;
        bool Picked_is_blocked;

        int i = 0;
        for (i = 0; data[i] != '|'; i++)
        {
            Picked_cnic += data[i];
        }

        i++;
        for (; data[i] != '|'; i++)
        {
            Picked_password += data[i];
        }

        i++;
        for (; data[i] != '|'; i++)
        {
            Picked_name += data[i];
        }
        i++;
        int Picked_name_size = Picked_name.length();

        if (Picked_name_size != name_size)
        {
            continue;
        }
        else
        {

            if ((Picked_name == name))
            {

                for (; data[i] != '|'; i++)
                {
                    Picked_Gender += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    Picked_phone += data[i];
                }

                i++;
                for (; data[i] != '|'; i++)
                {
                    Picked_city += data[i];
                }

                i++;
                for (; data[i] != '\0'; i++)
                {
                    Picked_is_blocked = (data[i] == '1');
                }
                customer->name = Picked_name;
                customer->phone = Picked_phone;
                customer->Gender = Picked_Gender;
                customer->city = Picked_city;
                customer->is_blocked = Picked_is_blocked;
                if (customer->is_blocked != 1)
                {
                    file.close();
                    return customer;
                }
                else
                {
                    cout << "User has been blocked by the admin \n ";
                    file.close();
                    return customer;
                }
            }
        }
    }
}
int main()
{
}