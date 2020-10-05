//C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <getopt.h>
#include "xcode_redirect.hpp"
#include <iostream>
#include <algorithm>
#include <string>    // needed for VS
#include <istream>
#include <queue>
#include <deque>
#include <cstdlib>
#include <vector>
#include <functional>
#include <cmath>
#include <unordered_map>
#include <map>
#include <iterator>
#include "TableEntry.h"

using namespace std;
class Big_Table{
private:
    string in, in1;
    char prompt = '/';
    struct table_object{
        string name;
        vector< vector<TableEntry>> table;
        vector<char> types;
        vector<string> col_names;
        unordered_map<string, int> map1;
        unordered_map<TableEntry, vector<size_t>> hash;
        std::map<TableEntry, vector<size_t>> bst;
        int index;
        char which = '/';
    };
    unordered_map<string, table_object> map;
    public:
    struct CompLess{
        int col_index;
        const TableEntry &comp;
        CompLess(int x, const TableEntry &y) : col_index(x), comp(y){}
        bool operator()(const vector<TableEntry> &x) const
        {
            return x[size_t(col_index)] < comp;
        }
    };
    struct CompGreater{
        int col_index;
        TableEntry comp;
        CompGreater(int x, TableEntry y) : col_index(x), comp(y){}
        bool operator()(vector<TableEntry> x) const
        {
            return x[size_t(col_index)] > comp;
        }
    };
    struct CompEqual{
        int col_index;
        TableEntry comp;
        CompEqual(int x, TableEntry y) : col_index(x), comp(y){}
        bool operator()(vector<TableEntry> x) const
        {
            return x[size_t(col_index)] == comp;
        }
    };
    int quit(){
        cout << "Thanks for being silly!\n";
        return 0;
    }
    void getMode_1(int argc, char * argv[]) {
        // These are used with getopt_long()
        opterr = true; // Give us help with errors
        int choice;
        int option_index = 0;
        option long_options[] = {
            // TODO: Fill in two lines, for the "mode" ('m') and
            // the "help" ('h') options.
            { "quiet", no_argument, nullptr, 'q' },
            { "help", no_argument, nullptr, 'h' },
            { nullptr, 0,                 nullptr, '\0' }
        };

        // TODO: Fill in the double quotes, to match the mode and help options.
        while ((choice = getopt_long(argc, argv, "qh", long_options, &option_index)) != -1) {
            switch (choice) {
            case 'h':
                prompt = 'h';
                break;
                    
            case 'q':
                prompt = 'q';
                break;
            default:
                cerr << "Error: invalid option\n";
                exit(1);
            } // switch
        } // while
    } // getMode()
    void input(){
        if(prompt == 'h'){
            cout << "Heeeeeelp\n";
            exit(0);
        }
        do {
            cout << "% ";
            cin >> in1;
            switch(in1[0]){
                case '#':
                    getline(cin, in);
                    break;
                case 'Q':
                    quit();
                    break;
                case 'C':
                    creat();
                    break;
                case 'I':
                    insert();
                    break;
                case 'P':
                    print();
                    break;
                case 'R':
                    remove();
                    break;
                case 'D':
                    Delete();
                    break;
                case 'J':
                    join();
                    break;
                case 'G':
                    generate();
                    break;
                default:
                    cout << "Error: unrecognized command\n";
                    getline(cin, in);
                    break;
            }
            
        } while (in1 != "QUIT");
    }
    void creat(){
        table_object temp;
        cin >> in;
        temp.name = in;
        if(map.find(in) != map.end()){
            cout <<"Error: Cannot create already existing table " << in << "\n";
            getline(cin, in);
            return;
        }
        int size;
        cin >> size;
        temp.col_names.reserve(size_t(size));
        temp.types.reserve(size_t(size));
        for(int i = 0; i < size; ++i){
            cin >> in;
            if(in == "string" || in == "bool" || in == "int" || in == "double"){
                temp.types.push_back(in[0]);
                continue;
            }
        }
        for(int i = 0; i < size; ++i){
            cin >> in;
            temp.col_names.push_back(in);
            temp.map1[in] = int(temp.col_names.size() - 1);
        }
        map.emplace(temp.name, temp);
        cout << "New table " << temp.name << " with column(s) ";
        int i = 0;
        while(i != int(temp.col_names.size())){
            cout << temp.col_names[size_t(i)] << " ";
            ++i;
        }
        cout << "created\n";
    }
    void remove(){
        cin >> in;
        if(map.find(in) == map.end()){
            cout <<"Error: " << in << " does not name a table in the database\n";
            getline(cin, in);
            return;
        }
        auto it = map.find(in);
        map.erase(it);
        cout << "Table " << in << " deleted\n";
    }
    void insert(){
        cin >> in;
        cin >> in;
        if(map.find(in) == map.end()){
            cout <<"Error: " << in << " does not name a table in the database\n";
            getline(cin, in);
            return;
        }
        int rows;
        cin >> rows;
        cout << "Added " << rows << " rows to " << in << " from position " <<map[in].table.size() << " to " << (int(map[in].table.size()) + rows-1) <<'\n';
        map[in].table.resize(size_t(int(map[in].table.size()) + rows));
        string dummy;
        cin >> dummy;
        for(size_t i = size_t(int(map[in].table.size())-rows); i < map[in].table.size(); ++i){
            map[in].table[i].reserve(size_t(int(map[in].col_names.size())));
            for(size_t j = 0; j < map[in].col_names.size(); ++j){
                if(map[in].types[j] == 's'){
                    string input;
                    cin >> input;
                    map[in].table[i].emplace_back(input);
                }
                if(map[in].types[j] == 'd'){
                    double input;
                    cin >> input;
                    map[in].table[i].emplace_back(input);
                }
                if(map[in].types[j] == 'i'){
                    int input;
                    cin >> input;
                    map[in].table[i].emplace_back(input);
                }
                if(map[in].types[j] == 'b'){
                    bool input;
                    cin >> input;
                    map[in].table[i].emplace_back(input);
                }
            }
        }
        if(map[in].which == 'h'){
            for(size_t i = size_t(int(map[in].table.size())-rows); i < map[in].table.size(); ++i){
                map[in].hash[map[in].table[size_t(i)][size_t(map[in].index)]].push_back(size_t(i));
            }
        }
        if(map[in].which == 'b'){
            for(size_t i = size_t(int(map[in].table.size())-rows); i < map[in].table.size(); ++i){
                map[in].bst[map[in].table[size_t(i)][size_t(map[in].index)]].push_back(size_t(i));
            }
        }
        
    }

    
    void generate(){
        string name, type, col;
        cin >> in;
        cin >> name;
        cin >> type;
        cin >> in >> in;
        cin >> col;
        if(map.find(name) == map.end()){
            cout <<"Error: " << name << " does not name a table in the database\n";
            getline(cin, in);
            return;
        }
        if(map[name].map1.find(col) == map[name].map1.end()){
            cout <<"Error: " << col << " does not name a column in the " << name << "\n";
            getline(cin, in);
            return;
        }
            if(map[name].which == 'h'){
                map[name].hash.clear();
            }
            else if(map[name].which == 'b'){
                map[name].bst.clear();
            }
        if(type == "hash"){
            map[name].which = 'h';
            map[name].index = map[name].map1[col];
            for(size_t i = 0; i < map[name].table.size(); ++i){
                map[name].hash[map[name].table[i][size_t(map[name].map1[col])]].push_back(i);
            }
            cout << "Created hash index for table " << name << " on column " << col << '\n';
        }
        else if(type == "bst"){
            map[name].which = 'b';
            map[name].index = map[name].map1[col];
            for(size_t i = 0; i < map[name].table.size(); ++i){
                map[name].bst[map[name].table[i][size_t(map[name].map1[col])]].push_back(i);
            }
            cout << "Created bst index for table " << name << " on column " << col << '\n';
        }
    }
    void join(){
        string name, name2, col, col2, temp;
        int count = 0;
        int cols, no;
        vector<string> t;
        vector<int> t1;
        cin >> name;
        cin >> in >> name2 >> in >> col >> in >> col2 >> in >> in;
        if(map.find(name) == map.end()){
            cout <<"Error: " << name << " does not name a table in the database\n";
            getline(cin, in);
            return;
        }
        if(map.find(name2) == map.end()){
            cout <<"Error: " << name2 << " does not name a table in the database\n";
            getline(cin, in);
            return;
        }
        cin >> cols;
        //t.reserve(size_t(cols));
        //t1.reserve(size_t(cols));
        for(int i = 0; i < cols; ++i){
            cin >> temp >> no;
            if(no ==1){
                if(map[name].map1.find(temp) == map[name].map1.end()){
                    cout << temp << "\n";
                    getline(cin, in);
                    return;
                }
            }
            else if(no == 2){
                if(map[name2].map1.find(temp) == map[name2].map1.end()){
                    cout << temp << "\n";
                    getline(cin, in);
                    return;
                }
            }
            if(prompt != 'q'){
                cout << temp << " ";
            }
            t.emplace_back(temp);
            t1.emplace_back(no);
        }
        if(prompt != 'q'){
            cout <<'\n';
        }
        if(map[name2].which == 'h' && map[name2].index == map[name2].map1[col2]){
            for(size_t i = 0; i < map[name].table.size(); ++i){
                if(map[name2].hash.find(map[name].table[i][size_t(map[name].map1[col])]) != map[name2].hash.end()){
                    if(prompt != 'q'){
                    for(size_t j = 0; j < map[name2].hash[map[name].table[i][size_t(map[name].map1[col])]].size(); ++j){//check
                            for(size_t x = 0; x < t.size(); ++x){
                                if(t1[x] == 1){
                                    cout << map[name].table[i][size_t(map[name].map1[t[x]])] << " ";
                                }
                                else{
                                    cout << map[name2].table[map[name2].hash[map[name].table[i][size_t(map[name].map1[col])]][j]][size_t(map[name2].map1[t[x]])] << " ";
                                }
                            }
                            cout << '\n';
                        ++count;
                        }
                    }
                    if(prompt == 'q') count += int(map[name2].hash[map[name].table[i][size_t(map[name].map1[col])]].size());
                }//if found
            }
        }
        /*else if(map[name2].which == 'b' && map[name2].index == map[name2].map1[col2]){
            for(size_t i = 0; i < map[name2].table.size(); ++i){
                map[name2].hash[map[name2].table[i][size_t(map[name2].map1[col2])]].push_back(i);
            }
            for(size_t i = 0; i < map[name].table.size(); ++i){
                if(map[name2].hash.find(map[name].table[i][size_t(map[name].map1[col])]) != map[name2].hash.end()){
                    for(size_t j = 0; j < map[name2].hash[map[name].table[i][size_t(map[name].map1[col])]].size(); ++j){//check
                        if(prompt != 'q'){
                            for(size_t x = 0; x < t.size(); ++x){
                                if(t1[x] == 1){
                                    cout << map[name].table[i][size_t(map[name].map1[t[x]])] << " ";
                                }
                                else{
                                    cout << map[name2].table[map[name2].hash[map[name].table[i][size_t(map[name].map1[col])]][j]][size_t(map[name2].map1[t[x]])] << " ";
                                }
                            }
                            cout << '\n';
                        }
                        ++count;
                    }
                }//if found
            }//for i
            map[name2].hash.clear();
        }*/
        else{
            unordered_map<TableEntry, vector<size_t>> temp;
            
            for(size_t i = 0; i < map[name2].table.size(); ++i){
                temp[map[name2].table[i][size_t(map[name2].map1[col2])]].push_back(i);
            }
            for(size_t i = 0; i < map[name].table.size(); ++i){
                if(temp.find(map[name].table[i][size_t(map[name].map1[col])]) != temp.end()){
                    if(prompt != 'q'){
                    for(size_t j = 0; j < temp[map[name].table[i][size_t(map[name].map1[col])]].size(); ++j){//check
                            for(size_t x = 0; x < t.size(); ++x){
                                if(t1[x] == 1){
                                    cout << map[name].table[i][size_t(map[name].map1[t[x]])] << " ";
                                }
                                else{
                                    cout << map[name2].table[temp[map[name].table[i][size_t(map[name].map1[col])]][j]][size_t(map[name2].map1[t[x]])] << " ";
                                }
                            }
                            cout << '\n';
                        ++count;
                        }
                    }
                    if(prompt == 'q') count += int(temp[map[name].table[i][size_t(map[name].map1[col])]].size());
                }//if found
            }//for i
            temp.clear();
        //}
        /*for(int i = 0; i < map[name].table.size(); ++i){
            for(int j = 0; j < map[name2].table.size(); ++j){
                if(map[name].table[i][map[name].map1[col]] == map[name2].table[j][map[name2].map1[col2]]){
                    for(int x = 0; x < t.size(); ++x){
                        if(map[name].map1.find(t[x]) != map[name].map1.end()){
                            cout << map[name].table[i][map[name].map1[t[x]]] << " ";
                        }
                        else{
                            cout << map[name2].table[j][map[name2].map1[t[x]]] << " ";
                        }
                    }
                }
            }
        }//for
        */
    }
        cout << "Printed " << count << " rows from joining " << name << " to " << name2 << '\n';
  }
    void print(){
        string name;
        cin >> name;
        cin >> name;
        int rows;
        cin >> rows;
        vector<int> index;
        vector<string> columns;
        if(map.find(name) == map.end()){
            cout <<"Error: " << name << " does not name a table in the database\n";
            getline(cin, in);
            return;
        }
        for(size_t i = 0; i < size_t(rows); ++i){
            cin >> in;
            if(map[name].map1.find(in) == map[name].map1.end()){
                cout << in << "\n";
                getline(cin, in);
                return;
            }
            index.push_back(map[name].map1[in]);
            columns.push_back(in);
        }
        cin >> in;
        if(in == "ALL"){
            if(prompt != 'q'){
                for(size_t i = 0; i < columns.size(); ++i){
                cout << columns[i] << " ";
                }
                cout << '\n';
                for(size_t i = 0; i < map[name].table.size(); ++i){
                    for(size_t j = 0; j < index.size(); ++j){
                        cout << map[name].table[i][size_t(index[j])] << " ";
                    }
                    cout <<'\n';
                }
            }
            cout << "Printed " << map[name].table.size() <<" matching rows from " << name << '\n';
        }
        else if(in == "WHERE"){
            string col_name;
            cin >> col_name;
            if(map[name].map1.find(col_name) == map[name].map1.end()){
                cout <<"Error: " << col_name << " does not name a column in the " << name << "\n";
                getline(cin, in);
                return;
            }
            if(prompt != 'q'){
            for(size_t i = 0; i < columns.size(); ++i){
                cout << columns[i] << " ";
                }
                cout << '\n';
            }
            char c;
            cin >> c;
            int count = 0;
            size_t ha = size_t(map[name].map1[col_name]);
            if(map[name].which == 'b' && size_t(map[name].index) == ha){
                if(map[name].types[ha] == 's'){
                        string value;
                        cin >> value;
                        if(c == '<'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                                if(it->first < value){
                                    if(prompt != 'q'){
                                    for(size_t i = 0; i < it->second.size(); ++i){
                                            for(size_t j = 0; j < index.size(); ++j){
                                                cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                            }
                                            cout <<'\n';
                                         ++count;
                                        }
                                     }
                                    if(prompt == 'q') count += int(it->second.size());
                                    }
                                }
                            }//if <
                        if(c == '>'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first > value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                       cout <<'\n';
                                       ++count;
                                   }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }
                        if(c == '='){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first == value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                        ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }//if
                    }
                    if(map[name].types[ha] == 'i'){
                        int value;
                        cin >> value;
                        if(c == '<'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first < value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                    ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }
                        if(c == '>'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first > value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                      ++count;
                                        cout <<'\n';
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }
                        if(c == '='){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first == value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                        cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                    ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }//if
                    }
                    if(map[name].types[ha] == 'd'){
                        double value;
                        cin >> value;
                        if(c == '<'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first < value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                        cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                        ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }
                        if(c == '>'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first > value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                        cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                    }
                                        cout <<'\n';
                                    ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }
                        if(c == '='){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first == value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                         ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }//if
                    }
                    if(map[name].types[ha] == 'b'){
                        bool value;
                        cin >> value;
                        if(c == '<'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first < value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                        cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                    }
                                        cout <<'\n';
                                    ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                
                                }
                            }
                        }
                        if(c == '>'){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first > value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                        ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }
                        if(c == '='){
                            for(auto it = map[name].bst.begin(); it != map[name].bst.end(); ++it){
                            if(it->first == value){
                                if(prompt != 'q'){
                                for(size_t i = 0; i < it->second.size(); ++i){
                                        for(size_t j = 0; j < index.size(); ++j){
                                            cout <<  map[name].table[it->second[i]][size_t(index[j])] << " ";
                                        }
                                        cout <<'\n';
                                    ++count;
                                    }
                                 }
                                if(prompt == 'q') count += int(it->second.size());
                                }
                            }
                        }//if
                    }
                }
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else{
                int h = map[name].map1[col_name];
                if(map[name].types[ha] == 's'){
                    string value;
                    cin >> value;
                    if(c == '<'){
                        CompLess tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                               if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                        cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                   cout <<'\n';
                               }
                                ++count;
                            }
                        }
                    }
                    if(c == '>'){
                        CompGreater tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                               if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                        cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                   cout <<'\n';
                               }
                                ++count;
                            }
                        }
                    }
                    if(c == '='){
                        CompEqual tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                        cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }//if
                }
                if(map[name].types[ha] == 'i'){
                    int value;
                    cin >> value;
                    if(c == '<'){
                        CompLess tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }
                    if(c == '>'){
                        CompGreater tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;

                            }
                        }
                    }
                    if(c == '='){
                        CompEqual tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }//if
                }
                if(map[name].types[ha] == 'd'){
                    double value;
                    cin >> value;
                    if(c == '<'){
                        CompLess tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }
                    if(c == '>'){
                        CompGreater tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }
                    if(c == '='){
                        CompEqual tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }//if
                }
                if(map[name].types[ha] == 'b'){
                    bool value;
                    cin >> value;
                    if(c == '<'){
                        CompLess tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }
                    if(c == '>'){
                        CompGreater tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }
                    if(c == '='){
                        CompEqual tem(h, TableEntry(value));
                        for(size_t i = 0; i < map[name].table.size(); ++i){
                            if(tem(map[name].table[i])){
                                if(prompt != 'q'){
                                    for(size_t j = 0; j < index.size(); ++j){
                                    cout << map[name].table[i][size_t(index[j])] << " ";
                                    }
                                    cout <<'\n';
                                }
                                ++count;
                            }
                        }
                    }//if
                }
            }//else
            cout << "Printed " << count <<" matching rows from "<< name << '\n';
        }//where
    }
    void Delete(){
        cin >> in;
        string name;
        cin >> name;
        if(map.find(name) == map.end()){
            cout <<"Error: " << name << " does not name a table in the database\n";
            getline(cin, in);
         return;
        }
        cin >> in;
        cin >> in;
        if(map[name].map1.find(in) == map[name].map1.end()){
            cout <<"Error: " << in << " does not name a column in the " << name << "\n";
            getline(cin, in);
            return;
        }
        char c;
        cin >> c;
        int count = 0;
        int h = map[name].map1[in];
        if(map[name].types[size_t(h)] == 's'){
            string value;
            cin >> value;
            
            if(c == '<'){
                auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompLess(h, TableEntry(value)));
                count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                map[name].table.erase(it, map[name].table.end());
            }
            if(c == '>'){
                auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompGreater(h, TableEntry(value)));
                count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                map[name].table.erase(it, map[name].table.end());
            }
            if(c == '='){
                auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompEqual(h, TableEntry(value)));
                count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                map[name].table.erase(it, map[name].table.end());
            }
        }
        if(map[name].types[size_t(h)] == 'i'){
            int value;
            cin >> value;
             if(c == '<'){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompLess(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
             if(c == '>'){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompGreater(h, TableEntry(value)));
                 //it- map[name].table.begin()
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
             if(c == '='){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompEqual(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
        }
        if(map[name].types[size_t(h)] == 'd'){
            double value;
            cin >> value;
             if(c == '<'){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompLess(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
             if(c == '>'){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompGreater(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
             if(c == '='){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompEqual(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
        }
        if(map[name].types[size_t(h)] == 'b'){
            bool value;
            cin >> value;
             if(c == '<'){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompLess(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
             if(c == '>'){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompGreater(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
             if(c == '='){
                 auto it = remove_if(map[name].table.begin(), map[name].table.end(), CompEqual(h, TableEntry(value)));
                 count = int(map[name].table.size()- size_t(it - map[name].table.begin()));
                 map[name].table.erase(it, map[name].table.end());
             }
        }
        cout << "Deleted " << count << " rows from " << name << '\n';
        if(map[name].which == 'h'){
            map[name].hash.clear();
            for(size_t i = 0; i < map[name].table.size(); ++i){
                map[name].hash[map[name].table[i][size_t(map[name].index)]].push_back(i);
            }
        }
        if(map[name].which == 'b'){
            map[name].bst.clear();
            for(size_t i = 0; i < map[name].table.size(); ++i){
                map[name].bst[map[name].table[i][size_t(map[name].index)]].push_back(i);
            }
        }
        
    }
};
int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    cin >> std::boolalpha;
    cout << std::boolalpha;
    Big_Table s;
    s.getMode_1(argc, argv);
    s.input();
}
