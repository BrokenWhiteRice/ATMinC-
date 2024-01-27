#include <iostream>  // I/O stream library
#include <fstream>   // File stream library
#include <vector>
#include <string>

using namespace std;

class Account {
// Data Member
private:
    string accountID;      // Account ID
    string accountPIN;     // Account PIN
    double accountBalance;  // Account Balance

// Member Functions
public:
    // Default constructor
    Account() : accountID(""), accountPIN(""), accountBalance(0.0) {}

    // Second constructor
    Account(string id, string pin, double balance);

    // Getter and Setter methods for accountID
    string getID() const { return accountID; }
    void setID(const string& id) { accountID = id; }

    // Getter and Setter methods for accountPIN
    string getPIN() const { return accountPIN; }
    void setPIN(const string& pin) { accountPIN = pin; }

    // Getter and Setter methods for accountBalance
    double getBalance() const { return accountBalance; }
    void setAccountBalance(double balance) { accountBalance = balance; }

    void Balance() const;   // Method to display account balance
    void Deposit(double amt);   // Method to deposit funds
    bool Withdraw(double amt);  // Method to withdraw funds
};

// Implementation of the second constructors
Account::Account(string id, string pin, double balance)
        : accountID(""), accountPIN(""), accountBalance(0.0) {

    // Check if ID is 8 characters long
    if (id.length() == 8) {
        setID(id);
    } else {
        cerr << "Invalid ID!" << endl;
    }

    // Check if PIN is digits only with 4 characters
    bool validPIN = true;
    if (pin.length() == 4) {
        for (char c : pin) {
            if (!isdigit(c)) {
                validPIN = false;
                break;
            }
            // not digits -> skip
        }
    } else {
        validPIN = false;
    }
    // if true -> we have a valid Pin
    if (validPIN) {
        setPIN(pin);
    } else {
        cerr << "Invalid PIN!" << endl;
    }

    // Check if balance is >= 0.0
    if (balance >= 0.0) {
        setAccountBalance(balance);
    } else {
        cerr << "Invalid balance!" << endl;
    }
}

// Implementation of Balance method
void Account::Balance() const {
    // use getter to print the current balance
    cout << "Balance: $" << getBalance() << endl;
}

// Implementation of Deposit method
void Account::Deposit(double amt) {
    // confirm amt passed
    if (amt >= 0.0) {
        // use setAccountBalance() to add amt to the accountBalance of Account Object
        setAccountBalance(getBalance() + amt);
    } else {
        cout << "Error" << endl;
    }
}

// Implementation of Withdraw method
bool Account::Withdraw(double amt) {
    // use getter and setter for accountBalance to perform
    if (amt > 0.0 && amt <= getBalance()) {
        setAccountBalance(getBalance() - amt);
        return true;
    } else {
        cout << "Error" << endl;
        return false;
    }
}

// Bank creates and maintains accounts found in the data file
class Bank {

// Data Members
private:
    vector<Account> allAccounts;  // Vector to store all accounts
    Account dummyAccount;  // initialized to default values

// Member Functions
public:
    Account & findAcct(const string& id, const string& pin);
    void addAcct(Account& acct);
    void loadAccounts(ifstream& inf);
};

// Implementation of findAcct method
// search vector of accounts matching ID and Pin
Account & Bank::findAcct(const string & id, const string & pin) {
    for (Account & acct : allAccounts) {
        // use getters to access ID and Pin
        if (acct.getID() == id && acct.getPIN() == pin) {
            return acct;
        }
    }
    return dummyAccount;  // Return dummyAccount if no match is found
}

// Implementation of addAcct method
// add acct to the next position of vector allAccounts
void Bank::addAcct(Account& acct) {
    allAccounts.push_back(acct);    // insert the account object
}

// Implementation of loadAccounts method
void Bank::loadAccounts(ifstream & inf) {
    string id, pin;
    double balance;

    // Read account details from the file and add them to the bank
    // >> means extracting data from input streams, makes it in order
    while (inf >> id >> pin >> balance) {
        Account acct(id, pin, balance);
        addAcct(acct);
    }
}

// ATM class allows user to login, perform transactions and exit
class ATM {

// Data Members
private:
    Bank & bank;   // single object of type Bank

// Member Functions
public:
    ATM(Bank & b) : bank(b) {}   // Constructor taking a Bank object
    int start();   // to start the ATM
    void transactions(Account& acct);   // to handle transactions with menu
};

// Implementation of the start method
int ATM::start() {
    int loginAttempts = 0;

    // attempts no more than 3 times
    while (loginAttempts < 3) {
        cout << "Please enter your command\n"
                "Q - quit/exit\n"
                "L - login to account\n";

        char choice;
        cin >> choice;

        switch (choice) {
            case 'Q':
                cout << "Goodbye." << endl;
                return 0;
                exit(1);
            case 'L':
                cout << "Please enter Account ID: ";
                string id, pin;
                cin >> id;
                cout << "Please enter PIN: ";
                cin >> pin;

                // check if we have account
                Account & foundAccount = bank.findAcct(id, pin);

                if (foundAccount.getID().empty()) {
                    cout << "Sorry - no match" << endl;
                    loginAttempts++;
                    cout << "Login attempts remaining: " << 3 - loginAttempts << endl;
                } else {
                    cout << "Account found" << endl;
                    transactions(foundAccount);

                    // reset attempt if found one
                    loginAttempts = 0;
                }
                break;
        }
    }

    cout << "No attempt remains. Exiting program." << endl;
    return 1;
}

// Implementation of transactions method
// present with menu to perform withdrawl, deposit, check balance, quit
void ATM::transactions(Account & acct) {
    while (true) {
        cout << "Please enter your selection or Q to quit.\n"
                "W - withdraw funds\n"
                "D - deposit funds\n"
                "B - check balance\n"
                "Q - quit/exit\n";

        char choice;
        cin >> choice;

        switch (choice) {
            case 'W':
                cout << "Please enter the amount of withdrawal: ";
                double withdrawalAmount;
                cin >> withdrawalAmount;

                if (acct.Withdraw(withdrawalAmount)) {
                    cout << "Successfully withdraw." << endl;
                }
                break;
            case 'D':
                cout << "Please enter amount of deposit: ";
                double depositAmount;
                cin >> depositAmount;

                acct.Deposit(depositAmount);
                cout << "Successfully deposit." << endl;
                break;
            case 'B':
                acct.Balance();
                break;
            case 'Q':
                cout << "Goodbye." << endl;
                return;
            default:
                cout << "Unrecognized command " << choice << endl;
                break;
        }
    }
}

// Main function
int main() {
    ifstream inf;
    Bank bk;

    string filePath;
    cout << "Enter path for account information: ";
    cin >> filePath;

    inf.open(filePath);
    if (!inf) {
        cerr << "Error. Exiting program." << endl;
        return 1;
    }

    bk.loadAccounts(inf);
    inf.close();

    ATM atm(bk);

    int result = atm.start();

    if (result == 0) {
        cout << "Normal Exit" << endl;
    } else if (result == 1) {
        cout << "Account not found - system shutting down" << endl;
    }

    return 0;
}