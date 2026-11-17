#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PATIENTS 1000
#define MAX_APPOINTMENTS 2000

#define PATIENT_FILE "patients.dat"
#define APPOINTMENT_FILE "appointments.dat"
#define MEDICAL_FILE "medical_records.txt"

typedef struct {
    int id;
    char name[60];
    int age;
    char gender[20];
    char contact[30];
    char disease[60];
    char bloodGroup[10];
    int followUp; /* 1 = follow-up required, 0 = not required */
} Patient;

typedef struct {
    int id;
    int patientId;
    char doctor[60];
    char date[11]; /* YYYY-MM-DD */
    char time[6];  /* HH:MM */
    char reason[100];
    int active;    /* 1 = scheduled, 0 = cancelled */
} Appointment;

Patient patients[MAX_PATIENTS];
Appointment appointments[MAX_APPOINTMENTS];
int patientCount = 0;
int appointmentCount = 0;

/* ---------- General input and validation ---------- */

void readLine(const char *prompt, char *buffer, size_t size)
{
    size_t length;

    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Discard excess input. */
        }
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\r') {
        buffer[length - 1] = '\0';
    }
}

int readInt(const char *prompt, int minimum, int maximum)
{
    char input[100];
    char *end;
    long value;

    for (;;) {
        readLine(prompt, input, sizeof(input));

        value = strtol(input, &end, 10);
        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (input[0] != '\0' && *end == '\0' &&
            value >= minimum && value <= maximum) {
            return (int)value;
        }

        printf("Please enter a number between %d and %d.\n",
               minimum, maximum);
    }
}

int equalsIgnoreCase(const char *left, const char *right)
{
    while (*left && *right) {
        if (tolower((unsigned char)*left) !=
            tolower((unsigned char)*right)) {
            return 0;
        }
        left++;
        right++;
    }

    return *left == '\0' && *right == '\0';
}

int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int validDate(const char *text)
{
    int year, month, day, consumed = 0;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30,
                         31, 31, 30, 31, 30, 31};

    if (strlen(text) != 10 ||
        sscanf(text, "%4d-%2d-%2d%n", &year, &month, &day, &consumed) != 3 ||
        consumed != 10) {
        return 0;
    }

    if (text[4] != '-' || text[7] != '-') {
        return 0;
    }

    if (year < 1900 || month < 1 || month > 12) {
        return 0;
    }

    if (isLeapYear(year)) {
        daysInMonth[2] = 29;
    }

    return day >= 1 && day <= daysInMonth[month];
}

int validTime(const char *text)
{
    int hour, minute, consumed = 0;

    if (strlen(text) != 5 ||
        sscanf(text, "%2d:%2d%n", &hour, &minute, &consumed) != 2 ||
        consumed != 5) {
        return 0;
    }

    if (text[2] != ':') {
        return 0;
    }

    return hour >= 0 && hour <= 23 &&
           minute >= 0 && minute <= 59;
}

void readDate(const char *prompt, char *dateText, size_t size)
{
    for (;;) {
        readLine(prompt, dateText, size);
        if (validDate(dateText)) {
            return;
        }
        printf("Invalid date. Use YYYY-MM-DD, for example 2026-04-25.\n");
    }
}

void readTime(const char *prompt, char *timeText, size_t size)
{
    for (;;) {
        readLine(prompt, timeText, size);
        if (validTime(timeText)) {
            return;
        }
        printf("Invalid time. Use 24-hour HH:MM format, for example 14:30.\n");
    }
}

/* ---------- File handling ---------- */

void loadData(void)
{
    FILE *file;

    file = fopen(PATIENT_FILE, "rb");
    if (file != NULL) {
        patientCount = (int)fread(patients, sizeof(Patient),
                                  MAX_PATIENTS, file);
        fclose(file);
    }

    file = fopen(APPOINTMENT_FILE, "rb");
    if (file != NULL) {
        appointmentCount = (int)fread(appointments, sizeof(Appointment),
                                      MAX_APPOINTMENTS, file);
        fclose(file);
    }
}

void savePatients(void)
{
    FILE *file = fopen(PATIENT_FILE, "wb");

    if (file == NULL) {
        printf("Error: could not save patient data.\n");
        return;
    }

    fwrite(patients, sizeof(Patient), (size_t)patientCount, file);
    fclose(file);
}

void saveAppointments(void)
{
    FILE *file = fopen(APPOINTMENT_FILE, "wb");

    if (file == NULL) {
        printf("Error: could not save appointment data.\n");
        return;
    }

    fwrite(appointments, sizeof(Appointment),
           (size_t)appointmentCount, file);
    fclose(file);
}

/* ---------- Patient management ---------- */

int patientIndexById(int id)
{
    int i;

    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }

    return -1;
}

void printPatient(const Patient *patient)
{
    printf("ID: %d | Name: %s | Age: %d | Gender: %s | "
           "Contact: %s | Disease: %s | Blood group: %s | "
           "Follow-up: %s\n",
           patient->id,
           patient->name,
           patient->age,
           patient->gender,
           patient->contact,
           patient->disease,
           patient->bloodGroup,
           patient->followUp ? "Yes" : "No");
}

void addPatient(void)
{
    Patient patient;

    if (patientCount >= MAX_PATIENTS) {
        printf("Patient storage is full.\n");
        return;
    }

    memset(&patient, 0, sizeof(patient));

    patient.id = 1;
    if (patientCount > 0) {
        int i;
        for (i = 0; i < patientCount; i++) {
            if (patients[i].id >= patient.id) {
                patient.id = patients[i].id + 1;
            }
        }
    }

    printf("\n--- Add Patient ---\n");
    readLine("Name: ", patient.name, sizeof(patient.name));
    while (patient.name[0] == '\0') {
        printf("Name cannot be empty.\n");
        readLine("Name: ", patient.name, sizeof(patient.name));
    }

    patient.age = readInt("Age (0-130): ", 0, 130);
    readLine("Gender: ", patient.gender, sizeof(patient.gender));
    readLine("Contact: ", patient.contact, sizeof(patient.contact));
    readLine("Disease/category: ", patient.disease, sizeof(patient.disease));
    readLine("Blood group: ", patient.bloodGroup, sizeof(patient.bloodGroup));
    patient.followUp = readInt("Follow-up required? (1=yes, 0=no): ", 0, 1);

    patients[patientCount++] = patient;
    savePatients();

    printf("Patient added successfully. Patient ID: %d\n", patient.id);
}

void viewPatients(void)
{
    int i;

    printf("\n--- Patient List ---\n");
    if (patientCount == 0) {
        printf("No patient records found.\n");
        return;
    }

    for (i = 0; i < patientCount; i++) {
        printPatient(&patients[i]);
    }
}

void searchPatientById(void)
{
    int id = readInt("Enter patient ID: ", 1, 1000000000);
    int index = patientIndexById(id);

    if (index < 0) {
        printf("Patient not found.\n");
        return;
    }

    printPatient(&patients[index]);
}

void updatePatient(void)
{
    int id = readInt("Enter patient ID to update: ", 1, 1000000000);
    int index = patientIndexById(id);
    Patient *patient;

    if (index < 0) {
        printf("Patient not found.\n");
        return;
    }

    patient = &patients[index];
    printf("Enter the updated patient details.\n");

    readLine("Name: ", patient->name, sizeof(patient->name));
    while (patient->name[0] == '\0') {
        printf("Name cannot be empty.\n");
        readLine("Name: ", patient->name, sizeof(patient->name));
    }

    patient->age = readInt("Age (0-130): ", 0, 130);
    readLine("Gender: ", patient->gender, sizeof(patient->gender));
    readLine("Contact: ", patient->contact, sizeof(patient->contact));
    readLine("Disease/category: ", patient->disease, sizeof(patient->disease));
    readLine("Blood group: ", patient->bloodGroup, sizeof(patient->bloodGroup));
    patient->followUp = readInt("Follow-up required? (1=yes, 0=no): ", 0, 1);

    savePatients();
    printf("Patient updated.\n");
}

void deletePatient(void)
{
    int id = readInt("Enter patient ID to delete: ", 1, 1000000000);
    int index = patientIndexById(id);
    int i;

    if (index < 0) {
        printf("Patient not found.\n");
        return;
    }

    for (i = index; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }
    patientCount--;

    /* Keep appointment history, but mark the patient's appointments cancelled. */
    for (i = 0; i < appointmentCount; i++) {
        if (appointments[i].patientId == id) {
            appointments[i].active = 0;
        }
    }

    savePatients();
    saveAppointments();
    printf("Patient deleted. Their appointments were cancelled.\n");
}

void displayPatientsByAgeOrDisease(void)
{
    int choice;
    int i;

    printf("\n1. Display patients in an age range\n");
    printf("2. Display patients by disease/category\n");
    choice = readInt("Choose: ", 1, 2);

    if (choice == 1) {
        int minimumAge = readInt("Minimum age: ", 0, 130);
        int maximumAge = readInt("Maximum age: ", minimumAge, 130);
        int found = 0;

        for (i = 0; i < patientCount; i++) {
            if (patients[i].age >= minimumAge &&
                patients[i].age <= maximumAge) {
                printPatient(&patients[i]);
                found = 1;
            }
        }

        if (!found) {
            printf("No patients found in that age range.\n");
        }
    } else {
        char category[60];
        int found = 0;

        readLine("Disease/category: ", category, sizeof(category));

        for (i = 0; i < patientCount; i++) {
            if (equalsIgnoreCase(patients[i].disease, category)) {
                printPatient(&patients[i]);
                found = 1;
            }
        }

        if (!found) {
            printf("No patients found in that category.\n");
        }
    }
}

/* ---------- Appointment management ---------- */

int hasAppointmentConflict(int patientId, const char *doctor,
                           const char *dateText, const char *timeText)
{
    int i;

    for (i = 0; i < appointmentCount; i++) {
        Appointment *appointment = &appointments[i];

        if (!appointment->active ||
            strcmp(appointment->date, dateText) != 0 ||
            strcmp(appointment->time, timeText) != 0) {
            continue;
        }

        /* Prevent both patient and doctor from being double-booked. */
        if (appointment->patientId == patientId ||
            equalsIgnoreCase(appointment->doctor, doctor)) {
            return 1;
        }
    }

    return 0;
}

void scheduleAppointment(void)
{
    Appointment appointment;
    char dateText[11];
    char timeText[6];

    if (appointmentCount >= MAX_APPOINTMENTS) {
        printf("Appointment storage is full.\n");
        return;
    }

    printf("\n--- Schedule Appointment ---\n");
    appointment.patientId =
        readInt("Patient ID: ", 1, 1000000000);

    if (patientIndexById(appointment.patientId) < 0) {
        printf("Patient not found. Add the patient first.\n");
        return;
    }

    readLine("Doctor name: ", appointment.doctor, sizeof(appointment.doctor));
    if (appointment.doctor[0] == '\0') {
        printf("Doctor name cannot be empty.\n");
        return;
    }

    readDate("Appointment date (YYYY-MM-DD): ", dateText, sizeof(dateText));
    readTime("Appointment time (HH:MM): ", timeText, sizeof(timeText));

    if (hasAppointmentConflict(appointment.patientId, appointment.doctor,
                               dateText, timeText)) {
        printf("Conflict detected: the patient or doctor already has an "
               "appointment at that date and time.\n");
        return;
    }

    appointment.id = 1;
    if (appointmentCount > 0) {
        int i;
        for (i = 0; i < appointmentCount; i++) {
            if (appointments[i].id >= appointment.id) {
                appointment.id = appointments[i].id + 1;
            }
        }
    }

    strcpy(appointment.date, dateText);
    strcpy(appointment.time, timeText);
    readLine("Reason: ", appointment.reason, sizeof(appointment.reason));
    appointment.active = 1;

    appointments[appointmentCount++] = appointment;
    saveAppointments();

    printf("Appointment scheduled. Appointment ID: %d\n", appointment.id);
}

void printAppointment(const Appointment *appointment)
{
    int patientIndex = patientIndexById(appointment->patientId);
    const char *patientName =
        patientIndex >= 0 ? patients[patientIndex].name : "[deleted patient]";

    printf("Appointment ID: %d | Patient ID: %d (%s) | Doctor: %s | "
           "Date: %s | Time: %s | Reason: %s | Status: %s\n",
           appointment->id,
           appointment->patientId,
           patientName,
           appointment->doctor,
           appointment->date,
           appointment->time,
           appointment->reason,
           appointment->active ? "Scheduled" : "Cancelled");
}

void viewAppointments(void)
{
    int i;

    printf("\n--- All Appointments ---\n");
    if (appointmentCount == 0) {
        printf("No appointments found.\n");
        return;
    }

    for (i = 0; i < appointmentCount; i++) {
        printAppointment(&appointments[i]);
    }
}

void cancelAppointment(void)
{
    int id = readInt("Appointment ID to cancel: ", 1, 1000000000);
    int i;

    for (i = 0; i < appointmentCount; i++) {
        if (appointments[i].id == id) {
            if (!appointments[i].active) {
                printf("This appointment is already cancelled.\n");
                return;
            }

            appointments[i].active = 0;
            saveAppointments();
            printf("Appointment cancelled.\n");
            return;
        }
    }

    printf("Appointment not found.\n");
}

void searchAppointmentsByPatient(void)
{
    int patientId = readInt("Patient ID: ", 1, 1000000000);
    int i;
    int found = 0;

    for (i = 0; i < appointmentCount; i++) {
        if (appointments[i].patientId == patientId) {
            printAppointment(&appointments[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No appointments found for that patient.\n");
    }
}

void displayDoctorAppointments(void)
{
    char doctor[60];
    int i;
    int found = 0;

    readLine("Doctor name: ", doctor, sizeof(doctor));

    for (i = 0; i < appointmentCount; i++) {
        if (equalsIgnoreCase(appointments[i].doctor, doctor)) {
            printAppointment(&appointments[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No appointments found for that doctor.\n");
    }
}

/* ---------- Health analysis and reports ---------- */

void showFollowUpPatients(void)
{
    int i;
    int found = 0;

    printf("\n--- Patients Requiring Follow-up ---\n");
    for (i = 0; i < patientCount; i++) {
        if (patients[i].followUp) {
            printPatient(&patients[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No patients currently require follow-up.\n");
    }
}

void showHealthAnalysis(void)
{
    int i;
    int totalAge = 0;
    int bestCount = 0;
    int bestIndex = -1;

    if (patientCount == 0) {
        printf("No patient data available for analysis.\n");
        return;
    }

    for (i = 0; i < patientCount; i++) {
        int j;
        int count = 0;

        totalAge += patients[i].age;

        if (patients[i].disease[0] == '\0') {
            continue;
        }

        for (j = 0; j < patientCount; j++) {
            if (equalsIgnoreCase(patients[i].disease,
                                 patients[j].disease)) {
                count++;
            }
        }

        if (count > bestCount) {
            bestCount = count;
            bestIndex = i;
        }
    }

    printf("Average patient age: %.2f\n",
           (double)totalAge / patientCount);

    if (bestIndex >= 0) {
        printf("Most common disease/category: %s (%d patient%s)\n",
               patients[bestIndex].disease,
               bestCount,
               bestCount == 1 ? "" : "s");
    } else {
        printf("No disease/category information has been recorded.\n");
    }
}

int comparePatientsByAge(const void *a, const void *b)
{
    const Patient *left = (const Patient *)a;
    const Patient *right = (const Patient *)b;

    if (left->age < right->age) return -1;
    if (left->age > right->age) return 1;
    return 0;
}

void listPatientsSortedByAge(void)
{
    Patient copy[MAX_PATIENTS];
    int i;

    if (patientCount == 0) {
        printf("No patient records found.\n");
        return;
    }

    memcpy(copy, patients, (size_t)patientCount * sizeof(Patient));
    qsort(copy, (size_t)patientCount, sizeof(Patient),
          comparePatientsByAge);

    printf("\n--- Patients Sorted by Age ---\n");
    for (i = 0; i < patientCount; i++) {
        printPatient(&copy[i]);
    }
}

void showMedicalNotes(int patientId)
{
    FILE *file = fopen(MEDICAL_FILE, "r");
    char line[512];
    int found = 0;

    if (file == NULL) {
        printf("No medical notes have been stored yet.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *firstSeparator = strchr(line, '|');
        char *secondSeparator;
        long recordPatientId;

        if (firstSeparator == NULL) {
            continue;
        }

        *firstSeparator = '\0';
        recordPatientId = strtol(line, NULL, 10);

        if (recordPatientId != patientId) {
            continue;
        }

        secondSeparator = strchr(firstSeparator + 1, '|');
        if (secondSeparator == NULL) {
            continue;
        }

        *secondSeparator = '\0';
        printf("  %s: %s",
               firstSeparator + 1,
               secondSeparator + 1);

        if (strchr(secondSeparator + 1, '\n') == NULL) {
            printf("\n");
        }
        found = 1;
    }

    fclose(file);

    if (!found) {
        printf("No medical notes found for this patient.\n");
    }
}

void addMedicalNote(void)
{
    int patientId = readInt("Patient ID: ", 1, 1000000000);
    int index = patientIndexById(patientId);
    char note[300];
    char timestamp[30];
    time_t now;
    struct tm *local;
    FILE *file;
    size_t i;

    if (index < 0) {
        printf("Patient not found.\n");
        return;
    }

    readLine("Medical note: ", note, sizeof(note));
    if (note[0] == '\0') {
        printf("Note cannot be empty.\n");
        return;
    }

    /* Keep the record format readable by replacing delimiter characters. */
    for (i = 0; note[i] != '\0'; i++) {
        if (note[i] == '|') {
            note[i] = '/';
        }
    }

    now = time(NULL);
    local = localtime(&now);
    if (local == NULL) {
        strcpy(timestamp, "unknown time");
    } else {
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M", local);
    }

    file = fopen(MEDICAL_FILE, "a");
    if (file == NULL) {
        printf("Could not open the medical record file.\n");
        return;
    }

    fprintf(file, "%d|%s|%s\n", patientId, timestamp, note);
    fclose(file);

    printf("Medical note saved for %s.\n", patients[index].name);
}

void patientHealthStatusReport(void)
{
    int patientId = readInt("Patient ID for report: ", 1, 1000000000);
    int index = patientIndexById(patientId);
    int i;
    int hasAppointments = 0;

    if (index < 0) {
        printf("Patient not found.\n");
        return;
    }

    printf("\n========== PATIENT HEALTH STATUS REPORT ==========\n");
    printPatient(&patients[index]);

    printf("\nAppointments:\n");
    for (i = 0; i < appointmentCount; i++) {
        if (appointments[i].patientId == patientId) {
            printAppointment(&appointments[i]);
            hasAppointments = 1;
        }
    }

    if (!hasAppointments) {
        printf("No appointments recorded.\n");
    }

    printf("\nMedical notes:\n");
    showMedicalNotes(patientId);
    printf("===================================================\n");
}

/* ---------- Login and main menu ---------- */

int login(void)
{
    char username[50];
    char password[50];
    int attempt;

    printf("=== Health Care Center Login ===\n");

    for (attempt = 1; attempt <= 3; attempt++) {
        readLine("Username: ", username, sizeof(username));
        readLine("Password: ", password, sizeof(password));

        if (strcmp(username, "admin") == 0 &&
            strcmp(password, "admin123") == 0) {
            printf("Login successful.\n");
            return 1;
        }

        printf("Incorrect username or password. Attempts remaining: %d\n",
               3 - attempt);
    }

    return 0;
}

void showMenu(void)
{
    printf("\n========== HEALTH CARE CENTER ==========\n");
    printf(" 1. Add patient\n");
    printf(" 2. View all patients\n");
    printf(" 3. Search patient by ID\n");
    printf(" 4. Update patient\n");
    printf(" 5. Delete patient\n");
    printf(" 6. Display patients by age/category\n");
    printf(" 7. Schedule appointment\n");
    printf(" 8. View appointments\n");
    printf(" 9. Cancel appointment\n");
    printf("10. Search appointments by patient ID\n");
    printf("11. Display appointments by doctor\n");
    printf("12. Show patients requiring follow-up\n");
    printf("13. Health analysis (average age/common disease)\n");
    printf("14. Sort patients by age\n");
    printf("15. Patient health status report\n");
    printf("16. Add medical note\n");
    printf(" 0. Exit\n");
}

int main(void)
{
    int choice;

    if (!login()) {
        printf("Too many failed login attempts. Exiting.\n");
        return 1;
    }

    loadData();

    do {
        showMenu();
        choice = readInt("Choose an option: ", 0, 16);

        switch (choice) {
            case 1:  addPatient(); break;
            case 2:  viewPatients(); break;
            case 3:  searchPatientById(); break;
            case 4:  updatePatient(); break;
            case 5:  deletePatient(); break;
            case 6:  displayPatientsByAgeOrDisease(); break;
            case 7:  scheduleAppointment(); break;
            case 8:  viewAppointments(); break;
            case 9:  cancelAppointment(); break;
            case 10: searchAppointmentsByPatient(); break;
            case 11: displayDoctorAppointments(); break;
            case 12: showFollowUpPatients(); break;
            case 13: showHealthAnalysis(); break;
            case 14: listPatientsSortedByAge(); break;
            case 15: patientHealthStatusReport(); break;
            case 16: addMedicalNote(); break;
            case 0:  printf("Goodbye.\n"); break;
        }
    } while (choice != 0);

    return 0;
}