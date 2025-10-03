#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
using namespace std;
using json = nlohmann::json;

// Global
int length = 0;

struct Item {
    int id;
    string title;
    bool completed;
    Item() : id(0), title(""), completed(false) {}
    Item(int i, const string &t, bool c)
        : id(i), title(t), completed(c) {}
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
    j.push_back(newElement);  // Add to JSON array
    ++length;
}

void Delete(json &j, int id) {
    if (!j.is_array()){
    	cerr << "JSON is not an array\n";
	return;
    }


    for (auto it = j.begin(); it != j.end(); ++it){
    	if (it->contains("Id") && (*it)["Id"].get<int>() == id){
		j.erase(it);
		cout << "Task with ID " << id << " was deleted Successfully\n";
		return;
	}
    }

    cerr << "Task with ID " << id << " was not found!\n";
}
 
void Update() {
    // TODO
}
	
void Search() {
    // TODO
}

void check(){
	
}

void Read(json &j) {
    cout << "Json File Content:\n" << j.dump(4) << endl;
}

void addTask(json &j, vector<Item> &tasks, const string &title) {
    int lastId = getLastItemID(j);
    int newId = lastId + 1;  // give next ID
    tasks.push_back(Item(newId, title, false));
    Create(tasks.back(), j);
}

int main(int argc, char *argv[]) {
    vector<Item> tasks;
    json j;
    
    // Read existing file first
    std::ifstream ifs("output.json");
    if (ifs.is_open()) {
        try {
            ifs >> j;  // Parse existing JSON
            // Make sure it's an array
            if (!j.is_array()) {
                j = json::array();
            }
        } catch (json::parse_error& e) {
            // If file is empty or invalid, start with empty array
            j = json::array();
        }
        ifs.close();
    } else {
        // File doesn't exist, start with empty array
        j = json::array();
    }

    string command = argv[1];
    
    // Process command
	if(argc < 3 && (command == "create" || command == "delete")){
		cerr << "Usage: " << argv[0] << " <cmomand> [args]\n";
		return 1;
	}
    if (command == "create"){
        addTask(j, tasks, argv[2]);
    }else if(command == "delete"){
    	Delete(j, stoi(argv[2]));
    }else if(command == "read"){
		Read(j);
	}else{
		cout << "Command Not Found!\n";
	}
    
    // Write back to file (overwrites with complete data)
    std::ofstream ofs("output.json");
    if (!ofs.is_open()) {
        std::cerr << "Could not open file for writing\n";
        return 1;
    }
    ofs << j.dump(4);  // dump(4) for pretty printing with 4-space indent
    ofs.close();
    
    return 0;
}
