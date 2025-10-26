#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> // Include for potential use, though not necessary 

// 1. Course Data Structure
/**
 * @brief Represents a single course with its number, title, and prerequisites.
 */
struct Course {
    std::string courseNumber;
    std::string title;
    // Store prerequisites as a vector of course numbers (strings)
    std::vector<std::string> prerequisites;
};

// 2. BST Node Structure
/**
 * @brief Represents a node in the Binary Search Tree.
 */
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Default constructor
    Node() : left(nullptr), right(nullptr) {}

    // Parameterized constructor
    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// 3. Binary Search Tree (BST) Class
/**
 * @brief Manages the collection of courses using a Binary Search Tree,
 * ordered by the courseNumber string for alphanumeric sorting and lookup.
 */
class CourseCatalog {
private:
    Node* root;

    /**
     * @brief Recursively inserts a Course into the BST.
     * @param node The current node pointer (reference) in the traversal.
     * @param course The Course object to insert.
     */
    void insertNode(Node*& node, const Course& course) {
        // If the current node is null, insert the new node here
        if (node == nullptr) {
            node = new Node(course);
        }
        // Compare course numbers to determine left or right traversal
        else if (course.courseNumber.compare(node->course.courseNumber) < 0) {
            insertNode(node->left, course);
        }
        // Assuming no duplicate course numbers, go right otherwise
        else {
            insertNode(node->right, course);
        }
    }

    /**
     * @brief Performs an in-order traversal of the BST to print courses alphanumerically.
     * @param node The current node pointer.
     */
    void inOrder(Node* node) {
        if (node != nullptr) {
            // Recurse left
            inOrder(node->left);

            // Print the current course (Middle/Root)
            // Output format: CSCIXXX, Course Title
            std::cout << node->course.courseNumber << ", " << node->course.title << std::endl;

            // Recurse right
            inOrder(node->right);
        }
    }

    /**
     * @brief Recursively searches the BST for a course by its number.
     * @param node The current node pointer.
     * @param courseNumber The course number string to search for.
     * @return The Node pointer if found, otherwise nullptr.
     */
    Node* searchNode(Node* node, const std::string& courseNumber) {
        // Base case: Course found or tree is empty
        if (node == nullptr || node->course.courseNumber == courseNumber) {
            return node;
        }
        // Search left subtree
        else if (courseNumber.compare(node->course.courseNumber) < 0) {
            return searchNode(node->left, courseNumber);
        }
        // Search right subtree
        else {
            return searchNode(node->right, courseNumber);
        }
    }

public:
    // Constructor initializes the root to null
    CourseCatalog() : root(nullptr) {}

    // Destructor to clean up memory 
    ~CourseCatalog() {
        // A helper function for post-order deletion would be here
    }

    /**
     * @brief Public wrapper to insert a course into the BST.
     * @param course The Course object to insert.
     */
    void Insert(const Course& course) {
        insertNode(root, course);
    }

    /**
     * @brief Public wrapper to print all courses in alphanumeric order (Option 2).
     */
    void PrintAllCourses() {
        if (root == nullptr) {
            std::cout << "The course catalog is empty." << std::endl;
            return;
        }
        inOrder(root);
    }

    /**
     * @brief Public wrapper to search for and retrieve a Course object (Option 3).
     * @param courseNumber The course number to search for.
     * @return The found Course object, or a default "NOT_FOUND" Course if not found.
     */
    Course Search(const std::string& courseNumber) {
        // Convert input to uppercase for case-insensitive search if needed, but sticking 
        // to exact match per typical course number standards.
        Node* node = searchNode(root, courseNumber);

        if (node != nullptr) {
            return node->course;
        }

        // Return a default/empty course with a special indicator if not found
        return Course{ "NOT_FOUND", "", {} };
    }
};

// 4. Function to load data from file (Option 1)
/**
 * @brief Reads course data from a CSV-like file and loads it into the CourseCatalog BST.
 * @param catalog The CourseCatalog object to load data into.
 * @param filename The name of the file to read.
 */
void loadDataStructure(CourseCatalog& catalog, const std::string& filename)
{
    std::ifstream file(filename);

    // Check for file open error (Industry Standard Best Practices)
    if (!file.is_open()) {
        std::cerr << "\nError: Could not open file " << filename << ". Please check the file name and path." << std::endl;
        return;
    }

    std::string line;
    // Read the file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        Course course;

        // 1. Course Number (Required - delimited by comma)
        if (!std::getline(ss, segment, ',')) continue;
        course.courseNumber = segment;

        // 2. Course Title (Required - delimited by comma)
        if (!std::getline(ss, segment, ',')) continue;
        course.title = segment;

        // 3+. Prerequisites (Optional, 0 to N - remaining segments)
        // This loop handles the rest of the comma-separated values on the line as prerequisites
        while (std::getline(ss, segment, ',')) {
            // Simple trim logic (optional, depends on exact file format)
            if (!segment.empty() && segment[0] == ' ') {
                segment.erase(0, 1);
            }
            if (!segment.empty()) {
                course.prerequisites.push_back(segment);
            }
        }

        // Insert the fully populated course into the catalog BST
        catalog.Insert(course);
    }

    std::cout << "\nCourse data loaded successfully." << std::endl;
}

// 5. Function to display the menu
/**
 * @brief Prints the user menu options to the console.
 */
void displayMenu() {
    std::cout << "\nWhat would you like to do?" << std::endl;
    std::cout << "  1. Load Data Structure." << std::endl;
    std::cout << "  2. Print Course List (Alphanumeric)." << std::endl;
    std::cout << "  3. Print Course Information (Lookup)." << std::endl;
    std::cout << "  9. Exit." << std::endl;
    std::cout << "\nEnter your choice (1, 2, 3, or 9): ";
}

// 6. Main application logic
int main() {
    // Instantiate the course catalog BST
    CourseCatalog courseCatalog;
    int choice = 0;
    bool dataLoaded = false;
    std::string filename;

    std::cout << "Welcome to the course planner!" << std::endl;

    // Main menu loop
    while (choice != 9) {
        displayMenu();

        // Input validation and getting the choice
        if (!(std::cin >> choice)) {
            // Handle non-integer input (Industry Standard Best Practices)
            std::cerr << "Input error. Please enter a valid menu number (1, 2, 3, or 9)." << std::endl;
            std::cin.clear(); // Clear the error flags
            std::cin.ignore(10000, '\n'); // Discard bad input up to 10000 characters
            continue;
        }

        switch (choice) {
        case 1: { // Load Data Structure
            std::cout << "\nEnter the filename containing the course data (e.g., ABCU_Input.csv): ";
            std::cin >> filename;
            loadDataStructure(courseCatalog, filename);
            dataLoaded = true;
            break;
        }
        case 2: { // Print Course List (Alphanumeric)
            if (!dataLoaded) {
                std::cout << "\nError: Please load the data structure first (Option 1)." << std::endl;
                break;
            }
            std::cout << "\nHere is a list of all courses (alphanumeric order):" << std::endl;
            courseCatalog.PrintAllCourses();
            break;
        }
        case 3: { // Print Course Information (Lookup)
            if (!dataLoaded) {
                std::cout << "\nError: Please load the data structure first (Option 1)." << std::endl;
                break;
            }
            std::string courseNum;
            std::cout << "\nWhat course number do you want to know about (e.g., MATH200)? ";
            // Clear any residual newline/whitespace before reading the course number
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, courseNum);

            Course courseInfo = courseCatalog.Search(courseNum);

            if (courseInfo.courseNumber == "NOT_FOUND") {
                // Output error message for a non-existent course (Industry Best Practices)
                std::cout << "\nCourse not found: " << courseNum << std::endl;
            }
            else {
                // Print the course title
                std::cout << "\n" << courseInfo.courseNumber << ", " << courseInfo.title << std::endl;

                // Print prerequisites
                std::cout << "Prerequisites: ";
                if (courseInfo.prerequisites.empty()) {
                    std::cout << "None" << std::endl;
                }
                else {
                    // Iterate and print all prerequisites
                    for (size_t i = 0; i < courseInfo.prerequisites.size(); ++i) {
                        std::cout << courseInfo.prerequisites[i];
                        // Add a comma and space unless it's the last one
                        if (i < courseInfo.prerequisites.size() - 1) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << std::endl;
                }
            }
            break;
        }
        case 9: { // Exit
            std::cout << "\nThank you for using the course planner!" << std::endl;
            break;
        }
        default: { // Invalid menu option (Input Error Handling)
            std::cout << "\n" << choice << " is not a valid option. Please choose 1, 2, 3, or 9." << std::endl;
            break;
        }
        }
    }

    return 0;
}
