#include<FileSystem.h>

void Helper()
{
    std::cout << "Commands: " << std::endl;
    cout << ".register -Make a new user" << endl;
    cout << ".login -Login as a existed user" << endl;
    cout << ".logout -Log out current user" << endl;
    cout << ".create -Make a new file in current dir" << endl;
    cout << ".read -Read file using file name" << endl;
    cout << ".write -Write to file using file name" << endl;
    cout << ".delete -Delete file by file name" << endl;
    cout << ".dir -Show current dir" << endl;
    cout << ".quit -Quit the system" << endl;
}

void Handle_cmd()
{
    FileSystem fs;

    std::cout << "______ _____  _  _____   _______   _______ _____ ________  ___" << std::endl;
    std::cout << "| ___|_   _| |   | ___| / ___ \  \ / /  ___ | _   _ | ___ | \/  |" << std::endl;
    std::cout << "| |_    | |  | |   | |__  \`--.  \  V / \ `- - .  | | | |__ | .  . |" << std::endl;
    std::cout << "|  _|   | |  | |   |  __|  `--. \ \  / `- - . \ | | |  __|| |\/| |" << std::endl;
    std::cout << "| |    _| |_ | |___| |___ /\__/ / | | /\__/ / | | | |___| |  | |" << std::endl;
    std::cout << "\_ |   \___/ \_____|____/ \____/  \_/ \____ / \_ / \____ / \_|  |_ /" << std::endl;
    cout << endl;
    cout << "enter .help for help" << endl;
    string cmd;
    while (cin >> cmd)
    {
        cin.ignore();
        if (cmd == ".help")
        {
            Helper();
        }
        else if (cmd == ".quit")
        {
            break;
        }
        else if (cmd == ".register")
        {
            string username, password;
            cout << "Enter username(may not be identical to exsited users): ";
            cin >> username;
            cin.ignore();
       
            cout << "Enter password: ";
            cin >> password;
            cin.ignore();
            fs.registerUser(username, password);
        }
        else if (cmd == ".login")
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cin.ignore();

            cout << "Enter password: ";
            cin >> password;
            cin.ignore();
            fs.loginUser(username, password);
        }
        else if (cmd == ".logout")
        {
            fs.logoutUser();
        }
        else if (cmd == ".create")
        {
            string file_name,file_content;
            bool r, w;
            cout << "File name: ";
            cin >> file_name;
            cin.ignore();
            cout << "Content: ";
            cin >> file_content;
            cin.ignore();
            cout << "Current user's r&w permission(1 for yes, 0 for no): ";
            cin >> r >> w;
            fs.createFile(file_name, file_content, r, w);
        }
        else if (cmd == ".read")
        {
            string file_name;
            cout << "Enter the name of file you want to read: ";
            cin >> file_name;
            cin.ignore();
            fs.readFile(file_name);
        }
        else if (cmd == ".write")
        {
            string file_name, content;
            cout << "Enter the name of file you want to write: ";
            cin >> file_name;
            cin.ignore();
            cout << "New content: ";
            cin >> content;
            cin.ignore();
            fs.writeFile(file_name, content);
        }
        else if (cmd == ".delete")
        {
            string file_name;
            cout << "Enter the name of file you want to delete: ";
            cin >> file_name;
            cin.ignore();
            fs.deleteFile(file_name);
        }
        else if (cmd == ".dir")
        {
            fs.showDirectory();
        }
        else
        {
            cout << "Invalid command, please enter again or enter .help for help" << endl;
        }
    }
}

int main() {
    Handle_cmd();
    
    return 0;
}

