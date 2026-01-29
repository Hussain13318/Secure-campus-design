#include <iostream>
#include <vector>
#include <string>
using namespace std ;

//making of an abstract classss
class secureentity
{
    public:
        virtual void grantaccess() = 0 ;
        virtual void logaccess()  = 0 ;
};

//accessloger class is made which will store all the log activites
class accesslogger
{
    private :
        vector <string> logs ; 

    public :
        accesslogger()
        {
            //default constructor// Default constructor initializes the access logger with an empty log list 
        }

        //using template
        template <typename t>
        void addlog(const t& logmeassage )       //login message
        {
            if (logs.size()<100)
            {
                cout << "\nLog added because we have space";
                logs.push_back(logmeassage) ;
            }
            else
            {
                cout << "\nWe can not add more logs because the space is already fulled" ;
            }
            
        }

        void viewlogs() const
        {
            cout << "\033[1;33m\n----------------------------Below are all the access logs-----------------------------\n\033[0m\n";

            if (logs.empty())
            {
                cout << "\nCan't display anything. No logs available!\n";
                return;
            }
            for (const auto& currentlog : logs)
            {
                cout << currentlog << endl;
            }
        }

        ~ accesslogger()
        {
            //destructor used to clear memory
            logs.clear() ;
        }

};

//making an object for access logger
accesslogger logobj ;

//making a base user class from which 3 other derived classes are made student , teacher and admin the user itself is derived form secure entity class
class user : public secureentity
{
    public:
        string name;
        int id;
        string role;
        bool loginstatus;
        string password;
        static int usercount;

        //user name constructor used to store defaultt values with this pointer
        user(string name ="" ,int id=0 ,string role="",string password="")
        {
            this -> name=name;
            this -> id =id;
            this -> role=role ;
            this -> password=password ;
            this -> loginstatus=false ;
            usercount ++ ; //this pointer can't be user on static variable
        }

        ~user()
        {
            //destructor
        }

        //return user name
        string getname()
        {
            return this-> name ;
        }

        //return user role
        string getrole()
        {
            return this-> role;
        }

        //return number of user
        static int getusercount()
        {
            return usercount ;
        }

        //return login status -> used in main program
        bool login(string enteredpassword)
        {
            if (enteredpassword==password)
            {
                cout << "\nCorrect password !";
                this -> loginstatus=true ;
                return true ;
            }
            else{
                return false ;
            }    
        }

        //logout function
        void logout()
        {
            this -> loginstatus ;
            loginstatus=false;
            cout << "\nYou have logout" ;
        }

};

int user::usercount = 0;

//student class derived form user class
class student : public user
{
    public:
        student(string name, int id ,string password="1234" ) : user (name,id ,"student",password) {}//passwd value is default it can be changed // Constructor ensures every student object always has the role "Student"

        void grantaccess()override
        {
            cout << "\nAccess granted.\nStudent has limited access only to library" ;
        }

        void logaccess() override
        {
            
            string login_message= name + "\033[1;32m who is a student tried to login in system\033[0m\n";
            logobj.addlog(login_message) ;
        }
};

//faculty class derived from user class
class faculty:public user
{
    public:
        faculty(string name , int id , string password ="1234" ) : user(name,id,"Faculty" ,password ) {}

        void grantaccess()override
        {
            cout << "\nFaculty has access to both digital library and cyber lab ! " ;
        }

        void logaccess() override
        {
            string login_message= name + "\033[1;34m who is a faculty member tried to login in system\033[0m\n";
            logobj.addlog(login_message) ;
        }

};

//admin class derived from user class
class admin:public user
{
    public:
        admin(string name , int id , string password ="1234" ) : user(name,id,"Admin" ,password){}

       void grantaccess()override
        {
            cout << "\nAdmin hass full access to all zones !" ;
        }

        void logaccess() override
        {
            string login_message= name + "\033[1;35m who is an admin member tried to login in system\033[0m\n";
            logobj.addlog(login_message) ;
        } 

        //we also knew that admin can view all logs so make that funtion as well
        void view_all_logs()const
        {
            logobj.viewlogs() ;
        }
};

//now lets make a zone class
class zone 
{
    private:
        string zone_name;
        int security_level ;

    public:
        zone(string n,int l)
        {
            zone_name=n;
            security_level=l;
        }

        //lets do operator overloading +=
        void operator +=(user &userobj)
        {
            if (security_level==1 && (userobj.getrole() == "student" || userobj.getrole() == "Faculty" || userobj.getrole() == "Admin") )
            {
                cout << "\nUser has been given access to library" ;
                userobj.grantaccess();
            }
            else if (security_level == 2 && (userobj.getrole() == "Faculty" || userobj.getrole() == "Admin"))
            {
                cout << "\nUser has been given access cyber security lab";
                userobj.grantaccess();
            }
            else if (security_level == 3 && userobj.getrole() == "Admin")
            {
                cout << "\nUser has been given access to Data center";
                userobj.grantaccess();
            }
            else
            {
                cout << "\nSecurity level didn't matched.\nNo access given" ;
            }
            
            userobj.logaccess() ;  
        }

        bool operator == (const zone& other) const 
        {
            return this -> security_level == other.security_level ;
        }

        friend void admingrantzone(zone &z , user & u) ;
};

    // Admin manually grants access to a user for a specific zone using operator overloading here  it checks if he can get access or not
    void admingrantzone(zone &z , user & u)
    {
        cout<<"\nAdmin will manualy manage zone and role access" ;
        z += u ;
    }

    // Stores pointers to user objects so we can access users by their memory addresses
    vector <user*> user_list ;

    //add user
    void adduser()
    {
        string name , password ;
        int type , id;

        cout << "\033[1;33m\nEnter the type of user you want to make\n1-> Students\n2-> Faculty\n3-> Admin\nYour choice : ";
        cin >> type ;

        cout <<"Enter name of the user:" ;
        cin.ignore();
        getline(cin, name);

        cout << "Enter user identity :";
        cin >> id ;
        cout  << "Enter user login password:" ;
        cin >> password ;

        if (user_list.size() < 100)
        {
            if (type == 1)
            {
                user_list.push_back(new student(name, id, password)) ;
                cout << "\nUser made successfully" ;
            }
            else if (type == 2)
            {
                user_list.push_back(new faculty(name, id, password));
                cout << "\nUser made successfully" ;
            }
            else if (type == 3)
            {
                user_list.push_back(new admin(name, id, password));
                cout << "\nUser made successfully" ;
            }
            else
            {
                cout << "\nIncorrect option no such user type exist and can be made ! ";
                
            }
        }
        else
        {
            cout << "\nSorry we can't add more user storage is already full";
        }
    }

    //login function used to check if particular user exist and can be loged in
    // Checks if user exists; if yes, returns the address of that user object
    user* loginuser(string roletype)
    {
        string name, password;

        cout <<"Enter name of the user:" ;
        cin.ignore();
        getline(cin, name);


        cout << "Enter logged in user password: ";
        cin >> password;

        for (auto user : user_list) 
        {
            if (user->getname() == name && user->getrole() == roletype) 
            {
                if (user->login(password)) 
                {
                    cout << "\nAll the credentials matched.Login successful!\n";
                    return user;
                } 
                else 
                {
                    cout << "Login failed.Incorrect password!\n";
                    return nullptr;
                }
            }
        }
        cout << "\nNo such user has ever logged";
        return nullptr;
    }

//main part of program
int main() 
{
    zone digital_library("Digital Library", 1);
    zone cyber_lab("Cyber Security Lab", 2);
    zone data_center("Data Center Room", 3);

    cout << "\033[36m\n|=========> Hussain Secure Campus Management System <=========|\033[0m\n";
    int choice;
    do 
    {
        cout << "\033[1;32m\nFrom the list below choose the operation you want to perform\033[0m\n";
        cout << "\n1. Add new user to system";
        cout << "\n2. Login as a Student";
        cout << "\n3. Login as a Faculty member";
        cout << "\n4. Login as an Admin";
        cout << "\n5. Compare Zone Security Levels";
        cout << "\n6. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (cin.fail()) //This code checks if the user entered something wrong (like a letter instead of a number)
        {
            cin.clear();// When invalid input is entered, cin enters an error state and stops accepting further input.cin.clear(); resets that state, telling the program (not the compiler) to get ready to accept input again.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');// Ignore (skip) characters in the input buffer up to the maximum number possible or until a newline character ('\n', Enter key) is found.
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        user* currentUser = nullptr;

        switch (choice)
        {
            case 1:
            {
                cout << "\nYou chose to add user.\nLet's add a new user to the system.\n";
                adduser();
                break;
            }
//----------------------------------------------------------------------------------------------------------------------------------------------
            case 2:
            {
                currentUser = loginuser("student");
                if (currentUser != nullptr)
                {
                    int c;
                    do
                    {
                        cout << "\nWelcome, " << currentUser->getname() << " (Student)";
                        cout << "\n1. Request access to library";
                        cout << "\n2. Request access to cyber_lab";
                        cout << "\n3. Request access to Data Center";
                        cout << "\n4. Logout as student";
                        cout << "\nEnter choice: ";
                        cin >> c;

                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Please enter a number.\n";
                            continue;
                        }

                        //-----------------------------------------------------------------------------------------------------------------------------

                        if (c == 1)
                        {
                            digital_library += *currentUser;
                        }

                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 2)
                        {
                            cyber_lab += *currentUser;
                        }
                        
                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 3)
                        {
                            data_center += *currentUser;
                        }

                    } while (c != 4);

                    currentUser->logout();
                }
                else
                {
                    cout << "Login failed: No such Student user found.\n";
                }
                break;
            }
//----------------------------------------------------------------------------------------------------------------------------------------------
            case 3:
            {
                currentUser = loginuser("Faculty");
                if (currentUser != nullptr)
                {
                    int c;
                    do
                    {
                        cout << "\nWelcome, " << currentUser->getname() << " (Faculty member)";
                        cout << "\n1. Request access to library";
                        cout << "\n2. Request access to cyber_lab";
                        cout << "\n3. Request access to Data Center";
                        cout << "\n4. Logout as faculty member";
                        cout << "\nEnter choice: ";
                        cin >> c;

                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Please enter a number.\n";
                            continue;
                        }

                        //-----------------------------------------------------------------------------------------------------------------------------

                        if (c == 1)
                        {
                            digital_library += *currentUser;
                        }
                        
                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 2)
                        {
                            cyber_lab += *currentUser;
                        }
                        
                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 3)
                        {
                            data_center += *currentUser;
                        }

                    } while (c != 4);

                    currentUser->logout();
                }
                else
                {
                    cout << "Login failed: No such Faculty user found.\n";
                }
                break;
            }
//----------------------------------------------------------------------------------------------------------------------------------------------
            case 4:
            {
                currentUser = loginuser("Admin");
                if (currentUser != nullptr)
                {
                    admin* adminPtr = dynamic_cast<admin*>(currentUser);
                    int c;
                    do
                    {
                        cout << "\nWelcome, " << currentUser->getname() << " (Admin)";
                        cout << "\n1. Request access to library";
                        cout << "\n2. Request access to cyber_lab";
                        cout << "\n3. Request access to Data Center";
                        cout << "\n4. View all the Logs";
                        cout << "\n5. Grant user access";
                        cout << "\n6. Logout as Admin";
                        cout << "\nEnter choice: ";
                        cin >> c;

                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Please enter a number.\n";
                            continue;
                        }

                        //-----------------------------------------------------------------------------------------------------------------------------

                        if (c == 1)
                        {
                            digital_library += *currentUser;
                        }
                        
                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 2)
                        {
                            cyber_lab += *currentUser;
                        }
                        
                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 3)
                        {
                            data_center += *currentUser;
                        }
                        
                        //-----------------------------------------------------------------------------------------------------------------------------

                        else if (c == 4)
                        {
                            if (adminPtr != nullptr)
                            {
                                adminPtr->view_all_logs();
                            }
                        }

                        //-----------------------------------------------------------------------------------------------------------------------------
                        
                        else if (c == 5)
                        {
                            if (adminPtr != nullptr)
                            {
                                if (!user_list.empty())
                                {
                                    int index;
                                    cout << "\nThe total number of user are " << user::usercount ;
                                    cout << "\nEnter index of user to grant access (0 to " << user_list.size() - 1 << "): ";
                                    cin >> index;

                                    if (cin.fail())
                                    {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cout << "Invalid input! Please enter a number.\n";
                                        continue;
                                    }

                                    if (index >= 0 && index < (int)user_list.size())
                                    {
                                        
                                        admingrantzone(cyber_lab, *user_list[index]);
                                    }
                                    
                                    else
                                    {
                                        cout << "Invalid index.\n";
                                    }
                                }
                            }
                        }

                    } while (c != 6);

                    currentUser->logout();
                }
                else
                {
                    cout << "Login failed: No such Admin user found.\n";
                }
                break;
            }
//----------------------------------------------------------------------------------------------------------------------------------------------
            case 5:
            {
                int compare_security_level_Choice;
                cout << "\033[1;31mnChoose an option to compare security levels:\033[0m\n";
                cout << "\n1. Cyber Lab vs Digital Library";
                cout << "\n2. Cyber Lab vs Data Center";
                cout << "\n3. Digital Library vs Data Center";
                cout << "\nEnter choice: ";
                cin >> compare_security_level_Choice;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number.\n";
                    break;
                }

                //-----------------------------------------------------------------------------------------------------------------------------

                if (compare_security_level_Choice == 1)
                {
                    if (cyber_lab == digital_library)
                    {
                        cout << "\nCyber Lab and Digital Library have the same security level.\n";
                    }
                    else
                    {
                        cout << "\nCyber Lab and Digital Library have different security levels.\n";
                    }
                }

                //-----------------------------------------------------------------------------------------------------------------------------
                
                else if (compare_security_level_Choice == 2)
                {
                    if (cyber_lab == data_center)
                    {
                        cout << "\nCyber Lab and Data Center have the same security level.\n";
                    }
                    else
                    {
                        cout << "\nCyber Lab and Data Center have different security levels.\n";
                    }
                }

                //-----------------------------------------------------------------------------------------------------------------------------

                else if (compare_security_level_Choice == 3)
                {
                    if (digital_library == data_center)
                    {
                        cout << "\nDigital Library and Data Center have the same security level.\n";
                    }
                    else
                    {
                        cout << "\nDigital Library and Data Center have different security levels.\n";
                    }
                }

                //-----------------------------------------------------------------------------------------------------------------------------

                else
                {
                    cout << "\nInvalid choice.\n";
                }
                break;
            }
//----------------------------------------------------------------------------------------------------------------------------------------------
            case 6:
            {
                cout << "\nExiting program.\n";
                break;
            }
//--------------------------------------------------------------------------------------------------------------------------------------------------
            default:
            {
                cout << "\nInvalid main menu choice.\n";
                break;
            }
        }

    } while (choice != 6);

    for (auto u : user_list)
    {
        delete u;
    }
    user_list.clear();

    cout << "\033[1;36m+---------------------+\033[0m\n";
    cout << "\033[1;35m|    Memory cleared   |\033[0m\n";
    cout << "\033[1;36m+---------------------+\n\033[0m\n";

    return 0;
}
