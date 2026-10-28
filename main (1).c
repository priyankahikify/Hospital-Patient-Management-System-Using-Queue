/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define ID_LENGTH 20
#define NAME_LENGTH 50

typedef struct {
    char id[ID_LENGTH];
    char name[NAME_LENGTH];
    int age;
} Patient;

static Patient queue[MAX_PATIENTS];
static int front = 0;
static int rear = -1;
static int count = 0;

/* Add a patient to the queue */
int add_patient(const char *id, const char *name, int age)
{
    if (id == NULL || name == NULL)
        return -2;

    if (count >= MAX_PATIENTS)
        return 0;

    if (age < 0 || strlen(id) == 0 || strlen(name) == 0)
        return -2;

    for (int i = 0; i < count; i++) {
        int index = (front + i) % MAX_PATIENTS;

        if (strcmp(queue[index].id, id) == 0)
            return -1;
    }

    rear = (rear + 1) % MAX_PATIENTS;

    snprintf(queue[rear].id, ID_LENGTH, "%s", id);
    snprintf(queue[rear].name, NAME_LENGTH, "%s", name);
    queue[rear].age = age;

    count++;
    return 1;
}

/* Serve the next patient */
int serve_next_patient(Patient *served)
{
    if (count == 0 || served == NULL)
        return 0;

    *served = queue[front];

    front = (front + 1) % MAX_PATIENTS;
    count--;

    if (count == 0) {
        front = 0;
        rear = -1;
    }

    return 1;
}

/* Remove a patient using Patient ID */
int remove_patient(const char *id, Patient *removed)
{
    if (id == NULL || removed == NULL || count == 0)
        return 0;

    int position = -1;

    for (int i = 0; i < count; i++) {
        int index = (front + i) % MAX_PATIENTS;

        if (strcmp(queue[index].id, id) == 0) {
            position = i;
            break;
        }
    }

    if (position == -1)
        return 0;

    int index = (front + position) % MAX_PATIENTS;
    *removed = queue[index];

    /* Shift the following patients forward */
    for (int i = position; i < count - 1; i++) {
        int current = (front + i) % MAX_PATIENTS;
        int next = (front + i + 1) % MAX_PATIENTS;

        queue[current] = queue[next];
    }

    count--;

    if (count == 0) {
        front = 0;
        rear = -1;
    } else {
        rear = (front + count - 1) % MAX_PATIENTS;
    }

    return 1;
}

/* Return the number of waiting patients */
int get_patient_count(void)
{
    return count;
}

/* Get a patient by queue position */
int get_patient_at(int position, Patient *result)
{
    if (position < 0 || position >= count || result == NULL)
        return 0;

    int index = (front + position) % MAX_PATIENTS;
    *result = queue[index];

    return 1;
}

/* Main program */
int main(void)
{
    int choice, age, status;
    char id[ID_LENGTH];
    char name[NAME_LENGTH];
    Patient served, p, removed;

    while (1) {
        printf("\n===== HOSPITAL PATIENT QUEUE =====\n");
        printf("1. Add Patient\n");
        printf("2. Serve Next Patient\n");
        printf("3. Display Waiting Patients\n");
        printf("4. Display Patient Count\n");
        printf("5. Remove Patient\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter Patient ID: ");
                scanf("%19s", id);

                printf("Enter Patient Name: ");
                scanf(" %49[^\n]", name);

                printf("Enter Patient Age: ");
                if (scanf("%d", &age) != 1) {
                    printf("Invalid age input.\n");
                    return 1;
                }

                status = add_patient(id, name, age);

                if (status == 1)
                    printf("Patient added successfully!\n");
                else if (status == -1)
                    printf("Patient ID already exists.\n");
                else if (status == 0)
                    printf("Queue is full.\n");
                else
                    printf("Invalid patient details.\n");

                break;

            case 2:
                if (serve_next_patient(&served)) {
                    printf("\nPatient is being served:\n");
                    printf("Patient ID: %s\n", served.id);
                    printf("Patient Name: %s\n", served.name);
                    printf("Patient Age: %d\n", served.age);
                } else {
                    printf("No patients waiting.\n");
                }
                break;

            case 3:
                if (get_patient_count() == 0) {
                    printf("No patients waiting.\n");
                } else {
                    printf("\n--- Waiting Patients ---\n");

                    for (int i = 0; i < get_patient_count(); i++) {
                        if (get_patient_at(i, &p)) {
                            printf("\nPatient ID: %s\n", p.id);
                            printf("Name: %s\n", p.name);
                            printf("Age: %d\n", p.age);
                        }
                    }
                }
                break;

            case 4:
                printf("Total patients waiting: %d\n",
                       get_patient_count());
                break;

            case 5:
                printf("Enter Patient ID to remove: ");
                scanf("%19s", id);

                if (remove_patient(id, &removed)) {
                    printf("\nPatient removed successfully!\n");
                    printf("Patient ID: %s\n", removed.id);
                    printf("Patient Name: %s\n", removed.name);
                    printf("Patient Age: %d\n", removed.age);
                } else {
                    printf("Patient not found or queue is empty.\n");
                }
                break;

            case 6:
                printf("Thank you! Program ended.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}