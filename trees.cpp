#include <iostream>
#include <string>
#include <vector>
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
        delete[] authors;
    }
    void print()
    {
        cout << "\n\nBook Details:\n";
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
    class List {
    private:
        struct Node {
        public:
            Book* data;
            Node* next;
            Node* prev;
            Node(Book* val = 0, Node* p = 0, Node* n = 0)
            {
                data = val;
                prev = p;
                next = n;
            }
        };
        Node* head;
        Node* tail;
    public:
        List()
        {
            head = new Node();
            tail = new Node();
            head->next = tail;
            tail->prev = head;
        }
        ~List()
        {
            while (!isEmpty())
            {
                deleteStart();
            }
        }
        void deleteStart()      //needed for destructor
        {
            if (!isEmpty())
            {
                Node* temp;
                temp = head->next;
                head->next = head->next->next;
                head->next->prev = head;
                delete temp;
            }
            else
                cout << "List Empty\n";
        }
        void insertSorted(Book*& val)
        {
            for (Node* temp = head->next; temp != tail; temp = temp->next)
            {
                if (temp->next->data->getIssn() > val->getIssn() && temp->data->getIssn() <= val->getIssn())    //smaller than next value
                {                                                                                           //and larger than prev value
                    temp->next = new Node(val, temp->next, temp);
                    break;
                }
            }
        }
        void deleteNode(Book*& book)
        {
            for (Node* temp = head->next; temp != tail; temp = temp->next)
            {
                if (temp->next->data == book)   //searches for matching index
                {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;  //pointers update
                    delete temp;
                    break;
                }
            }
        }
        void print()
        {
            for (Node* temp = head->next; temp != tail; temp = temp->next)
            {
                temp->data->print();

            }
            cout << endl;
        }
        bool isEmpty()
        {
            if (head->next == tail)
                return true;
            else
                return false;
        }
    };

    class AVLTree {
    private:
        struct AVLNode {
        public:
            int issn;
            Book* address;
            int height;
            AVLNode* left;
            AVLNode* right;
            AVLNode(int i, Book* a, AVLNode* l = 0, AVLNode* r = 0, int h = 0)
            {
                issn = i;
                address = a;
                left = l;
                right = r;
                height = h;
            }
        };
        AVLNode* root;
    public:
        AVLTree()
        {
            root = 0;
        }
        ~AVLTree()
        {
            Destroy(root);
        }
        void Destroy(AVLNode* node)
        {
            if (node)
            {
                Destroy(node->left);        //post order visit
                Destroy(node->right);
                delete node;
            }
        }
        void  inorderPrint()
        {
            inorderPrint(root);
            cout << endl;
        }
        void inorderPrint(AVLNode* node)
        {
            if (node)
            {
                inorderPrint(node->left);       //arrange in ascending order
                (node->address)->print();
                inorderPrint(node->right);
            }
        }
        void print()
        {
            cout << "\nTree:\n";
            inorderPrint();
            cout << "\n-------------------------------------------------------------------------------\n";
        }
        AVLNode* search(int key)
        {
            return search(root, key);
        }
        AVLNode* search(AVLNode* node, int key)
        {
            if (node)
            {
                if (node->issn == key)
                {
                    return node;
                }
                else
                {
                    if (node->issn < key)
                    {
                        return search(node->right, key);
                    }
                    else
                        return search(node->left, key);
                }
            }
            else
                return 0;
        }
        void rightRotation(AVLNode*& x) {
            AVLNode* orphan = x->left->right;

            AVLNode* y = x->left;

            y->right = x;

            x->left = orphan;

            x->height = height(x);
            y->height = height(y);

            x = y;
        }
        void leftRotation(AVLNode*& x)
        {
            AVLNode* orphan = x->right->left;

            AVLNode* y = x->right;

            y->left = x;

            x->right = orphan;

            x->height = height(x);
            y->height = height(y);

            x = y;
        }

        void doubleLeftRightRotation(AVLNode*& x)
        {
            leftRotation(x->left);
            rightRotation(x);
        }

        void doubleRightLeftRotation(AVLNode*& x)
        {
            rightRotation(x->right);
            leftRotation(x);
        }
        void insert(Book*& val)
        {
            insert(root, val);
        }
        void insert(AVLNode*& node, Book*& val) {
            if (node == 0)
            {
                node = new AVLNode(val->getIssn(), val);
                return;
            }
            else
            {
                if (node->issn <= val->getIssn())    //sort according to issn
                {
                    insert(node->right, val);
                }
                else if (node->issn > val->getIssn())
                {
                    insert(node->left, val);
                }
            }

            balance(node);
        }
        void balance(AVLNode*& node) {
            if (node)
            {
                if (height(node->right) - height(node->left) < -1)
                    if (height(node->left->left) >= height(node->left->right))      //checks for sign of balance factor
                        rightRotation(node);
                    else
                        doubleLeftRightRotation(node);

                else if (height(node->right) - height(node->left) > 1)
                    if (height(node->right->right) >= height(node->right->left))        //same sign single rotation else double
                        leftRotation(node);
                    else
                        doubleRightLeftRotation(node);

                node->height = max(height(node->left), height(node->right)) + 1;
            }
        }
        void remove(int key)
        {
            remove(root, key);
        }
        void remove(AVLNode*& node, int key)
        {
            if (node)
            {
                if (key < node->issn)
                    remove(node->left, key);
                else if (node->issn < key)
                    remove(node->right, key);

                else
                {
                    remove(node->right, key);       //checks for duplicate cases
                    remove(node->left, key);
                    if (node->left != nullptr && node->right != nullptr)        //no child case
                    {
                        node->issn = (findMin(node->right))->issn;
                        remove(node->right, node->issn);
                    }
                    else {
                        AVLNode* oldNode = node;
                        if (node->left != nullptr)      //one child or both
                            node = node->left;
                        else
                            node = node->right;
                        delete oldNode;
                    }
                }
                balance(node);      //need to balance after every deletion
            }
        }
        int height(AVLNode* node)
        {
            if (node)
            {
                int left;
                int right;
                left = height(node->left);
                right = height(node->right);
                if (left > right)           //compares whether left tree is bigger or right one. gets height of bigger one
                {
                    return left + 1;

                }
                else
                    return right + 1;
            }
            return 0;
        }
        AVLNode* findMin(AVLNode* node)
        {
            if (node)
            {
                AVLNode* current = node;

                while (current->left != NULL)
                    current = current->left;

                return current;
            }
        }
    };

    AVLTree index;
    List record;

public:
    friend class Book;
    LibraryMS() {
        index;
        record;
    }
    ~LibraryMS() {}
    void createRecord()
    {
        Book* temp = new Book;
        temp->input();
        index.insert(temp);
        record.insertSorted(temp);
        cout << "\nRecord Created Successfully\n";
    }
    void reviewRecord(int key)
    {
        if (index.search(key))
            ((index.search(key))->address)->print();    //print content of location stored at the node of the given index
        else
            cout << "\nBook not Found\n";
    }
    void updateRecord(int key)
    {
        if (index.search(key))
        {
            Book* book = index.search(key)->address;
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
        if (index.search(key))              //checks whether the book exists or not
        {
            Book* book = index.search(key)->address;
            record.deleteNode(book);
            index.remove(key);              //first remove from record, then index because index has address of the record
            cout << "\nRecord Deleted Successfully\n";
        }
        else
            cout << "\nBook not Found\n";
    }

};

int main()
{
    LibraryMS lib;
    int operation = 0;
    while (operation != 5)
    {
        cout << "Enter the task in the library: \n";
        cout << "Enter 1 to create record\n";
        cout << "Enter 2 to review record\n";
        cout << "Enter 3 to update record\n";
        cout << "Enter 4 to delete record\n";
        cout << "Enter 5 to exit\n";
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
        else if (operation != 5)
            cout << "\nInvalid Option. Please Enter a Valid Number\n\n";
        system("pause");
        system("cls");
    }
}