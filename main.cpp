#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

int length = 0;

struct Item {
    int id;
    string title;
    bool completed;
    
    Item() : id(0), title(""), completed(false) {}
    Item(int i, const string &t, bool c) : id(i), title(t), completed(c) {}
};


int getLastItemID(const json &j) {
    if (j.empty() || !j.is_array()) {
        return 0;
    }
    const json& last = j.back();
    if (last.contains("Id") && last["Id"].is_number()) {
        return last["Id"].get<int>();
    }
    return 0;
}


void Create(const Item &task, json &j) {
    json newElement;
    newElement["Id"] = task.id;
    newElement["Title"] = task.title;
    newElement["Completed"] = task.completed;
    j.push_back(newElement);
    ++length;
    
    cout << GREEN << "✓ Task created successfully!" << RESET << endl;
    cout << CYAN << "  ID: " << BOLD << task.id << RESET << endl;
    cout << CYAN << "  Title: " << RESET << task.title << endl << endl;
}

void Delete(json &j, int id) {
    if (!j.is_array()){
        cerr << RED << "✗ Error: JSON is not an array" << RESET << endl;
        return;
    }

    for (auto it = j.begin(); it != j.end(); ++it){
        if (it->contains("Id") && (*it)["Id"].get<int>() == id){
            j.erase(it);
            cout << GREEN << "✓ Task with ID " << BOLD << id << RESET 
                 << GREEN << " was deleted successfully" << RESET << endl << endl;
            return;
        }
    }

    cerr << RED << "✗ Task with ID " << BOLD << id << RESET 
         << RED << " was not found!" << RESET << endl << endl;
}

void Update(json &j, int id, string content) {
    if (!j.is_array()){
        cerr << RED << "✗ Error: JSON is not an array" << RESET << endl;
        return;
    }
    
    for (auto &task : j){
        if(task.contains("Id") && task["Id"].get<int>() == id){
            task["Title"] = content;
            cout << GREEN << "✓ Task with ID " << BOLD << id << RESET 
                 << GREEN << " was updated successfully" << RESET << endl;
            cout << CYAN << "  New content: " << RESET << content << endl << endl;
            return;
        }
    }

    cerr << RED << "✗ Task with ID " << BOLD << id << RESET 
         << RED << " was not found!" << RESET << endl << endl;
}

void Search(json &j, int id) {
    if(!j.is_array()){
        cerr << RED << "✗ Error: JSON is not an array" << RESET << endl;
        return;
    }
    
    for (auto &task : j){
        if(task.contains("Id") && task["Id"].get<int>() == id){
            cout << endl << BOLD << MAGENTA << "═══ Task Details ═══" << RESET << endl;
            cout << CYAN << "ID: " << RESET << BOLD << task["Id"].get<int>() << RESET << endl;
            cout << CYAN << "Title: " << RESET << task["Title"].get<string>() << endl;
            cout << CYAN << "Status: " << RESET 
                 << (task["Completed"].get<bool>() ? GREEN "✓ Completed" : YELLOW "○ Not Completed") 
                 << RESET << endl;
            cout << BOLD << MAGENTA << "═══════════════════" << RESET << endl << endl;
            return;
        }
    }
    
    cerr << RED << "✗ Task with ID " << BOLD << id << RESET 
         << RED << " was not found!" << RESET << endl << endl;
}

void check(json &j, int id){
    if(!j.is_array()){
        cerr << RED << "✗ Error: JSON is not an array" << RESET << endl;
        return;
    }
    
    for (auto &task : j){
        if (task.contains("Id") && task["Id"].get<int>() == id){
            bool newStatus = !task["Completed"].get<bool>();
            task["Completed"] = newStatus;
            cout << GREEN << "✓ Task with ID " << BOLD << id << RESET 
                 << GREEN << " marked as " << RESET 
                 << (newStatus ? GREEN BOLD "Completed" : YELLOW BOLD "Not Completed") 
                 << RESET << endl << endl;
            return;
        }
    }
    
    cerr << RED << "✗ Task with ID " << BOLD << id << RESET 
         << RED << " was not found!" << RESET << endl << endl;
}

void Read(json &j) {
    cout << endl << BOLD << BLUE << "═══════════════════════════" << RESET << endl;
    cout << BOLD << BLUE << "    JSON File Content" << RESET << endl;
    cout << BOLD << BLUE << "═══════════════════════════" << RESET << endl << endl;
    cout << j.dump(4) << endl << endl;
}

void addTask(json &j, vector<Item> &tasks, const string &title) {
    int lastId = getLastItemID(j);
    int newId = lastId + 1;
    tasks.push_back(Item(newId, title, false));
    Create(tasks.back(), j);
}


int main(int argc, char *argv[]) {
    vector<Item> tasks;
    json j;
    
    ifstream ifs("output.json");
    if (ifs.is_open()) {
        try {
            ifs >> j;
            if (!j.is_array()) {
                j = json::array();
            }
        } catch (json::parse_error& e) {
            j = json::array();
        }
        ifs.close();
    } else {
        j = json::array();
    }

    if (argc < 2) {
        cerr << RED << "✗ Error: No command specified" << RESET << endl;
        cerr << YELLOW << "Usage: " << RESET << argv[0] << " <command> [args]" << endl << endl;
        return 1;
    }

    string command = argv[1];
    
    if(argc < 3 && (command == "create" || command == "delete" || command == "check" || command == "search")){
        cerr << RED << "✗ Error: Missing arguments" << RESET << endl;
        cerr << YELLOW << "Usage: " << RESET << argv[0] << " <command> [args]" << endl << endl;
        return 1;
    }
    
    if(argc < 4 && command == "update"){
        cerr << RED << "✗ Error: Missing arguments" << RESET << endl;
        cerr << YELLOW << "Usage: " << RESET << argv[0] << " update [ID] [New-Content]" << endl << endl;
        return 1;
    }
    
    if (command == "create"){
        addTask(j, tasks, argv[2]);
    }
    else if(command == "delete"){
        Delete(j, stoi(argv[2]));
    }
    else if(command == "read"){
        Read(j);
    }
    else if(command == "check"){
        check(j, stoi(argv[2]));
    }
    else if(command == "update"){
        Update(j, stoi(argv[2]), argv[3]);
    }
    else if(command == "search"){
        Search(j, stoi(argv[2]));
    }
    else{
        cerr << RED << "✗ Command not found: " << RESET << command << endl << endl;
        return 1;
    }
    
    ofstream ofs("output.json");
    if (!ofs.is_open()) {
        cerr << RED << "✗ Error: Could not open file for writing" << RESET << endl;
        return 1;
    }
    ofs << j.dump(4);
    ofs.close();
    
    return 0;
}
