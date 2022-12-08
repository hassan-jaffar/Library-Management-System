#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
using namespace std;

class Helper            //Helper class for string functionality
{
public:
    static int StringLength(char* str)
    {
        int count = 0;
        while (str[count] != '\0')            //stops counting when finds null
            count++;
        return count;

    }
    static void StringCopy(char*& dest, char*& src) {
        int i = 0;
        while (src[i] != '\0')              //deep copy till null is found
        {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';     //placing null at last index
    }
    static char* GetStringFromBuffer(char* str)
    {
        char* string1 = new char[StringLength(str) + 1];
        StringCopy(string1, str);                     //re-usability of existing functions

        return string1;
    }
};

class LibraryMS;

class Book {
private:
    int issn;
    char* title;
    int totalAuthors;
    char** authors;
    int referenceCopies;
    int issueCopies;
public:
    Book(int i = 0, int ta = 0, int r = 0, int c = 0)
    {
        issn = i;
        totalAuthors = ta;
        referenceCopies = r;
        issueCopies = c;
        title = 0;
        authors = 0;
    }
    ~Book()
    {
        delete title;
        for (int i = 0; i < totalAuthors; i++)
            delete authors[i];
        delete authors;
    }
    void printInTable()
    {
        cout << "\tISSN: " << issn;
        cout << "  Title: " << title << "\t";
    }
    void print()
    {
        cout << "\nBook Details:\n";
        cout << "ISSN:\t" << issn << endl;
        cout << "Title:\t" << title << endl;
        cout << "Authors:\t";
        for (int i = 0; i < totalAuthors; i++)
            cout << authors[i] << ",\t";
        cout << endl;
        cout << "Number of Reference Copies:\t" << referenceCopies << endl;
        cout << "Number of Issuable Books:\t" << issueCopies << endl << endl << endl;
    }
    void input()
    {
        char temp[100];
        cout << "Enter Book Details:\n";
        cout << "Enter ISSN:\t";
        cin >> issn;
        cout << "Enter Title:\t";
        cin >> temp;
        title = Helper::GetStringFromBuffer(temp);
        cout << "Enter Total Number of Authors:\t";
        cin >> totalAuthors;
        authors = new char* [totalAuthors];
        for (int i = 0; i < totalAuthors; i++)
        {
            cout << "Enter Author:" << "\t";
            cin >> temp;
            authors[i] = Helper::GetStringFromBuffer(temp);
        }
        cout << "Enter number of reference copies:\t";
        cin >> referenceCopies;
        cout << "Enter number of issuable copies:\t";
        cin >> issueCopies;
    }
    int getIssn() { return issn; }
    void setTitle(char* t)
    {
        title = Helper::GetStringFromBuffer(t);
    }
    void addAuthor(char* a)
    {                      //copy all data in a new array. recreate array with increased size. copy all content back with new inclusion
        char** temp = new char* [totalAuthors];
        for (int i = 0; i < totalAuthors; i++)
            temp[i] = Helper::GetStringFromBuffer(authors[i]);
        delete[] authors;
        authors = new char* [totalAuthors + 1];
        for (int i = 0; i < totalAuthors; i++)
            authors[i] = Helper::GetStringFromBuffer(temp[i]);
        authors[totalAuthors] = Helper::GetStringFromBuffer(a);
        totalAuthors++;
        delete[] temp;
    }
    void setRefNo(int r)
    {
        referenceCopies = r;
    }
    void setIssueNo(int i)
    {
        issueCopies = i;
    }
    const Book& operator=(const Book& rhs)      //assignment operator needed for insertion
    {
        if (this != &rhs)
        {
            if (title != 0)
                delete title;
            if (authors != 0)
                delete[] authors;

            issn = rhs.issn;
            title = Helper::GetStringFromBuffer(rhs.title);
            totalAuthors = rhs.totalAuthors;
            authors = new char* [totalAuthors];
            for (int i = 0; i < totalAuthors; i++)
                authors[i] = Helper::GetStringFromBuffer(rhs.authors[i]);
            referenceCopies = rhs.referenceCopies;
            issueCopies = rhs.issueCopies;
        }
        return *this;
    }
    Book(const Book& rhs)      //assignment operator needed for insertion
    {
        issn = rhs.issn;
        title = Helper::GetStringFromBuffer(rhs.title);
        totalAuthors = rhs.totalAuthors;
        authors = new char* [totalAuthors];
        for (int i = 0; i < totalAuthors; i++)
            authors[i] = Helper::GetStringFromBuffer(rhs.authors[i]);
        referenceCopies = rhs.referenceCopies;
        issueCopies = rhs.issueCopies;

    }
    friend class LibraryMS;     //needs to declare friend to use private functionalities
};

class LibraryMS {
private:
    class Hash {
    private:
        vector<Book*>* hashArray;
        int capacity;
    public:
        Hash(int const _capacity = 10)      //setting table size 10 by default
        {
            capacity = _capacity;
            hashArray = new vector<Book*>[capacity];
        }
        ~Hash()
        {
            for (int i = 0; i < capacity; i++)
            {
                hashArray[i].clear();   //built-in function that clears all index and sets size 0
            }
            delete[] hashArray;
        }
        bool isPrime(int n)
        {
            if (n <= 1)  return false;
            if (n <= 3)  return true;       //boundary conditions

            if (n % 2 == 0 || n % 3 == 0) return false;

            for (int i = 5; i * i <= n; i = i + 6)
                if (n % i == 0 || n % (i + 2) == 0)
                    return false;

            return true;
        }
        int nextPrime(int N)
        {

            if (N <= 1)
                return 2;   //base case

            int prime = N;
            bool found = false;

            while (!found) {
                prime++;

                if (isPrime(prime))
                    found = true;   //break loop when prime number found
            }
            return prime;
        }
        int hashFunction(int key)
        {
            srand(key);     //gives same random number everytime because of same seed
            int index;
            int p = nextPrime(capacity);        //p is next prime number greater than hashTable size
            int a = rand();         //0 <= a
            while (a < 0)
                a = rand();
            int b = rand() % p;    // b < p
            int k = ((a * key) + b) % p;
            index = k % capacity;

            return index;
        }
        void insert(Book*& obj)
        {
            int key = obj->getIssn();
            int index = hashFunction(key);
            hashArray[index].push_back(obj);
        }
        Book* search(int key)
        {
            int index = hashFunction(key);
            vector <Book*> ::iterator i;
            for (i = hashArray[index].begin(); i != hashArray[index].end(); i++)
            {
                if ((*i)->getIssn() == key)     //*i returns the pointer to the book
                    return *i;
            }
            return 0;
        }
        void deleteKey(Book*& obj)
        {
            int key = obj->getIssn();
            int index = hashFunction(key);
            vector <Book*> ::iterator i;
            for (i = hashArray[index].begin(); i != hashArray[index].end(); i++) 
            {
                if (*i == obj)
                    break;
            }
            if (i != hashArray[index].end())
                hashArray[index].erase(i);
        }
        void printHashTable()       //will give complete list view of the library 
        {
            for (int i = 0; i < capacity; i++) {
                cout << "Index " << i << ": " << char(237);
                for (auto x : hashArray[i])
                {
                    x->printInTable();
                    cout << " --> ";
                }
                cout << endl;
            }
            cout << "\n--------------------------------------------------------------------\n";
        }

    };

    Hash record;

public:
    friend class Book;
    LibraryMS() {
        record;
    }
    ~LibraryMS() {}
    void createRecord()
    {
        Book* temp = new Book;
        temp->input();
        record.insert(temp);
        cout << "\nRecord Created Successfully\n";
    }
    void reviewRecord(int key)
    {
        if (record.search(key))
            record.search(key)->print();    //print content of location stored at the node of the given index
        else
            cout << "\nBook not Found\n";
    }
    void updateRecord(int key)
    {
        if (record.search(key))
        {
            Book* book = record.search(key);
            cout << "Enter Which Detail To Update:\n";
            cout << "Enter 1 for Title:\n";
            cout << "Enter 2 for Adding Author:\n";
            cout << "Enter 3 to update number of reference copies:\n";
            cout << "Enter 4 to update number of issuable copies:\n";
            int choice = 0;
            cin >> choice;
            while (choice < 1 && choice > 4)
            {
                cout << "Enter a Valid Option:\t";
                cin >> choice;
            }
            if (choice == 1)
            {
                cout << "Enter the new title:\t";
                char temp[100];
                cin >> temp;
                book->setTitle(temp);
            }
            else if (choice == 2)
            {
                cout << "Enter the new author:\t";
                char temp[100];
                cin >> temp;
                book->addAuthor(temp);
            }
            else if (choice == 3)
            {
                cout << "Enter the new number of reference books:\t";
                int temp;
                cin >> temp;
                book->setRefNo(temp);
            }
            else if (choice == 4)
            {
                cout << "Enter the new number of issuable books:\t";
                int temp;
                cin >> temp;
                book->setIssueNo(temp);
            }
            cout << "\nRecord Updated Successfully\n";
        }
        else
            cout << "\nBook not Found\n";
    }
    void deleteRecord(int key)
    {
        if (record.search(key))              //checks whether the book exists or not
        {
            Book* book = record.search(key);
            record.deleteKey(book);              //first remove from record, then index because index has address of the record
            cout << "\nRecord Deleted Successfully\n";
        }
        else
            cout << "\nBook not Found\n";
    }
    void viewLib()
    {
        record.printHashTable();
    }
};

int main()
{
    LibraryMS lib;
    int operation = 0;
    while (operation != 6)
    {
        cout << "Enter the task in the library: \n";
        cout << "Enter 1 to create record\n";
        cout << "Enter 2 to review record\n";
        cout << "Enter 3 to update record\n";
        cout << "Enter 4 to delete record\n";
        cout << "Enter 5 to view all books\n";
        cout << "Enter 6 to view exit\n";
        cin >> operation;
        if (operation == 1)
            lib.createRecord();
        else if (operation == 2)
        {
            int id;
            cout << "Enter the ISSN:\t";
            cin >> id;
            lib.reviewRecord(id);
        }
        else if (operation == 3)
        {
            int id;
            cout << "Enter the ISSN:\t";
            cin >> id;
            lib.updateRecord(id);
        }
        else if (operation == 4)
        {
            int id;
            cout << "Enter the ISSN:\t";
            cin >> id;
            lib.deleteRecord(id);
        }
        else if (operation == 5)
        {
            lib.viewLib();
        }
        else if (operation != 6)
            cout << "\nInvalid Option. Please Enter a Valid Number\n\n";
        system("pause");
        system("cls");
    }
}