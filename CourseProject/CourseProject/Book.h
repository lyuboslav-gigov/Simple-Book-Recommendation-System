#pragma once
#include <string>
using namespace std;

enum Gender{male, female, other};

enum Context{school, vacation, home, work, gift};

enum PersonalInterests{sport, science, technology, cars, cooking, electronics};

enum Hobbies{gardening, running, cycling, mountaineering, acting};

struct SearchCriteria {
  int age;
  Gender g;
  Context c;
  PersonalInterests p;
  Hobbies h;
};

class Book {
public:
  Book(string n, string a, double p);

  virtual int Search(SearchCriteria s1) = 0; // main polymorphic function
  virtual void DisplayGenre() = 0; // Function to display the genre of a given book
private:
  string name;
  string author;
  double price;
};

class Fiction : public Book {};

class ScienceFiction : public Fiction {
public:
  virtual int Search(SearchCriteria s2);
  virtual void DisplayGenre();
};

class Fantasy : public Fiction {
public:
  virtual int Search(SearchCriteria s3);
  virtual void DisplayGenre();
};

class Romance : public Fiction {
public:
  virtual int Search(SearchCriteria s4);
  virtual void DisplayGenre();
};

class NonFiction : public Book {};

class Biographies : public NonFiction {
public:
  virtual int Search(SearchCriteria s5);
  virtual void DisplayGenre();
};

class Encyclopedias : public NonFiction {
public:
  virtual int Search(SearchCriteria s6);
  virtual void DisplayGenre();
};


  //Book(); // default constructor
  //Book(string n, string a, double p, Genre* bG);

//Book StringToBookGenre(string s); // F-n to convert from string to a certain book genre, will need this when reading from input file