#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

// Structures used in program
struct car // structure for car
{
    string name;
    string numPlate;
    int model;
    bool availability;
    float rate;
    int maintained;
    int daysRented;
    float income;
    int duration;
    string rentTime;
    string returnTime;
};
struct user // structure for user
{
    string Name;
    string username;
    string password;
    string address;
    string contact;
    bool rented;
    car rentedCar;
    car* log; // pointer for array to store log history
};
struct login // structure for login validation
{
    string username;
    string password;
};

// -------------Functions Protoypes----------------------

// shows the name box on top
void refreshDisplay();

// Login and Verification
void adminLogin(user*&, short& size, car*&, short& carSize); // admin verificaton
void customerLogin(user*, short size, car*, short carSize);  // customer verification

// File handling
user* loadLoginInfo(short& size);                                // loads all the user details on program startup
void writeLoginInfo(user*, short size);                         // writes all the user details before exitting the program
car* loadCarInventory(short& size);                              // lads all the cars on program startup
void writeCarInventory(car*, short size);                       // writes all te cars info before exitting the program
void loadUserLog(user& customer, short& cSize, string filename); // load the log history from file after user login
void writeUserLog(user& customer, short size, string filename);  // write the log history to file after user logs out

// displays car inventory in both customer and aadmin menu
void displayCar(car* inventory, short inventorySize); // displays car inventory

// Customer Functions
void customerMenu(user& customer, car* carInventory, short invntorySize); // customer controls
void rentCar(user& customer, short& logSize, car* inventory, short inventroySize);
void returnCar(user& customer, short& logSize, car* inventory, short inventorySize);
void generateReport(user customer, short logSize);
void generatePreviousReport(user customer, short logSize);
void generateBill(user customer);

// Admin Functions
void adminMenu(user*&, short& size, car*&, short& carSize); // admin controls
void manageCustomers(user*& users, short& userSize);
void viewCustomer(user* users, short userSize);
void addCustomer(user*& users, short& userSize);
void removeCustomer(user*& users, short& userSize);
void updateCustomers(user* users, short userSize);
void updatecustomerData(user& customer);
void viewCustomerReport(user* users, short userSize);
void manageCar(car*& carInventory, short& inventorySize);
void addCar(car*& carInventory, short& size);
void removeCar(car*& carInventory, short& size);
void viewCarReport(car* carInventory, short size);
void generateCarReport(car vehicle);

// ---------------Main Function--------------------

int main()
{
    user* loginInfo;
    short usersSize;
    loginInfo = loadLoginInfo(usersSize); // loading user info

    car* carInventory;
    short carSize;
    carInventory = loadCarInventory(carSize); // loading cars info

    char userCtgry; // char is used to handle exceptionswhile entering choice
    do
    {
        refreshDisplay();
        cout << endl
            << endl
            << "Choose One : " << endl
            << endl
            << "0. Close" << endl
            << "1. Admin" << endl
            << "2. Customer" << endl
            << "Your choice : <   >\b\b\b";
        userCtgry = _getche();

        while (userCtgry != '1' && userCtgry != '2' && userCtgry != '0')
        {
            cout << endl
                << "Enter a Valid Choice (0 || 1 || 2) : ";
            userCtgry = _getche();
        }

        switch (userCtgry)
        {
        case '1':
            adminLogin(loginInfo, usersSize, carInventory, carSize); // login [0] means that only the user at first index can be admin
            break;

        case '2':
            customerLogin(loginInfo, usersSize, carInventory, carSize); // all the users including the admin can be customers
            break;

        default:
            break;
        }

    } while (userCtgry != '0'); // exit code when 0 pressed

    writeCarInventory(carInventory, carSize);
    writeLoginInfo(loginInfo, usersSize);

    _getch();
    return 0;
}

// -------------Funtion Definitions-----------------

void refreshDisplay()
{
    system("cls");
    cout << endl
        << endl;
    cout << "\t\t\t*****************************************************************\n";
    cout << "\t\t\t***                                                           ***\n";
    cout << "\t\t\t***        WELCOME TO CAR RENTAL AND MANAGEMENT SYSTEM        ***\n";
    cout << "\t\t\t***                                                           ***\n";
    cout << "\t\t\t*****************************************************************\n";
    cout << endl;
}

user* loadLoginInfo(short& size)
{
    ifstream loginFile;
    loginFile.open("./textFiles/userData.txt");
    if (loginFile.fail())
        cout << "failed";

    loginFile >> size;
    loginFile.ignore();

    user* customPtr = new user[size];

    for (int i = 0; i < size; i++)
    {
        loginFile.ignore();
        getline(loginFile, customPtr[i].Name);
        getline(loginFile, customPtr[i].username);
        getline(loginFile, customPtr[i].password);
        getline(loginFile, customPtr[i].address);
        getline(loginFile, customPtr[i].contact);
        loginFile >> customPtr[i].rented;
        loginFile.ignore();
        if (customPtr[i].rented == 1)
        {
            getline(loginFile, customPtr[i].rentedCar.name);
            getline(loginFile, customPtr[i].rentedCar.numPlate);
            loginFile >> customPtr[i].rentedCar.model;
            loginFile.ignore();
            loginFile >> customPtr[i].rentedCar.rate;
            loginFile.ignore();
            loginFile >> customPtr[i].rentedCar.duration;
            loginFile.ignore();
            getline(loginFile, customPtr[i].rentedCar.rentTime);
        }
    }

    loginFile.close();
    return customPtr;
}

car* loadCarInventory(short& size)
{
    ifstream carInventory;
    carInventory.open("./textFiles/carInventory.txt");
    if (carInventory.fail())
        cout << "failed";

    carInventory >> size; // size = 5;
    carInventory.ignore();

    car* carPtr = new car[size];

    for (int i = 0; i < size; i++)
    {
        carInventory.ignore();
        getline(carInventory, carPtr[i].name);
        getline(carInventory, carPtr[i].numPlate);
        carInventory >> carPtr[i].model;
        carInventory.ignore();
        carInventory >> carPtr[i].availability;
        carInventory.ignore();
        carInventory >> carPtr[i].rate;
        carInventory.ignore();
        carInventory >> carPtr[i].maintained;
        carInventory.ignore();
        carInventory >> carPtr[i].daysRented;
        carInventory.ignore();
        carInventory >> carPtr[i].income;
        carInventory.ignore();
    }

    carInventory.close();
    return carPtr;
}

void loadUserLog(user& customer, short& cSize, string filename)
{
    ifstream userfile;
    userfile.open(filename);
    if (userfile.fail())
        cout << endl
        << "Failed";
    userfile >> cSize;
    userfile.ignore();
    customer.log = new car[cSize];
    for (int i = 0; i < cSize; i++)
    {
        userfile.ignore();
        getline(userfile, customer.log[i].name);
        getline(userfile, customer.log[i].numPlate);
        userfile >> customer.log[i].model;
        userfile.ignore();
        userfile >> customer.log[i].rate;
        userfile.ignore();
        userfile >> customer.log[i].duration;
        userfile.ignore();
        getline(userfile, customer.log[i].rentTime);
        getline(userfile, customer.log[i].returnTime);
    }
}

void adminLogin(user*& users, short& userSize, car*& carInventory, short& inventorySize)
{
    cout << endl
        << endl;
    login test;
    user admin = users[0];
    bool authenticated = false;
    short attempts = 3;
    do
    {
        refreshDisplay();
        cout << endl
            << "Enter your username : ";
        cin >> test.username;
        if (test.username == admin.username)
        {
            cout << endl
                << "Enter your password : ";
            cin >> test.password;
            if (test.password == admin.password)
                authenticated = true;
        }
        if (!authenticated)
        {
            attempts--;
            cout << endl
                << "You have entered wrong credentials," << endl
                << "You have " << attempts << " attempt(s) left";
            _getch();
        }
    } while (!authenticated && attempts > 0);

    if (authenticated)
    {
        cout << endl
            << endl
            << "Admin Authenticated Successfully";
        _getch();

        adminMenu(users, userSize, carInventory, inventorySize);
    }
}

void customerLogin(user* customer, short size, car* carInventory, short inventorySize)
{
    login test;
    bool authenticated = false;
    short attempts = 3, idx;
    do
    {
        refreshDisplay();
        cout << endl
            << "Enter your username : ";
        cin >> test.username;

        for (int i = 0; i < size; i++)
        {
            if (test.username == customer[i].username)
            {
                cout << endl
                    << "Enter your password : ";
                cin >> test.password;
                if (test.password == customer[i].password)
                {
                    authenticated = true;
                    idx = i;
                    break;
                }
            }
        }

        if (!authenticated)
        {
            attempts--;
            cout << endl
                << "You have entered wrong credentials," << endl
                << "You have " << attempts << " attempt(s) left";
            _getch();
        }

    } while (!authenticated && attempts > 0);

    if (authenticated)
    {
        cout << endl
            << endl
            << "Customer Authenticated Successfully" << endl
            << endl;
        _getch();

        customerMenu(customer[idx], carInventory, inventorySize);
    }
}

void customerMenu(user& customer, car* carInventory, short invntorySize)
{

    short logSize;
    string filename = "./textFiles/customerInventory/" + customer.username + ".txt";
    loadUserLog(customer, logSize, filename);
    char choice;

    do
    {
        refreshDisplay();
        cout << endl
            << "Car Rental Menu " << endl
            << " 0. Go Back" << endl
            << " 1. Rent A Car" << endl
            << " 2. Reutrn A Car" << endl
            << " 3. Display Car Inventory" << endl
            << " 4. Generate Rental Report" << endl;

        cout << endl
            << "Enter your choice : ";
        choice = _getche();
        while (choice > 52 || choice < 48)
        {
            cout << endl
                << "Enter a Valid Choice : ";
            choice = _getche();
        }

        switch (choice)
        {
        case '1':
            rentCar(customer, logSize, carInventory, invntorySize);
            break;

        case '2':
            returnCar(customer, logSize, carInventory, invntorySize);
            break;

        case '3':
            displayCar(carInventory, invntorySize);
            break;

        case '4':
            generateReport(customer, logSize);
            break;

            // default :
            //     /* code */
        }
    } while (choice != '0');
    writeUserLog(customer, logSize, filename);
}

void adminMenu(user*& users, short& userSize, car*& carInventory, short& inventorySize)
{

    char choice;
    do
    {
        refreshDisplay();
        cout << endl
            << "Admin Control Menu " << endl
            << " 0. Go Back" << endl
            << " 1. Manage Car Inventory" << endl
            << " 2. Display Car Inventory" << endl
            << " 3. Manage Customers" << endl
            << " 4. View Customer Report" << endl
            << " 5. View Car Report" << endl;

        cout << endl
            << "Enter your choice : ";
        choice = _getche();;
        while (choice > 53 || choice < 48)
        {
            cout << endl
                << "Enter a Valid Choice : ";
            choice = _getche();;
        }

        switch (choice)
        {
        case '1':
            manageCar(carInventory, inventorySize);
            break;

        case '2':
            displayCar(carInventory, inventorySize);
            break;

        case '3':
            manageCustomers(users, userSize);
            break;

        case '4':
            viewCustomerReport(users, userSize);
            break;

        case '5':
            viewCarReport(carInventory, inventorySize);
            break;

        default:
            break;
        }
    } while (choice != '0');
}

void rentCar(user& customer, short& logSize, car* inventory, short inventorySize)
{
    if (customer.rented == 0)
    {
        string choice;
        displayCar(inventory, inventorySize);
        cout << endl
            << endl
            << "Press 0 to go back..." << endl
            << "Choose the car you want to rent (1 - " << inventorySize << ") : ";
        cin >> choice;
        int idx = stoi(choice);
        if (idx != 0)
        {
            while (idx < 0 || idx > inventorySize || inventory[idx - 1].availability == 0)
            {
                cout << endl
                    << "Enter a valid choice : ";
                cin >> choice;
                idx = stoi(choice);
            }
            idx--;
            customer.rented = 1;
            customer.rentedCar = inventory[idx];
            customer.rentedCar.rentTime = __DATE__;
            do
            {
                cout << endl
                    << "For how many days you want to rent the car : ";
                cin >> customer.rentedCar.duration;
            } while (customer.rentedCar.duration < 1);
            customer.rentedCar.rate *= customer.rentedCar.duration;

            inventory[idx].daysRented += customer.rentedCar.duration;
            inventory[idx].income += customer.rentedCar.rate;
            inventory[idx].availability = 0;
            generateBill(customer);
        }
    }
    else
    {
        cout << endl
            << "You have already rented a car..." << endl
            << "To rent a new car first return the previous one." << endl;
        _getch();
    }
}

void displayCar(car* inventory, short inventorySize)
{
    refreshDisplay();
    cout << endl
        << setfill(' ')
        << setw(7) << left << "S.No" << setw(17) << "Car Name" << setw(15) << "Number Plate" << setw(10) << "Model" << setw(15) << "Availability" << setw(12) << right << "Rate/Day" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < inventorySize; i++)
    {
        cout << setw(2) << i + 1 << ". " << setw(20) << left << inventory[i].name << setw(15) << inventory[i].numPlate << setw(10) << inventory[i].model;
        cout << setw(15) << ((inventory[i].availability == 1) ? "available" : "rented") << setw(10) << right << inventory[i].rate << " /=" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
    }
    cout << endl
        << "Press any key to continue...";
    _getch();
}

void writeLoginInfo(user* customer, short size)
{
    ofstream file;
    file.open("./textFiles/userData.txt");

    file << size << endl;

    for (int i = 0; i < size; i++)
    {
        file << endl;
        file << customer[i].Name << endl;
        file << customer[i].username << endl;
        file << customer[i].password << endl;
        file << customer[i].address << endl;
        file << customer[i].contact << endl;
        file << customer[i].rented << endl;
        if (customer[i].rented == 1)
        {
            file << customer[i].rentedCar.name << endl;
            file << customer[i].rentedCar.numPlate << endl;
            file << customer[i].rentedCar.model << endl;
            file << customer[i].rentedCar.rate << endl;
            //cout << customer[i].rentedCar.duration << endl;
            file << customer[i].rentedCar.duration << endl;
            //cout << customer[i].rentedCar.rentTime << endl;
            file << customer[i].rentedCar.rentTime << endl;
        }
    }
    file.close();
}

void writeCarInventory(car* inventory, short size)
{
    ofstream file;
    file.open("./textFiles/carInventory.txt");

    file << size << endl;

    for (int i = 0; i < size; i++)
    {
        file << endl;
        file << inventory[i].name << endl;
        file << inventory[i].numPlate << endl;
        file << inventory[i].model << endl;
        file << inventory[i].availability << endl;
        file << inventory[i].rate << endl;
        file << inventory[i].maintained << endl;
        file << inventory[i].daysRented << endl;
        //cout << inventory[i].availability << endl;
        file << inventory[i].income << endl;
    }
    file.close();
}

void returnCar(user& customer, short& logSize, car* inventory, short inventorySize)
{
    if (customer.rented == 0)
    {
        cout << endl
            << "You have not rented any car yet...";
        _getch();
    }
    else
    {
        cout << endl
            << endl;
        cout << setw(25) << "Car Name         : " << customer.rentedCar.name << endl;
        cout << setw(25) << "Number Plate     : " << customer.rentedCar.numPlate << endl;
        cout << setw(25) << "Car Model        : " << customer.rentedCar.model << endl;
        cout << setw(25) << "Rent Date        : " << customer.rentedCar.rentTime << endl;
        cout << setw(25) << "Allowed duration : " << customer.rentedCar.duration << " Days" << endl;
        cout << setw(25) << "Charges Payed    : " << customer.rentedCar.rate << endl;

        char choice;
        cout << endl
            << "Press 0 to go back..." << endl
            << "Press any key to continue...";
        choice = _getch();

        if (choice != '0')
        {

            for (int i = 0; i < inventorySize; i++)
            {
                if (customer.rentedCar.name == inventory[i].name)
                {
                    if (customer.rentedCar.numPlate == inventory[i].numPlate)
                    {
                        inventory[i].availability = 1;
                        inventory[i].maintained++;
                    }
                }
            }

            customer.rented = 0;
            customer.rentedCar.returnTime = __DATE__;
            logSize++;
            car* newPtr = new car[logSize];
            for (int i = 0; i < logSize - 1; i++)
                newPtr[i] = customer.log[i];
            newPtr[logSize - 1] = customer.rentedCar;
            delete[] customer.log;
            customer.log = newPtr;
        }
    }
}

void writeUserLog(user& customer, short size, string filename)
{
    ofstream file;
    file.open(filename);

    file << size << endl;

    for (int i = 0; i < size; i++)
    {
        file << endl;
        file << customer.log[i].name << endl;
        file << customer.log[i].numPlate << endl;
        file << customer.log[i].model << endl;
        file << customer.log[i].rate << endl;
        file << customer.log[i].duration << endl;
        file << customer.log[i].rentTime << endl;
        file << customer.log[i].returnTime << endl;
    }
    file.close();
    delete[] customer.log;
}

void generateReport(user customer, short logSize)
{
    char choice;
    do
    {
        refreshDisplay();
        cout << endl
            << "Report Generator " << endl
            << " 0. Go Back" << endl
            << " 1. Generate Current Bill" << endl
            << " 2. Generate History Report" << endl;

        cout << endl
            << "Enter your choice : ";
        choice = _getche();
        while (choice > 50 || choice < 48)
        {
            cout << endl
                << "Enter a Valid Choice : ";
            choice = _getche();
        }

        switch (choice)
        {
        case '1':
            generateBill(customer);
            break;

        case '2':
            generatePreviousReport(customer, logSize);
            break;

        default:
            break;
        }

    } while (choice != '0');
}

void generateBill(user customer)
{
    if (customer.rented == 1)
    {
        cout << endl
            << endl;
        cout << setw(20) << "Car Name : " << customer.rentedCar.name << endl;
        cout << setw(20) << "Number Plate : " << customer.rentedCar.numPlate << endl;
        cout << setw(20) << "Car Model : " << customer.rentedCar.model << endl;
        cout << setw(20) << "Rent Date : " << customer.rentedCar.rentTime << endl;
        cout << setw(20) << "Allowed duration : " << customer.rentedCar.duration << " Days" << endl;
        cout << setw(20) << "Renting charges : " << customer.rentedCar.rate << " /=" << endl;
        _getch();
    }
    else
    {
        refreshDisplay();
        cout << endl
            << "You do not have any rented car at the moment..." << endl;
        _getch();
    }
}

void generatePreviousReport(user customer, short logSize)
{
    refreshDisplay();
    double totalAmount = 0;
    for (int i = 0; i < logSize; i++)
    {
        cout << endl
            << i + 1 << " . " << endl
            << setw(30) << setfill('-') << '-' << endl
            << setfill(' ');
        cout << setw(20) << "Rent Date : " << customer.log[i].rentTime << endl;
        cout << setw(20) << "Return Date : " << customer.log[i].returnTime << endl;
        cout << setw(20) << "Car Name : " << customer.log[i].name << endl;
        cout << setw(20) << "Number Plate : " << customer.log[i].numPlate << endl;
        cout << setw(20) << "Car Model : " << customer.log[i].model << endl;
        cout << setw(20) << "Allowed duration : " << customer.log[i].duration << " Days" << endl;
        cout << setw(20) << "Renting charges : " << customer.log[i].rate << " /=" << endl;
        cout << setw(30) << setfill('-') << '-' << endl;
        totalAmount += customer.log[i].rate;
    }
    cout << endl
        << "Total amount spent by the customer = " << totalAmount << " /=" << endl;
    cout << endl
        << "Press any key to continue...";
    _getch();
}

void manageCustomers(user*& users, short& userSize)
{
    char choice;

    do
    {
        refreshDisplay();
        cout << "Choose from the given menu --" << endl
            << " 0. Go Back " << endl
            << " 1. View Customers " << endl
            << " 2. Add Customers " << endl
            << " 3. Remove Customers " << endl
            << " 4. Update Customer Data " << endl
            << endl
            << "Your Choice : ";

        choice = _getche();

        while (choice < 48 || choice > 52)
        {
            cout << endl
                << "Enter a valid choice : ";
            choice = _getche();
        }

        switch (choice)
        {
        case '1':
            viewCustomer(users, userSize);
            break;

        case '2':
            addCustomer(users, userSize);
            break;

        case '3':
            removeCustomer(users, userSize);
            break;

        case '4':
            updateCustomers(users, userSize);
            break;

        default:
            break;
        }

    } while (choice != '0');
}

void viewCustomer(user* users, short userSize)
{
    refreshDisplay();
    for (int i = 0; i < userSize; i++)
    {
        cout << endl
            << i + 1 << " . " << endl
            << setw(30) << setfill('-') << '-' << endl
            << setfill(' ');
        cout << setw(20) << "Name         : " << users[i].Name << endl;
        cout << setw(20) << "Username     : " << users[i].username << endl;
        cout << setw(20) << "Password     : " << users[i].password << endl;
        cout << setw(20) << "Address      : " << users[i].address << endl;
        cout << setw(20) << "Contact No.  : " << users[i].contact << endl;
        cout << setw(30) << setfill('-') << '-' << endl;
    }
    cout << endl
        << "Press any key to continue... " << endl;
    _getch();
}

void addCustomer(user*& users, short& userSize)
{
    refreshDisplay();
    bool alreadyExist = 0;
    user test;
    cout << endl
        << "You are registring a new customer... " << endl
        << endl;
    do
    {
        alreadyExist = 0;
        cout << "Enter Username : ";
        cin >> test.username;
        for (int i = 0; i < userSize; i++)
        {
            if (test.username == users[i].username)
                alreadyExist = 1;
        }
    } while (alreadyExist != 0);

    cout << "Enter Name : ";
    cin.ignore();
    getline(cin, test.Name);
    cout << "Enter Password : ";
    cin >> test.password;
    cout << "Enter Address : ";
    cin.ignore();
    getline(cin, test.address);
    cout << "Enter 8-Digit Contact No : ";
    cin >> test.contact;
    int number = stoi(test.contact);
    while(number < 10000000 || number > 99999999){
        cout<<endl<<"  Enter 8 digits : ";
        cin >> test.contact;
        number = stoi(test.contact);
    }

    test.rented = 0;

    ofstream file;
    file.open("./textFiles/customerInventory/" + test.username + ".txt");
    file << 0 << endl;
    file.close();

    userSize++;
    user* newPtr = new user[userSize];

    for (int i = 0; i < userSize - 1; i++)
        newPtr[i] = users[i];

    newPtr[userSize - 1] = test;
    delete[] users;
    users = newPtr;

    viewCustomer(users, userSize);
}

void removeCustomer(user*& users, short& userSize)
{
    refreshDisplay();
    string choice;
    viewCustomer(users, userSize);
    cout << endl
        << "Press 0 to go back..." << endl
        << "Enter the serial number of user \nyou want to remove : ";
    cin >> choice;
    int idx = stoi(choice);
    while (idx < 0 || idx > userSize)
    {
        cout << endl
            << "Enter a valid number : ";
        cin >> choice;
        idx = stoi(choice);
    }
    if (idx == 0)
    {
    }
    else if (idx == 1)
    {
        cout << endl
            << "You cannot remove an admin..." << endl;
        _getch();
    }
    else
    {

        idx--;
        string filename = "./textFiles/customerInventory/" + users[idx].username + ".txt";
        userSize--;
        user* newPtr = new user[userSize];

        for (int i = 1; i < userSize; i++)
        {
            if (i == idx)
            {
                for (; i < userSize; i++)
                {
                    users[i] = users[i + 1];
                }
            }
        }
        for (int i = 0; i < userSize; i++)
            newPtr[i] = users[i];

        remove(filename.c_str());
        delete[] users;
        users = newPtr;

        viewCustomer(users, userSize);
    }
}

void updateCustomers(user* users, short userSize)
{
    string choice;
    refreshDisplay();
    viewCustomer(users, userSize);
    cout << endl
        << "Press 0 to go back..." << endl
        << "Enter the serial number of the customer whose\ndata you want to update : ";
    cin >> choice;
    int idx = stoi(choice);
    while (idx < 0 || idx > userSize)
    {
        cout << endl
            << "Enter a valid number : ";
        cin >> choice;
        idx = stoi(choice);
    }
    if (idx != 0)
    {
        idx--;
        updatecustomerData(users[idx]);
    }
}

void updatecustomerData(user& customer)
{
    char choice;
    do
    {
        refreshDisplay();
        viewCustomer(&customer, 1);

        cout << endl
            << "Choose from the given menu --" << endl
            << " 0. Go Back " << endl
            << " 1. Change Name " << endl
            << " 2. Change Password " << endl
            << " 3. Change Address " << endl
            << " 4. Change Contact No. " << endl
            << endl
            << "Your Choice : ";

        choice = _getche();

        while (choice < 48 || choice > 52)
        {
            cout << endl
                << "Enter a valid choice : ";
            choice = _getche();
        }

        switch (choice)
        {
        case '1':
            cout << endl
                << "Enter new Name : ";
            cin.ignore();
            getline(cin, customer.Name);
            break;

        case '2':
            cout << endl
                << "Enter new Password : ";
            cin >> customer.password;
            break;

        case '3':
            cout << endl
                << "Enter new Address : ";
            cin.ignore();
            getline(cin, customer.address);
            break;
        
        case '4':
            int number;
            cout << endl
                << "Enter new Contact No. : ";
            cin >> customer.contact;
            number = stoi(customer.contact);
            while(number < 10000000 || number > 99999999){
                cout<<endl<<"  Enter 8 digits : ";
                cin >> customer.contact;
                number = stoi(customer.contact);
            }
            break;

        default:
            break;
        }
    } while (choice != '0');
}

void viewCustomerReport(user* users, short userSize)
{
    string choice;
    user customer;
    refreshDisplay();
    cout << endl
        << endl;
    viewCustomer(users, userSize);
    cout << endl
        << endl << "Press 0 to go back..." << endl
        << "Enter the serial number of user you want \nto generate report of : ";
    cin >> choice;
    int idx = stoi(choice);
    while (idx < 0 || idx > userSize)
    {
        cout << endl
            << "Enter a valid number : ";
        cin >> choice;
        idx = stoi(choice);
    }
    if (idx != 0) {

        idx--;
        customer = users[idx];
        short logSize;
        string filename = "./textFiles/customerInventory/" + customer.username + ".txt";
        loadUserLog(customer, logSize, filename);
        generatePreviousReport(customer, logSize);
    }
}

void manageCar(car*& carInventory, short& inventorySize)
{
    char choice;

    do
    {
        refreshDisplay();
        cout << endl
            << "Manage Car Inventory " << endl;
        cout << endl
            << " 0. Go Back" << endl
            << " 1. Add New Car" << endl
            << " 2. Remove Car" << endl;

        cout << endl
            << "Enter your choice : ";
        choice = _getche();
        while (choice < 48 || choice > 50)
        {
            cout << "Enter a valid choice : ";
            cin >> choice;
        }

        switch (choice)
        {
        case '1':
            addCar(carInventory, inventorySize);
            break;

        case '2':
            removeCar(carInventory, inventorySize);
            break;

        default:
            break;
        }
    } while (choice != '0');
}

void addCar(car*& carInventory, short& size)
{
    car test;
    bool available = 0;
    refreshDisplay();
    cout << endl
        << "You are adding a new car..." << endl;
    cout << endl
        << "Enter the credentials for new car..." << endl;
    do
    {
        available = 0;

        cin.ignore();
        cout << "Name : ";
        getline(cin, test.name);
        cout << "Number Plate : ";
        getline(cin, test.numPlate);
        // cout<<test.name<<endl<<test.numPlate<<endl;
        for (int i = 0; i < size; i++)
        {
            if (test.numPlate == carInventory[i].numPlate)
                if (test.name == carInventory[i].name)
                    available = 1;
        }

        if (available == 1)
            cout << endl
            << endl
            << "Car with same credentials is already registered..." << endl
            << endl;

    } while (available != 0);

    cout << "Model : ";
    cin >> test.model;
    while(test.model < 1000 || test.model > 2023){
        cout<<" Enter valid Model : ";
        cin >> test.model;
    }
    cout << "Per Day Rate : ";
    cin >> test.rate;
    while(test.rate < 1000){
        cout<<" Enter valid Rate : ";
        cin >> test.rate;
    }
    test.availability = 1;
    test.income = 0;
    test.daysRented = 0;
    test.maintained = 0;

    size++;
    car* newPtr = new car[size];

    for (int i = 0; i < size - 1; i++)
        newPtr[i] = carInventory[i];

    newPtr[size - 1] = test;
    delete[] carInventory;
    carInventory = newPtr;
}

void removeCar(car*& carInventory, short& size)
{
    string choice;
    refreshDisplay();
    displayCar(carInventory, size);
    cout << endl
        << "Press 0 to go back..." << endl
        << "Enter the serial number of car \nyou want to remove : ";
    cin >> choice;
    int idx = stoi(choice);

    if (idx != 0)
    {
        while (idx < 0 || idx > size || carInventory[idx - 1].availability == 0)
        {
            if ((idx > 0 && idx <= size) && (carInventory[idx - 1].availability == 0))
            {
                cout << endl
                    << "You can not remove the car because\nit is rented at the moment..." << endl;
            }
            cout << endl
                << "Enter a valid number : ";
            cin >> choice;
            idx = stoi(choice);
        }
        idx--;
        size--;
        car* newPtr = new car[size];

        for (int i = 0; i < size; i++)
        {
            if (i == idx)
            {
                for (; i < size; i++)
                {
                    carInventory[i] = carInventory[i + 1];
                }
            }
        }

        for (int i = 0; i < size; i++)
            newPtr[i] = carInventory[i];

        delete[] carInventory;
        carInventory = newPtr;
    }
}

void viewCarReport(car* carInventory, short size)
{
    string choice;
    refreshDisplay();
    displayCar(carInventory, size);
    cout << endl << "Press 0 to go back..." << endl
        << "Enter the serial number of car whose report\nyou want to view : ";
    cin >> choice;
    int idx = stoi(choice);

    while (idx < 0 || idx > size)
    {
        cout << endl
            << "Enter a valid number : ";
        cin >> choice;
        idx = stoi(choice);
    }
    if (idx != 0) {

        idx--;
        generateCarReport(carInventory[idx]);
    }
}

void generateCarReport(car vehicle)
{
    refreshDisplay();
    cout << endl
        << endl;
    cout << setw(20) << "Car Name     : " << vehicle.name << endl;
    cout << setw(20) << "Number Plate : " << vehicle.numPlate << endl;
    cout << setw(20) << "Car Model    : " << vehicle.model << endl;
    cout << endl
         << "Renting Status : " << endl;
    cout << "-----------------" << endl;

    cout << endl
        << "    Times Rented : " << ((vehicle.availability == 1) ? vehicle.maintained : (vehicle.maintained + 1));
    cout << endl
        << "    Total Days Rented : " << vehicle.daysRented;
    cout << endl
        << "    Total Income Generated : " << vehicle.income;

    cout << endl
        << endl
        << "Maintenance Report : " << endl;
    cout << "---------------------" << endl;
    cout << endl
        << "    Times Maintained : " << vehicle.maintained;
    if (vehicle.availability == 0)
        cout << endl
        << "    Another maintenance task is scheduled within the next few days..." << endl;
    else
        cout << endl
        << "    No upcoming maintenance task is scheduled..." << endl;

    cout << endl
        << "Press any key to continue..." << endl;
    _getch();
}
