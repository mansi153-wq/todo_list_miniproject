#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define USER_FILE "users.txt"
#include <conio.h>
void get_hidden_input(char *buffer, size_t size) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < size - 1) { // '\r' = Enter
        if (ch == '\b' && i > 0) { // handle backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            buffer[i++] = ch;
            printf("*"); // show * instead of real char
        }
    }
    buffer[i] = '\0';
    printf("\n");
}


bool user_exists(const char *username) {
    FILE *f = fopen(USER_FILE, "r");
    if (!f) return false;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        char user[50];
        sscanf(line, "%49[^:]", user);
        if (strcmp(user, username) == 0) {
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

bool signup(char *username, char *password) {
    if (user_exists(username)) {
        printf("Username already exists.\n");
        return false;
    }
    FILE *f = fopen(USER_FILE, "a");
    if (!f) { perror("fopen"); return false; }
    fprintf(f, "%s:%s\n", username, password);
    fclose(f);
    printf("Sign up successful! Please log in.\n");
    return true;
}

bool login(char *username, char *password) {
    FILE *f = fopen(USER_FILE, "r");
    if (!f) return false;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        char user[50], pass[50];
        sscanf(line, "%49[^:]:%49[^\n]", user, pass);
        if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

#define MAX_DESC 200
#define MAX_DUE  11   // "YYYY-MM-DD"
#define DB_FILE  "tasks.csv"

typedef struct {
    int id;
    int done;        // 0 or 1
    int priority;    // 1=High, 2=Med, 3=Low
    char due[MAX_DUE];
    char desc[MAX_DESC];
} Task;

typedef struct {
    Task *items;
    size_t size;
    size_t cap;
    int next_id;
} TaskList;

void init_list(TaskList *list) {
    list->cap = 16;
    list->size = 0;
    list->items = (Task*)malloc(list->cap * sizeof(Task));
    list->next_id = 1;
}

void free_list(TaskList *list) {
    free(list->items);
    list->items = NULL;
    list->size = list->cap = 0;
}

void ensure_capacity(TaskList *list) {
    if (list->size >= list->cap) {
        list->cap *= 2;
        list->items = (Task*)realloc(list->items, list->cap * sizeof(Task));
    }
}
void save_tasks(TaskList *list, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fopen"); return; }
    for (size_t i = 0; i < list->size; ++i) {
        Task *t = &list->items[i];
        fprintf(f, "%d,%d,%d,%s,\"%s\"\n", t->id, t->done, t->priority, t->due, t->desc);
    }
    fclose(f);
}

void load_tasks(TaskList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return; // file doesn't exist yet → first time user

    char line[512];
    int max_id = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0; // remove newline

        Task t;
        t.desc[0] = '\0';
        t.due[0] = '\0';
        t.done = 0;
        t.priority = 3; // default low

        // Try to parse CSV line: id,done,priority,due,"description"
        char desc_raw[MAX_DESC];
        int parsed = sscanf(line, "%d,%d,%d,%10[^,],\"%199[^\"]\"", 
                            &t.id, &t.done, &t.priority, t.due, desc_raw);

        if (parsed == 5) {
            // Description with quotes
            strncpy(t.desc, desc_raw, MAX_DESC - 1);
            t.desc[MAX_DESC - 1] = '\0';
        } else {
            // Try parsing without quotes (everything after 4th comma)
            int n = sscanf(line, "%d,%d,%d,%10[^,],%199[^\n]", 
                           &t.id, &t.done, &t.priority, t.due, desc_raw);
            if (n == 5) {
                strncpy(t.desc, desc_raw, MAX_DESC - 1);
                t.desc[MAX_DESC - 1] = '\0';
            } else if (parsed >= 4) {
                t.desc[0] = '\0'; // no description
            } else {
                continue; // skip invalid line
            }
        }

        ensure_capacity(list);
        list->items[list->size++] = t;
        if (t.id > max_id) max_id = t.id;
    }

    list->next_id = max_id + 1;
    fclose(f);
}


void add_task(TaskList *list, const char *filename) {
    Task t;
    t.id = list->next_id++;
    t.done = 0;

    printf("Description (no '|' pipe char): ");
    fflush(stdout);
    fgets(t.desc, MAX_DESC, stdin);
    t.desc[strcspn(t.desc, "\r\n")] = 0;

    printf("Due date (YYYY-MM-DD or leave blank): ");
    fflush(stdout);
    fgets(t.due, MAX_DUE, stdin);
    t.due[strcspn(t.due, "\r\n")] = 0;

    printf("Priority (1=High,2=Med,3=Low) [3]: ");
    fflush(stdout);
    char buf[8]; fgets(buf, sizeof(buf), stdin);
    int p = atoi(buf);
    if (p < 1 || p > 3) p = 3;
    t.priority = p;

    ensure_capacity(list);
    list->items[list->size++] = t;
    save_tasks(list, filename);
    printf("Added task #%d\n", t.id);
}





void list_tasks(TaskList *list) {
    if (list->size == 0) {
        printf("No tasks yet.\n");
        return;
    }

    printf("\t=================YOUR TO DO TASKS=================\t");
    printf(" \n");    
    // Calculate max widths for each column
    int w_id = 2, w_status = 7, w_pri = 8, w_due = 8, w_desc = 11;
    for (size_t i = 0; i < list->size; ++i) {
        Task *t = &list->items[i];
        int len_desc = strlen(t->desc);
        int len_due = strlen(t->due);
        if (len_desc > w_desc) w_desc = len_desc;
        if (len_due > w_due) w_due = len_due;
    }

    // Print table header
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
        w_id, "----", w_status, "---------", w_pri, "--------", w_due, "--------", w_desc, "----------------");
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
        w_id, "ID", w_status, "Status", w_pri, "Priority", w_due, "Due", w_desc, "Description");
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
        w_id, "----", w_status, "---------", w_pri, "--------", w_due, "--------", w_desc, "----------------");

    // Print each task row
    for (size_t i = 0; i < list->size; ++i) {
        Task *t = &list->items[i];
        const char *status = t->done ? "Done" : "Pending";
        const char *priority = t->priority == 1 ? "High" : (t->priority == 2 ? "Med" : "Low");
        printf("| %-*d | %-*s | %-*s | %-*s | %-*s |\n",
            w_id, t->id,
            w_status, status,
            w_pri, priority,
            w_due, t->due[0] ? t->due : "-",
            w_desc, t->desc);
    }

    // Print table footer
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
        w_id, "----", w_status, "---------", w_pri, "--------", w_due, "--------", w_desc, "----------------");
    printf("Total records: %zu\n\n", list->size);
}

void mark_done(TaskList *list, const char *filename) {
    printf("Enter task ID to mark done: ");
    char buf[16]; fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i].id == id) {
            list->items[i].done = 1;
            save_tasks(list, filename);
            printf("Task #%d marked done.\n", id);
            return;
        }
    }
    printf("Task not found.\n");
}

void delete_task(TaskList *list, const char *filename) {
    printf("Enter task ID to delete: ");
    char buf[16]; fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i].id == id) {
            for (size_t j = i + 1; j < list->size; ++j) {
                list->items[j - 1] = list->items[j];
            }
            list->size--;
            save_tasks(list, filename);
            printf("Task #%d deleted.\n", id);
            return;
        }
    }
    printf("Task not found.\n");
}


void edit_task(TaskList *list, const char *filename) {
    printf("Enter task ID to edit: ");
    char buf[16]; fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);

    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i].id == id) {
            Task *t = &list->items[i];
            printf("Editing task #%d\n", t->id);

            printf("New description (leave blank to keep): ");
            char desc[MAX_DESC]; fgets(desc, sizeof(desc), stdin);
            desc[strcspn(desc, "\r\n")] = 0;
            if (strlen(desc) > 0) strncpy(t->desc, desc, MAX_DESC);
             printf("New due date YYYY-MM-DD (leave blank to keep): ");
            char due[MAX_DUE]; fgets(due, sizeof(due), stdin);
            due[strcspn(due, "\r\n")] = 0;
            if (strlen(due) > 0) strncpy(t->due, due, MAX_DUE);

            printf("New priority 1=High,2=Med,3=Low (leave blank to keep): ");
            char prio[8]; fgets(prio, sizeof(prio), stdin);
            int p = atoi(prio);
            if (p >= 1 && p <= 3) t->priority = p;

            save_tasks(list, filename);
            printf("Task #%d updated.\n", id);
            return;
        }
    }
    printf("Task not found.\n");
}



void show_stats(TaskList *list) {
    int open = 0, done = 0;
    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i].done) done++;
        else open++;
    }
    printf("Stats: %d open, %d done, total %zu tasks.\n", open, done, list->size);
}

// ...existing code above...
int main(void) {
    char username[50], password[50];
    int auth_choice;
    printf("=================Welcome to the To-Do List Interface!=================\n");
    printf("1) Sign Up\n2) Login\nChoice: ");
    scanf("%d", &auth_choice);
    getchar(); // consume newline

    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\r\n")] = 0;

    printf("Password: ");
    get_hidden_input(password, sizeof(password));

    if (auth_choice == 1) {
        if (!signup(username, password)) return 0;
        printf("Now login:\n");
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\r\n")] = 0;
        printf("Password: ");
        get_hidden_input(password, sizeof(password));
    }

    if (!login(username, password)) {
        printf("Login failed.\n");
        return 0;
    }
    printf("Login successful! Welcome, %s.\n", username);

    // Set per-user task file
    char user_task_file[100];
    snprintf(user_task_file, sizeof(user_task_file), "%s_tasks.csv", username);

    TaskList list; init_list(&list); load_tasks(&list, user_task_file);

    for (;;) {
        printf("=================TO-DO LIST ===================\n");
        printf("\n");
        printf("Choose your options:\n");
        printf("1) Add    2) View    3) Done    4) Delete    5) Exit\n");
        printf("6) Edit   7) Stats\n");
        printf("Choice: ");
        char buf[8];
        if (!fgets(buf, sizeof(buf), stdin)) break;
        int choice = atoi(buf);
        switch (choice) {
            case 1: add_task(&list, user_task_file); break;
            case 2: list_tasks(&list); break;
            case 3: mark_done(&list, user_task_file); break;
            case 4: delete_task(&list, user_task_file); break;
            case 5: save_tasks(&list, user_task_file); free_list(&list); return 0;
            case 6: edit_task(&list, user_task_file); break;
            case 7: show_stats(&list); break;
            default: printf("Invalid choice.\n");
        }
    }
    free_list(&list);
    return 0;
}