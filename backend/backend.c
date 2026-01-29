#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Event {
    int eventID;
    char eventName[50];
    char location[50];
    char date[15];
    int seats;
    struct Event *prev, *next;
} Event;

Event *head = NULL;

/* ---------- Utility ---------- */

int dateToInt(const char *date) {
    int d, m, y;
    sscanf(date, "%d-%d-%d", &d, &m, &y);
    return y * 10000 + m * 100 + d;
}

Event* createEvent(int id, char name[], char loc[], char date[], int seats) {
    Event* e = malloc(sizeof(Event));
    e->eventID = id;
    strcpy(e->eventName, name);
    strcpy(e->location, loc);
    strcpy(e->date, date);
    e->seats = seats;
    e->prev = e->next = NULL;
    return e;
}

void removeNode(Event *node) {
    if (node->prev) node->prev->next = node->next;
    else head = node->next;

    if (node->next) node->next->prev = node->prev;
}

/* ---------- Operations ---------- */

void insertEvent(int id, char name[], char loc[], char date[], int seats) {
    Event *newEvent = createEvent(id, name, loc, date, seats);

    if (!head) {
        head = newEvent;
        printf("Event inserted\n");
        return;
    }

    Event *cur = head, *prev = NULL;
    while (cur && dateToInt(cur->date) < dateToInt(date)) {
        prev = cur;
        cur = cur->next;
    }

    if (!prev) {
        newEvent->next = head;
        head->prev = newEvent;
        head = newEvent;
    } else {
        newEvent->next = cur;
        newEvent->prev = prev;
        prev->next = newEvent;
        if (cur) cur->prev = newEvent;
    }

    printf("Event inserted\n");
}

void deleteEvent(int id) {
    Event *cur = head;
    while (cur && cur->eventID != id) cur = cur->next;

    if (!cur) {
        printf("Event not found\n");
        return;
    }

    removeNode(cur);
    free(cur);
    printf("Event deleted\n");
}

void searchEvent(int id) {
    Event *cur = head;
    while (cur && cur->eventID != id) cur = cur->next;

    if (!cur) {
        printf("Event not found\n");
        return;
    }

    printf("%d|%s|%s|%s|%d\n",
           cur->eventID, cur->eventName,
           cur->location, cur->date, cur->seats);
}

void displayForward() {
    Event *cur = head;
    if (!cur) {
        printf("No events\n");
        return;
    }

    while (cur) {
        printf("%d|%s|%s|%s|%d\n",
               cur->eventID, cur->eventName,
               cur->location, cur->date, cur->seats);
        cur = cur->next;
    }
}

/* ---------- MAIN ---------- */

int main() {
    char command[20];
    scanf("%s", command);

    if (strcmp(command, "INSERT") == 0) {
        int id, seats;
        char name[50], loc[50], date[15];
        scanf("%d %s %s %s %d", &id, name, loc, date, &seats);
        insertEvent(id, name, loc, date, seats);
    }

    else if (strcmp(command, "DELETE") == 0) {
        int id;
        scanf("%d", &id);
        deleteEvent(id);
    }

    else if (strcmp(command, "SEARCH") == 0) {
        int id;
        scanf("%d", &id);
        searchEvent(id);
    }

    else if (strcmp(command, "DISPLAY_F") == 0) {
        displayForward();
    }

    return 0;
}
