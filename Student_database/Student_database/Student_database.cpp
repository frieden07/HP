#include <iostream>
#include<string>
#include "sqlite/sqlite3.h"
#include <stdio.h>
using namespace std;

class Database
{
public:
    char* err;
    sqlite3* db;
    sqlite3_stmt* stmt;

    void open()
    {
        const char* dir = R"(C:\Users\HP\Desktop\Intern Orientation Program\Student_database\Database.db)";
        int exist = sqlite3_open(dir, &db);

        if (exist != SQLITE_OK)
        {
            cout << "Error opening DB" << sqlite3_errmsg(db) << std::endl;
        }
    }

    void close()
    {
        sqlite3_close(db);
    }

    void createTable()
    {

        open();

        string query = "CREATE TABLE IF NOT EXISTS STUDENT ('Reg' INT PRIMARY KEY, 'Name' VARCHAR(20) NOT NULL, 'DSBDL' INT NOT NULL, 'AI' INT NOT NULL, 'WT' INT NOT NULL, 'CC' INT NOT NULL, 'ML' INT NOT NULL);";
        int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);

        if (rc != SQLITE_OK)
        {
            cout << "\n\nError : " << err << " occured during database creation!!";
        }

        close();
    }

    static int callback(void* NotUsed, int argc, char** argv, char** azColName)
    {
        for (int i = 0; i < argc; i++) {
            cout << azColName[i] << ": " << argv[i] << endl;
        }

        cout << endl;

        return 0;
    }
};


class Staff : private Database
{

public:
    Database D;

    void create() {
        D.createTable();
    }

    void admit_student(int reg, string name, int ds, int ai, int wt, int cc, int ml)
    {
        D.open();
        string s_reg = to_string(reg);
        string s_name = "'" + name + "'";
        string s_ds = to_string(ds);
        string s_ai = to_string(ai);
        string s_wt = to_string(wt);
        string s_cc = to_string(cc);
        string s_ml = to_string(ml);

        string query = "INSERT INTO STUDENT VALUES (" + s_reg + "," + s_name + "," + s_ds + "," + s_ai + "," + s_wt + "," + s_cc + "," + s_ml + ");";
        int rc = sqlite3_exec(D.db, query.c_str(), NULL, NULL, &D.err);

        if (rc != SQLITE_OK)
        {
            cout << "\nError : " << D.err << " ocuured while inserting !!";
        }
        else
        {
            cout << "\nSuccessfully inserted into databse !!";
        }

        D.close();
    }

    void remove_student(int Regis)
    {
        D.open();
        string temp = to_string(Regis);
        string query = "DELETE FROM STUDENT WHERE Reg = " + temp + ";";
        int rc = sqlite3_exec(D.db, query.c_str(), NULL, NULL, &D.err);

        if (rc != SQLITE_OK)
        {
            cout << "\n\nError : " << D.err << " occured during database creation!!";
        }
        else
        {
            cout << "\nSuccessfully Deleted Record of Registration Number " << Regis << " !!";
        }

        D.close();
    }

    void search_details(int Regis)
    {
        D.open();
        string temp = to_string(Regis);
        string query = "SELECT * FROM Student WHERE Reg = " + temp + ";";
        cout << "\n";
        int rc = sqlite3_exec(D.db, query.c_str(), callback, NULL, &D.err);
        if (rc != SQLITE_OK)
        {
            cout << "\n\nError : " << D.err << " No file found !!";
        }

        D.close();
    }

    void modify_deatils(int Regis)
    {
        int target;
        int correct_marks;
        cout << "\n\nGuide :\n\n1. To edit DSBDL marks\n2. To edit AI marks\n3. To edit WT marks\n4. To edit CC marks\n5. To edit ML marks";
        cout << "\nPlease Enter choice : ";
        cin >> target;
        cout << "Correct marks : ";
        cin >> correct_marks;
        string sub = "0";

        if (target == 1)
        {
            sub = "DSBDL";
        }
        else if (target == 2)
        {
            sub = "AI";
        }
        else if (target == 3)
        {
            sub = "WT";
        }
        else if (target == 4)
        {
            sub = "CC";
        }
        else if (target == 5)
        {
            sub = "ML";
        }
        else
        {
            cout << "\n Invalid input !!";
        }

        if (sub != "0")
        {
            D.open();
            string temp = to_string(Regis);
            string s_target = to_string(correct_marks);
            string query = "UPDATE STUDENT SET " + sub + " = " + s_target + " WHERE Reg = " + temp + "; ";
            int rc = sqlite3_exec(D.db, query.c_str(), NULL, NULL, &D.err);

            if (rc != SQLITE_OK)
            {
                cout << "\n\nError : " << D.err << " occured during database Updation!!";
            }
            else
            {
                cout << "\nSuccessfully Updated Record of Registration Number " << Regis << " !!";
            }

            D.close();
        }
    }

    void showall() {
        D.open();
        string query = "SELECT * FROM Student ;";
        cout << "\n";
        int rc = sqlite3_exec(D.db, query.c_str(), callback, NULL, &D.err);
        if (rc != SQLITE_OK)
        {
            cout << "\n\nError : " << D.err << " No file found !!";
        }

        D.close();
    }
};

int main()
{
    cout << "\n\n\t\t\t------------ Student Database Management System -------------\n";

    int flag = 1;
    Staff s;
    s.create();

    while (flag)
    {
        cout << "\n\nUser Manual : \n\n1. Create a new record \n2. Delete a record \n3. Update a record \n4. Show a record \n5. Show all record\n6. Exit from the system";
        int choice;
        cout << "\n\nEnter Your Choice : ";
        cin >> choice;

        if (choice == 1)
        {
            int regis;
            string name;
            int ds, ai, wt, cc, ml;
            cout << "\nEnter details of Student : ";
            cout << "\n\nName :";
            cin >> name;
            cout << "Registration Number : ";
            cin >> regis;
            cout << "DSBDL marks : ";
            cin >> ds;
            cout << "AI marks : ";
            cin >> ai;
            cout << "WT marks : ";
            cin >> wt;
            cout << "CC marks : ";
            cin >> cc;
            cout << "ML marks : ";
            cin >> ml;

            s.admit_student(regis, name, ds, ai, wt, cc, ml);
        }
        else if (choice == 2)
        {
            int reg_no;
            cout << "Enter Registration number of student : ";
            cin >> reg_no;
            s.remove_student(reg_no);
        }
        else if (choice == 3)
        {
            int reg_no;
            cout << "Enter Registration number of student : ";
            cin >> reg_no;
            s.modify_deatils(reg_no);
        }
        else if (choice == 4)
        {
            int reg_no;
            cout << "Enter Registration number of student : ";
            cin >> reg_no;
            s.search_details(reg_no);
        }
        else if (choice == 5)
        {
            s.showall();
        }
        else if (choice == 6) {
            cout << "\nSuccessfully out of system !!";
            flag = 0;
        }
        else
        {
            cout << "\n Invalid Choice !!";
        }
    }

    return 0;
}