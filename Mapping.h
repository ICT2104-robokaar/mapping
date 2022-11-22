#ifndef MAPPING_H_
    #define MAPPING_H_

    #ifndef DRIVERLIB_H
        #define DRIVERLIB_H
        #include "driverlib.h"
    #endif
    //Include all other modules
    #ifndef SENSOR_H_
        #define SENSOR_H_
        #include "Sensor/sensor.h"
    #endif

    #include <stdio.h>
    #include <stdbool.h>

    #define NORTH 0
    #define EAST 1
    #define SOUTH 2
    #define WEST 3
    #define LEFT 0
    #define FRONT 1
    #define RIGHT 2
    #define MAXNODE 100
    #define OFFSET 10

    typedef struct node {
        struct node *northNode;
        struct node *southNode;
        struct node *eastNode;
        struct node *westNode;
        uint16_t distanceNorth;
        uint16_t distanceSouth;
        uint16_t distanceEast;
        uint16_t distanceWest;
        uint16_t x;
        uint16_t y;
    } Node;

    extern uint16_t distanceTravel;
    uint16_t visited[MAXNODE];
    extern int16_t visitedCounter;
    uint16_t visitedCoord[MAXNODE][3];
    uint16_t visitedCoordCounter;
    extern uint16_t xTravel;
    extern uint16_t yTravel;
    Node* allNode[MAXNODE];
    extern uint16_t allNodeCounter;
    Node head;

    int mappingMain();
    void navigateDijkstra(int16_t *facing);
    void turn(int16_t currentDirection, int16_t targetDirection);
    void relax(uint16_t dist[], uint16_t edgeTo[], uint16_t direction[], uint16_t workingi, Node* targetNode, uint16_t targetDistance, uint16_t facing);
    Node* getNextLeastDistance(uint16_t dist[], bool flag[], uint16_t *workingi);
    void rec(int facing, Node *workingNode, int counter);
    void turnLeft();
    void turnRight();
    int getDistance(Node *node, int facing);
    int mod(int x, int y);
    void reverse(int distance, int facing);
    Node* getNearestNode(uint16_t x, uint16_t y, uint16_t *extraDistance, uint16_t *extraDirection);
    Node* getNearestNodes(uint16_t x, uint16_t y, Node* node, uint16_t *extraDistance, uint16_t *extraDirection);
    Node* getNode(uint16_t x, uint16_t y);
    Node* getNodes(uint16_t x, uint16_t y, Node *node);
    int isVisited(Node *currentNode, int facing);
    uint16_t displayData(Node *node);
    uint16_t displayDatas(Node *node);
    void moveForward();
    void moveForwardBy(uint16_t distance);
    void initNode(Node *node);
    Node* linkNode(Node *baseNode, int facing);
    void releaseMemory(Node *workingNode);

#endif
