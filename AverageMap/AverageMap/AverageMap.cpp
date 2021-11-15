#include <iostream>
#include <map>

struct StudentInfo {
	std::string firstName;
    std::string secondName;
    int averageScore;
};

bool FindStudent(std::pair<int, StudentInfo> st, int averageScore) {
    return st.second.averageScore == 100;
}

int main()
{
    std::map<int, StudentInfo> students;
    students.insert(std::make_pair(10, StudentInfo{ "Paul", "Paulov", 65 }));
    students.insert(std::make_pair(20, StudentInfo{ "Peter", "Peterov", 80 }));
    students.insert(std::make_pair(30, StudentInfo{ "Gashish", "Gashishov", 100 }));
    students.insert(std::make_pair(40, StudentInfo{ "Maul", "Maulov", 76 }));

    /*std::cout << "enter average points" << std::endl;
    int averageScore;
    std::cin >> averageScore;*/
    std::map<int, StudentInfo>::iterator it = std::find_if(students.begin(), students.end(), FindStudent);

    std::cout << it->second.firstName << std::endl;
}
