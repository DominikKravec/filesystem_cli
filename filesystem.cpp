#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;

namespace fs = std::filesystem;

std::vector<std::string> parseCommand(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}
void list_curr_path(const fs::path curr_path){

    for(auto const& dir_entry : fs::directory_iterator{curr_path}){
        cout << dir_entry.path() << "\n";
    }

}
void change_dir(fs::path *curr_path, string arg){

    fs::path temp = *(curr_path) / arg;
    temp = temp.lexically_normal();

    if( !fs::exists(temp) ){
        cout << "Directory doesnt exist\n";
        return;
    }

    if(!fs::is_directory(temp)){
        cout << "Requested path isn't a directory\n";
        return;
    }

    *curr_path = temp;
}

long recursive_size(fs::path file){
    if(fs::is_regular_file(file)){
        return (long) fs::file_size(file);
    }

    long res = 0;

    for(auto const& dir_entry : fs::directory_iterator{file}){
        res += recursive_size(dir_entry);
    }

    return res;
}

void print_size(fs::path curr_path, string file_name){

    fs::path file = curr_path / file_name;

    if( !fs::exists(file) ){
        cout << "No such file: " << file.string() << "\n";
        return;
    }

    cout << recursive_size(file) << "B\n";    

}

int main() {
    cout << "--------------------------------------\nWelcome to the cli\n----------------------------------------------\n\n";

    fs::path currentPath = fs::current_path();
    
    while(1){
        cout << "[" << currentPath.string() << "] > ";
    
        string input;
    
        getline(std::cin, input);

        std::vector<std::string> args = parseCommand(input);
        std::string command = args[0];

        if(command == "exit" || command == "e"){
            break;
        }else if(command == "help" || command == "h"){
            cout << "exit - exits the programme\n"
                    "list - list contents of current directory\n"
                    "cd - change current directory\n"
                    "size - prints size of file or dir\n";
        }else if(command == "list" || command == "ls"){
            list_curr_path(currentPath);
        }else if(command == "cd"){

            if(args.size() == 1) cout << "Missing argument";

            change_dir(&currentPath, args[1]);
        }else if(command == "size"){

            if(args.size() < 2) cout << "Missing argument";

            print_size(currentPath, args[1]);
        }else{
            cout << "Unknown command, try help for guide\n";
        }

    }

    return 0;
} 