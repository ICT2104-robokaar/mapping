#include "Mapping.h"
uint16_t distanceTravel = 0;
int16_t visitedCounter = 0;
uint16_t xTravel = 500;
uint16_t yTravel = 500;
uint16_t allNodeCounter = 0;

int mappingMain() {
    initNode(&head);
    head.x = xTravel;
    head.y = yTravel;
    Node *workingNode = &head;
    allNode[allNodeCounter] = &head;
    allNodeCounter += 1;
    int16_t facing = SOUTH;
    facing = mod(facing-1, 4);
    distanceTravel += 27*4;

    Node *newNodePtr1;
    newNodePtr1 = linkNode(workingNode, facing);
    facing = mod(facing+1, 4);
    distanceTravel += 27*3;

    Node *newNodePtr2;
    newNodePtr2 = linkNode(newNodePtr1, facing);
    facing = mod(facing+1, 4);
    distanceTravel += 27;

    Node *newNodePtr3;
    newNodePtr3 = linkNode(newNodePtr2, facing);
    facing = mod(facing+1, 4);
    distanceTravel += 27*2;

    Node *newNodePtr4;
    newNodePtr4 = linkNode(newNodePtr3, facing);
    facing = mod(facing-1, 4);
    distanceTravel += 27*3;

    Node *newNodePtr5;
    newNodePtr5 = linkNode(newNodePtr4, facing);
    newNodePtr5->northNode = &head;
    newNodePtr5->distanceNorth = 27;
    head.southNode = newNodePtr5;
    head.distanceSouth= 27;
    facing = mod(facing-1, 4);
    distanceTravel += 27*2;

    Node *newNodePtr6;
    newNodePtr6 = linkNode(newNodePtr5, facing);
    facing = mod(facing-1, 4);
    distanceTravel += 27*2;

    Node *newNodePtr7;
    newNodePtr7 = linkNode(newNodePtr6, facing);
    facing = mod(facing-1, 4);
    distanceTravel += 27*1;

    Node *newNodePtr8;
    newNodePtr8 = linkNode(newNodePtr7, facing);
    facing = mod(facing-1, 4);
    distanceTravel += 27*1;

    Node *newNodePtr9;
    newNodePtr9 = linkNode(newNodePtr8, facing);
    facing = mod(facing-2, 4);
    distanceTravel += 27*1;
    xTravel += 27;
    yTravel += 27;

    newNodePtr3->westNode = newNodePtr7;
    newNodePtr3->distanceWest = 27;
    newNodePtr7->eastNode = newNodePtr3;
    newNodePtr7->distanceEast = 27;
    xTravel = 500;
    yTravel = 500;
    distanceTravel = 0;
    facing = SOUTH;
    displayData(&head);

    while (true) {
        facing = SOUTH;
        navigateDijkstra(&facing);
    }

    //uint32_t left = 0;
    //uint32_t right = 0;
    //uint32_t front = 0;
    //sensorMain();
    //int i = 0;
    //while (i < 10000000) {
    //    getBorder(&left, &front, &right);
    //}
    releaseMemory(&head);
}

/**
 * From current node, map out shortest path to all other node
 * Get User input on where to goto
 * Populate a list of instruction to move the car
 */
void navigateDijkstra(int16_t *facing) {

    /*Setup array needed for Dijkstra*/
    uint16_t dist[MAXNODE];
    uint16_t edgeTo[MAXNODE];
    uint16_t direction[MAXNODE];
    bool flag[MAXNODE];
    uint16_t doneCounter = 0;
    int i = 0;
    /*Set all to max value*/
    for (i = allNodeCounter-1;i >= 0;i--) {
        dist[i] = 65535;
        flag[i] = false;
        edgeTo[i] = 65535;
        direction[i] = 65535;
    }
    /* get starting node and set value*/
    Node* currentNode = getNode(xTravel, yTravel);
    Node* myNode = currentNode;
    uint16_t workingi = 0;
    for (workingi = allNodeCounter-1;workingi >= 0 ; workingi--) {
        if (allNode[workingi] == currentNode) {
            flag[workingi] =true;
            dist[workingi] = 0;
            edgeTo[workingi] = 65535;
            direction[workingi] == NULL;
            break;
        }
    }
    /* Do until all node visited */
    while (doneCounter < allNodeCounter) {
        /* Mark node as visited */
        for (i = allNodeCounter-1;i >= 0; i--) {
            if (currentNode == allNode[i]) {
                flag[i] = true;
                break;
            }
        }
        /* Relax for all movable direction */
        if (currentNode->distanceNorth > 0) {
            relax(dist,edgeTo,direction,workingi,currentNode->northNode, currentNode->distanceNorth, NORTH);
        }
        if (currentNode->distanceEast > 0) {
            relax(dist,edgeTo,direction,workingi,currentNode->eastNode, currentNode->distanceEast, EAST);
        }
        if (currentNode->distanceSouth > 0) {
            relax(dist,edgeTo,direction,workingi,currentNode->southNode, currentNode->distanceSouth, SOUTH);
        }
        if (currentNode->distanceWest > 0) {
            relax(dist,edgeTo,direction,workingi,currentNode->westNode, currentNode->distanceWest, WEST);
        }
        currentNode = getNextLeastDistance(dist, flag, &workingi);
        doneCounter++;
        //printf("Index\taddress\t\tdistance\tfronIndex\tdirection\n");
        //for(i = 0;i < allNodeCounter;i++) {
        //        printf("%d\t%x\t%d\t\t%d\t\t%d\n", i, allNode[i], dist[i], edgeTo[i], direction[i]);
        //    }
        //printf("==============================================================\n");
    }
    //Get user input for coordinate
    uint16_t targetx = 0;
    uint16_t targety = 0;
    printf("Please enter a coordinate x y:");
    scanf("%d %d", &targetx, &targety);

    //Get a list of instructions
    uint16_t directions[MAXNODE]; //Used to store instruction direction
    uint16_t distances[MAXNODE];  //Used to store instruction distance
    uint16_t counter = 0;         //Used to point to the index

    Node *targetNode = getNode(targetx, targety);
    if (targetNode == NULL) {
        uint16_t extraDistance = 0;
        uint16_t extraDirection = 0;
        targetNode = getNearestNode(targetx, targety, &extraDistance, &extraDirection);
        if (targetNode == NULL) {
            printf("Invalid Input Please try again\n");
            return;
        }
        directions[counter] = extraDirection;
        distances[counter] = extraDistance;
        counter++;
    }
    while (targetNode != myNode) {
        for (i = allNodeCounter-1; i>=0;i--) {
            if (allNode[i] == targetNode) {
                directions[counter] = direction[i];
                targetNode = allNode[edgeTo[i]];
                switch(direction[i]) {
                case (NORTH):
                    distances[counter] = targetNode->distanceNorth;
                    break;
                case (EAST):
                    distances[counter] = targetNode->distanceEast;
                    break;
                case (SOUTH):
                    distances[counter] = targetNode->distanceSouth;
                    break;
                case (WEST):
                    distances[counter] = targetNode->distanceWest;
                    break;
                }
                if (counter != 0) {
                    if (abs(directions[counter-1] - directions[counter]) == 2) {
                        directions[counter-1] = directions[counter];
                        distances[counter-1] = distances[counter] - distances[counter-1];
                        counter--;
                    }
                    else if(directions[counter-1] == directions[counter]) {
                        distances[counter-1] += distances[counter];
                        counter--;
                    }
                }
                counter++;
                break;
            }
        }
    }
    printf("index\tdirection\tdistance\n");
    for(i = 0;i < counter;i++) {
        printf("%d\t%d\t\t%d\n", i, directions[i], distances[i]);
    }

    //Start moving to target area
    while (counter > 0) {
        turn(*facing, directions[counter-1]);
        *facing = directions[counter-1];
        moveForwardBy(distances[counter-1]);
        counter--;
    }
}
void turn(int16_t currentDirection, int16_t targetDirection) {
    if (currentDirection - targetDirection < 0) {
        if (currentDirection - targetDirection < -2) {
            turnLeft();
        }
        else {
            turnRight();
        }
    }
    else if (currentDirection - targetDirection > 0) {
        if (currentDirection - targetDirection > 2) {
            turnRight();
        }
        else {
            turnLeft();
        }
    }

}

/**
 * Find and relax
 */
void relax(uint16_t dist[], uint16_t edgeTo[], uint16_t direction[], uint16_t workingi, Node* targetNode, uint16_t targetDistance, uint16_t facing) {
    int i = 0;
    for (i = allNodeCounter-1;i >= 0;i--) {
        if (allNode[i] == targetNode) {
            if (dist[workingi]+targetDistance < dist[i]) {
                dist[i] = dist[workingi]+targetDistance;
                edgeTo[i] = workingi;
                direction[i] = facing;
            }
            break;
        }
    }
}

/**
 * Return the node that has the least distance AND not visited
 */
Node* getNextLeastDistance(uint16_t dist[], bool flag[], uint16_t *workingi) {
    int i = 0;
    Node* currentNode;
    uint16_t minDistance = 65535;
    for (i = allNodeCounter-1; i >= 0;i--) {
        if (!flag[i] && minDistance > dist[i]) {
            minDistance = dist[i];
            currentNode = allNode[i];
            *workingi = i;
        }
    }
    return currentNode;
}

int main2(void) {

    //init sensor
    sensorMain();

    //Mapping
    initNode(&head);
    Node *workingNode = &head;
    int facing = SOUTH;

    rec(facing, workingNode, 0);

    printf("Final linkage\n");
    displayData(&head);

    //Navigation
        //Read user input for coordinate
        //Run dijkstra algo to get the shortest path, return a list of steps
        //Perform movement
    releaseMemory(&head);

}

/**
 * A recursive backtracking function to create and link adjacent node
 * to map out the whole maze
 */
void rec(int facing, Node *workingNode, int counter) {
    printf("Rec %d\n", counter);
    printf("facing %d\n", facing);
    uint32_t left = 0;
    uint32_t right = 0;
    uint32_t front = 0;
    int leftRelease = 0;
    int rightRelease = 0;
    int doFront = 1;
    int hasVisited = 0;
    int tempTraveled = 0;
    if (counter==0) {
        leftRelease = 1;
        rightRelease = 1;
    }
    Node *newNodePtr;
    while (1) {
        //displayData(&head);
        getBorder(&left, &front, &right);
        printf("%d %d %d\n", left, front, right);
        printf("%d %d %d\n", xTravel, yTravel, distanceTravel);
        if (!left && leftRelease || !right && rightRelease) {
            doFront = 0;
            hasVisited = isVisited(workingNode, facing);
            tempTraveled = distanceTravel;
            if (!hasVisited) {
                //stop movement
                //Node newNode;
                //initNode(&newNode);
                //newNodePtr = linkNode(workingNode, &newNode, facing);
                newNodePtr = linkNode(workingNode, facing);
                if (!left && leftRelease && !(getDistance(newNodePtr, mod(facing-1, 4)))) {
                    leftRelease = 0;
                    printf("Turn left and face %d\n", mod(facing - 1, 4));
                    turnLeft();
                    rec(mod(facing - 1, 4), newNodePtr, counter+=1);

                    //turn RIGHT
                    printf("Turn right return\n");
                }
                if (!right && rightRelease && !(getDistance(newNodePtr, mod(facing+1, 4)))) {
                    rightRelease = 0;
                    printf("Turn right\n");
                    turnRight();
                    rec(mod(facing + 1, 4), newNodePtr, counter+=1);

                    //turn LEFT
                    printf("Turn left return\n");
                }
            }
            else {
                distanceTravel = 0;
                //reverse car
                break;
            }
        }
        if (left) {
            printf("Left has border releasing left\n");
            leftRelease = 1;
        }
        if (right) {
            printf("right has border releasing right\n");
            rightRelease = 1;
        }
        if (front) {
            printf("front has border ending\n");
            if (left && right) {
                tempTraveled = distanceTravel;
                //Node newNode;
                //initNode(&newNode);
                //printf("Front node testing\n");
                //printf("%d: %d\n", newNode.northNode, newNode.distanceNorth);
                //printf("%d: %d\n", newNode.eastNode, newNode.distanceEast);
                //printf("%d: %d\n", newNode.southNode, newNode.distanceSouth);
                //printf("%d: %d\n", newNode.westNode, newNode.distanceWest);

                //newNodePtr = linkNode(workingNode, &newNode, facing);
                newNodePtr = linkNode(workingNode, facing);


            }
            else {

            }
            reverse(tempTraveled, facing);
             //AND reverse car by tempTraveled
            break;
        }
        else if (!doFront) {
            if (!getDistance(workingNode, facing)) {
                rec(mod(facing, 4), newNodePtr, counter+=1);
            }
            reverse(tempTraveled, facing);
            break;
        }
        else {
            moveForward();
        }
    }

}

/**
 * Turn left for manual turning of car
 */
void turnLeft() {
    printf("Turning Left\n");
    int i = 0;
    while (i < 1000000) {
        i+=1;
    }
}
/**
 * Turn right for manual turning of car
 */
void turnRight() {
    printf("Turning Right\n");
    int i = 0;
    while (i < 1000000) {
            i+=1;
        }

}
/**
 * Get the distance of a node at a direction
 */
int getDistance(Node *node, int facing) {
    switch(facing) {
        case NORTH:
            printf("GetDistance North: %d\n", node->distanceNorth);
            return node->distanceNorth;
        case EAST:
            printf("GetDistance East: %d\n", node->distanceEast);
            return node->distanceEast;
        case SOUTH:
            printf("GetDistance South: %d\n", node->distanceSouth);
            return node->distanceSouth;
        case WEST:
            printf("GetDistance West: %d\n", node->distanceWest);
            return node->distanceWest;
    }
}

/**
 * A modulus function that can works for negative number
 */
int mod(int x, int y)
{
   int t = x - ((x / y) * y);
   if (t < 0) t += y;
   return t;
}
/**
 * For motor module to reverse
 * Used to update current xTravel and yTravel
 */
void reverse(int distance, int facing) {
    printf("Reversing %d from direction %d\n", distance, facing);
    switch(facing) {
        case(NORTH):
            yTravel += distance;
            break;
        case(EAST):
            xTravel -= distance;
            break;
        case(SOUTH):
            yTravel -= distance;
            break;
        case(WEST):
            xTravel += distance;
            break;
    }
    printf("Reversed to %d %d\n", xTravel, yTravel);
}
/**
 * Get the nearest node to the given coordinate
 */
Node* getNearestNode(uint16_t x, uint16_t y, uint16_t *extraDistance, uint16_t *extraDirection) {
     visitedCounter = 0;
     return getNearestNodes(x, y, &head, extraDistance, extraDirection);
 }
Node* getNearestNodes(uint16_t x, uint16_t y, Node* node, uint16_t *extraDistance, uint16_t *extraDirection) {
    if (node == NULL) {
        return node;
    }
    int i;
    Node *ptr = NULL;
    for (i = visitedCounter-1;i >= 0;i--) {
        if (visited[i] == (uint16_t)node) {
            return ptr;
        }
    }
    visited[visitedCounter] = node;
    visitedCounter+=1;
    //Found condition
    uint16_t northx = node->x;
    uint16_t northy = node->y - node->distanceNorth;
    uint16_t eastx = node->x + node->distanceEast;
    uint16_t easty = node->y;
    uint16_t southx = node->x;
    uint16_t southy = node->y + node->distanceSouth;
    uint16_t westx = node->x - node->distanceWest;
    uint16_t westy = node->y;
    if (node->northNode != NULL && northx-OFFSET <= x && x <= northx+OFFSET && northy-OFFSET <= y && y <= node->y+OFFSET) {
        if ((y - northy) > (node->y - y)) {
            *extraDistance = node->y - y;
            *extraDirection = NORTH;
            return node;
        }
        else {
            *extraDistance = y - northy;
            *extraDirection = SOUTH;
            return node->northNode;
        }
    }
    else if (node->eastNode != NULL && node->x-OFFSET <= x && x <= eastx+OFFSET && easty-OFFSET <= y && y <= easty+OFFSET) {
        if ((eastx - x) > (x - node->x)) {
            *extraDistance = x - node->x;
            *extraDirection = EAST;
            return node;
        }
        else {
            *extraDistance = x - eastx - x;
            *extraDirection = WEST;
            return node->eastNode;
        }
    }
    else if (node->southNode != NULL && southx-OFFSET <= x && x <= southx+OFFSET && node->y-OFFSET <= y && y <= southy+OFFSET) {
        if ((southy - y) > (y - node->y)) {
            *extraDistance = y - node->y;
            *extraDirection = SOUTH;
            return node;
        }
        else {
            *extraDistance = southy - y;
            *extraDirection = NORTH;
            return node->southNode;
        }
    }
    else if (node->westNode != NULL && westx-OFFSET <= x && x <= node->x+OFFSET && westy-OFFSET <= y && y <= westy+OFFSET) {
        if ((x - westx) > (node->x - x)) {
            *extraDistance = node->x - x;
            *extraDirection = WEST;
            return node;
        }
        else {
            *extraDistance = x - westx;
            *extraDirection = EAST;
            return node->westNode;
        }
    }
    else {
        if (node->northNode != NULL && ptr == NULL) {
            ptr = getNearestNodes(x, y, node->northNode, extraDistance, extraDirection);
        }
        if (node->eastNode != NULL && ptr == NULL) {
            ptr = getNearestNodes(x, y, node->eastNode, extraDistance, extraDirection);
        }
        if (node->southNode != NULL && ptr == NULL) {
            ptr = getNearestNodes(x, y, node->southNode, extraDistance, extraDirection);
        }
        if (node->westNode != NULL && ptr == NULL) {
            ptr = getNearestNodes(x, y, node->westNode, extraDistance, extraDirection);
        }
    }
    return ptr;


}
/**
 * get the Node that is around the given x and y coordinate
 * return a NULL pointer if none are near
 */
Node* getNode(uint16_t x, uint16_t y) {
    visitedCounter = 0;
    return getNodes(x, y, &head);
}
Node* getNodes(uint16_t x, uint16_t y, Node *node) {
    int i;
    Node *ptr = NULL;
    for (i = visitedCounter-1;i >= 0;i--) {
        if (visited[i] == (uint16_t)node) {
            return ptr;
        }
    }
    if ((node->x-OFFSET <= x && x <= node->x+OFFSET) && (node->y-OFFSET <= y && y <= node->y+OFFSET)) {
        return node;
    }
    visited[visitedCounter] = node;
    visitedCounter+=1;

    if (node->northNode != NULL && ptr == NULL) {
        ptr = getNodes(x, y, node->northNode);
    }
    if (node->eastNode != NULL && ptr == NULL) {
        ptr = getNodes(x, y, node->eastNode);
    }
    if (node->southNode != NULL && ptr == NULL) {
        ptr = getNodes(x, y, node->southNode);
    }
    if (node->westNode != NULL && ptr == NULL) {
        ptr = getNodes(x, y, node->westNode);
    }
    return ptr;
}
/**
 * Used during navigation to check of area has existing node
 */
int isVisited(Node *currentNode, int facing) {
    printf("Checking visited\n");
    int found = 0;
    int x = xTravel;
    int y = yTravel;
    switch(facing){
        case(NORTH):
            y -= distanceTravel;
            break;
        case(EAST):
            x += distanceTravel;
            break;
        case(SOUTH):
            y += distanceTravel;
            break;
        case(WEST):
            x -= distanceTravel;
            break;
    }
    printf("Checking Coordinate %d %d\n", x, y);
    int i = 0;
    for (i = visitedCoordCounter-1; i >= 0; i--) {
        int visitedx = visitedCoord[i][0];
        int visitedy = visitedCoord[i][1];
        printf("Checking against %d %d\n", visitedx, visitedy);
        if ((x-OFFSET < visitedx && visitedx < x+OFFSET) && (y-OFFSET < visitedy && visitedy < y+OFFSET)) {
            found = 1;
            Node *ptr = getNode(visitedx, visitedy);
            switch(facing) {
                case(NORTH):
                    ptr->southNode = currentNode;
                    currentNode->northNode = ptr;
                    ptr->distanceSouth = distanceTravel;
                    currentNode->distanceNorth = distanceTravel;
                    break;
                case(EAST):
                    ptr->westNode = currentNode;
                    currentNode->eastNode = ptr;
                    ptr->distanceWest = distanceTravel;
                    currentNode->distanceEast = distanceTravel;
                    break;
                case(SOUTH):
                    ptr->northNode = currentNode;
                    currentNode->southNode = ptr;
                    ptr->distanceNorth = distanceTravel;
                    currentNode->distanceSouth = distanceTravel;
                    break;
                case(WEST):
                    ptr->eastNode = currentNode;
                    currentNode->westNode = ptr;
                    ptr->distanceEast = distanceTravel;
                    currentNode->distanceWest = distanceTravel;
                    break;
            }
            break;
        }
    }
    return found;
}

/**
 * Display all memory used to map out the map
 */
uint16_t displayData(Node *node) {
    visitedCounter = 0;
    int totalSize = displayDatas(node);
    printf("Total memory used to link whole map: %d\n", totalSize);
}
uint16_t displayDatas(Node *node) {
    int i;
    int totalSize = 0;
    for (i = visitedCounter-1;i >= 0;i--) {
        if ((uint16_t)visited[i] == (uint16_t)node) {
            return 0;
        }
    }
    visited[visitedCounter] = node;
    visitedCounter+=1;
    printf("NewNode %x %x:%d  %x:%d  %x:%d  %x:%d\n", node, node->northNode, node->distanceNorth, node->eastNode, node->distanceEast, node->southNode, node->distanceSouth, node->westNode, node->distanceWest);

    if (node->northNode != NULL) {
        printf("North\n");
        totalSize += displayDatas(node->northNode);
        printf("Return North\n");
    }
    if (node->eastNode != NULL) {
        printf("east\n");
        totalSize += displayDatas(node->eastNode);
        printf("Return East\n");
    }
    if (node->southNode != NULL) {
        printf("south\n");
        totalSize += displayDatas(node->southNode);
        printf("Return South\n");
    }
    if (node->westNode != NULL) {
        printf("west\n");
        totalSize += displayDatas(node->westNode);
        printf("Return West\n");
    }
    return totalSize + sizeof(Node);
}
/**
 * For motor to move forward
 * Used to ask for movement input and update the distanceTravel
 */
void moveForward() {
    int travel;
    printf("MoveForward: ");
    scanf("%d", &travel);
    distanceTravel+=travel;
    printf("Current distance: %d\n", distanceTravel);
}
void moveForwardBy(uint16_t distance) {
    printf("Car moving forward by: %d\n", distance);
}
/**
 * Initialize a new node with default value
 */
void initNode(Node *node) {
    node->distanceNorth = 0;
    node->distanceSouth = 0;
    node->distanceEast = 0;
    node->distanceWest = 0;
    node->northNode = NULL;
    node->southNode = NULL;
    node->eastNode = NULL;
    node->westNode = NULL;
    node->x = 0;
    node->y = 0;
}

/**
 * Link existing node to a new node with direction
 */
Node* linkNode(Node *baseNode, int facing) {
    printf("Linking node %d \n", facing);
    Node *ptr;
    switch((facing)) {
        case(WEST):
            xTravel -= distanceTravel;
            printf("Case3\n");
            //newNode->eastNode = baseNode;
            baseNode->westNode = (Node *) malloc(sizeof(Node));
            ptr = baseNode->westNode;
            initNode(ptr);
            ptr->eastNode = baseNode;
            baseNode->distanceWest = distanceTravel;
            ptr->distanceEast = distanceTravel;
            break;
        case(NORTH):
            yTravel -= distanceTravel;
            printf("Case0\n");
            //newNode->southNode = baseNode;
            baseNode->northNode = (Node *) malloc(sizeof(Node));
            ptr = baseNode->northNode;
            initNode(ptr);
            ptr->southNode = baseNode;
            baseNode->distanceNorth = distanceTravel;
            ptr->distanceSouth = distanceTravel;
            break;
        case(EAST):
            xTravel += distanceTravel;
            printf("Case1\n");
            //newNode->westNode = baseNode;
            baseNode->eastNode = (Node *) malloc(sizeof(Node));
            ptr = baseNode->eastNode;
            initNode(ptr);
            ptr->westNode = baseNode;
            baseNode->distanceEast = distanceTravel;
            ptr->distanceWest = distanceTravel;
            break;
        case(SOUTH):
            yTravel += distanceTravel;
            printf("Case2\n");
            //newNode->northNode = baseNode;
            baseNode->southNode = (Node *) malloc(sizeof(Node));
            ptr = baseNode->southNode;
            initNode(ptr);
            ptr->northNode = baseNode;
            baseNode->distanceSouth = distanceTravel;
            ptr->distanceNorth = distanceTravel;
            break;

    }
    ptr->x = xTravel;
    ptr->y = yTravel;
    visitedCoord[visitedCoordCounter][0] = xTravel;
    visitedCoord[visitedCoordCounter][1] = yTravel;
    visitedCoord[visitedCoordCounter][2] = ptr;
    visitedCoordCounter+=1;
    distanceTravel = 0;
    allNode[allNodeCounter] = ptr;
    allNodeCounter += 1;
    return ptr;
}

/**
 * Using user input to act as an ultrasonic
 */
//void readUltrasonic(int *left, int *front, int *right) {
//    printf("===================================================Left front right: ");
//    scanf("%1d%1d%1d", left, front, right);
//
//}

/**
 * Free up memory used for mapping
 */
void releaseMemory(Node *workingNode) {
    printf("Releasing Memory\n");
    Node *northNode;
    Node *southNode;
    Node *eastNode;
    Node *westNode;
    Node *tempNode;
    northNode = workingNode->northNode;
    eastNode = workingNode->eastNode;
    southNode = workingNode->southNode;
    westNode = workingNode->westNode;
    if (northNode != NULL) {
        northNode->southNode = NULL;
        workingNode->northNode = NULL;
        releaseMemory(northNode);
    }
    if (eastNode != NULL) {
        eastNode->westNode = NULL;
        workingNode->eastNode = NULL;
        releaseMemory(eastNode);
    }
    if (southNode != NULL) {
        southNode->northNode = NULL;
        workingNode->southNode = NULL;
        releaseMemory(southNode);
    }
    if (westNode != NULL) {
        westNode->eastNode = NULL;
        workingNode->westNode = NULL;
        releaseMemory(westNode);
    }
    free(workingNode);
    printf("Released\n");
}
