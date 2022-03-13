/* This program will implement a simulation of Schelling's segregation model to
study the phenomenon of housing segregation. The simulation occurs on a
two-dimensional array, representing a housing market. Each cell in the array,
representing a house, is either vacant, or occupied by one of two kinds
of agents, denoted by . and $.
Authors: Malavika Kalani and Meredith Haines
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>

void checkNumRead(FILE* inputFile,int numRead,int expectedCount){
    if (numRead != expectedCount){
        printf("Sorry, input file is misformatted!");
        fclose(inputFile);
        exit(2);
    }
}
// This function prints the initial/starting state of the housing market.


//this function calculates the threshold for every cell
float calculateThreshold(char *housingArray,int currRow,int currColumn,int numRows, int numColumns){
    //printf("%d",currentRow);
    float numDollar = 0; //surrounding dollar neighbours
    float numDot = 0; //surrounding dot neighbours
    float totalSymbol = 0; //surrounding dot and dollar in total
    for (int x = -1; x <=1; x++){
        for (int y = -1; y <=1; y++){
            if (((0 <= (currRow+x)) && ((currRow+x) < numRows)) && ((0 <= (currColumn+y)) && ((currColumn+y)< numColumns))){
                //check if surrounding neighbour is within bounds
                    if (( * (housingArray + (numColumns)* (currRow+x) + (currColumn+y))) == '$'
                       || ( * (housingArray + (numColumns)* (currRow+x) + (currColumn+y))=='S')){
                            numDollar++;
                            totalSymbol++;
                    }
                    if (( * (housingArray + (numColumns)* (currRow+x) + (currColumn+y))) == '.'
                       || ( * (housingArray + (numColumns)* (currRow+x) + (currColumn+y))=='D')){
                        numDot++;
                        totalSymbol++;
                    }
                    if (x == 0 && y == 0){
                        numDollar -- ; //excluding the cell we are currently on
                        numDot -- ; //only accounting for surrounding neighbours
                        totalSymbol -- ;
                    }
            }
        }
    }
    float relativeThreshold;
    if (totalSymbol == 0){ //prevent division-by-zero errors
        relativeThreshold = 0.5; //satisfied
    }
    else{
       if ((* (housingArray + (numColumns)* currRow + currColumn)) == '$'){
        relativeThreshold = numDollar/totalSymbol;
       }
       if ((* (housingArray + (numColumns)* currRow + currColumn)) == '.') {
        relativeThreshold = numDot/totalSymbol;
       }
    }

    return relativeThreshold;
}

//finds dissatisfied agent in order to move it
int markDissatisfiedAgent(char *housingArray, int numRows, int numColumns,float satisfaction){

    int dissatisfiedCount = 0;
    for (int currRow = 0; currRow < (numRows);currRow++){
        //if it is occupied and agent is dissatisfied
        for (int currColumn = 0; currColumn < (numColumns); currColumn++){
            if ((( * (housingArray + (numColumns)* currRow + currColumn)) == '$') &&
                (calculateThreshold(housingArray,currRow,currColumn,numRows,numColumns) < satisfaction)){
                     ( * (housingArray + (numColumns)* currRow + currColumn)) = 'S';
                     dissatisfiedCount++;
            }
            if ((( * (housingArray + (numColumns)* currRow + currColumn)) == '.') &&
                (calculateThreshold(housingArray,currRow,currColumn,numRows,numColumns) < satisfaction)){
                 ( * (housingArray + (numColumns)* currRow + currColumn)) = 'D';
                 dissatisfiedCount++;
        }
        //printf("%c",* (housingArray + (numColumns)* currRow + currColumn));
        }
        //printf("\n");
    }
    //printf("%d\n",dissatisfiedCount);
    return dissatisfiedCount;
}

//it is filling every blank with the same DA until it moves to the outer loop
// to get to the next DA

int checkSpacesAfter(char* housingArray,int numRows,int numColumns,int length,int dissatifiedIndex){
    int j = dissatifiedIndex;
    while((j < length) && (*(housingArray+j) != ' ')){
        j++;
    }
    if ((j<length) && *(housingArray + j) == ' '){
       *(housingArray + j) = *(housingArray + dissatifiedIndex);
       *(housingArray + dissatifiedIndex) = ' ';
       if (*(housingArray + j) == 'S'){
           *(housingArray + j) = '$';
       }
       else if (*(housingArray + j) == 'D'){
           *(housingArray + j) = '.';
       }
       return j;
    }
    else{ //there was no vacant space after dissatisfied
        return -1;
    }

}

int checkSpacesBefore(char* housingArray, int numRows,int numColumns,int dissatifiedIndex){
    int n = 0;
    while((n < dissatifiedIndex) && (*(housingArray+n) != ' ') ){
        n++;
    }
    if (*(housingArray + n) == ' '){
       *(housingArray + n) = *(housingArray + dissatifiedIndex);
       *(housingArray + dissatifiedIndex) = ' ';
       if (*(housingArray + n) == 'S'){
           *(housingArray + n) = '$';
       }
       else if (*(housingArray + n) == 'D'){
           *(housingArray + n) = '.';
       }
       return n;
    }
    else{
        return 0;
    }

}



void moveToBlank(char* housingArray,int numRows, int numColumns){
    int length = numColumns*numRows;
    for (int i = 0; i < length; i++){
        if ((*(housingArray + i) == 'S') || (*(housingArray + i) == 'D')){ //If you encounter a dissatisfied agent
           //char agent = *(housingArray + i);
           int dissatifiedIndex = i;
           //first while loop for spaces after
           int spaceIndex = checkSpacesAfter(housingArray,numRows,numColumns,length,dissatifiedIndex);

           if (spaceIndex == -1){ //this means there were no vacant spaces after the agent
               checkSpacesBefore(housingArray,numRows,numColumns,dissatifiedIndex);

           }
        }



    }

}


void runUntilSatisfied(char* housingArray,int numRows, int numColumns,float satisfaction,int verbosity,int numRounds){
    //int length = numColumns*numRows;
    int iterationCounter = 0;
    if (verbosity == 2){ //print out the state of the array after every iteration
       while ((iterationCounter < numRounds) && (markDissatisfiedAgent(housingArray,numRows, numColumns,satisfaction) > 0)){
           iterationCounter++;
            moveToBlank(housingArray, numRows,numColumns);
            for (int currRow = 0; currRow < (numRows);currRow++){
                for (int currColumn = 0; currColumn < (numColumns); currColumn++){
                    printf("%c",* (housingArray + (numColumns)* currRow + currColumn));
                }
            printf("\n");
            }
            usleep(200000);
            system("clear");
        }
        for (int currRow = 0; currRow < (numRows);currRow++){
                for (int currColumn = 0; currColumn < (numColumns); currColumn++){
                    printf("%c",* (housingArray + (numColumns)* currRow + currColumn));
                }
            printf("\n");
        }
    }

    if (verbosity == 1){ //only print the final state of the simulation
        while ((iterationCounter < numRounds) && (markDissatisfiedAgent(housingArray,numRows, numColumns,satisfaction) > 0)){
            iterationCounter++;
            moveToBlank(housingArray, numRows,numColumns);
        }

        for (int currRow = 0; currRow < (numRows);currRow++){
            for (int currColumn = 0; currColumn < (numColumns); currColumn++){
                printf("%c",* (housingArray + (numColumns)* currRow + currColumn));
            }
        printf("\n");
        }
    }

    if (verbosity == 0){
        while ((iterationCounter < numRounds) && (markDissatisfiedAgent(housingArray,numRows, numColumns,satisfaction) > 0)){
            iterationCounter++;
            moveToBlank(housingArray, numRows,numColumns);
        }
    }

}



int main(int argc, char *argv[]){

    //setHousingMarket(argv[1],atoi(argv[2]));

    if (argc != 3){
        printf("Error: incorrect number of command line arguments\n");
        exit(1);
    }

    if ((*argv[2]!='0') && (*argv[2]!='1') && (*argv[2]!='2')){
        printf("Error: not a valid verbosity level");
        exit(1);
    }
    int verbosity = atoi(argv[2]);

    FILE* inputFile = fopen(argv[1], "rb");

    if (inputFile == NULL){
        printf("Error reading the given file.");
        exit(1);
    }

    if (argv[1] == NULL){
        printf("Error: unable to open the file.");
        exit(1);

    }

    int numRows; //denoting no. of rows
    int numColumns; //denoting no.of columns
    int* housingRows = &numRows ; //points to no. of rows
    //int* housingColumns = &numColumns; // points to no. of columns
    int numRounds;
    //int* rounds = &numRounds; //points to no. of iterations
    float satisfaction; //denotes threshold
    //float* threshold = &satisfaction;
    int occupiedHouses1; //denotes no. of grid cells to be initialised to $
    //int* occupancy1 = &occupiedHouses1;

    int numRead = fscanf(inputFile, "%d %d", &numRows, &numColumns);
    int expectedCount = 2;
    checkNumRead(inputFile,numRead,expectedCount); //helper function to check if numRead is 2


    numRead = fscanf(inputFile,"%d",&numRounds);
    expectedCount = 1;
    checkNumRead(inputFile,numRead,expectedCount); //helper function to check if numRead is 1


    numRead = fscanf(inputFile,"%f", &satisfaction);
    expectedCount = 1;
    checkNumRead(inputFile,numRead,expectedCount);


    numRead = fscanf(inputFile, "%d", &occupiedHouses1);
    expectedCount = 1;
    checkNumRead(inputFile,numRead,expectedCount);


    char* housingArray = (char*) malloc(sizeof(char) * (numRows) * (numColumns));
    if (housingArray == NULL){
        printf("Malloc call failed!\n");
        fclose(inputFile);
        exit(3);
    }

    int numArray;
    int numIndex;
    //int* arrayNumber = &numArray; //reading in cell positions from text file
    //int* index = &numIndex;//reading in cell positions from text file

    for (int currRow = 0; currRow < numRows;currRow++){
        for (int currColumn = 0; currColumn < numColumns; currColumn++){

                  ( * (housingArray + (numColumns)* currRow + currColumn)) = ' ';

               //char* initialArray = &(* (housingArray + (* housingColumns)* currRow + currColumn));
               //printf("%c",* (housingArray + (numColumns)* currRow + currColumn));
          }
         //printf("\n");
    }


    for (int i = 0; i < (occupiedHouses1);i++){
        numRead = fscanf(inputFile,"%d %d", &numArray,&numIndex);
        expectedCount = 2;
        checkNumRead(inputFile,numRead, 2);
        if (((numArray >= 0) && (numArray < numRows)) && (numIndex >= 0) && (numIndex < numColumns)){
           * (housingArray + (numColumns)* (numArray) + (numIndex)) = '$';
        }

    }

    int occupiedHouses2; //denotes no. of grid cells to be initialised to .
    //int* occupancy2 = &occupiedHouses2;
    numRead = fscanf(inputFile, "%d", &occupiedHouses2);
    expectedCount = 1;
    checkNumRead(inputFile,numRead,expectedCount);


    for (int i = 0; i < (occupiedHouses2);i++){
        numRead = fscanf(inputFile,"%d %d", &numArray,&numIndex);
        expectedCount = 2;
        checkNumRead(inputFile,numRead, 2);
        if (((numArray >= 0) && (numArray < numRows)) && (numIndex >= 0) && (numIndex < numColumns)){
           * (housingArray + (numColumns)* (numArray) + (numIndex)) = '.';
        }

    }




    runUntilSatisfied(housingArray, *housingRows,numColumns,satisfaction, verbosity,numRounds);
    free(housingArray);
    housingArray = NULL;
    return 0;

}

