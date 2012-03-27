

#include <driver/elev.h>


typedef struct {
	long long id;
	int floor;
	int floorSensor;
	elev_direction_t direction;
	char floorButtonState[N_FLOORS*2];
	char commandButtonState[N_FLOORS];
	char obstruction;
	char inMotion;
	char doorOpen;
	char emStop;
} elevator_t;



void startElevator();
