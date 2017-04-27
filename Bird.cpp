#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//Bird Object that holds all the data
class Bird {
   public:

   //argument constructor
   Bird (string inname, int inday, int inmonth, int inyear, string incity, string incountry) {
       name = inname;
       day = inday;
       month = inmonth;
       year = inyear;
       city  = incity;
       country = incountry;
   }

   //default constructor
   Bird ( ) {

   }

   //displays content of the Bird Object
   void display () {
       cout << endl;
       cout << "--------------------------------" <<endl;
       cout << "Name: " << name << endl;
       cout << "Day: " << day << endl;
       cout << "Month: " << month << endl;
       cout << "Year: " << year << endl;
       cout << "City: " << city << endl;
       cout << "Country: " << country << endl;       
       cout << "--------------------------------" <<endl;
       cout << endl;
   }

   ~Bird () {
       name = "NULL";
       day = -1;
       month = -1;
       year = -1;
       city  = "NULL";
       country = "NULL";

   }


   //allows sorting algorithm to see the names it will sort
   string name;
   int day;
   int month;
   int year;
   string city;
   string country;

};


//lets the sorting algoritm sort by name
bool SortByBirdName ( const Bird &left, const Bird &right) {
   //for the third parameter for the std::sort algorithm

   //converts all letters in the "name" field to uppercase to sort/compare properly
   string leftUpper = (left.name);
   string rightUpper = (right.name);
   for (int a = 0; a < strlen(leftUpper.c_str()); a++){
      leftUpper[a] = toupper(leftUpper[a]);
   }
   for (int a = 0; a < strlen(rightUpper.c_str()); a++){
      rightUpper[a] = toupper(rightUpper[a]);
   }

   //actually compares the strings
   int compare = strcmp(leftUpper.c_str(), rightUpper.c_str());
   //returns true if the left side is first, false if right side is first
   if ( compare < 0 ) {
      return true;
   } else if ( compare >= 0) {
      return false;
   }  
}

//prints the poccessed data from a .csv file to JavaScript so that 
// JavaScript has a function that makes an array of Objects with data from the .csv file in them
void PrintJS ( vector<Bird>birdOut ) {

   //open up the file
   ofstream outfile;
   outfile.open ("birdData.js");

   //beginning object constuctor
   string beginText = 
     "function Bird ( Birdname , daySpot, monthSpot, yearSpot, citySpot, countrySpot ) { \n"
     "  this.Bname= Birdname; \n"
     "  this.day= daySpot; \n"
     "  this.month= monthSpot; \n"
     "  this.year= yearSpot; \n"
     "  this.city= citySpot; \n"
     "  this.country= countrySpot; \n"
     "} \n"
     "\n"
     "function getBirdData() { \n\n"
     "   var BirdArray = [ ]; \n";
    
    //sends the object constructor to the .js file
    outfile << beginText;
    outfile << endl;
   
    //Declares an iterator for which will access the vector that stores up all 
    //the bird objects 
    vector<Bird>::iterator getData;
    int count = 0;
    for ( getData = birdOut.begin(); getData != birdOut.end(); getData++){

       //sends all the objects one at a time to be made into a JS object
       outfile << "   BirdArray[ ";
       outfile << count;
       outfile << " ] = new Bird( \"";
       outfile << getData -> name;
       outfile << "\" , ";
       outfile << getData -> day;
       outfile << " , ";
       outfile << getData -> month;
       outfile << " , ";
       outfile << getData -> year;
       outfile << " , \"";
       outfile << getData -> city;
       outfile << "\" , \"";
       outfile << getData -> country;
       outfile << "\" ); \n";

       //iterates on what object number we are on
       count++;
   }

   
   //ending JS code
   string endText2 = "\n   return BirdArray; \n"; 
   string endText1 = "\n};"; 

   //sends the JS code to the JS file
   outfile << endText2;
   outfile << endText1;

   //closes the file
   outfile.close();
}

//takes input files from commandline arguments
int main (int argc, char* argv[]) {

    //vector that will hold content of all objects from all input files 
    vector<Bird> AllBirds;

    //for every file entered
    for ( int i = 1 ; i <= (argc-1) ; i++) {

        //Read the file info
        ifstream csvfile;
        csvfile.open (argv[i]);


        //warning if a file does not load
        if (csvfile.fail()) {
           cerr << "ERROR, NO INPUT" <<endl;
           return -1;
        }   

        //vector that holds all the objects for any 1 file
        vector<Bird> BirdVector;


        //declare 3 ints and 6 strings that will hold the fields of the object
        string dayhold;
        string monthhold;
        string yearhold;
        string birdhold;
        string cityhold;
        string countryhold;
        int intdayhold;
        int intmonthhold;
        int intyearhold;
   
     
        //reads the data for the fields and assigns them to the temporary variables
        //stops when no data   
        while ( csvfile.peek() != EOF ) {
           getline(csvfile,birdhold,',');
           getline(csvfile,dayhold,',');
           getline(csvfile,monthhold,',');
           getline(csvfile,yearhold,',');
           getline(csvfile,cityhold,',');
           getline(csvfile,countryhold);
 


           //convert the integer variables from a string type to an int type
           intdayhold = atoi(dayhold.c_str());
           intmonthhold = atoi(monthhold.c_str());
           intyearhold = atoi(yearhold.c_str());

           //made an instance of the object
           Bird birds (birdhold, intdayhold, intmonthhold, intyearhold, cityhold, countryhold);
 
           //pushed instance into a vector
           BirdVector.push_back( birds );

        }

        //uses the sort method in STL to sort the bird object's by their name
        //uses the SortBirdName function from earlier in the program
        sort (BirdVector.begin(),BirdVector.end(),SortByBirdName );

        if ( i == 1){
           //copying directly from 1st instance of BirdVector to AllBirds
           AllBirds = BirdVector;
           
        }else if ( i > 1 ){
 
           //holds content of merged AllBirds and BirdVector 
           vector<Bird> tempBirds;

           //merge AllBirds and current BirdVector into tempBirds
           merge ( BirdVector.begin(), BirdVector.end(), AllBirds.begin(), AllBirds.end(), back_inserter(tempBirds), SortByBirdName);

           //deleting all of AllBirds
           AllBirds.clear();          
 
           //copying from tempbirds to AllBirds
           AllBirds = tempBirds;           

        }


    }

    /////////////////////JavaScriptConnection/////////////////////////

    //prints all the objects from c++ to JavaScript
    PrintJS ( AllBirds );

    //////////////////////////////////////////////////////////////////

}


