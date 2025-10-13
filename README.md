📝 To-Do List in C

Console-based To-Do List application with user authentication and per-user task storage.

🔹 Description

This is a C console application for managing personal tasks.
Users can sign up and log in.
User credentials are stored securely in users.txt.
Passwords are hidden while typing using masked input (*).
Each user has a separate CSV file to store their tasks.
Tasks can be added, edited, deleted, or marked as done, with priority levels and optional due dates.
Users can also view task statistics.

🔹 Features
Feature	Description
User Authentication	Sign Up / Login with password masking
Credential Storage	Credentials stored in users.txt
Task CRUD	Add, Edit, Delete tasks
Task Completion	Mark tasks as Done
Priority	Set task priority: High / Medium / Low
Due Date	Optional due date in YYYY-MM-DD format
Statistics	View number of open, done, and total tasks
Per-User Storage	Each user gets a separate CSV file (<username>_tasks.csv)

🔹 How it Works
User signs up or logs in.
Usernames and passwords are saved in users.txt.
Tasks are stored in a per-user CSV file.
Users interact with a console menu to manage tasks.
All changes are saved automatically to the CSV file.

🔹 Usage
Compile:
gcc todo.c -o todo
Run:
./todo

🔹 Notes
Works on Windows console (uses _getch() for password masking).
Each user has a separate CSV file, allowing multiple users to use the program independently.

Technologies & Concepts Used
C Programming: Functions, loops, conditionals, and console I/O
Data Structures: Structs and dynamic arrays (TaskList)
File Handling: Reading/writing CSV files and storing user credentials
String Manipulation: strcpy, strcmp, strlen, strcspn
Algorithms / Logic: CRUD operations, task search, statistics calculation
Other Techniques: Boolean logic (stdbool.h), password masking (_getch()), input validation
Passwords are hidden while entering for security.



![image alt}(https://github.com/mansi153-wq/todo_list_miniproject/blob/main/1.png?raw=true)









