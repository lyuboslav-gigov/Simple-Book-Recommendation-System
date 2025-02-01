#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <iomanip>
using namespace std;

enum Gender { male, female, other, invalidGender};

enum Context { school, vacation, home, work, gift, invalidContext};

enum PersonalInterests { sport, science, technology, celebrities, history, mythology, invalidInterest};

struct SearchCriteria {
  int age;
  double maxPrice; // Maximum price the user is willing to pay for a book
  Gender g;
  Context c;
  PersonalInterests p;
} preferences;

class Book {
public:
  virtual int Search(SearchCriteria s1) = 0; // main polymorphic function
  virtual string GetGenre() = 0; // Function to display the genre of a given book

  // Getters and setters
  string GetName() { return name; }
  string GetAuthor() { return author; }
  double GetPrice() { return price; }
  int GetGrade() { return grade; }
  void SetGrade(int g) { assert(0 <= g && g <= 100); grade = g; }
protected:
  string name;
  string author;
  double price;
  int grade; // 0 - 100
};

class Fiction : public Book {};

class ScienceFiction : public Fiction {
private:
  int scientificAccuracy; // 0-100 based on how scientifically accurate the book is
  int actionLevel; // 0-100 based on how much action takes place in the book
public:
  ScienceFiction(string n, string a, double p, int sciAcc, int actionLvl) {
    name = n;
    author = a;
    assert(p > 0); assert(0 <= sciAcc && sciAcc <= 100); assert(0 <= actionLvl && actionLvl <= 100); // Enforcing the axioms
    price = p;
    scientificAccuracy = sciAcc;
    actionLevel = actionLvl;
  }
  virtual int Search(SearchCriteria s2) {
    if (price > s2.maxPrice) { // if book is too expensive, it immediately receives a zero
      return 0;
    }

    double weightAge = 0.1, weightGender = 0.1, weightContext = 0.2, weightInterests = 0.4, weightAccuracy = 0.1, weightAction = 0.1;
    int ageR, genderR, contextR, interestR; // age relevance, gender relevance, etc.
    int grade;
    // These variables will be used to calculate the grade. grade = weightAge*ageR + weightGender*genderR + ...

    // A child that is no more than 12 years old and doesn't have any interest in science or technology would not enjoy sci-fi books
    if (s2.age < 13 && s2.p != science && s2.p != technology) {
      return 0;
    }

    // A person above the age of 13 with an interest in science/technology who is searching for a book to read at home/on vacation
    // is very likely to enjoy sci-fi books
    if (s2.age > 13 && (s2.p == science || s2.p == technology) && (s2.c == home || s2.c == vacation)) {
      return 80 + weightAccuracy * scientificAccuracy + weightAction * actionLevel;
    }

    // Calculating age relevance
    if (s2.age < 13) { ageR = 0; }
    else if (13 <= s2.age && s2.age <= 25) { ageR = 100; }
    else if (25 < s2.age && s2.age <= 40) { ageR = 60; }
    else { ageR = 50; }

    // Calculating gender relevance
    if (s2.g == male) { genderR = 60; }
    else if (s2.g == female) { genderR = 40; }
    else { genderR = 50; }

    //Calculating context relevance
    switch (s2.c) {
    case school:
      contextR = 0;
      break;
    case work:
      contextR = 0;
      break;
    case home:
      contextR = 100;
      break;
    case vacation:
      contextR = 80;
      break;
    case gift:
      contextR = 50;
      break;
      // no default case needed since context is guaranteed to match one of these cases by stringToEnumC()
    }

    //Calculating interest relevance
    switch (s2.p) {
    case sport:
      interestR = 0;
      break;
    case celebrities:
      interestR = 0;
      break;
    case history:
      interestR = 20;
      break;
    case technology:
      interestR = 80;
      break;
    case science:
      interestR = 100;
      break;
    case mythology:
      interestR = 20;
      break;
    } // no default case needed as above

    grade = weightAge * ageR + weightGender * genderR + weightContext * contextR + weightInterests * interestR + weightAccuracy * scientificAccuracy + weightAction * actionLevel;

    return grade;
  }
  virtual string GetGenre() {
    string gnr = "Science fiction";
    return gnr;
  }
};

class Fantasy : public Fiction {
private:
  int characterComplexity; // characterizes how well characters (heroes) are developed - are they relatable, one-dimensional, morally good, etc.
  int settingRichness; // the setting in fantasy novels plays a huge role. This variable encapsulates how well the fantasy world is built - magic system, variety of non-human creatures, etc.
public:
  Fantasy(string n, string a, double p, int charComp, int settRich) {
    name = n;
    author = a;
    assert(p > 0); assert(0 <= charComp && charComp <= 100); assert(0 <= settRich && settRich <= 100);
    price = p;
    characterComplexity = charComp;
    settingRichness = settRich;
  }
  virtual int Search(SearchCriteria s3) {
    if (price > s3.maxPrice) { return 0; }

    double weightAge = 0.2, weightGender = 0.1, weightContext = 0.1, weightInterests = 0.2, weightCharacter = 0.2, weightSetting = 0.2;
    int ageR, genderR, contextR, interestR; // age relevance, gender relevance, etc.
    int grade;

    if (s3.age <= 18 && s3.g == male && s3.c == home && s3.p == mythology) { // A person with these characteristics is very likely to enjoy fantasy stories.
      weightCharacter = 0.3; weightSetting = 0.3;            // Their enjoyment depends mostly on how well the book is written.
      grade = 40 + weightCharacter * characterComplexity + weightSetting * settingRichness;
      return grade;
    }

    // Such a person is highly unlikely to enjoy the fantasy genre
    if (s3.age > 18 && s3.g != male && (s3.c == work || s3.c == school) && s3.p != mythology) { return 0; }

    // Calculating age relevance
    if (s3.age <= 18) { ageR = 100; }
    else { ageR = 50; } // Younger people are more likely to read fantasy

    // Predominantly men read fantasy
    if (s3.g == male) { genderR = 100; }
    else { genderR = 50; }

    // Context relevance
    if (s3.c == school || s3.c == work) { contextR = 0; }
    else if (s3.c == vacation) { contextR = 40; }
    else if (s3.c == home) { contextR = 100; }
    else { contextR = 60; }

    // Interests Relevance
    if (s3.p == sport || s3.p == celebrities || s3.p == history) { interestR = 0; }
    else if (s3.p == science || s3.p == technology) { interestR = 40; }
    else { interestR = 100; }

    grade = weightAge * ageR + weightGender * genderR + weightContext * contextR + weightInterests * interestR + weightCharacter * characterComplexity
      + weightSetting * settingRichness;

    return grade;
  }
  virtual string GetGenre() {
    string gnr = "Fantasy";
    return gnr;
  }
};

class Romance : public Fiction {
private:
  int emotionalDepth; // characterizes how deep the romance between characters is, i.e. is their love superficial, or is it true love
  int dramaLevel; // how much drama there is in a romantic novel. The drama is the main thing that makes romantic novels interesting
public:
  Romance(string n, string a, double p, int eD, int dL) {
    name = n;
    author = a;
    assert(p > 0); assert(0 <= eD && eD <= 100); assert(0 <= dL && dL <= 100);
    price = p;
    emotionalDepth = eD;
    dramaLevel = dL;
  }

  virtual int Search(SearchCriteria s4) { 
    if (price > s4.maxPrice) { return 0; }

    // Romance books are not suitable for people younger than 16
    if (s4.age < 16) { return 0; }

    double weightAge = 0.2, weightGender = 0.2, weightContext = 0.2, weightInterests = 0.2, weightDepth = 0.1, weightDrama = 0.1;
    int ageR, genderR, contextR, interestR; // age relevance, gender relevance, etc.
    int grade;

    // Mostly women between the ages of 18-54 read romance
    if (18 <= s4.age && s4.age <= 54 && s4.g == female && (s4.c == home || s4.c == vacation)) {
      grade = 70 + weightDepth * emotionalDepth + weightDrama * dramaLevel;
      return grade;
    }

    // Romance books are a popular option for relatively cheap gifts
    if (s4.g == female && s4.c == gift && s4.maxPrice <= 30.00) {
      weightDepth = 0.1; weightDrama = 0.1;
      grade = 80 + weightDepth * emotionalDepth + weightDrama * dramaLevel;
      return grade;
    }

    // Only 18% of romance readers are men. Further, if the book isn't exceptionally well-written, then it wouldn't be of interest to a man.
    if (s4.g == male && (emotionalDepth < 80 || dramaLevel < 80)) {
      return 0;
    }

    // Calculating age relevance
    if (16 <= s4.age && s4.age <= 55) { ageR = 100; } // most readers of romance fall into this range
    else { ageR = 50; }

    // Gender relevance
    if (s4.g == female) { genderR = 100; }
    else { genderR = 30; }

    // Context relevance
    switch (s4.c) {
    case school:
      contextR = 0;
      break;
    case vacation:
      contextR = 100;
      break;
    case home:
      contextR = 100;
      break;
    case work:
      contextR = 0;
      break;
    case gift:
      contextR = 80;
      break;
    } // no default case needed as user is forced to type one of these contexts

    // Interest relevance
    if (s4.p == celebrities) { interestR = 100; }
    else if (s4.p == sport) { interestR = 40; }
    else { interestR = 0; }

    grade = weightAge * ageR + weightGender * genderR + weightContext * contextR + weightInterests * interestR + weightDepth * emotionalDepth + weightDrama * dramaLevel;

    return grade;
  }

  virtual string GetGenre() {
    string gnr = "Romance";
    return gnr;
  }
};

class NonFiction : public Book {};

class Biography : public NonFiction {
private:
  int worldImpact; // Has the subject of the biography significantly impacted the world in a positive way, or are their successes purely individual?
  int lessonsLearnt; // Characterizes how useful the biography was. Can readers learn something useful they can apply in their own lives?
public:
  Biography(string n, string a, double p, int wI, int lL) {
    name = n;
    author = a;
    assert(p > 0); assert(0 <= wI && wI <= 100); assert(0 <= lL && lL <= 100);
    price = p;
    worldImpact = wI;
    lessonsLearnt = lL;
  }
  virtual int Search(SearchCriteria s5) {
    double weightImpact, weightLessons;
    int grade;

    if (price > s5.maxPrice) { return 0; }

    // Biographies are not suitable for children
    if (s5.age < 15) { return 0; }

    // Biographies don't have anything related to mythology in them
    if (s5.p == mythology) { return 0; }

    // A biography of a famous sportsperson, such as Cristiano Ronaldo or Lionel Messi, is a very suitable gift for a man interested in sport
    if (s5.g == male && s5.p == sport && s5.c == gift) {
      if ((name.find("Messi") != string::npos) || (name.find("Ronaldo") != string::npos)) {
        return 100;
      }
    }

    // Women are often interested in biographies of celebrities
    if (s5.p == celebrities && s5.g == female) {
      weightImpact = 0.1; weightLessons = 0.1;
      grade = 80 + weightImpact * worldImpact + weightLessons * lessonsLearnt;
      return grade;
    }

    if (s5.p == sport || s5.p == celebrities) {
      weightImpact = 0.2; weightLessons = 0.1;
      grade = 60 + weightImpact * worldImpact + weightLessons * lessonsLearnt;
      return grade;
    }
    else { // Our bookstore offers only biographies of sportspeople and celebrities, so people not into sport/celebrities are unlikely to enjoy them
      weightImpact = 0.2; weightLessons = 0.2;
      grade = 20 + weightImpact * worldImpact + weightLessons * lessonsLearnt;
      return grade;
    }
  }
  virtual string GetGenre() {
    string gnr = "Biography";
    return gnr;
  }
};

class Encyclopedia : public NonFiction {
private:
  int factualAccuracy; // Characterizes the accuracy of the information in the encyclopedia
  int depthnessInformation; // How in-depth the information is
public:
  Encyclopedia(string n, string a, double p, int fA, int dI) {
    name = n;
    author = a;
    assert(p > 0); assert(0 <= fA && fA <= 100); assert(0 <= dI && dI <= 100);
    price = p;
    factualAccuracy = fA;
    depthnessInformation = dI;
  }
  virtual int Search(SearchCriteria s6) {
    double weightFactual, weightDepthness;
    int grade;

    if (price > s6.maxPrice) { return 0; }

    // Children younger than that wouldn't be able to comprehend the information presented in encyclopedias
    if (s6.age < 16) { return 0; }

    // Checks if the user wishes to find a historical book to use as reference for school/work
    if (s6.p == history && (s6.c == school || s6.c == work)) {
      if (name.find("History") != string::npos) {
        return 100;
      }
    }

     //Casual readers with an interest in history
    if (s6.p == history && (s6.c == home || s6.c == gift)) {
      if (depthnessInformation > 70) { // In-depth encyclopedias are not suitable for casual readers
        return 0;
      }
      else {
        if (0 <= factualAccuracy && factualAccuracy <= 50) { return 85; }
        else if (50 < factualAccuracy && factualAccuracy <= 70) { return 95; }
        else { return 100; }
      }
    }

    // Checks if the user wishes to find a scientific book to use as reference for school/work
    if (s6.p == science && (s6.c == school || s6.c == work)) {
      if (name.find("Science") != string::npos) {
        return 100;
      }
    }

    if (s6.c == school || s6.c == work) {
      weightFactual = 0.2; weightDepthness = 0.1;
      grade = 60 + weightFactual * factualAccuracy + weightDepthness * depthnessInformation;
      return grade;
    }
    else {    // Encyclopedias are only appropriate for school/work
      return 0;
    }
  }
  virtual string GetGenre() {
    string gnr = "Encyclopedia";
    return gnr;
  }
};

void readInput(vector<Book*>& b) {
  ifstream inputFile;
  inputFile.open("Bookstore_Inventory.txt");
  if (inputFile.fail()) {
    cout << "Error opening file. Please check file name and try again." << endl;
    abort();
  }

  // This loop's purpose is to ignore (discard) the introductory lines of the input file
  for (int i = 0; i < 5; i++) {
    string line;
    getline(inputFile, line);
  }

  while (!inputFile.eof()) {
    string name, author, genre;
    char c; // used to store '|' character so that numerical values are read properly
    int genreCharacteristic1, genreCharacteristic2;
    double price;
    Book* book;

    getline(inputFile, name, '|');
    getline(inputFile, author, '|');
    getline(inputFile, genre, '|');
    inputFile >> genreCharacteristic1 >> c >> genreCharacteristic2 >> c >> price;

    // ignores all chars up to and including nwln so that next line is read properly
    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');

    if (genre == "Science fiction") {
      book = new ScienceFiction(name, author, price, genreCharacteristic1, genreCharacteristic2);
    }
    else if (genre == "Fantasy") {
      book = new Fantasy(name, author, price, genreCharacteristic1, genreCharacteristic2);
    }
    else if (genre == "Romance") {
      book = new Romance(name, author, price, genreCharacteristic1, genreCharacteristic2);
    }
    else if (genre == "Biography") {
      book = new Biography(name, author, price, genreCharacteristic1, genreCharacteristic2);
    }
    else if (genre == "Encyclopedia") {
      book = new Encyclopedia(name, author, price, genreCharacteristic1, genreCharacteristic2);
    }
    else{
      cout << "Invalid genre, please check the spelling in the input file and try again.";
      abort();
    }

    b.push_back(book);
  }

  inputFile.close();
}

// Converts string input by user to enum gender
Gender stringToEnumG(string s) {
  if (s == "Male" || s == "male") {
    return male;
  }
  else if (s == "Female" || s == "female") {
    return female;
  }
  else if (s == "Other" || s == "other") {
    return other;
  }
  else {
    return invalidGender;
  }
}

// Converts user input into context enum
Context stringToEnumC(string s1) {
  if (s1 == "School" || s1 == "school") {
    return school;
  }
  else if (s1 == "Vacation" || s1 == "vacation") {
    return vacation;
  }
  else if (s1 == "Home" || s1 == "home") {
    return home;
  }
  else if (s1 == "Work" || s1 == "work") {
    return work;
  }
  else if (s1 == "Gift" || s1 == "gift") {
    return gift;
  }
  else
    return invalidContext;
}

// Converts from user-entered string to enum
PersonalInterests stringToEnumPI(string s2) {
  if (s2 == "Sport" || s2 == "sport") {
    return sport;
  }
  else if (s2 == "Science" || s2 == "science") {
    return science;
  }
  else if (s2 == "Technology" || s2 == "technology") {
    return technology;
  }
  else if (s2 == "Celebrities" || s2 == "celebrities") {
    return celebrities;
  }
  else if (s2 == "History" || s2 == "history") {
    return history;
  }
  else if (s2 == "Mythology" || s2 == "mythology") {
    return mythology;
  }
  else
    return invalidInterest;
}

bool isInteger(string num) {
  for (int i = 0; i < num.size(); i++) {
    if (!isdigit(num[i])) {
      return false;
    }
  }

  return true;
}

void getUserPreferences() {
  int a; // age of the user
  double mP; // max price
  Gender gndr;
  Context ctxt;
  PersonalInterests pI;
  string tmpAge,tmpMP, tmp1, tmp2, tmp3;

  cout << "Greetings! This program can help you find the most suitable book for your needs." << endl;
  cout << "Please, enter the required data in the given order." << endl;
  cout << "Age: ";
  cin >> tmpAge;
  // Checking for valid integer input
  while (!isInteger(tmpAge)) {
    cout << "Please enter a valid number for your age: ";
    cin >> tmpAge;
  }

  do {
    cout << "Maximum price you are willing to pay (in US $): ";
    cin >> tmpMP;

    // Check if the input contains only digits and at most one decimal point to ensure it's a double
    bool isValidInput = true;
    bool hasDecimalPoint = false;

    for (char c : tmpMP) {
      if (!isdigit(c)) {
        if (c == '.' && !hasDecimalPoint) {
          hasDecimalPoint = true;
        }
        else {
          isValidInput = false;
          break;
        }
      }
    }

    if (isValidInput) {
      mP = stod(tmpMP);
      break;
    }
    else {
      cout << "Invalid price entered.\n";
    }
  } while (true);

  do {
    cout << "Gender (male/Male, female/Female, other/Other): ";
    cin >> tmp1;
    gndr = stringToEnumG(tmp1);
  } while (gndr == invalidGender);

  do {
    cout << "Context for buying the book ((s/S)chool, (v/V)acation, (h/H)ome, (w/W)ork, (g/G)ift): ";
    cin >> tmp2;
    ctxt = stringToEnumC(tmp2);
  } while (ctxt == invalidContext);

  do {
    cout << "Your personal interests ((s/S)port, (s/S)cience, (t/T)echnology, (c/C)elebrities, (h/H)istory, (m/M)ythology): ";
    cin >> tmp3;
    pI = stringToEnumPI(tmp3);
  } while (pI == invalidInterest);

  a = stoi(tmpAge); // converting from string to int

  // Storing user's preferences which will be used by Search() function
  preferences.age = a;
  preferences.maxPrice = mP;
  preferences.g = gndr;
  preferences.c = ctxt;
  preferences.p = pI;
}

// Comparison function for sorting the Books vector by book grades (in descending order)
bool compareGrades(Book* b1, Book* b2) {
  return b1->GetGrade() > b2->GetGrade();
}

void sortByGrades(vector<Book*>& boo) {
  sort(boo.begin(), boo.end(), compareGrades);
}

void displayTopFive(vector<Book*>& bo) {
  cout << "\nThe five most suitable books for you are:\n";
  cout << "----------------------------------------------------------------------------------------------------------------------\n";
  cout << "No.\tName\t\t\t\tAuthor\t\t\tGenre\t\t\tPrice\t\tGrade\n";
  cout << "----------------------------------------------------------------------------------------------------------------------\n";
  for (int i = 0; i < 5; i++) {
    cout << i + 1 << ")\t" << setw(25) << left << bo[i]->GetName() << "\t" << setw(20) << left << bo[i]->GetAuthor()
      << "\t" << setw(15) << left << bo[i]->GetGenre() << "\t" << setw(13) << right << fixed << setprecision(2) << bo[i]->GetPrice() << "$\t";
    // Fixing alignment issue if grade is 100
    int grade = bo[i]->GetGrade();
    if (grade == 100) { cout << " "; }
    cout << setw(10) << right << grade << "pts./100pts.\n";
  }
}

void gradeBooks(vector<Book*>& b) {
  for (int i = 0; i < b.size(); i++) {
    int grade = b[i]->Search(preferences);
    b[i]->SetGrade(grade);
  }
}

int main()
{
  vector<Book*> books;
  getUserPreferences();

  // Reads from input file and stores data into books vector
  readInput(books);

  // Grading all books according to user preferences
  gradeBooks(books);

  // Sorting all books by their respective grades using IntroSort algorithm
  sortByGrades(books);

  // Displaying the best five books to the user
  displayTopFive(books);

  // Deallocating memory
  for (int i = 0; i < books.size(); i++) {
    delete books[i];
  }
  return 0;
}