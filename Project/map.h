void initMap();
void updateMap(int grid[][20]);
void updateGrid(int grid[][20], nunchuk_t* nunchuk);

void initEnvironment(int grid[][20]);
int checkPlayer(int grid[][20]);

struct enityPosition{
    int x;
    int y;
};

struct Log{
    int x;
    int y;
    struct enityPosition tailLog;
};

struct Car{
    int x;
    int y;
    struct enityPosition tailCar;
};
void printGrid(int grid[][20]);
struct enityPosition player;
struct enityPosition lastPlayer;
struct enityPosition cars[20];
struct enityPosition logs[24];