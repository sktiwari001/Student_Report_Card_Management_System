#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

// Database credentials
const string SERVER = "tcp://127.0.0.1:3306";
const string USERNAME = "root";
const string PASSWORD = "your password";
const string DATABASE = "mydb";

class Student
{
private:
    string Name, Rollno, Grade;
    float Avg;

public:
    Student(string name, string rollno, float avg, string grade)
        : Name(name), Rollno(rollno), Avg(avg), Grade(grade) {}

    string getName() { return Name; }
    string getRollno() { return Rollno; }
    string getGrade() { return Grade; }
    float getAvg() { return Avg; }
};

void report(sql::Connection *conn)
{
    string rollNo, grade;
    int sub1, sub2, sub3, total;
    float avg;

    cout << "Enter roll number: ";
    cin >> rollNo;

    cout << "Enter the marks of subject 1: ";
    cin >> sub1;

    cout << "Enter the marks of subject 2: ";
    cin >> sub2;

    cout << "Enter the marks of subject 3: ";
    cin >> sub3;

    total = sub1 + sub2 + sub3;
    avg = total / 3.0f;

    if (avg >= 90)
        grade = "A+";
    else if (avg >= 80)
        grade = "A";
    else if (avg >= 70)
        grade = "B+";
    else if (avg >= 60)
        grade = "B";
    else if (avg >= 50)
        grade = "C+";
    else if (avg >= 40)
        grade = "C";
    else
        grade = "D";

    try
    {
        sql::PreparedStatement *pstmt = conn->prepareStatement(
            "UPDATE Student SET Avg = ?, Grade = ? WHERE RollNo = ?");
        pstmt->setDouble(1, avg);
        pstmt->setString(2, grade);
        pstmt->setString(3, rollNo);
        pstmt->execute();
        delete pstmt;

        sql::PreparedStatement *selectStmt =
            conn->prepareStatement("SELECT * FROM Student WHERE RollNo = ?");
        selectStmt->setString(1, rollNo);
        sql::ResultSet *res = selectStmt->executeQuery();

        cout << "\t| RollNo | Name       | Avg   | Grade" << endl;
        while (res->next())
        {
            cout << "\t" << res->getString("RollNo") << " | "
                 << res->getString("Name") << " | "
                 << res->getDouble("Avg") << " | "
                 << res->getString("Grade") << endl;
        }

        delete res;
        delete selectStmt;
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL Error: " << e.what() << endl;
    }
}

int main()
{
    sql::Driver *driver;
    sql::Connection *conn;

    try
    {
        driver = get_driver_instance();
        conn = driver->connect(SERVER, USERNAME, PASSWORD);
        conn->setSchema(DATABASE);
        cout << "Connected to Database!" << endl;
    }
    catch (sql::SQLException &e)
    {
        cout << "Connection failed: " << e.what() << endl;
        return 1;
    }

    Student s1("Shubham Kumar", "ab123", 0.0, "NULL");
    Student s2("Sonu Kumar", "bc234", 0.0, "NULL");
    Student s3("Rohit Raushan", "cd345", 0.0, "NULL");

    try
    {
        sql::PreparedStatement *pstmt = conn->prepareStatement(
            "INSERT INTO Student (RollNo, Name, Avg, Grade) VALUES (?, ?, ?, ?)");

        pstmt->setString(1, s1.getRollno());
        pstmt->setString(2, s1.getName());
        pstmt->setDouble(3, s1.getAvg());
        pstmt->setString(4, s1.getGrade());
        pstmt->execute();

        pstmt->setString(1, s2.getRollno());
        pstmt->setString(2, s2.getName());
        pstmt->setDouble(3, s2.getAvg());
        pstmt->setString(4, s2.getGrade());
        pstmt->execute();

        pstmt->setString(1, s3.getRollno());
        pstmt->setString(2, s3.getName());
        pstmt->setDouble(3, s3.getAvg());
        pstmt->setString(4, s3.getGrade());
        pstmt->execute();

        delete pstmt;
        cout << "Data inserted successfully!" << endl;
    }
    catch (sql::SQLException &e)
    {
        cout << "Error inserting data: " << e.what() << endl;
    }

    bool exit = false;
    while (!exit)
    {
        system("cls");
        cout << "Welcome to Student Report Card System" << endl;
        cout << "1. Report Card" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        int val;
        cin >> val;

        if (val == 1)
        {
            report(conn);
        }
        else if (val == 2)
        {
            exit = true;
            cout << "Goodbye!" << endl;
        }
        else
        {
            cout << "Invalid input, try again." << endl;
        }
    }

    delete conn;
    return 0;
}