## 📝 To-Do List in C

Console-based To-Do List application with user authentication and per-user task storage.


## 🧮Description 
This is a C console application for managing personal tasks.
Users can sign up and log in.
User credentials are stored securely in users.txt.
Passwords are hidden while typing using masked input (*).
Each user has a separate CSV file to store their tasks.
Tasks can be added, edited, deleted, or marked as done, with priority levels and optional due dates.
Users can also view task statistics.


Here are some previews of the program:

Signup/Login interface

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/1.png?raw=true)

Adding new task :

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/2.png?raw=true)

Changing Status as Done :

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/3.png?raw=true)

Deleting a task :

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/4.png?raw=true)

Editing a task :

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/5.png?raw=true)

Checking Stats :

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/6.png?raw=true)

Final output :

![image alt](https://github.com/mansi153-wq/todo_list_miniproject/blob/main/screenshots/7.png?raw=true)


todo_list_miniproject/
│
├── main.c               # Main source code
├── users.txt            # User credentials
├── tasks/               # User-specific task files
├── screenshots/         # Screenshots for README
└── README.md            # Project documentation




## 🚀 Features
- 👤 User authentication (Sign-up / Login)
- 📁 Per-user CSV task storage
- 🧾 Add, edit, delete, mark tasks as done
- 📊 Task statistics (total, completed, pending)
- 🔒 Hidden password input using `conio.h`
- 💾 Persistent data storage in text files


## 🔹 How it Works
User signs up or logs in.
Usernames and passwords are saved in users.txt.
Tasks are stored in a per-user CSV file.
Users interact with a console menu to manage tasks.
All changes are saved automatically to the CSV file.

## 🔹 Usage
Compile:
gcc todo.c -o todo
Run:
./todo

## 🔹 Notes
Works on Windows console (uses _getch() for password masking).
Each user has a separate CSV file, allowing multiple users to use the program independently.

## Technologies & Concepts Used
C Programming: Functions, loops, conditionals, and console I/O
Data Structures: Structs and dynamic arrays (TaskList)
File Handling: Reading/writing CSV files and storing user credentials
String Manipulation: strcpy, strcmp, strlen, strcspn
Algorithms / Logic: CRUD operations, task search, statistics calculation
Other Techniques: Boolean logic (stdbool.h), password masking (_getch()), input validation
Passwords are hidden while entering for security.


## 🧩 Future Enhancements
- 🔐 Encrypt passwords instead of plain text
- 🗓 Add due dates and reminders
- 📈 Add task sorting by priority or date
- 🧍 Multi-user management improvements
- 🖥 GUI version using C graphics or Python frontend










