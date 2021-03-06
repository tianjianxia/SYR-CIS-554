//HW3 by Tianjian Xia
//SU Net ID: txia09  SUID: 350385039
//CIS554 HW3



//HW3
//Due: Thursday (10/3) at 11:59PM


#include <iostream>
#include <list>
#include <map>
#include <string>
using namespace std;

class course {
public:
	string name;
	int section;
	int credits;
	course() { name = ""; section = 1; credits = 0; }
	course(string n, int s, int c) { name = n; section = s; credits = c; }
	//Add additional needed member functions and implement them.
	//You also need to implement some needed functions for overloading operator<< .

};
//Implement the following functions

ostream& operator<<(ostream& str, const course& c) {
	str << c.name << "  " << c.section << "  " << c.credits << "  ";
	return str;
}

template <class T> ostream& operator<<(ostream& str, const list<T*>& L) {
	for (auto i : L) { str << *i << " "; }
	return str;
}

template <class X1, class X2> ostream& operator<<(ostream& str, map<X1, X2*>& M) {
	for (auto i : M) {
		str << "semester = " << i.first << endl;
		str << *i.second << endl;
	}
	return str;
}

template <class X1, class X2> ostream& operator<<(ostream& str, map<X1, X2>& M) {
	for (auto i : M) {
		str << "student id = " << i.first << endl;
		str << i.second << endl;
	}
	return str;
}


void add_student(map<int, map<int, list<course*>* > >& DB, int id);
void remove_student(map<int, map<int, list<course*>* > >& DB, int id);
void add_course(map<int, map<int, list<course*>* > >& DB, int semester, int id, course c); //20171 Spring semester of 2017; 20172: Fall semester of 2017

void drop_course(map<int, map<int, list<course*>* > >& DB, int semester, int id, course c);
void print_student_semester_courses(map<int, map<int, list<course*>* > >& DB, int semester, int id);
void print_student_all_courses(map<int, map<int, list<course*>* > >& DB, int id);
void print_DB(map<int, map<int, list<course*>* > >& DB);

//For the print funcitons, you need to add more fucntions to overload operator<<.
//for example print_DB is simply cout << DB;
//Courses in a semeste are sorted alphabetically.

int main() {
	//Do not change code for main function
	map<int, map<int, list <course*>*> > DB;

	add_student(DB, 11111);
	course C1("CIS554", 1, 3), C2("CSE674", 1, 3), C3("MAT296", 8, 4), C4("WRT205", 5, 3);

	add_course(DB, 20171, 11111, C1);
	add_course(DB, 20171, 11111, C4);
	add_course(DB, 20171, 11111, C3);
	add_course(DB, 20171, 11111, C2);
	print_student_semester_courses(DB, 20171, 11111);

	drop_course(DB, 20171, 11111, C1);
	print_student_semester_courses(DB, 20171, 11111);

	add_course(DB, 20172, 11111, C2);
	add_course(DB, 20172, 11111, C4);
	add_course(DB, 20172, 11111, C3);
	add_course(DB, 20172, 11111, C1);
	print_student_all_courses(DB, 11111);

	add_student(DB, 11112);
	add_course(DB, 20171, 11112, C2);
	add_course(DB, 20171, 11112, C4);
	add_course(DB, 20171, 11112, C3);
	add_course(DB, 20171, 11112, C1);
	print_student_semester_courses(DB, 20171, 11112);

	add_course(DB, 20172, 11112, C2);
	add_course(DB, 20172, 11112, C4);
	add_course(DB, 20172, 11112, C3);
	add_course(DB, 20172, 11112, C1);
	print_student_semester_courses(DB, 20172, 11112);
	print_student_all_courses(DB, 11112);
	print_DB(DB);
	remove_student(DB, 11111);
	print_DB(DB);
	getchar();
	getchar();
	return 0;
}

void add_student(map<int, map<int, list<course*>* >>& DB, int id) {
	if (DB.find(id) != DB.end()) return;
	map<int, list<course*>*> L;
	DB[id] = L;
}

void remove_student(map<int, map<int, list<course*>* >>& DB, int id) {
	if (DB.find(id) == DB.end()) return;
	for (auto it = DB.begin(); it != DB.end(); it++) {
		if (it->first == id) {
			for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
				for (auto it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {
					delete* it2;
				}
				delete it1->second;
			}
			DB.erase(it);
			break;
		}
	}
}


void add_course(map<int, map<int, list<course*>* >>& DB, int semester, int id, course c) {
	bool flag = false;
	if (DB.find(id) == DB.end()) return;
	for (auto it = DB.begin(); it != DB.end(); it++) {
		if (it->first == id) {
			if (it->second.find(semester) == it->second.end()) {
				list<course*>* L_t = new list<course*>;
				it->second.insert({ semester,L_t });
				course* c_t = new course(c);
				DB[id][semester]->push_back(c_t);
			}
			else {
				if (DB[id][semester]->empty()) {
					course* c_t = new course(c);
					DB[id][semester]->push_back(c_t);
				}
				else {
					for (auto itad = DB[id][semester]->begin(); itad != DB[id][semester]->end(); itad++) {
						if ((*itad)->name == c.name) { return; }
						else if ((*itad)->name > c.name) {
							course* c_t = new course(c);
							DB[id][semester]->insert(itad, c_t);
							flag = true;
							break;
						}
					}
					if (flag == false) {
						course* c_t = new course(c);
						DB[id][semester]->push_back(c_t);
					}
				}
			}
		}
	}
}

void drop_course(map<int, map<int, list<course*>* >>& DB, int semester, int id, course c) {
	bool flag = false;
	if (DB.find(id) == DB.end()) return;
	if (DB[id].find(semester) == DB[id].end()) return;
	for (auto it = DB[id][semester]->begin(); it != DB[id][semester]->end(); it++) {
		if ((**it).name == c.name) {  //wondering if the course should be exactly the same with c
			delete* it;
			DB[id][semester]->erase(it);
			flag = true;
			break;
		}
	}
}

void print_student_semester_courses(map<int, map<int, list<course*>* >>& DB, int semester, int id) {
	cout << "student id = " << id << endl;
	cout << "semester = " << semester << endl;
	cout << *DB[id][semester] << endl;
}

void print_student_all_courses(map<int, map<int, list<course*>* >>& DB, int id) {
	cout << "student id = " << id << endl;
	cout << DB[id] << endl;
}


void print_DB(map<int, map<int, list<course*>* >>& DB) {
	cout << DB << endl;
}
//Some additional functions for overloading operator<<

/*
//Your output needs to keep the identical format
//Sample Screenshot
student id = 11111
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11111
semester = 20171
CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11111
semester = 20171
CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11112
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11111
semester = 20171
CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
*/
