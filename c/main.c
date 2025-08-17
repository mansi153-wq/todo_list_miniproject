#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESC 200
#define MAX_DUE  11   // "YYYY-MM-DD"
#define DB_FILE  "tasks.db"

typedef struct {
    int id;
    int done;        // 0 or 1
    int priority;    // 1=High, 2=Med, 3=Low (optional)
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

void save_tasks(TaskList *list) {
    FILE *f = fopen(DB_FILE, "w");
    if (!f) { perror("fopen"); return; }
    for (size_t i = 0; i < list->size; ++i) {
        Task *t = &list->items[i];
        // Format: id|done|priority|due|desc
        fprintf(f, "%d|%d|%d|%s|%s\n", t->id, t->done, t->priority, t->due, t->desc);
    }
    fclose(f);
}

void load_tasks(TaskList *list) {
    FILE *f = fopen(DB_FILE, "r");
    if (!f) return; // first run: no file
    char line[512];
    int max_id = 0;
    while (fgets(line, sizeof(line), f)) {
        // Strip newline
        line[strcspn(line, "\r\n")] = 0;
        // Tokenize by '|'
        char *id = strtok(line, "|");
        char *done = strtok(NULL, "|");
        char *prio = strtok(NULL, "|");
        char *due = strtok(NULL, "|");
        char *desc = strtok(NULL, "|");
        if (!id || !done || !prio || !due || !desc) continue;

        Task t;
        t.id = atoi(id);
        t.done = atoi(done);
        t.priority = atoi(prio);
        strncpy(t.due, due, MAX_DUE - 1); t.due[MAX_DUE-1] = '\0';
        strncpy(t.desc, desc, MAX_DESC - 1); t.desc[MAX_DESC-1] = '\0';

        ensure_capacity(list);
        list->items[list->size++] = t;
        if (t.id > max_id) max_id = t.id;
    }
    list->next_id = max_id + 1;
    fclose(f);
}

void add_task(TaskList *list) {
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
    save_tasks(list);
    printf("Added task #%d\n", t.id);
}

void list_tasks(TaskList *list) {
    if (list->size == 0) {
        printf("No tasks yet.\n");
        return;
    }
    printf("\nID  Status  Pri  Due         Description\n");
    printf("==  ======  ===  ==========  ======================================\n");
    for (size_t i = 0; i < list->size; ++i) {
        Task *t = &list->items[i];
        printf("%-3d %-6s  %-3d  %-10s  %s\n",
               t->id, t->done ? "Done" : "Open",
               t->priority, t->due[0] ? t->due : "-", t->desc);
    }
    printf("\n");
}

void mark_done(TaskList *list) {
    printf("Enter task ID to mark done: ");
    char buf[16]; fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i].id == id) {
            list->items[i].done = 1;
            save_tasks(list);
            printf("Task #%d marked done.\n", id);
            return;
        }
    }
    printf("Task not found.\n");
}

void delete_task(TaskList *list) {
    printf("Enter task ID to delete: ");
    char buf[16]; fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i].id == id) {
            for (size_t j = i + 1; j < list->size; ++j) {
                list->items[j - 1] = list->items[j];
            }
            list->size--;
            save_tasks(list);
            printf("Task #%d deleted.\n", id);
            return;
        }
    }
    printf("Task not found.\n");
}

int main(void) {
    TaskList list; init_list(&list); load_tasks(&list);

    for (;;) {
        printf("=== TO-DO CLI ===\n");
        printf("1) Add    2) List    3) Done    4) Delete    5) Exit\n");
        printf("Choice: ");
        char buf[8];
        if (!fgets(buf, sizeof(buf), stdin)) break;
        int choice = atoi(buf);

        switch (choice) {
            case 1: add_task(&list); break;
            case 2: list_tasks(&list); break;
            case 3: mark_done(&list); break;
            case 4: delete_task(&list); break;
            case 5: save_tasks(&list); free_list(&list); return 0;
            default: printf("Invalid choice.\n");
        }
    }
    free_list(&list);
    return 0;
}

