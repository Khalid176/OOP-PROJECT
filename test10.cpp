#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    for (int i = 0; i < 44; i++)
    {
        cout << "=";
    }
    cout << endl;
    cout << "||";
    cout << setw(40) << left << " WELCOME TO PHOTO EDITING SOFTWARE" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " PLEASE SELECT AN OPTION" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 1. ADMIN LOGIN" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 2. CUSTOMER LOGIN" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 3. CUSTOMER SIGNUP" << setw(40) << left << "||" << endl;
    cout << "||";
    cout << setw(40) << left << " 4. EXIT" << setw(45) << left << "||" << endl;
    for (int i = 0; i < 44; i++)
    {
        cout << "=";
    }
    cout << endl;
    int choice1 = -1;
    do
    {
        cout << "Please Enter Your choice : ";
        cin >> choice1;
        if (choice1 != 1 || choice1 != 2 || choice1 != 3)
        {
            cout << "Invalid choice " << endl;
        }

    } while (choice1 != 1 || choice1 != 2 || choice1 != 3);
}