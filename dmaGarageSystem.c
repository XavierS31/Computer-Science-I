/*
 * COP3502 Assignment 1
 * This program is written by: Xavier A. Soto Baron
 * 09/08/2024

 Description:
   This program implements a dynamic parking monitoring system for Monster University.
   It tracks registered vehicles and garages using dynamic memory allocation and pointer arrays.
   The system supports commands to park, relocate, and remove vehicles, generate utilization reports,
   register new vehicles, and resize or remove garages.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 22

typedef struct RegisteredVehicle{ //for one registered vehicle
    char* license_plate; //to be used for string license plate# of the vehicle
    char* owner_name; //to be used for storing owner name
} RegisteredVehicle;

typedef struct Garage{ //for one garage
    char* garage_name; //to be used for garage name
    int total_capacity; //total capacity of the garage
    int current_count; // number of vehicles in the garage in a particular time
    RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct Campus{
    Garage** garages; //list of garages in campus
    int total_garages; //number of garages in the campus
    RegisteredVehicle** registered_vehicles; //list of registered vehicles
    int total_registered_vehicles; //total number of registered vehicles
} Campus;

Garage* createGarage(const char* name, int capacity);

RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner);

void registerVehicle(Campus* campus, const char* license, const char* owner);

void parkVehicle(Garage* garage, RegisteredVehicle* vehicle);

int removeVehicleFromGarage(Garage* garage, const char* license);

RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license);

int countTotalVehicles(const Campus* campus);

int resizeGarage(Garage* garage, int new_capacity);

int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name);

void displayVehiclesByOwner(const Campus* campus, const char* owner_name);

int removeGarage(Campus* campus, const char* garage_name);

void generateGarageUtilizationReport(const Campus* campus);

void freeCampusMemory(Campus* campus);

Garage* findGarageByVehicle(const Campus *campus, const char *license);

Garage* findGarageByName(const Campus *campus, const char *garage_name);

int main(){
    //Create and allocate campus Pointer
    Campus* campus = malloc(sizeof(Campus));
    //initialize variables to 0 that keep track
    campus->total_garages = 0;
    campus->total_registered_vehicles = 0;

    //Declare and initialize variables for # garages, vehicles, and commands
    int g=0, v=0, c=0;

    //Scan the number of garages, vehicles and commands from user
    scanf("%d %d %d", &g, &v, &c);

    //Allocate memory for garage name and capacity
    char* garageName = malloc(MAX_STRING_SIZE*sizeof(char));
    int garageCapacity;

    //Pointer to garages and allocates memory
    campus->garages = (Garage**) malloc(g*sizeof(Garage*));

    //ForLoop through the number of garages
    for (int i = 0; i <g; i++) {
        //Scan the garage name and capacity
        scanf("%s %d", garageName, &garageCapacity);

        //Creates the garage with the given data and adds it to the array of garages
        campus->garages[i] = createGarage(garageName, garageCapacity);
        campus->total_garages++;
    }

    //Frees memory for garage name
    free(garageName);

    //Declares and Allocate memory for license plate and owner name
    char *LicensePlate = malloc(MAX_STRING_SIZE*sizeof(char));
    char *OwnerName=malloc(MAX_STRING_SIZE*sizeof(char));

    //Pointer to register vehicles and Allocates memory for all the vehicles
    campus->registered_vehicles = (RegisteredVehicle**) malloc(v*sizeof(RegisteredVehicle*));

    //For Loop to scan the number of vehicles inputted by user
    for(int i=0; i<v; i++){
        //Scan the license plate and owner name for each vehicle
        scanf("%s %s", LicensePlate, OwnerName);

        //Register vehicles with given license plate and Owner name and adds it to the array of register vehicles
        campus->registered_vehicles[i] = createRegisteredVehicle(LicensePlate, OwnerName);
        //Increase count of vehicles by one to update # of vehicles
        campus->total_registered_vehicles++;
    }

    //Free memory for License Plate and Owner Name
    free(LicensePlate);
    free(OwnerName);

    //Declares Allocate memory for the commands
    char* commands = malloc(MAX_STRING_SIZE*sizeof(char));

    //For Loop through all the number of commands
    for(int i=0; i<c; i++){
        //Scan the command from the user
        scanf("%s", commands);

        //Compare inputted command to the possible commands
        if (strcmp(commands, "PARK") == 0) {

            //Allocates memory for future possible inputs and variables
            char *license_plate = malloc(MAX_STRING_SIZE*sizeof(char));
            char *garage_name = malloc(MAX_STRING_SIZE*sizeof(char));

            //Scan License Plate and garage name
            scanf("%s %s", license_plate, garage_name);

            //Find garage by its name
            Garage* garage = findGarageByName(campus, garage_name);

            //find vehicle by its license
            RegisteredVehicle* vehicle = searchVehicleByLicense(campus, license_plate);

            //Park the car into the given garage
            parkVehicle(garage, vehicle);

            //Frees memory one more time for License plate and garage name
            free(license_plate);
            free(garage_name);

            //beginning of alternatives for commands
        } else if (strcmp(commands, "UTILIZATION_REPORT") == 0) {
            //calls function and Prints utilization report
            generateGarageUtilizationReport(campus);

            //command to resize garage
        } else if (strcmp(commands, "RESIZE") == 0) {

            //Allocate memory for garage name and new garage size for future functions
            char *garage_name = malloc(MAX_STRING_SIZE*sizeof(char));
            int* new_size = malloc(sizeof(int));

            //Scan the garage name and the new capacity
            scanf("%s %d", garage_name, new_size);

            //Find garage by the given name
            Garage* garage = findGarageByName(campus, garage_name);

            //calls function to resize garage
            resizeGarage(garage, *new_size);

            //Free memory for garage name and new size
            free(garage_name);
            free(new_size);

            //command to search owner
        } else if (strcmp(commands, "SEARCH_OWNER") == 0) {

            //Allocate memory for owner name
            char* owner_name = malloc(MAX_STRING_SIZE*sizeof(char));

            //Scan owner's name
            scanf("%s", owner_name);

            //Calls function to Print all vehicles by owner
            displayVehiclesByOwner(campus, owner_name);

            //Free memory for Owner name
            free(owner_name);

            //Command to Relocate vehicle
        } else if (strcmp(commands, "RELOCATE") == 0) {

            //Allocate memory for License plate and garage Name as they will be required
            char *license_plate = malloc(MAX_STRING_SIZE*sizeof(char));
            char *garage_name = malloc(MAX_STRING_SIZE*sizeof(char));

            //Scan the License Plate and garage name
            scanf("%s %s", license_plate, garage_name);

            //Calls function to relocate the vehicle to a new garage
            relocateVehicle(campus, license_plate, garage_name);

            //Free memory for License plate and Garage name as they were DMA above
            free(license_plate);
            free(garage_name);

            //command to count the total vehicles
        } else if (strcmp(commands, "COUNT_TOTAL") == 0) {
            //Declare and Allocate memory for total Vehicles
            int *totalVehicles = malloc(sizeof(int));

            //Calls function to Find total number of vehicles
            *totalVehicles = countTotalVehicles(campus);

            //Print total
            printf("%d\n", *totalVehicles);

            //Free memory for the total number of vehicle variables used above
            free(totalVehicles);

            //Command to register vehicle
        } else if (strcmp(commands, "REGISTER_VEHICLE") == 0) {

            //Allocate memory for License Plate and Owner name as they will be used
            char *license_plate = malloc(MAX_STRING_SIZE*sizeof(char));
            char* owner_name = malloc(MAX_STRING_SIZE*sizeof(char));

            //Scan the License plate and Owner Name from the user
            scanf("%s %s", license_plate, owner_name);

            //Calls Function to Register the vehicle with given license plate and owner name on campus
            registerVehicle(campus, license_plate, owner_name);

            //Free the memory that was used for License plate and Owner Name
            free(license_plate);
            free(owner_name);

            //Command to remove a vehicle from a garage
        } else if (strcmp(commands, "REMOVE_VEHICLE_GARAGE") == 0) {

            //Allocate memory for License plate as it's going to be used
            char *license_plate = malloc(MAX_STRING_SIZE*sizeof(char));

            //Scan the License Plate of the vehicle
            scanf("%s", license_plate);

            //Calls function to Find the garage by the vehicle
            Garage* garage = findGarageByVehicle(campus, license_plate);

            if(garage){
                //Remove the given vehicle from the garage
                int x = removeVehicleFromGarage(garage, license_plate);

                //Check if the removal was successful or not
                if(x){
                    //print removed
                    printf("REMOVED FROM %s\n", garage->garage_name);
                } else{
                    //Print fail to alert user
                    printf("NOT FOUND IN CAMPUS\n");
                }
            } else{
                //Print Not found in campus to alert uer
                printf("NOT FOUND IN CAMPUS\n");
            }

            //Free memory for used variable for License Plate
            free(license_plate);

            //command to remove garage from campus
        } else if (strcmp(commands, "REMOVE_GARAGE") == 0) {
            //Allocate memory for garage name as its going to be used
            char *garage_name = malloc(MAX_STRING_SIZE*sizeof(char));

            //Scan the garage name from user
            scanf("%s", garage_name);

            //call function to remove garage from campus with given garage name
            int x = removeGarage(campus, garage_name);

            //Check if removal was successful or not
            if(x){
                //prints removed for a successful removal
                printf("%s REMOVED\n", garage_name);
            } else{
                //Print if garage not found to alert user
                printf("%s NOT FOUND\n", garage_name);
            }

            //Free memory for the variable garage name used above
            free(garage_name);
        }
    }

    //Free memory used for the commands
    free(commands);

    //Calls function to free the memory of all the program
    freeCampusMemory(campus);

    return 0;
}


//Function to Create garage with name, capacity and current count 0.
Garage *createGarage(const char *name, int capacity) {
    //Allocates memory for 1 garage
    Garage* garage = (Garage*)malloc(sizeof(Garage));

    //Allocates memory for garage name
    garage->garage_name =  malloc(MAX_STRING_SIZE*sizeof(char));

    //Sets the name, capacity and current count of garage
    strcpy(garage->garage_name,name);
    garage->total_capacity = capacity;
    garage->current_count = 0;

    //Allocates memory for an array of vehicles size capacity
    garage->parked_vehicles = (RegisteredVehicle**)malloc(capacity * sizeof(RegisteredVehicle*));

    //Checks for proper memory allocation
    if (garage->parked_vehicles == NULL) {
        fprintf(stderr, "Failed to allocate memory for parked vehicles\n");
        exit(1);
    }

    //Returns the pointer to the garage
    return garage;
}

//Function to Create and return a registered vehicle with license plate and owner
RegisteredVehicle *createRegisteredVehicle(const char *license, const char *owner) {
    //Allocates memory for one registered vehicle
    RegisteredVehicle* vehicle = (RegisteredVehicle*)malloc(sizeof(RegisteredVehicle));

    //Creates and sets the license plate and owner to the vehicle
    vehicle->license_plate = strdup(license);
    vehicle->owner_name = strdup(owner);

    //Returns pointer to the registered vehicle
    return vehicle;
}

//Function to Register a vehicle in the campus
void registerVehicle(Campus *campus, const char *license, const char *owner) {
    //Creates a registered vehicle calling the function to create register vehicle
    RegisteredVehicle* vehicle = createRegisteredVehicle(license, owner);

    //Increases the allocated memory using realloc for registered vehicles to fit another vehicle
    campus->registered_vehicles = realloc(campus->registered_vehicles,
                                          (campus->total_registered_vehicles + 1) * sizeof(RegisteredVehicle*));

    //New register vehicle will be added to the array
    campus->registered_vehicles[campus->total_registered_vehicles] = vehicle;

    //update count of registered vehicles
    campus->total_registered_vehicles++;

    //print registered to inform usder
    printf("REGISTERED\n");
}

//Function to Park a registered vehicle in a garage
void parkVehicle(Garage *garage, RegisteredVehicle *vehicle) {

    //checks for proper memory allocation
    if (garage == NULL) {
        fprintf(stderr, "Failed to allocate memory for garage\n");
        exit(1);
    }

    //checks for proper memory allocation for pointer to parked vehicles
    if (garage->parked_vehicles == NULL) {
        fprintf(stderr, "Garage parking space not initialized\n");
        return;
    }

    //checks if the garage is full
    if (garage->current_count == garage->total_capacity) {
        //Prints FULL is the garage is full
        printf("FULL\n");
    } else {
        //If the garage has space, add vehicle to the array
        garage->parked_vehicles[garage->current_count] = vehicle;

        //Increase count of the parked vehicles
        garage->current_count++;

        //prints PARKED to inform user
        printf("PARKED\n");
    }
}

//Function to remove a vehicle from the garage
//If Its successful, returns 1, if not, returns 0
int removeVehicleFromGarage(Garage *garage, const char *license) {
    //For Loop through the vehicles in the garage
    for (int i = 0; i < garage->current_count; i++) {

        //Compare license plates inside a garage to find the given vehicle
        if (strcmp(garage->parked_vehicles[i]->license_plate, license) == 0) {


            //Find vehicle in garage and remove it, moving the next vehicle in the next memory slot
            for (int j = i; j < garage->current_count - 1; j++) {
                garage->parked_vehicles[j] = garage->parked_vehicles[j + 1];
            }

            //reduce count
            garage->current_count--;

            //return 1 for successful task
            return 1;
        }
    }

    //return 0 for failed task
    return 0;
}

//Search and return vehicle by its license plate. ret null if not found
RegisteredVehicle *searchVehicleByLicense(const Campus *campus, const char *license) {
    //Loop through all the vehicles on campus
    for (int i = 0; i < campus->total_registered_vehicles; i++) {

        //Compare license plates
        if (strcmp(campus->registered_vehicles[i]->license_plate, license) == 0) {
            //return the registered vehicle if found
            return campus->registered_vehicles[i];
        }
    }
    return NULL; //return null if not found
}

//Function to return total number of vehicles parked on campus
int countTotalVehicles(const Campus *campus) {

    //Initialize total Vehicles to 0 to count
    int totalVehicles = 0;

    //Loops through all garages current count and adds the count to total
    for (int i = 0; i < campus->total_garages; i++) {
        totalVehicles += campus->garages[i]->current_count;
    }

    //returns total vehicles count
    return totalVehicles; //returns number of vehicles
}


//Function that Reallocates memory to change a garages' capacity to new given capacity
int resizeGarage(Garage *garage, int new_capacity) {

    //Check that new_capacity is valid
    if (new_capacity < garage->current_count) {

        //if invalid, alert user with message
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");

        //return 0 for failed task
        return 0;
    }

    //Update total_capacity of garage to new_capacity
    garage->total_capacity = new_capacity;

    //Update the memory of the garage using realloc to fit new_capacity number of registered vehicles
    garage->parked_vehicles = realloc(garage->parked_vehicles,new_capacity * sizeof(RegisteredVehicle*));

    //print success to alert user of task completion
    printf("SUCCESS\n");

    //return 1 for a successful task
    return 1;
}


//Function that relocates a vehicle from one garage to another
int relocateVehicle(Campus *campus, const char *license, const char *target_garage_name) {
    //Find garage by its name and vehicle by its license
    Garage* target_garage = findGarageByName(campus, target_garage_name);
    RegisteredVehicle* vehicle = searchVehicleByLicense(campus, license);

    //Find garage where the vehicle is parked at
    Garage* current_garage = findGarageByVehicle(campus, license);

    //check if garage exists
    if (!target_garage) {
        //If garage is not found, print NOT FOUND
        printf("%s NOT FOUND.\n", target_garage_name);

        //return 0 for a Failed task
        return 0;
    }

    //check if vehicle is in campus
    if (!vehicle || !current_garage) {
        //If vehicle is not found, print LicensePlate NOT IN CAMPUS
        printf("%s NOT IN CAMPUS.\n", license);

        //Return 0 for a failed task
        return 0;
    }


    if (target_garage->current_count >= target_garage->total_capacity) {
        //If target garage is at full capacity, print garageName FULL
        printf("%s IS FULL.\n", target_garage_name);

        //return 0 for failed task
        return 0;
    }

    //Call function to remove vehicle from previous garage
    removeVehicleFromGarage(current_garage, license);

    //Call function to Park vehicle on new given garage
    parkVehicle(target_garage, vehicle);

    //print Relocation successful to indicate user
    printf("RELOCATION SUCCESSFUL.\n");

    //return 1 for successful task
    return 1;
}

//Function that displays all the registered vehicles by one owner
void displayVehiclesByOwner(const Campus *campus, const char *owner_name) {
    //initialize variable to track Owner vehicles to false/0
    int found_Vehicles = 0;

    //Loop through all the registered vehicles in campus system
    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        //Assign vehicle to the ith registered vehicle
        RegisteredVehicle* vehicle = campus->registered_vehicles[i];

        //Compare vehicle's owner name to given owner name
        if (strcmp(vehicle->owner_name, owner_name) == 0) {
            //set found to true/1
            found_Vehicles = 1;

            //Find which garage the vehicle is in calling function to find garage by one vehicle
            Garage* garage = findGarageByVehicle(campus, vehicle->license_plate);

            //with the previous function, check
            if (garage) {
                //If it is in a garage, print License plate and Garage name
                printf("%s %s\n", vehicle->license_plate, garage->garage_name);
            } else {
                //If not, print License Plate and NOT ON CAMPUS
                printf("%s NOT ON CAMPUS\n", vehicle->license_plate);
            }
        }
    }

    if (!found_Vehicles) {
        //If owner isn't found print the following
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }
}

//Function that Removes a garage and frees its memory without removing and freeing the vehicles
int removeGarage(Campus *campus, const char *garage_name) {

    //Loops through all the garages in campus to look for given garage to remove
    for (int i = 0; i < campus->total_garages; i++) {

        //Compares the garages with the garage given to be removed
        if (strcmp(campus->garages[i]->garage_name, garage_name) == 0) {

            //Frees the array of parked vehicles but not vehicles
            free(campus->garages[i]->parked_vehicles);
            //Frees the name of the garage
            free(campus->garages[i]->garage_name);
            //Frees the garage itself
            free(campus->garages[i]);

            //For loop to Shift garages number after freeing the garage
            for (int j = i; j < campus->total_garages - 1; j++) {
                campus->garages[j] = campus->garages[j + 1];
            }

            //decrease number of garages
            campus->total_garages--;

            //Return 1 for successful task
            return 1;
        }
    }
    //return 0 for failed task
    return 0;
}

//Function that Generates a report of utilization for all garages
void generateGarageUtilizationReport(const Campus *campus) {

    //declare variables
    double min_utilization = 100.0;
    char* least_utilized_garage = NULL;

    //Loop through all the garages
    for (int i = 0; i < campus->total_garages; i++) {
        //Make a garage pointer for the current garage
        Garage* garage = campus->garages[i];

        //Find out the utilization percentage for the current garage
        double utilization = ((double)garage->current_count / garage->total_capacity) * 100.0;

        //Print the garage name, capacity, number of occupied spots, and utilization percentage
        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2f%%\n",
               garage->garage_name, garage->total_capacity, garage->current_count, utilization);

        //If the utilization is lower than the previous minimum utilization then make it the new minimum
        if (utilization < min_utilization) {
            min_utilization = utilization;
            least_utilized_garage = garage->garage_name;
        }
    }

    //Print the least utilized garage
    printf("Least Utilized: %s\n", least_utilized_garage);

}

//Function that frees all the memory used by the program
void freeCampusMemory(Campus *campus) {
    //Free all vehicles and its components
    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        free(campus->registered_vehicles[i]->license_plate);
        free(campus->registered_vehicles[i]->owner_name);
        free(campus->registered_vehicles[i]);
    }
    free(campus->registered_vehicles);

    //Free all garages and its components
    for (int i = 0; i < campus->total_garages; i++) {
        free(campus->garages[i]->parked_vehicles);
        free(campus->garages[i]->garage_name);
        free(campus->garages[i]);
    }
    free(campus->garages);

    //Free the campus
    free(campus);
}

//Function that Finds in which garage a vehicle is parked in
Garage* findGarageByVehicle(const Campus *campus, const char *license) {
    //Loop through all garages to check for cars parked
    for (int i = 0; i < campus->total_garages; i++) {
        //Make a pointer to the current ith garage
        Garage* garage = campus->garages[i];

        //Loop through the vehicles in the current ith garage
        for (int j = 0; j < garage->current_count; j++) {
            //Compare license plates to find the vehicle
            if (strcmp(garage->parked_vehicles[j]->license_plate, license) == 0) {
                //Vehicle was found in the garage that its being returned
                return garage;
            }
        }
    }
    //return NULL as vehicle was not found in any garage
    return NULL;
}

//Function that Finds a garage by its name
Garage* findGarageByName(const Campus *campus, const char *garage_name) {
    //Loop through all garages to look for the garage by its name
    for (int i = 0; i < campus->total_garages; i++) {

        //Compare the garages' name with the given search name
        if (strcmp(campus->garages[i]->garage_name, garage_name) == 0) {

            //return garage that was found by its name
            return campus->garages[i];
        }
    }
    //return NULL if garage was not found
    return NULL;
}
