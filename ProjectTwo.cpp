//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jeremy Pauley
// Version     : 1.0
// Copyright   : Copyright 2026 Jeremy Pauley
// Description : This C++ program provides ABCU academic advisors with a menu-driven course planning tool that uses a hash table
//               to load and validate course data, display courses in alphanumeric order, and retrieve individual course information,
//               including titles and prerequisites.
//============================================================================


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;


// ============================================================
// 1. COURSE DATA STRUCTURE
// ============================================================
struct Course {
    string courseNumber;           // Unique course identifier
    string name;                   // Course title
    vector<string> prerequisites;  // Prerequisite course numbers
};


// ============================================================
// 2. HASH TABLE DATA STRUCTURE
// ============================================================
// Stores Course objects using their course numbers as keys.
// Uses separate chaining to handle hash collisions.

class CourseHashTable {
private:

    // Number of buckets available in the hash table.
    static const unsigned int DEFAULT_BUCKET_COUNT = 179;

    // Each bucket holds a list of Course objects.
    vector<vector<Course>> buckets;

    // Tracks the total number of courses stored.
    unsigned int courseCount = 0;

    // Calculate the bucket index for a course number.
    unsigned int hash(const string& courseNumber) const {
        unsigned int hashValue = 0;

        // Process each character in the course number.
        for (char character : courseNumber) {
            hashValue = (hashValue * 31
                         + static_cast<unsigned char>(character))
                         % DEFAULT_BUCKET_COUNT;
        }

        return hashValue;
    }

public:

    // Create the hash table with the default number of buckets.
    CourseHashTable() {
        buckets.resize(DEFAULT_BUCKET_COUNT);
    }

    // FOR DebugTest Function Only: Expose the bucket calculation for testing.
    unsigned int testHash(const string& courseNumber) const {
        return hash(courseNumber);
    }

    // Insert a Course into the appropriate bucket.
    bool insert(const Course& course) {
        unsigned int bucketIndex = hash(course.courseNumber);

        // A collision is allowed; an identical course number is not.
        for (const Course& existingCourse : buckets[bucketIndex]) {

        	// Return false when the course number is already present.
            if (existingCourse.courseNumber == course.courseNumber) {
                return false;
            }
        }

        buckets[bucketIndex].push_back(course);
        ++courseCount;
        return true;
    }

    // Search for a Course using its course number.
    const Course* search(const string& courseNumber) const {
        unsigned int bucketIndex = hash(courseNumber);

        for (const Course& storedCourse : buckets[bucketIndex]) {
            if (storedCourse.courseNumber == courseNumber) {
                return &storedCourse;
            }
        }
        return nullptr;
    }

    // Check whether a course number exists.
	bool contains(const string& courseNumber) const {

		// Search for the course and check whether it was found.
		return search(courseNumber) != nullptr;
	}


	// Retrieve all Course objects stored in the hash table.
	vector<Course> getAllCourses() const {

		vector<Course> allCourses;

		// Visit every bucket in the hash table.
		for (const vector<Course>& bucket : buckets) {

			// Add each stored course to the result.
			for (const Course& course : bucket) {
				allCourses.push_back(course);
			}
		}

		return allCourses;
	}

    // Clear the hash table.
	void clear() {
	    for (vector<Course>& bucket : buckets) {
	        bucket.clear();
	    }

	    courseCount = 0;
	}

    // Return the number of stored courses.
	unsigned int size() const {
	    return courseCount;
	}

    // Check whether the hash table is empty.
	bool empty() const {
	    return courseCount == 0;
	}
};


// ============================================================
// 3. HELPER FUNCTIONS
// ============================================================

// FUNCTION trim(String text) RETURNS String
string trim(string text) {

    // FIND the first character that is not whitespace
    size_t first = text.find_first_not_of(" \t\n\r\f\v");

    // IF the text contains only whitespace, RETURN an empty string
    if (first == string::npos) {
        return "";
    }

    // FIND the last character that is not whitespace
    size_t last = text.find_last_not_of(" \t\n\r\f\v");

    // RETURN the text between the first and last characters
    return text.substr(first, last - first + 1);
}



// FUNCTION splitOnComma(String line) RETURNS Vector<String>
vector<string> splitOnComma(string line) {

    // CREATE an empty list of tokens
    vector<string> tokens;

    // CREATE an empty current field
    string currentField;

    // FOR each character in the line
    for (char character : line) {

        // IF the character is a comma THEN
        if (character == ',') {

            // ADD the trimmed current field to the tokens
            tokens.push_back(trim(currentField));

            // CLEAR the current field
            currentField.clear();

        // ELSE
        } else {

            // ADD the character to the current field
            currentField += character;
        }
    }

    // ADD the trimmed final field to the tokens
    tokens.push_back(trim(currentField));

    // RETURN tokens
    return tokens;
}


// ============================================================
// 4. CREATE COURSE OBJECTS
// ============================================================
// FUNCTION parseLine(Vector<String> tokens) RETURNS Course
Course parseLine(const vector<string>& tokens) {

    // CREATE an empty Course object
    Course course;

    // IF fewer than two fields exist, RETURN the empty Course
    if (tokens.size() < 2) {
        return course;
    }

    // SAVE the first field as the course number
    course.courseNumber = trim(tokens[0]);

    // SAVE the second field as the course title
    course.name = trim(tokens[1]);

    // FOR each remaining field
    for (size_t i = 2; i < tokens.size(); ++i) {

        // REMOVE surrounding whitespace from the prerequisite
        string prerequisite = trim(tokens[i]);

        // IF the prerequisite is not empty THEN
        if (!prerequisite.empty()) {

        	// ADD the prerequisite to the Course's prerequisite list
            course.prerequisites.push_back(prerequisite);
        }
    }
    // RETURN the completed Course
    return course;
}


// ============================================================
// 5. PREREQUISITE VALIDATION
// ============================================================
// FUNCTION validatePrerequisites(HashTable courseTable) RETURNS Boolean
bool validatePrerequisites(const CourseHashTable& courseTable) {

	// ASSUME all prerequisites are valid
    bool valid = true;

    // FOR each Course stored in the hash table
    for (const Course& course : courseTable.getAllCourses()) {

        // FOR each prerequisite belonging to that Course
        for (const string& prerequisite : course.prerequisites) {

        	// IF the Course lists itself as a prerequisite THEN
            if (prerequisite == course.courseNumber) {

                // PRINT an error identifying the self-reference
                cout << "Invalid prerequisite: " << course.courseNumber
                     << " cannot require itself." << endl;

                // MARK the prerequisite data as invalid
                valid = false;
            }

            // ELSE IF the prerequisite does not exist in the hash table THEN
            else if (!courseTable.contains(prerequisite)) {

                // PRINT an error identifying the missing prerequisite
                cout << "Missing prerequisite: " << prerequisite
                     << " required by " << course.courseNumber << "." << endl;

                // MARK the prerequisite data as invalid
                valid = false;
            }
        }
    }
    // RETURN the overall validation result
    return valid;
}


// ============================================================
// 6. LOAD COURSE DATA INTO THE HASH TABLE
// ============================================================
// FUNCTION loadCourses(String filePath, HashTable courseTable) RETURNS Boolean
bool loadCourses(const string& filePath, CourseHashTable& courseTable) {

	// OPEN the course data file for reading
    ifstream inputFile(filePath);

    // IF the file cannot be opened THEN
    if (!inputFile.is_open()) {

        // PRINT an error identifying the file
        cout << "Error: Could not open file: " << filePath << endl;

        // RETURN false without changing existing course data
        return false;
    }

    // CREATE a temporary hash table for the incoming courses
    CourseHashTable newCourses;

    // PREPARE to read each line and track its line number
    string line;
    unsigned int lineNumber = 0;

    // WHILE another line can be read
    while (getline(inputFile, line)) {
        ++lineNumber;

        // IF the line contains only whitespace THEN
        if (trim(line).empty()) {

            // CONTINUE to the next line
            continue;
        }

        // SPLIT the line into comma-separated fields
        vector<string> fields = splitOnComma(line);

        // IF the course number or title is missing THEN
        if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {

            // PRINT the invalid record and its line number
            cout << "Invalid course record on line " << lineNumber
                 << ": " << line << endl;

            // RETURN false without changing existing course data
            return false;
        }

        // CREATE a Course from the fields
        Course course = parseLine(fields);

        // ATTEMPT to insert the Course into the temporary hash table
        if (!newCourses.insert(course)) {

            // PRINT an error when a duplicate course number is found
            cout << "Duplicate course number on line " << lineNumber
                 << ": " << course.courseNumber << endl;

            // RETURN false without changing existing course data
            return false;
        }
    }

    // IF no courses were loaded THEN
    if (newCourses.empty()) {

        // PRINT an error message
        cout << "Error: The course data file contains no courses." << endl;

        // RETURN false
        return false;
    }

    // IF prerequisite validation fails THEN
    if (!validatePrerequisites(newCourses)) {

        // PRINT an error and RETURN false without replacing existing data
        cout << "Error: Course prerequisites are invalid." << endl;
        return false;
    }

    // REPLACE the existing course table only after successful validation
    courseTable = newCourses;

    // PRINT the number of courses loaded successfully
    cout << "Successfully loaded " << courseTable.size() << " courses." << endl;

    // RETURN true to indicate a successful load
    return true;
}


// ============================================================
// 7. SEARCH FOR ONE COURSE AND PRINT ITS PREREQUISITES
// ============================================================
// FUNCTION searchCourse(HashTable courseTable, String courseNumber)
void searchCourse(const CourseHashTable& courseTable, string courseNumber) {

    // REMOVE surrounding whitespace from the input
    courseNumber = trim(courseNumber);

    // CONVERT the course number to uppercase
    for (char& character : courseNumber) {
        character = static_cast<char>(toupper(static_cast<unsigned char>(character)));
    }

    // SEARCH the hash table for the requested course
    const Course* foundCourse = courseTable.search(courseNumber);

    // IF the course was not found THEN
    if (foundCourse == nullptr) {

        // PRINT that the course was not found
        cout << "Course not found: " << courseNumber << endl;

        // RETURN to the calling function
        return;
    }

    // PRINT the course number and title
    cout << foundCourse->courseNumber << ", " << foundCourse->name << endl;

    // IF the Course has no prerequisites THEN
    if (foundCourse->prerequisites.empty()) {

        // PRINT "Prerequisites: none"
        cout << "Prerequisites: none" << endl;

    // ELSE
    } else {
        // PRINT "Prerequisites:"
        cout << "Prerequisites:" << endl;

        // FOR each prerequisite course number
        for (const string& prerequisiteNumber : foundCourse->prerequisites) {

            // FIND the prerequisite Course using its key
            const Course* prerequisite = courseTable.search(prerequisiteNumber);

            // IF the prerequisite is found THEN
            if (prerequisite != nullptr) {

                // PRINT the prerequisite number and title
                cout << "   " << prerequisite->courseNumber << ", "
                     << prerequisite->name << endl;

            // ELSE
            } else {

                // PRINT the missing prerequisite number
                cout << "   " << prerequisiteNumber << " (not found)" << endl;
            }
        }
    }
}


// ============================================================
// 8. PRINT COMPLETE COURSE LIST IN ALPHANUMERIC ORDER
// ============================================================
// FUNCTION printCourseList(HashTable courseTable)
void printCourseList(const CourseHashTable& courseTable) {

    // CREATE a temporary list containing all stored courses
    vector<Course> sortedCourses = courseTable.getAllCourses();

    // IF no courses are stored, PRINT a message and RETURN
    if (sortedCourses.empty()) {
        cout << "No courses available to display." << endl;
        return;
    }

    // SORT the temporary list by course number, smallest to largest
    sort(sortedCourses.begin(), sortedCourses.end(),
         [](const Course& first, const Course& second) {
             return first.courseNumber < second.courseNumber;
         });

    // FOR each Course in the sorted temporary list
    for (const Course& course : sortedCourses) {
        // PRINT the course number and title
        cout << course.courseNumber << ", " << course.name << endl;
    }
}

// ============================================================
// 9. MAIN MENU AND PROGRAM FLOW
// ============================================================
// FUNCTION runAdvisingProgram()
void runAdvisingProgram() {
    // CREATE an empty hash table to hold the active courses
    CourseHashTable courseTable;
    string input;

    // REPEAT until the user chooses to exit
    while (true) {
        // DISPLAY the advising assistance menu
        cout << "\n========== ABCU ADVISING ASSISTANCE ==========\n";
        cout << "1. Load the file data into the data structure\n";
        cout << "2. Print an alphanumeric list of all the courses in the Computer Science department\n";
        cout << "3. Print the course title and the prerequisites for any individual course\n";
        cout << "9. Exit the program\n";
        cout << "Select an option: ";

        // READ the user's menu selection
        if (!getline(cin, input)) {
            break;
        }

        // IF the user chooses to exit THEN
        if (input == "9") {
            // END the menu loop
            break;
        }

        // IF the user chooses to load course data THEN
        if (input == "1") {
            // ASK for the name or path of the course file
            cout << "Enter the course data file name: ";
            string filePath;
            if (!getline(cin, filePath)) {
                break;
            }

            // REMOVE extra whitespace from the file name
            filePath = trim(filePath);

            // IF no file name was entered THEN
            if (filePath.empty()) {
                // DISPLAY an error message
                cout << "Error: File name cannot be empty." << endl;
                continue;
            }

            // LOAD the file into the hash table
            loadCourses(filePath, courseTable);
        }
        // ELSE IF the user chooses to print the complete list THEN
        else if (input == "2") {
            // DISPLAY a message if no courses have been loaded
            if (courseTable.empty()) {
                cout << "Please load course data first." << endl;
            }
            // OTHERWISE print the courses in alphanumeric order
            else {
                printCourseList(courseTable);
            }
        }
        // ELSE IF the user chooses to print one course THEN
        else if (input == "3") {
            // DISPLAY a message if no courses have been loaded
            if (courseTable.empty()) {
                cout << "Please load course data first." << endl;
            }
            else {
                // ASK for the course number
                cout << "Enter the course number: ";
                string courseNumber;
                if (!getline(cin, courseNumber)) {
                    break;
                }

                // SEARCH for the course and print its information
                searchCourse(courseTable, courseNumber);
            }
        }
        // OTHERWISE the menu selection is invalid
        else {
            // DISPLAY the valid menu options
            cout << "Invalid selection. Enter 1, 2, 3, or 9." << endl;
        }
    }

    // DISPLAY the exit message
    cout << "Thank you for using ABCU Advising Assistance." << endl;
}


// ============================================================
// 10. INCREMENTAL INTEGRATION TESTING
// ============================================================
// Run development tests for the course data structure and
// advising program. This function is not called by main().
void DebugTest() {
    string input;

    // ============================================================
    // MAIN TEST MENU
    // ============================================================

    while (true) {

        cout << "\n========== FUNCTION TO TEST ==========\n";
        cout << "1. Hash Table Data Structure\n";
        cout << "2. Helper Functions\n";
        cout << "3. Create Course Objects\n";
        cout << "4. Prerequisite Validation\n";
        cout << "5. Load Course Data\n";
        cout << "6. Single Course Search & Print\n";
        cout << "7. Complete Course Print - Alphabetical\n";
        cout << "8. Main Menu & Program Flow\n";
        cout << "0. Exit Debug Tests\n";
        cout << "Select a function: ";

        if (!getline(cin, input) || input == "0") {
            break;
        }

        if (input.size() != 1 || input[0] < '1' || input[0] > '8') {
            cout << "Invalid selection. Enter a number from 0 to 8." << endl;
            continue;
        }

        // ============================================================
        // 1. HASH TABLE DATA STRUCTURE
        // ============================================================

        if (input == "1") {

            while (true) {

                cout << "\n========== HASH TABLE DEBUG TESTS ==========\n";
                cout << "1. Create a Course object\n";
                cout << "2. Display course information\n";
                cout << "3. Display prerequisites\n";
                cout << "4. Create an empty hash table\n";
                cout << "5. Calculate bucket indices\n";
                cout << "6. Insert courses and reject duplicates\n";
                cout << "7. Search for courses\n";
                cout << "8. Check whether a course exists\n";
                cout << "9. Retrieve all stored courses\n";
                cout << "10. Clear the hash table\n";
                cout << "11. Count stored courses\n";
                cout << "12. Check whether the hash table is empty\n";
                cout << "0. Back to Function Menu\n";
                cout << "Select a test: ";

                if (!getline(cin, input)) {
                    return;
                }

                if (input == "0") {
                    break;
                }

                // Accept only the exact numbered options shown.
                if (input != "1" && input != "2" && input != "3" &&
                    input != "4" && input != "5" && input != "6" &&
                    input != "7" && input != "8" && input != "9" &&
                    input != "10" && input != "11" && input != "12") {

                    cout << "Invalid selection. Enter a number from 0 to 12." << endl;
                    continue;
                }

                cout << endl;

                switch (stoi(input)) {

                    case 1: {
                        cout << "Test 1 - Create a Course object" << endl;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.name = "Introduction to Algorithms";
                        testCourse.prerequisites.push_back("CSCI200");
                        testCourse.prerequisites.push_back("MATH201");

                        cout << "Course object created successfully." << endl;
                        break;
                    }

                    case 2: {
                        cout << "Test 2 - Display course information" << endl;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.name = "Introduction to Algorithms";

                        cout << testCourse.courseNumber << ", "
                             << testCourse.name << endl;
                        break;
                    }

                    case 3: {
                        cout << "Test 3 - Display prerequisites" << endl;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.prerequisites.push_back("CSCI200");
                        testCourse.prerequisites.push_back("MATH201");

                        if (testCourse.prerequisites.empty()) {
                            cout << "Prerequisites: none" << endl;
                        }
                        else {
                            cout << "Prerequisites:" << endl;

                            for (const string& prerequisite : testCourse.prerequisites) {
                                cout << "   " << prerequisite << endl;
                            }
                        }
                        break;
                    }

                    case 4: {
                        cout << "Test 4 - Create an empty hash table" << endl;

                        CourseHashTable courseTable;
                        cout << "Hash table created successfully." << endl;
                        break;
                    }

                    case 5: {
                        cout << "Test 5 - Calculate bucket indices" << endl;

                        CourseHashTable courseTable;

                        cout << "CSCI300 -> Bucket "
                             << courseTable.testHash("CSCI300") << endl;

                        cout << "CSCI200 -> Bucket "
                             << courseTable.testHash("CSCI200") << endl;

                        cout << "MATH201 -> Bucket "
                             << courseTable.testHash("MATH201") << endl;

                        cout << "CSCI300 (repeat) -> Bucket "
                             << courseTable.testHash("CSCI300") << endl;
                        break;
                    }

                    case 6: {
                        cout << "Test 6 - Insert courses and reject duplicates" << endl;

                        CourseHashTable courseTable;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.name = "Introduction to Algorithms";

                        Course secondCourse;
                        secondCourse.courseNumber = "CSCI200";
                        secondCourse.name = "Data Structures";

                        cout << "Insert CSCI300: "
                             << (courseTable.insert(testCourse)
                                 ? "Success" : "Duplicate rejected") << endl;

                        cout << "Insert CSCI200: "
                             << (courseTable.insert(secondCourse)
                                 ? "Success" : "Duplicate rejected") << endl;

                        cout << "Insert CSCI300 again: "
                             << (courseTable.insert(testCourse)
                                 ? "Success" : "Duplicate rejected") << endl;
                        break;
                    }

                    case 7: {
                        cout << "Test 7 - Search for courses" << endl;

                        CourseHashTable courseTable;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.name = "Introduction to Algorithms";

                        Course secondCourse;
                        secondCourse.courseNumber = "CSCI200";
                        secondCourse.name = "Data Structures";

                        courseTable.insert(testCourse);
                        courseTable.insert(secondCourse);

                        for (const string number : {"CSCI300", "CSCI200", "CSCI999"}) {

                            const Course* foundCourse = courseTable.search(number);

                            if (foundCourse != nullptr) {
                                cout << "Found: " << foundCourse->courseNumber
                                     << ", " << foundCourse->name << endl;
                            }
                            else {
                                cout << number << " not found." << endl;
                            }
                        }
                        break;
                    }

                    case 8: {
                        cout << "Test 8 - Check whether a course exists" << endl;

                        CourseHashTable courseTable;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.name = "Introduction to Algorithms";

                        courseTable.insert(testCourse);

                        cout << "CSCI300: "
                             << (courseTable.contains("CSCI300")
                                 ? "Exists" : "Not found") << endl;

                        cout << "CSCI999: "
                             << (courseTable.contains("CSCI999")
                                 ? "Exists" : "Not found") << endl;
                        break;
                    }

                    case 9: {
                        cout << "Test 9 - Retrieve all stored courses" << endl;

                        CourseHashTable courseTable;

                        Course firstCourse;
                        firstCourse.courseNumber = "CSCI300";
                        firstCourse.name = "Introduction to Algorithms";

                        Course secondCourse;
                        secondCourse.courseNumber = "CSCI200";
                        secondCourse.name = "Data Structures";

                        Course thirdCourse;
                        thirdCourse.courseNumber = "MATH201";
                        thirdCourse.name = "Discrete Mathematics";

                        courseTable.insert(firstCourse);
                        courseTable.insert(secondCourse);
                        courseTable.insert(thirdCourse);

                        vector<Course> allCourses = courseTable.getAllCourses();

                        cout << "Retrieved " << allCourses.size() << " courses." << endl;

                        for (const Course& course : allCourses) {
                            cout << course.courseNumber << ", "
                                 << course.name << endl;
                        }

                        cout << "All expected courses retrieved: "
                             << ((allCourses.size() == 3 &&
                                  courseTable.contains("CSCI300") &&
                                  courseTable.contains("CSCI200") &&
                                  courseTable.contains("MATH201"))
                                  ? "Yes" : "No") << endl;
                        break;
                    }

                    case 10: {
                        cout << "Test 10 - Clear the hash table" << endl;

                        CourseHashTable courseTable;

                        Course firstCourse;
                        firstCourse.courseNumber = "CSCI300";
                        firstCourse.name = "Introduction to Algorithms";

                        Course secondCourse;
                        secondCourse.courseNumber = "CSCI200";
                        secondCourse.name = "Data Structures";

                        courseTable.insert(firstCourse);
                        courseTable.insert(secondCourse);

                        cout << "Courses before clear: "
                             << courseTable.size() << endl;

                        courseTable.clear();

                        cout << "Courses after clear: "
                             << courseTable.size() << endl;

                        cout << "CSCI300 after clear: "
                             << (courseTable.contains("CSCI300")
                                 ? "Exists" : "Not found") << endl;

                        cout << "CSCI200 after clear: "
                             << (courseTable.contains("CSCI200")
                                 ? "Exists" : "Not found") << endl;

                        cout << "Courses retrieved after clear: "
                             << courseTable.getAllCourses().size() << endl;

                        cout << "Insert CSCI300 after clear: "
                             << (courseTable.insert(firstCourse)
                                 ? "Success" : "Failed") << endl;
                        break;
                    }

                    case 11: {
                        cout << "Test 11 - Count stored courses" << endl;

                        CourseHashTable courseTable;

                        cout << "Initial size: " << courseTable.size() << endl;

                        Course firstCourse;
                        firstCourse.courseNumber = "CSCI300";
                        firstCourse.name = "Introduction to Algorithms";

                        Course secondCourse;
                        secondCourse.courseNumber = "CSCI200";
                        secondCourse.name = "Data Structures";

                        courseTable.insert(firstCourse);

                        cout << "Size after first insertion: "
                             << courseTable.size() << endl;

                        courseTable.insert(secondCourse);

                        cout << "Size after second insertion: "
                             << courseTable.size() << endl;

                        courseTable.insert(firstCourse);

                        cout << "Size after duplicate attempt: "
                             << courseTable.size() << endl;
                        break;
                    }

                    case 12: {
                        cout << "Test 12 - Check whether the hash table is empty" << endl;

                        CourseHashTable courseTable;

                        cout << "Initially empty: "
                             << (courseTable.empty() ? "Yes" : "No") << endl;

                        Course testCourse;
                        testCourse.courseNumber = "CSCI300";
                        testCourse.name = "Introduction to Algorithms";

                        courseTable.insert(testCourse);

                        cout << "Empty after insertion: "
                             << (courseTable.empty() ? "Yes" : "No") << endl;

                        courseTable.clear();

                        cout << "Empty after clear: "
                             << (courseTable.empty() ? "Yes" : "No") << endl;
                        break;
                    }

                    default:
                        break;
                }
            }

            // Return to the main debug menu.
            continue;
        }

        // ============================================================
        // 2. HELPER FUNCTIONS
        // ============================================================

        if (input == "2") {

            while (true) {

                cout << "\n========== HELPER FUNCTIONS DEBUG TESTS ==========\n";
                cout << "1. Remove leading and trailing whitespace\n";
                cout << "2. Split a course record on commas\n";
                cout << "3. Handle empty fields and trailing commas\n";
                cout << "4. Split and trim a complete course record\n";
                cout << "0. Back to Function Menu\n";
                cout << "Select a test: ";

                if (!getline(cin, input)) {
                    return;
                }

                if (input == "0") {
                    break;
                }

                if (input != "1" && input != "2" &&
                    input != "3" && input != "4") {

                    cout << "Invalid selection. Enter a number from 0 to 4." << endl;
                    continue;
                }

                cout << endl;

                switch (stoi(input)) {

                    case 1: {
                        cout << "Test 1 - Trim whitespace" << endl;

                        cout << "Trimmed: ["
                             << trim("  CSCI300  ") << "]" << endl;

                        cout << "Tabs: ["
                             << trim("\tMATH201\t") << "]" << endl;

                        cout << "Whitespace only: ["
                             << trim("   \t  ") << "]" << endl;
                        break;
                    }

                    case 2: {
                        cout << "Test 2 - Split a course record" << endl;

                        vector<string> tokens = splitOnComma(
                            "CSCI300,Introduction to Algorithms,CSCI200,MATH201"
                        );

                        cout << "Fields: " << tokens.size() << endl;

                        for (size_t i = 0; i < tokens.size(); ++i) {
                            cout << "Field " << i + 1 << ": ["
                                 << tokens[i] << "]" << endl;
                        }
                        break;
                    }

                    case 3: {
                        cout << "Test 3 - Empty fields and trailing commas" << endl;

                        vector<string> tokens = splitOnComma(
                            "CSCI300,,MATH201,"
                        );

                        cout << "Fields: " << tokens.size() << endl;

                        for (size_t i = 0; i < tokens.size(); ++i) {
                            cout << "Field " << i + 1 << ": ["
                                 << tokens[i] << "]" << endl;
                        }
                        break;
                    }

                    case 4: {
                        cout << "Test 4 - Split and trim a course record" << endl;

                        vector<string> tokens = splitOnComma(
                            "  CSCI300 , Introduction to Algorithms , CSCI200 , MATH201  "
                        );

                        cout << "Fields: " << tokens.size() << endl;

                        for (size_t i = 0; i < tokens.size(); ++i) {
                            cout << "Field " << i + 1 << ": ["
                                 << tokens[i] << "]" << endl;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }

            // Return to the main debug menu.
            continue;
        }

        // ============================================================
        // 3. CREATE COURSE OBJECTS
        // ============================================================
        if (input == "3") {
            while (true) {
                cout << "\n========== CREATE COURSE OBJECTS DEBUG TESTS ==========\n";
                cout << "1. Course with multiple prerequisites\n";
                cout << "2. Course without prerequisites\n";
                cout << "3. Missing course number\n";
                cout << "4. Missing course title\n";
                cout << "5. Ignore empty prerequisite fields\n";
                cout << "6. Trim whitespace in course fields\n";
                cout << "7. Fewer than two fields\n";
                cout << "0. Back to Function Menu\n";
                cout << "Select a test: ";

                if (!getline(cin, input)) return;
                if (input == "0") break;
                if (input.size() != 1 || input[0] < '1' || input[0] > '7') {
                    cout << "Invalid selection. Enter a number from 0 to 7." << endl;
                    continue;
                }

                vector<string> tokens;
                switch (stoi(input)) {
                    case 1:
                        tokens = splitOnComma("CSCI300,Introduction to Algorithms,CSCI200,MATH201");
                        break;
                    case 2:
                        tokens = splitOnComma("CSCI100,Introduction to Computer Science");
                        break;
                    case 3:
                        tokens = splitOnComma(",Introduction to Algorithms,CSCI200");
                        break;
                    case 4:
                        tokens = splitOnComma("CSCI300,,CSCI200");
                        break;
                    case 5:
                        tokens = splitOnComma("CSCI300,Introduction to Algorithms,,CSCI200,");
                        break;
                    case 6:
                        tokens = {"  CSCI300  ", " Introduction to Algorithms ", " CSCI200 ", " MATH201 "};
                        break;
                    case 7:
                        tokens = {"CSCI300"};
                        break;
                }

                Course course = parseLine(tokens);
                cout << "\nTest " << input << " - Parse course fields" << endl;
                cout << "Course Number: [" << course.courseNumber << "]" << endl;
                cout << "Course Title: [" << course.name << "]" << endl;
                cout << "Prerequisite Count: " << course.prerequisites.size() << endl;
                for (const string& prerequisite : course.prerequisites) {
                    cout << "Prerequisite: [" << prerequisite << "]" << endl;
                }
                if (course.courseNumber.empty() || course.name.empty()) {
                    cout << "Required fields missing (checked during loading)." << endl;
                }
            }
            continue;
        }

        // ============================================================
        // 4. PREREQUISITE VALIDATION
        // ============================================================
        if (input == "4") {
            while (true) {
                cout << "\n========== PREREQUISITE VALIDATION DEBUG TESTS ==========\n";
                cout << "1. All prerequisites exist\n";
                cout << "2. Missing prerequisite\n";
                cout << "3. Course without prerequisites\n";
                cout << "4. Forward reference to a later course\n";
                cout << "5. Course references itself\n";
                cout << "6. Multiple errors in one table\n";
                cout << "0. Back to Function Menu\n";
                cout << "Select a test: ";

                if (!getline(cin, input)) return;
                if (input == "0") break;
                if (input.size() != 1 || input[0] < '1' || input[0] > '6') {
                    cout << "Invalid selection. Enter a number from 0 to 6." << endl;
                    continue;
                }

                int testNumber = stoi(input);
                CourseHashTable courseTable;
                Course mainCourse;
                mainCourse.courseNumber = "CSCI300";
                mainCourse.name = "Introduction to Algorithms";
                Course prerequisiteCourse;
                prerequisiteCourse.courseNumber = "CSCI200";
                prerequisiteCourse.name = "Data Structures";
                Course mathCourse;
                mathCourse.courseNumber = "MATH201";
                mathCourse.name = "Discrete Mathematics";

                switch (testNumber) {
                    case 1:
                        mainCourse.prerequisites = {"CSCI200", "MATH201"};
                        courseTable.insert(mainCourse);
                        courseTable.insert(prerequisiteCourse);
                        courseTable.insert(mathCourse);
                        break;
                    case 2:
                        mainCourse.prerequisites = {"CSCI999"};
                        courseTable.insert(mainCourse);
                        break;
                    case 3:
                        courseTable.insert(mainCourse);
                        break;
                    case 4:
                        // Insert the dependent course before its prerequisite.
                        mainCourse.prerequisites = {"CSCI200"};
                        courseTable.insert(mainCourse);
                        courseTable.insert(prerequisiteCourse);
                        break;
                    case 5:
                        mainCourse.prerequisites = {"CSCI300"};
                        courseTable.insert(mainCourse);
                        break;
                    case 6:
                        mainCourse.prerequisites = {"CSCI300", "CSCI999"};
                        prerequisiteCourse.prerequisites = {"MATH999"};
                        courseTable.insert(mainCourse);
                        courseTable.insert(prerequisiteCourse);
                        break;
                }

                cout << "\nTest " << testNumber << " - Validate prerequisites" << endl;
                bool valid = validatePrerequisites(courseTable);
                cout << "Prerequisites valid: " << (valid ? "Yes" : "No") << endl;
            }
            continue;
        }

        // ============================================================
        // 5. LOAD COURSE DATA
        // ============================================================
        if (input == "5") {
                    while (true) {
                        cout << "\n========== LOAD COURSE DATA DEBUG TESTS ==========\n";
                        cout << "1. Load valid courses and prerequisites\n";
                        cout << "2. File does not exist\n";
                        cout << "3. Reject missing course title\n";
                        cout << "4. Reject duplicate course numbers\n";
                        cout << "5. Reject missing prerequisite\n";
                        cout << "6. Load blank lines and forward references\n";
                        cout << "7. Reject empty file\n";
                        cout << "8. Preserve existing courses after failed reload\n";
                        cout << "0. Back to Function Menu\n";
                        cout << "Select a test: ";

                        if (!getline(cin, input)) return;
                        if (input == "0") break;
                        if (input.size() != 1 || input[0] < '1' || input[0] > '8') {
                            cout << "Invalid selection. Enter a number from 0 to 8." << endl;
                            continue;
                        }

                        int testNumber = stoi(input);
                        const string testFile = "cs300_load_test.csv";
                        CourseHashTable courseTable;

                        // Prepare the file contents needed for this test.
                        // The missing-file test deliberately does not create a file.
                        if (testNumber != 2) {
                            ofstream outputFile(testFile);
                            if (!outputFile.is_open()) {
                                cout << "Error: Cannot create test file in this folder." << endl;
                                continue;
                            }
                            switch (testNumber) {
                                case 1:
                                    outputFile << "CSCI300,Introduction to Algorithms,CSCI200,MATH201\n"
                                               << "CSCI200,Data Structures\n"
                                               << "MATH201,Discrete Mathematics\n";
                                    break;
                                case 3:
                                    outputFile << "CSCI300,,CSCI200\n";
                                    break;
                                case 4:
                                    outputFile << "CSCI200,Data Structures\n"
                                               << "CSCI200,Duplicate Course\n";
                                    break;
                                case 5:
                                    outputFile << "CSCI300,Introduction to Algorithms,CSCI999\n";
                                    break;
                                case 6:
                                    outputFile << "\n  \nCSCI300,Introduction to Algorithms,CSCI200\n"
                                               << "CSCI200,Data Structures\n";
                                    break;
                                case 7:
                                    break;
                                case 8:
                                    outputFile << "CSCI300,,CSCI200\n";
                                    break;
                            }
                            outputFile.close();
                        }

                        if (testNumber == 8) {
                            Course existing;
                            existing.courseNumber = "CSCI100";
                            existing.name = "Introduction to Computer Science";
                            courseTable.insert(existing);
                        }

                        cout << "\nTest " << testNumber << " - Load course data" << endl;
                        const string filePath = (testNumber == 2)
                            ? "cs300_file_that_does_not_exist_12345.csv" : testFile;
                        bool loaded = loadCourses(filePath, courseTable);
                        cout << "Load successful: " << (loaded ? "Yes" : "No") << endl;
                        cout << "Courses stored: " << courseTable.size() << endl;
                        if (testNumber == 1 || testNumber == 6) {
                            cout << "CSCI300 found: "
                                 << (courseTable.contains("CSCI300") ? "Yes" : "No") << endl;
                        }
                        if (testNumber == 8) {
                            cout << "Previous CSCI100 preserved: "
                                 << (courseTable.contains("CSCI100") ? "Yes" : "No") << endl;
                        }
                        if (testNumber != 2) {
                            // Remove the temporary test file after the test.
                            remove(testFile.c_str());
                        }
                    }
                    continue;
                }

        // ============================================================
        // 6. SINGLE COURSE SEARCH & PRINT
        // ============================================================
        if (input == "6") {
            while (true) {
                cout << "\n========== SINGLE COURSE SEARCH & PRINT DEBUG TESTS ==========\n";
                cout << "1. Find a course with multiple prerequisites\n";
                cout << "2. Find a course without prerequisites\n";
                cout << "3. Search for a course that does not exist\n";
                cout << "4. Trim whitespace and normalize lowercase input\n";
                cout << "5. Find a course with one prerequisite\n";
                cout << "6. Search an empty course table\n";
                cout << "7. Display a missing prerequisite safely\n";
                cout << "0. Back to Function Menu\n";
                cout << "Select a test: ";

                if (!getline(cin, input)) return;
                if (input == "0") break;
                if (input.size() != 1 || input[0] < '1' || input[0] > '7') {
                    cout << "Invalid selection. Enter a number from 0 to 7." << endl;
                    continue;
                }

                int testNumber = stoi(input);
                CourseHashTable courseTable;
                Course mainCourse;
                mainCourse.courseNumber = "CSCI300";
                mainCourse.name = "Introduction to Algorithms";
                Course prerequisiteCourse;
                prerequisiteCourse.courseNumber = "CSCI200";
                prerequisiteCourse.name = "Data Structures";
                Course mathCourse;
                mathCourse.courseNumber = "MATH201";
                mathCourse.name = "Discrete Mathematics";
                Course introCourse;
                introCourse.courseNumber = "CSCI100";
                introCourse.name = "Introduction to Computer Science";

                string searchNumber = "CSCI300";
                switch (testNumber) {
                    case 1:
                        mainCourse.prerequisites = {"CSCI200", "MATH201"};
                        courseTable.insert(mainCourse);
                        courseTable.insert(prerequisiteCourse);
                        courseTable.insert(mathCourse);
                        break;
                    case 2:
                        courseTable.insert(introCourse);
                        searchNumber = "CSCI100";
                        break;
                    case 3:
                        courseTable.insert(mainCourse);
                        searchNumber = "CSCI999";
                        break;
                    case 4:
                        courseTable.insert(mainCourse);
                        searchNumber = "  csci300  ";
                        break;
                    case 5:
                        mainCourse.prerequisites = {"CSCI200"};
                        courseTable.insert(mainCourse);
                        courseTable.insert(prerequisiteCourse);
                        break;
                    case 6:
                        break;
                    case 7:
                        // This table is deliberately invalid to test safe output.
                        mainCourse.prerequisites = {"CSCI999"};
                        courseTable.insert(mainCourse);
                        break;
                }

                cout << "\nTest " << testNumber << " - Search and print a course" << endl;
                searchCourse(courseTable, searchNumber);
            }
            continue;
        }

        // ============================================================
        // 7. COMPLETE COURSE PRINT - ALPHABETICAL
        // ============================================================
        if (input == "7") {
            while (true) {
                cout << "\n========== COMPLETE COURSE PRINT DEBUG TESTS ==========\n";
                cout << "1. Sort courses inserted out of order\n";
                cout << "2. Print an empty course table\n";
                cout << "3. Print a single course\n";
                cout << "4. Sort courses from different departments\n";
                cout << "5. Sort course numbers within one department\n";
                cout << "0. Back to Function Menu\n";
                cout << "Select a test: ";

                if (!getline(cin, input)) return;
                if (input == "0") break;
                if (input.size() != 1 || input[0] < '1' || input[0] > '5') {
                    cout << "Invalid selection. Enter a number from 0 to 5." << endl;
                    continue;
                }

                int testNumber = stoi(input);
                CourseHashTable courseTable;
                cout << "\nTest " << testNumber << " - Print complete course list" << endl;

                switch (testNumber) {
                    case 1:
                        courseTable.insert({"MATH201", "Discrete Mathematics", {}});
                        courseTable.insert({"CSCI300", "Introduction to Algorithms", {}});
                        courseTable.insert({"CSCI100", "Introduction to Computer Science", {}});
                        courseTable.insert({"CSCI200", "Data Structures", {}});
                        break;
                    case 2:
                        break;
                    case 3:
                        courseTable.insert({"CSCI200", "Data Structures", {}});
                        break;
                    case 4:
                        courseTable.insert({"MATH201", "Discrete Mathematics", {}});
                        courseTable.insert({"PHYS150", "General Physics", {}});
                        courseTable.insert({"CSCI200", "Data Structures", {}});
                        courseTable.insert({"BIOL101", "Introduction to Biology", {}});
                        break;
                    case 5:
                        courseTable.insert({"CSCI400", "Advanced Algorithms", {}});
                        courseTable.insert({"CSCI100", "Introduction to Computer Science", {}});
                        courseTable.insert({"CSCI300", "Introduction to Algorithms", {}});
                        courseTable.insert({"CSCI200", "Data Structures", {}});
                        break;
                }

                printCourseList(courseTable);
            }
            continue;
        }

        // ============================================================
        // 8. MAIN MENU & PROGRAM FLOW
        // ============================================================
        if (input == "8") {

            // RUN the actual advising program for end-to-end testing
            runAdvisingProgram();

            // RETURN to the debug function menu after exiting the advising program
            continue;
        }
    }

    cout << "Exiting debug tests." << endl;
}




// ============================================================
// 11. MAIN FUNCTION
// ============================================================
// FUNCTION main() RETURNS Integer
int main() {

	// START the ABCU Advising Assistance Program
	runAdvisingProgram();

	// RETURN zero to indicate successful program termination
    return 0;
}
