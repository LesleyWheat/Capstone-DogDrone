/* #include <iostream>
#include <cmath>

using namespace std;

int main()
{
    double number, squareRoot;
    cout << "Enter a number: ";
    cin >> number;

    // sqrt() is a library function to calculate square root
    squareRoot = sqrt(number);
    cout << "Square root of " << number << " = " << squareRoot;
    return 0;
} */

#include <iostream>
#include <cmath>

using namespace std;

struct coordinates{
    int x;
    int y;
};

coordinates current, next, prev;

struct Obstacle{
    coordinates North, South, East, West;
}ObstacleArray[1000];

struct Point{
    coordinates Source, Goal;
    int pointNumReach [4];
    int pointNum;
    int goalPointNum;
    int pointAccessibility [10][10];
    int numReachPoints;
    int visited;
    int isPointReachable;
    int obstacleNum;
    int alternateGoalNum;
    int weight;
}PointArray[1000];

    int pointNum = 1;
    int count = 1;
    int fieldLength = 100;
    int fieldWidth = 100;
    int vehicleWidth = 5;
    int vehicleLength = 8; // Includes field of vision (3)
    int numXPoints = (fieldWidth/vehicleWidth);
    int numYPoints = (fieldLength/vehicleLength);
    int xPointRange = fieldWidth/(numXPoints - 1);
    int yPointRange = fieldLength/(numYPoints - 1);
    int totalNumberPoints = numXPoints * numYPoints;
    int temp = numYPoints;
    int xCoordinates;
    int yCoordinates;
    int icount;
    float speedLeft;
    float speedRight;


void initialize(){
    for (int countX = 0; countX < numXPoints; countX = countX+1 ){
        for (int countY = 0; countY < numYPoints; countY = countY+1){
        xCoordinates = (countX * xPointRange);
        yCoordinates = (countY * yPointRange);

            if (countX % 2 == 1){
                pointNum = icount + numYPoints - (countY) - 1;
            }
            else{
                pointNum = icount;
                Point point;
                point.pointNum = pointNum;
                point.Source.x = xCoordinates;
                point.Source.y = yCoordinates;
            icount++;
            point.numReachPoints = 0;
            }
        }
    }
    for (int i = 1; i < totalNumberPoints; i++){
       PointArray[i].goalPointNum = PointArray[i].goalPointNum + 1;
       PointArray[i].Goal.x = PointArray[i+1].Source.x;
       PointArray[i].Goal.y = PointArray[i+1].Source.y;
       PointArray[i].isPointReachable = 1;
       PointArray[i].visited = 0;
       PointArray[i].weight = 1;

    }
};

int Detection(Point pt1, Point pt2){
    float distance;
    distance = sqrt(pow(pt2.Source.x - pt1.Source.x, 2) +  
                pow(pt2.Source.y - pt1.Source.y, 2));

    if (distance < 0.3){
        pt1.pointNum = pt1.pointNum - 1;
    }
    else
    {
        nextPoint(pt1);
    }
};

void nextPoint(Point pts){

    pts.pointNum = pts.pointNum + 1;
};

void move(Point start, Point end, Point previous){

    int direction;
    if (end.Source.x == previous.Source.X){
        Direction(0); //striaght
    }

    else{
        temp = (end.Source.y - previous.Source.y)/(end.Source.x - previous.Source.y);
    
        if (temp == 0){
            Direction(0);
        }
        else if (temp < 0){
                Direction(1);

            }
        else if (temp > 0){
            Direction(2);
        }
    }
};

int Direction(int direction){
    //0 - straight
    //1 - left 
    //2 - right

    //I need criteria for turning, right now I used speed of left and right wheels
    // left turn is 0 speed for left wheel, 1 for right wheel and so on.

    if (direction == 0){
        //Direction is straight

        speedLeft = 1.0;
        speedRight = 1.0;
    }

    else if (direction == 1){
        speedLeft = 0.0;
        speedRight = 1.0;

    }

    else if (direction == 2){
        speedLeft = 1.0;
        speedRight = 0.0;

    }

};

int main()
{
   cout << "Call moveVehicle from here";
}
