#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Task {
public:
	string title;
	string description;
	string duedate;
	bool isDone;

	Task(string t, string d, string date) {
		title = t;
		description = d;
		duedate = date;
		isDone = false;
	}
};

class TodoApp {
private:
	vector<Task> tasks;
public:
	void addTask(
		const string& title,
		const string& description,
		const string& duedate)
	{
		Task newTask(title, description, duedate);

		if (title.empty() || description.empty() || duedate.empty()) {
			cerr << "title , description or duedate cannot be empty" << endl;
		}
		else {
			tasks.push_back(newTask);
		}

		cout << "Press Enter key to continue..." << endl;
		cin.get();
	}

	void showTask() {

		if (tasks.empty()){
			cout << "No tasks to show!" << endl;
			return;
		}

		for (int i = 0; i < tasks.size(); i++) {
			cout << "\nTask #" << i + 1 << endl;
			cout << "Title: " << tasks[i].title << endl;
			cout << "Desc: " << tasks[i].description << endl;
			cout << "DueDate: " << tasks[i].duedate << endl;
			cout << "Task Status: " << (tasks[i].isDone ? "Done" : "Not Done") << endl;
		}
	}

	void removeTask() {

		showTask();

		string input;
		cout << "Enter the task id to remove: ";
		getline(cin, input);

		try {

			int index = stoi(input);

			if (index < 1 || index > tasks.size()) {
				cout << "Invalid task ID!" << endl;
				return;
			}

			string confirmation;
			cout << "Are you sure you want to remove this task? (y/n): ";
			getline(cin, confirmation);
			if (confirmation != "y" && confirmation != "Y") {
				cout << "Failed to remove task." << endl;
				return;
			}
			cout << "Task removal confirmed...." << endl;
			tasks.erase(tasks.begin() + index - 1);
			cout << "Task removed successfully!" << endl;
		}
		catch (...) {
			cout << "error: invalid input, please enter a number." << endl;
		}
	}

	void markTask() {
		showTask();

		string input;
		cout << "Enter the task id to mark as done: ";
		getline(cin, input);

		try {
			int index = stoi(input);
			if (index < 1 || index > tasks.size()) {
				cout << "Invalid task ID!" << endl;
				return;
			}

			tasks[index - 1].isDone = true;
			cout << "Task marked as done!" << endl;
		}
		catch (...) {
			cout << "error: invalid input, please enter a number." << endl;
		}
	}

	void EditTask() {
		showTask();
		string choice;
		cout << "Enter the task id to edit: ";
		getline(cin, choice);
		try {
			int index = stoi(choice);

			if (index < 1 || index > tasks.size()) {
				cout << "Invalid task ID!" << endl;
				return;
			}

			string field;
			cout << "What do you want to edit? (title/description/duedate): ";
			getline(cin, field);

			transform(field.begin(), field.end(), field.begin(), ::tolower);
				
			if (field == "title") {
				string newTitle;
				cout << "Enter the new title: ";
				getline(cin, newTitle);

				tasks[index - 1].title = newTitle;
			}

			else if (field == "description") {
				string newDesc;
				cout << "Enter the new description: ";
				getline(cin, newDesc);

				tasks[index - 1].description = newDesc;
			}

			else if (field == "duedate") {
				string newDate;
				cout << "Enter the new duedate: ";
				getline(cin, newDate);

				tasks[index - 1].duedate = newDate;
			}

			else {
				cout << "Invalid field!" << endl;
			}


			cout << "Task updated successfully!" << endl;

			SaveToFile();

		}
		catch (...) {
			cout << "error: invalid input, please enter a number." << endl;
		}
	}

	void SaveToFile() {
		ofstream file("tasks.csv");
		
		for (const auto& task : tasks) {
			file <<
				task.title << "|" <<
				task.description << "|" <<
				task.duedate << "|" <<
				task.isDone << endl;
		}
		file.close();
	}

	void loadFromFile() {
		ifstream file("tasks.csv");
		string Text;
		while (getline(file, Text)){
			stringstream ss(Text);

			string title, description, duedate, isDone;

			getline(ss, title, '|');
			getline(ss, description, '|');	
			getline(ss, duedate, '|');
			getline(ss, isDone, '|');

			Task t(title, description, duedate);
			t.isDone = (isDone == "1") ? true : false;
		
			tasks.push_back(t);
		}
		file.close();
	}
};

void MainMenu() {
	cout << "--------TODO--------" << endl;
	cout << "1. Add Task" << endl;
	cout << "2. Show Tasks" << endl;
	cout << "3. Remove Task" << endl;
	cout << "4. Mark Task as Done" << endl;
	cout << "5. Edit Task" << endl;
	cout << "6. Exit" << endl;
}

void collectTaskInfo(string& title, string& desc, string& duedate) {
	cout << "--------Add Task--------" << endl;

	cout << "Task Title: ";
	getline(cin, title);

	cout << "Task description: ";
	getline(cin, desc);

	cout << "Task Due Date (MM/DD/YYYY): ";
	getline(cin, duedate);

	cout << endl;
}

int main() {

	TodoApp app;	
	app.loadFromFile();
	string title, desc, duedate;

	while (true) {

		MainMenu();

		string input ;
		cout << "Enter your choice: ";
		getline(cin, input);
		'\n';

		try {
			int c = stoi(input); 
			switch (c) {
			case 1:

				collectTaskInfo(title, desc, duedate);
				app.addTask(title, desc, duedate);
				app.SaveToFile();
				break;

			case 2:
				app.showTask();
				cout << endl;
				break;
			case 3:
				app.removeTask();
				app.SaveToFile();
				break;
			case 4:
				app.markTask();
				app.SaveToFile();
				break;
			case 5:
				app.EditTask();
				break;
			case 6:
				cout << "Exiting the application. Goodbye!" << endl;
				return 0;
				break;
			default:
				cout << "invalid choice, please try again." << endl;
				break;
			}
		}
		catch (...) {
			cout << "error: invalid input, please enter a number." << endl;
		}

	}
	return 0;
}