#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

bool check_input(int argc, char**argv, string &input_file, int &capacity);
void parse (vector<int> &output, string input);
pair<int, vector <int>*> knapSack (int capacity, int item);
vector <int> value;
vector <int> weight;
map<pair<int,int>, pair<int, vector <int>*>> table;
int total_items = 0;

int main(int argc, char**argv){
    string input_file = "";
    int capacity;
    string line;

    //Check to make sure command input is valid
    if(!check_input(argc, argv, input_file, capacity))
        cout << "Invalid Arguments - Bye~\n";
    else{
        //try to open file in read mode
        ifstream file (input_file, ifstream::in);
        if(!file) {cout << "Failed to open file\n";}
        else{
            if(getline(file, line)){ //read the first line
                parse(weight, line); //parse first line (weight)
                if(getline(file, line)) //read the second line
                   parse(value, line); //parse second line (value)
            }    
        }
        file.close(); //close file after reading

        if (!value.empty()) total_items = value.size(); //assign total number of item to be equal the size of vector
        if (total_items > 0){//call knapsack to solve the problem if total item is greater than 0
            pair<int, vector<int>*> optimal = knapSack(capacity, 0);
            
            //Output solution
            printf ("Knapsack Capacity = %d; Optimal value = %d\n", capacity, optimal.first);
            printf ("Item in optimal solution:\n");

            for(int i = 0; i < optimal.second->size(); i++){
                int item = optimal.second->at(i);
                printf("Weight: %d, Value: %d\n", weight[item], value[item]);
            }
        } 
    }
    return 0;
}

/*Function reads from a string of input and parse each element,
 (separate by comman) except for the first one (which is value/
 weight tag) and store into the corresponding vector of value/weight 
*/
void parse (vector<int> &output, string input){
    //this one find the first delimitter that separate name tag from the value/weight of item
    int pre_i = input.find(","); 
    if(pre_i != string::npos){ //if the first delimitter exist, then start parsing the rest of string
        for(int i = pre_i; i < input.length();){ //start check after first "," until end of string
            pre_i = i;
            i = input.find(',', i+1); //start search after previous ,
            
            string val = "";
            if (i != string::npos){ //if no more , can be found
                val = input.substr(pre_i +1, i - pre_i -1); //assign everything after previous , to string val
            }else{ //otherwise
                val = input.substr(pre_i +1, input.length()-pre_i-1); //assign the string value between two , to val
            }
            int right_space = val.find_last_not_of(" \t\r\n"); //find index of the last non space character (right)
            int left_space = val.find_first_not_of(" \t"); //find index of the first non space character (left)
            if(right_space != string::npos){
                val = val.erase(right_space+1, val.length()); //strip all right trailing whitespace
            }
            if(left_space != string::npos){
                val = val.erase(0, left_space); //strip all left trailing whitespace
            }
            output.push_back(stoi(val)); //convert value to string and add to corresponding vector
        }
    }
}
/*
This function check to make sure that command input has at exactly 3 arguments (program name, input fle, and capacity)
Function assigns corresponding input to file name/pathname, and capacity for knapsack problem
*/
bool check_input(int argc, char**argv, string &input_file, int &capacity){
    bool valid = true; //value indicating if the command input is valid, assume it's true by default
    if(argc != 3){
        valid = false;
        cout << "Invalid number of arguments\n";   
    }else{
        input_file = argv[1];   //assign 2nd input for file name
        capacity = stoi(string(argv[2])); //assign 3rd input for capacity and convert to integer type
    }
    return valid;
}
/* This function solve the 0-1 knapsack problem using dynamic programming
Function takes 2 inputs, knapsack capacity of type int, and the index of the element to be evaluate
Function needs access to 2 lists of corresponding weight and value of 
all items in the problem
Function also accesses and modifies a map with its key is a pair of capacity and next index to look at,
and value is a pair of (int) total profit generated and a (vector<int>) list of elements' indexes that contribute
to the total profit
Function returns a pair of (int) maximum profit and a list of elements'indexes that made up the optimal solution
*/
pair<int, vector <int>*> knapSack (int capacity, int item){
    map <pair<int,int>,pair<int, vector<int>*>>::iterator exist; //iterator to search through table
    //If the item index is not less than total item, then record
    //its value into the table as 0 and no element in list of solution
    if (item >= total_items){
        pair<int, int> capacity_item (capacity,item); //key pair (current item)
        exist = table.find(capacity_item); //search for key pair in table
        if (exist == table.end()){ //if not exist
            //then add key pair_value pair (profit 0, empty list of item) into table
            pair<int, vector<int>*> result (0, new vector<int>()); 
            table.insert(pair<pair<int,int>, pair<int, vector<int>*>>(capacity_item,result));
        } 
        return table[capacity_item]; //return value_pair to its calling function
    }else{
        //if weight of current item exceed capacity, then move on and perform knapsack on next item
        if(weight[item] > capacity){ 
            pair<int, int> capacity_item_next (capacity,item+1); //key pair (on next item)
            exist = table.find(capacity_item_next); //search for key pair in table
            if (exist == table.end()){ //if not exist
                //then recursively compute the knapsack of same capacity and next item : value_pair
                //add key pair_value pair into table
                table.insert(pair<pair<int,int>,pair<int, vector<int>*>>(capacity_item_next, knapSack(capacity, item+1)));
            }
            return table[capacity_item_next];
        }else{ //otherwise, 2 choices (take or leave current item)

            //ignore current item and do knapsack on next item
            pair<int, int> capacity_item_next (capacity,item+1); //key pair(on next item)
            exist = table.find(capacity_item_next);//search for key pair in table
            if (exist == table.end()){//if not exist
                //then recursively compute the knapsack of same capacity and next item : value_pair
                //add key pair_value pair into table
                table.insert(pair<pair<int,int>,pair<int, vector<int>*>>(capacity_item_next, knapSack(capacity, item+1)));
            }

            //take current item, then perform knapsack on next item with updated capacity
            //update capacity = current capacity - weight of item to take
            pair<int, int> capacity_item_weight (capacity-weight[item], item+1); //key pair (on updated capacity, next item)
            exist = table.find(capacity_item_weight);//search for key pair in table
            if (exist == table.end()){//if not exist
                //then recursively compute the knapsack of updated capacity and next item : value_pair
                //add key pair_value pair into table
                table.insert(pair<pair<int,int>,pair<int, vector<int>*>> (capacity_item_weight, knapSack(capacity - weight[item], item+1)));
            }

            //Compare values from table if take/leave the current item will yield bigger profit
            if(table[capacity_item_next].first > value[item] + table[capacity_item_weight].first){
                //leave current item yields bigger profit, 
                //return the solution
                return table[capacity_item_next]; 
            }else{  //take current item yields bigger or equal profit
                int profit = value[item] + table[capacity_item_weight].first; //profit generated
                
                //copy on write, otherwise share 1 copy of vector of elements
                //for same capacity and next item index
                vector<int>* temp_list = table[capacity_item_weight].second; //obtain a pointer to the list of solution
                vector<int>* list = new vector<int>(); //create a new vector for this new set of solution
                for(int i = 0; i < temp_list->size();i++){//perform a deep copy of the solution
                    list->push_back(temp_list->at(i));
                }
               
                list->push_back(item); //add the item that we would like to keep into list
                return pair<int, vector<int>*>(profit, list); //return a pair of new profit and new list of elements
            }
        }
    }
}
