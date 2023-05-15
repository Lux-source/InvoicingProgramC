#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int stock;
    float price;
    char description[150];
} Item;

#define MAX_ITEMS 100

int read_integer(const char *prompt);

float read_float(const char *prompt);

void
invoice_preparation(Item items[], int number_of_items, int delivery_notes[][100 * 2], int *number_of_delivery_notes,
                    int delivery_note_item_counts[]) {
    if (*number_of_delivery_notes == 0) {
        printf("Error: No delivery notes to process. \n");
        return;
    }

    float invoice_total = 0;
    int total_quantity = 0;
    // Printing the header

    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("INVOICE -- WHAREHOUSE ALBACETE --\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    printf("\n%-50s%-15s%-15s%-15s\n", "Description", "Quantity", "Unit Price", "Amount");

    for (int i = 0; i < number_of_items; i++) {
        total_quantity = 0;

        for (int j = 0; j < *number_of_delivery_notes; j++) {
            for (int k = 0; k < delivery_note_item_counts[j]; k++) {
                if (delivery_notes[j][k * 2] == items[i].id) {
                    total_quantity += delivery_notes[j][k * 2 + 1];
                }
            }
        }

        if (total_quantity > 0) {
            float item_amount = total_quantity * items[i].price;
            printf("%-50s%-15d%-15.2f%-15.2f\n", items[i].description, total_quantity, items[i].price, item_amount);
            invoice_total += item_amount;
        }
    }

    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Total invoice amount: %.2f\n", invoice_total);
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    *number_of_delivery_notes = 0; // Reset the delivery note storage to 0

};

void introduce_new_item(Item **items, int *number_of_items) {
    if (*number_of_items >= MAX_ITEMS) {
        printf("Error: Warehouse is full. Cannot add more items.\n");
        return;
    }

    Item new_item;

    new_item.id = read_integer("Enter the item identifier: ");
    new_item.stock = read_integer("Enter the stock: ");
    new_item.price = read_float("Enter the price: ");

    printf("Enter the description: ");
    fgets(new_item.description, 150, stdin);
    new_item.description[strcspn(new_item.description, "\n")] = 0;

    Item *temp = (Item *) realloc(*items, (*number_of_items + 1) * sizeof(Item));
    if (temp == NULL) {
        printf("Memory allocation error. \n");
        return;
    }

    *items = temp;

    (*items)[*number_of_items] = new_item;
    (*number_of_items)++;

};

void introduction_of_delivery_notes(Item items[], int number_of_items, int delivery_notes[][100 * 2],
                                    int *number_of_delivery_notes, int delivery_note_item_counts[]) {
    if (*number_of_delivery_notes >= 5) {
        printf("No more delivery notes can be accepted.\n");
        return;
    }

    int number_of_delivery_items;
    number_of_delivery_items = read_integer("Enter the number of different items: ");

    for (int i = 0; i < number_of_delivery_items; i++) {

        char prompt[50];
        int item_id_valid = 0;

        do {
            sprintf(prompt, "Enter the item identifier for item %d: ", i + 1);
            delivery_notes[*number_of_delivery_notes][i * 2] = read_integer(prompt);

            for (int j = 0; j < number_of_items; j++) {
                if (items[j].id == delivery_notes[*number_of_delivery_notes][i * 2]) {
                    item_id_valid = 1;
                    break;
                }
            }

            if (!item_id_valid) {
                printf("Invalid item ID, try again with a valid ID.\n");
            }
        } while (!item_id_valid);

        sprintf(prompt, "Enter the quantity for item %d: ", i + 1);
        delivery_notes[*number_of_delivery_notes][i * 2 + 1] = read_integer(prompt);

        for (int j = 0; j < number_of_items; j++) {
            if (items[j].id == delivery_notes[*number_of_delivery_notes][i * 2]) {
                if (delivery_notes[*number_of_delivery_notes][i * 2 + 1] > items[j].stock) {
                    delivery_notes[*number_of_delivery_notes][i * 2 + 1] = items[j].stock;
                }
                items[j].stock -= delivery_notes[*number_of_delivery_notes][i * 2 + 1];
                break;
            }
        }
        delivery_note_item_counts[*number_of_delivery_notes] = number_of_delivery_items;
    }

    float total_amount = 0;
    for (int i = 0; i < number_of_delivery_items; i++) {
        for (int j = 0; j < number_of_items; j++) {
            if (items[j].id == delivery_notes[*number_of_delivery_notes][i * 2]) {
                total_amount += items[j].price * delivery_notes[*number_of_delivery_notes][i * 2 + 1];
                break;
            }
        }
    }

    printf("Total amount for the delivery note: %.2f\n", total_amount);
    (*number_of_delivery_notes)++;
}


int read_integer(const char *prompt) {
    int value;
    int status;
    do {
        printf("%s", prompt);
        status = scanf("%d", &value);
        while (getchar() != '\n') {
            // Discard remaining characters in the input buffer
        }
        if (status != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    } while (status != 1);
    return value;
}

float read_float(const char *prompt) {
    float value;
    int status;
    do {
        printf("%s", prompt);
        status = scanf("%f", &value);
        while (getchar() != '\n') {
            //Discard remaining characters in the input buffer
        }
        if (status != 1) {
            printf("Invalid input. Please enter a valid input number.\n");
        }
    } while (status != 1);
    return value;
}

int main() {

    int initial_number_of_items = 11;
    Item *items = (Item *) calloc(initial_number_of_items, sizeof(Item));
    if (items == NULL) {
        printf("Memory allocation error.\n");
        return 1;
    }

    Item initial_items[] = {
            {1,  50, 4.88,  "Carpenter's hammer with fibreglass handle"},
            {2,  4,  8.22,  "Rubber mallet"},
            {3,  10, 11.99, "Combination spanner set"},
            {4,  12, 5.54,  "Adjustable spanner with screw"},
            {5,  20, 6.95,  "Allen key set"},
            {6,  10, 21.99, "Socket spanner set"},
            {7,  15, 18.46, "Precision screwdriver set"},
            {8,  3,  5.99,  "Stripping pliers"},
            {9,  12, 5.63,  "Universal pliers"},
            {10, 7,  18.30, "Pocket digital multimeter"},
            {11, 12, 32.0,  "Multifunction digital multimeter"}
    };

    memcpy(items, initial_items, initial_number_of_items * sizeof(Item));
    int number_of_items = initial_number_of_items;

    int delivery_notes[5][number_of_items * 2]; // [item_id, quantity]
    int delivery_note_item_counts[5]; // Add a new array to store the number of different items in each delivery note
    int number_of_delivery_notes = 0;

    int menu_option = 0;

    while (1) {

        printf("\nMain Menu\n");
        printf("1. Create Delivery Note\n");
        printf("2. Invoice Preparation\n");
        printf("3. Introduce New Item\n");
        printf("4. Exit\n");
        //printf("Enter your option: ");
        menu_option = read_integer("Enter your option: ");

        switch (menu_option) {
            case 1:
                introduction_of_delivery_notes(items, number_of_items, delivery_notes, &number_of_delivery_notes,
                                               delivery_note_item_counts);
                break;
            case 2:
                invoice_preparation(items, number_of_items, delivery_notes, &number_of_delivery_notes,
                                    delivery_note_item_counts);
                break;
            case 3:
                introduce_new_item(&items, &number_of_items);
                break;
            case 4:
                printf("Exiting...\n");
                free(items);
                return 0;
            default:
                printf("Invalid option. Please try again. \n");
                break;
        }
    }
}
