/*
**Robert
**CPCS 2430
**assignment4.cpp
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, char del){
    vector<string> result;
    // declaring temp string to store the curr "word" upto del
    string temp = "";
    for(int i=0; i<(int)str.size(); i++){
        // If current char is not ',', then append it to the "word", otherwise you have completed the word, and start the next word
        if(str[i] != del){
	    temp += str[i];
        }else{
            result.push_back(temp);
            temp = "";
        }
    }
    // push the last substring after the last ',' to the result
    result.push_back(temp);
    return result;
}

class cvsdata{
  string date;
  string country;
  int cases;
  int deaths;
public:
  cvsdata(string date, string country, int cases, int deaths){
    this->date = date;
    this->country = country;
    this->cases = cases;
    this->deaths = deaths;
  }

  //All MY GETTERS
    string getDate() {

    return(date);
  }

  string getCountry(){

    return(country);
  }

  int getC_cases(){

    return(cases);
  }

  int getC_deaths(){

    return(deaths);
  }
};

class DataEntry{
  private:
      string date;
      string country;
      int c_cases;
      int c_deaths;
  public:

      DataEntry() {}

      //Stores the given countries, date ,cases and deaths into data
      void store(cvsdata data){

        date = data.getDate();
        country = data.getCountry();
        c_cases = data.getC_cases();
        c_deaths = data.getC_deaths();

      }

      //GETTERS AND SETTERS
      string getDate() 
      {
        return date;
      }
      

      string getCountry() {
        return country;
      }

      int getC_cases(){

        return c_cases;
      }

      int getC_deaths(){

        return c_deaths;
      }

      void setDate(string str_date){

        date = str_date;
      }

      void setCountry(string str_country){
      
        country = str_country;
      }

      void setC_cases(int int_c_cases){

        c_cases = int_c_cases;
      }

      void setC_deaths(int int_c_deaths){

        c_deaths = int_c_deaths;
      }

};

class covidDB
{
private:
    //Table Size will be 17
    int tableSize = 17;
    vector<vector<DataEntry> > dataTable;

public:
    covidDB() {
      //Our Table will be a vector of vectors
      dataTable = vector<vector<DataEntry> >(tableSize);
    }


   int hash(string country){

      //Hashing function given by professor
      
      int sum = 0;

      //This changes the characters in the country to int and returns the sum of the country when converted to int
      for(int i = 0; i < country.length();i++){

        sum = (i* country[i]) % tableSize;
      }

      return sum;
   }


  //Parses our date and changes it into int, similar to split function
   vector<int> date_to_int(string date){
      vector<int> v;
      string str = "";

      for(int i = 0; i < date.length(); i++){
        
        
        if(date[i] != '/'){

          str+= date[i];
        } else {
          //When it encounters a backslash, stoi our current int date
          v.push_back(stoi(str));
          str = "";
        }
      }

    v.push_back(stoi(str));
    str = "";
    return v;
   }
   
   //Compares date 1 with date 2, return true if date 1 is bigger than date 2
   bool date_comparison(string date1, string date2){

      vector<int> v_date1 = date_to_int(date1);
      vector<int> v_date2 = date_to_int(date2);

      if(v_date1[2] < v_date2[2]){ //Checking year

        return false;
      }
      else if(v_date1[2] > v_date2[2]){ //checking year

        return true;
      } else {

          if(v_date1[1] < v_date2[1]){ //Checking month

            return false;
          } else {

            if(v_date1[0] < v_date2[0]){ //Checking day

              return false;
            }
            else{
              return true;
            }
          }
      }
   }

  //Check if the country is in our data table by hasing it and checking that particular index vector
   int check_country(string country){
        int index = hash(country);

        for(int i = 0 ; i < dataTable[index].size(); i++){

            if(dataTable[index][i].getCountry() == country) return i;

        }
      
      return -1;
   }

   bool add(DataEntry entry){
      //index will be where we will place our country
      int index = hash(entry.getCountry());
      int found = check_country(entry.getCountry());

      //if country is not found, we just push it into our vector
      if(found == -1){
            dataTable[index].push_back(entry);
      } else {

        //get the current date for that country
        string date = dataTable[index][found].getDate();
        //check if that country is more current, if it is replace and set it.
        if(date_comparison(entry.getDate(), date)) {
            dataTable[index][found].setDate(entry.getDate());
            dataTable[index][found].setC_cases(entry.getC_cases() + dataTable[index][found].getC_cases());
            dataTable[index][found].setC_deaths(entry.getC_deaths() + dataTable[index][found].getC_deaths());

        } else {

            return false;
        }
      }
      return true;
   }

    //This creates the table of our given CVS data
    void create_table(vector<cvsdata> list){

        for(int i = 0; i < list.size(); i++){
            DataEntry entry;
            entry.store(list[i]);
            add(entry);
        }
    }

    //This get function will get the data entry of that country
    DataEntry get(string country) {
			int index = hash(country);
			int found = check_country(country);
      //if country isn't in there
			if (found == -1) {
				DataEntry entry;
				entry.setC_cases(0);
				return entry;
        //if country is there, return the entry of that country
			} else {
				return dataTable[index][found];
			}
		}

    void remove(string country){

        int index = hash(country);
        int found = check_country(country);

        if(found == -1){

            cout << "Country does not exist in data table" << endl;
        } else {
            //If country exist in the dataTable, erase it
            dataTable[index].erase(dataTable[index].begin() + found);
            cout << country << " has successfully been deleted from our data table. " << endl;
        }
    }

    //Simple display function using get methods
    void display() {
        cout << "\nCovid Data Table" << endl;
        for (int i = 0; i < tableSize; i++) {
            cout << "\tTABLE ENTRY " << i << ": ";
            for (int j = 0; j < dataTable[i].size(); j++) {
                cout << dataTable[i][j].getDate() << ", ";
                cout << dataTable[i][j].getCountry() << ", ";
                cout << dataTable[i][j].getC_cases() << ", ";
                cout << dataTable[i][j].getC_deaths() << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

};

//UI function that will be the user input and user interface
void userInterface(vector<cvsdata> data){

    covidDB dataTable;
    bool ask = true;
    //This boolean table is used to check if the table has already been initalized or hasn't been
    bool table = false;

    while(ask){
        int input;
        cout << "Covid Tracker" << endl;
        cout << "1. Create the initial hash table" << endl;
        cout << "2. Add a new data entry" << endl;
        cout << "3. Get a data entry" << endl;
        cout << "4. Remove a data entry" << endl;
        cout << "5. Display hash table" << endl;
        cout << "0. Quit the system" << endl;
        cout << "Please choose the operation you want: ";

        cin >> input;
        if(input == 1){

            if(!table){

                dataTable.create_table(data);
                table = true;
                cout << "\n";
            } else {

                cout << "\nTable had already been created!\n" << endl;
            }
        }
        else if(input == 2){

            if(table){
                
                DataEntry new_entry;
                string date;
                string country;
                int death;
                int cases;

                //Inputting date, country, cases and deaths
                cout << "\nEnter the date in the format of (mm/dd/yyyy)->(please I beg you): ";
                cin >> date;
                new_entry.setDate(date);
                cout << "\nEnter country name: ";
                cin >> country;
                new_entry.setCountry(country);
                cout << "\nEnter your cases: ";
                cin >> cases;\
                new_entry.setC_cases(cases);
                cout << "\nEnter your deaths: ";
                cin >> death;
                new_entry.setC_deaths(death);

                if(dataTable.add(new_entry)){

                    cout << "\nYour entry has been added successfully.\n" << endl;
                } else {

                    cout << "\nYou may have written the date in the wrong format.\n" << endl;
                }

            } else {

                cout << "\nTable hasn't been intialized!\n" << endl;
            }
        } 
        else if(input == 3) {

            if (table) {
                string country;
                cout << "\nEnter the country name: ";
                cin >> country;
                DataEntry result = dataTable.get(country);

                if (result.getC_cases() == 0) {
                    cout << "Not found, there is no such entry\n" << endl;

                //Get the data of the entry
                } else {
                    cout << "Entry found: ";
                    cout << result.getDate() << ", ";
                    cout << result.getCountry() << ", ";
                    cout << result.getC_cases() << ", ";
                    cout << result.getC_deaths() << "\n" << endl;
                }
                
            } else {
                cout << "\nTable hasn't been initialized!\n" << endl;
            }
        }
        else if (input == 4) {	
            if (table) {
                string country;
                cout << "\nEnter the country name: ";
                cin >> country;
                //call remove on the country
                dataTable.remove(country);

            } else {
                cout << "\nTable hasn't been initialized\n" << endl;
            }
        }
        else if (input == 5) {		
            if (table) {
                dataTable.display();
            } else {
                cout << "\nTable hasn't been initialized\n" << endl;
            }
        
        } else {
            cout << "\nYou have exited the program!\n" << endl;
            ask = false;
        }
    }
}
int main()
{
  vector<cvsdata> recordList;

  // open the file
  //File may be corrupted
  ifstream file("WHO-COVID-Data.csv");

  if(file.is_open()){

    cout << "Open File Success" << endl;

    string line;

    while(getline(file, line)){
      
      // split the content in each line
      vector<string> result = split(line, ',');

      // wrap up all the data in the string to a cvsdata type and push it to the vector
      cvsdata oneEntry = cvsdata(result[0], result[1], std::stoi(result[2]), std::stoi(result[3]));
      recordList.push_back(oneEntry);
    }
    cout << "There are " << recordList.size() << " records in total." << endl;
    
  }else{
    cout << "Open file failed" << endl;
  }


  userInterface(recordList);
  return 0;
};