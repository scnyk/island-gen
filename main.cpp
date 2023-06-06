/*
Name: scnyk
Date: April 01, 2023
Description: This program takes several inputs and builds three maps for a virtual island using 2-D arrays, and generates this island using random movement generations for particles
Usage: <exe> | <exe> <-s> <seed integer> 
*/

#include <iostream> // necessary library for most things
#include <stdio.h>  // necessary library for most things
#include <fstream>  // necessary for writing to the output file which assn4 requires
#include <cmath>    // necessary for cos and sin to create a dropzone
#include <iomanip>  // necessary for setw(x) 
#include <cstring>  // necessary for strlen and strcmp

using namespace std;



/* makeNum takes a string input (designed for integers that are in a data type string i.e. integer inputs from command line char** argv) and turns it into a data type int */
int makeNum(char* stringInt) {  
    int result = 0; 
    int lenStr = strlen(stringInt); 
    
    for(int 
    = 0; i < lenStr; i++) {
        if(isdigit(stringInt[i]))
        {
            int digit = stringInt[i] - '0'; // convert each charcter to integer 
            int pos = lenStr - i - 1; // calculate the correct power of 10 so that the digit is at the right place
            
            result += digit * pow(10, pos); // add the digit multiplied by the correct power of 10 so it doesn't just add to the number in the wrong place
        }
        else 
        {
            return -1;  // -1 is the error or invalid return value 
        }
    }
    
    return result; 
}


/* changeSrand takes the command-line args from the main function, and both sets the srand and checks if command-line args are valid and/or contain the '-s' argument */
int changeSrand(int argc, char** argv)
{
    srand(time(0)); // sets srand to computer time(0), which will change if the proper args are passed in
    if(argc==3)
    {
        int seed = makeNum(argv[2]);    // makeNum returns -1 if arg #3 is not an integer, or returns the integer version of the argument
        if(strcmp(argv[1],"-s")==0 && seed!=-1) // -s only acceptable input (since im not doing -E extra credit)
        {
            srand(seed);    // set srand to input seed
        }
        else
        {
            return -1;  // error or invalid return value -> results in usage message
        }
    } 
    else if (argc!=1){  // if the user doesnt put in 3 args (for the <-s> <int>)
        return -1;  
    }
    return 0;   // returns 0 if no error value was returned, which when called lets the main menu continue
}


/* Complete(!) */
/* makeParticleMap creates an int** given the correct parameters */
/* Honestly didn't need to use this */
int** makeParticleMap(int width, int height, int centerX, int centerY, int radius, int numParticles, int maxLife)
{
    int** map = new int*[height];   // creates first dimension array
    for (int i = 0; i < height; ++i) {
        map[i] = new int[width];    // makes the array 2D by making 'width' size arrays in map
    }

    return map;
}

/* Complete(!) */
/* Iterates through each value of the int** map and prints it, with an endl at the end of each row */
void printMap(int** map, int width, int height, const char* filename)
{

    ofstream outfile;
    outfile.open(filename,fstream::app); // writes the islands (assn4 says just the islands, not the inputs or prompts) to filename in append mode
    /* ^ assn4 doesnt specify to make the file changeable so I'm leaving it hard-coded */
    for (int i = 0 ; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << std::setw(4) << map[i][j];  // space between each number (including the number itself) set to 4
            outfile << std::setw(4) << map[i][j]; // ^
            /* Add outfile compatability */
        }
        cout << endl;   // newline at the end of each row
        outfile << endl; // ^
    }
    outfile.close();    // deallocate memory
    
}

/* Incomplete */
/* Iterates through each value of the char** map and prints it, with an endl at the end of each row */
/* Literally the same as printMap, but made for char**, since the polished map is char** and can't be printed with printMap */
void printCharMap(char** map, int width, int height, const char* filename)
{

    ofstream outfile;
    outfile.open(filename,fstream::app); // writes the islands to filename in append mode
    for (int i = 0 ; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << std::setw(1) << map[i][j];
            outfile << std::setw(1) << map[i][j];

        }
        cout << endl; // newline at each new row
        outfile << endl; // ^
    }
    outfile.close();    // deallocate memory
}

/* Complete(!) */
/* Uses a running max value, set initially to the first value of the map, and just iterates through the map, changing the max anytime a value is greater than it */
int findMax(int** map, int width, int height)
{
    int max = map[0][0];
    for (int y = 0; y < height; y++)    // iterates through y
    {
        for (int x = 0; x < width; x++) // iterates through x
        {
            if(map[y][x]>max)   // if greater than running max
            {
                max=map[y][x];  // replace the running max
            }
        }
    }
    return max;
}


/* Complete(!) */
/* numMoves returns the number of possible moves around a given location within the map */
int numMoves (int** map, int width, int height, int x, int y)
{
    int count = 0;  // initial count to be incremented as moves become valid
    for (int cx = -1; cx <= 1; cx++)
    {
        for (int cy = -1; cy <= 1; cy++)
        {
            if (!(cx==0 && cy==0))  // cx and cy can't be 0 or it will be checking the current index
            {
                if((x+cx)>=0 && (x+cx)<width)    // x is within width
                {
                    if((y+cy)>=0 && (y+cy)<height)   // y is within height
                    {
                        if(map[y+cy][x+cx]<=map[y][x])  // checks if new index is less than or equal to curremt index
                        {
                            count++;    // increment count of valid moves
                        }
                    }
                }
            }
        }
    }
    return count;
}


/* Complete(!) */
/* possibleMoves returns an array of coordinate-pairs around a given index in the map that are within the map bounds and are considered valid */
int** possibleMoves(int** map, int width, int height, int x, int y)
{
    int num = numMoves(map,width,height,x,y);
    int** answers = new int*[num];   // creates first dimension array
    for (int i = 0; i < num; ++i) { // should iterate up to num, not height
        answers[i] = new int[2];    // makes the array 2D by making width size arrays in map
    }
    int index = 0;
    
    for (int cx = -1; cx <= 1; cx++)
    {
        for (int cy = -1; cy <= 1; cy++)
        {
            if (!(cx==0 && cy==0))  // cx and cy can't be 0 or it will be checking the current index
            {
                if((x+cx)>=0 && (x+cx)<width)    // x is within width
                {
                    if((y+cy)>=0 && (y+cy)<height)   // y is within height
                    {
                        if(map[y+cy][x+cx]<=map[y][x])  // checks if new index <= current index
                        {   
                            answers[index][0] = y+cy;   //      adds new y to array
                            answers[index][1] = x+cx;   //      adds new x to array
                            index++;    // increase the index (next row in 2D array) for the next coordinate pair's location
                        }
                    }
                }
            }
        }
    }
    
    return answers;
}


/* Complete(!)*/
/* Checks if there are any possible moves and returns a boolean true the second one is found */
/* Functionally could be replaced with numMoves but it's a little bit faster so I just did */
bool moveExists(int** map, int width, int height, int x, int y)
{
    
    for (int cx = -1; cx <= 1; cx++)
    {
        for (int cy = -1; cy <= 1; cy++)
        {
            if (!(cx==0 && cy==0))  // cx and cy can't be 0 or it will be checking the current index
            {
                if((x+cx)>=0 && (x+cx)<width)    // x is within width
                {
                    if((y+cy)>=0 && (y+cy)<height)   // y is within height
                    {
                        if(map[y+cy][x+cx]<=map[y][x])  // checks new index with current index
                        {
                            return true;    // return true immediately if any spots are valid
                        }
                    }
                }
            }
        }
    }
    return false;   // if none were true, returns false
}


/* Complete(!) */
/* returns a random double between 0-1 */
double frand()
{
    double output = (double)rand() / RAND_MAX;

    return output;
}
/* Usage; 4*frand() = 0-3*/


/* Complete(!) */
/* normalize iterates through each int** map value and sets it to the value / max * 255 */
void normalize(int** map,int width,int height)   // passing in the int** map, any alterations change the actual map
{
    int max = findMax(map,width,height);    // uses findMax for the actual max value in the entire map
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            double a = (double) map[i][j];
            double b = (double) max;        // used multiple variables to make the double and int cast and operations aa bit easier and more clear
            double diff = a/b;

            map[i][j] = (int) (255.0 * diff) ; // divide by max and multiple by 255 range is 0-255
        }
    }
}


/* Complete(!) */
/* polish creates a new char** polmap, and iterates through the int** map, checking the value and assigning a respective character to the polmap */
char** polish(int** map, int width, int height,int waterLine)
{
    char** polmap = new char*[height];   // intializes the polished map with the specified height and width
    for (int i = 0; i < height; ++i) {
        polmap[i] = new char[width];
    } 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int mv = map[i][j]; // mv = map value, just shortening for ease of typing repetition
            double landZone = (double)(255-waterLine);   // remaining landZone to calculate later chars
            if (mv<(waterLine/2))                           // deep water
            {   
                polmap[i][j]='#';
            } 
            else if (mv<=waterLine)                         // shallow/water
            {
                polmap[i][j]='~';
            }
            else if (mv<(waterLine+(int)(0.15*landZone)))   // coast/beach
            {
                polmap[i][j]='.';
            }
            else if (mv<(waterLine+(int)(0.40*landZone)))   // plains/grass
            {
                polmap[i][j]='-';
            }
            else if (mv<(waterLine+(int)(0.8*landZone)))    // forests
            {
                polmap[i][j]='*';
            }
            else                                            // mountains
            {
                polmap[i][j]='^';
            }
        }
    }
    return polmap;
}

/* Complete(!) */
/* dropParticle takes the below inputs and calculates a new dropX and dropY, modifying the inputted dropX,dropY address values accordingly*/
void dropParticle(int** map,int width, int height, int centerX, int centerY, int radius, int numParticles, int maxLife, int& dropX, int& dropY)
{       // changes dropX and dropY to a random coordinate pair within a radius of centerX and centerY
    
    double sqrtfrand = sqrt(frand());   
    double a = (double)radius;

    radius = (int)(a * sqrtfrand)+1;    // takes the radius times the sqrt of frand and casts it to an integer +1 since it rounds down to zero

    double theta = frand() * 2.0 * 3.1415;  // separated variables and operations for easier double and int casting 

    int cosrad = ((double)radius * cos(theta));
    dropX = (int)(centerX + cosrad);
    int sinrad = ((double)radius * sin(theta)); 
    dropY = (int)(centerY + sinrad); 
    dropX = max(0,dropX);   //  makes sure dropX,dropY are greater than zero and less than width,height so that there are no segmentation faults
    dropY = max(0,dropY);   //^
    dropX = min(dropX,width-1); //^
    dropY = min(dropY,height-1); //^

   
}


/* Complete(!) */
/* deleteIntMap deletes each dynamic array in the inputted 2d array, for after the program is complete in order to deallocate memory from the maps */
void deleteIntMap(int** map, int height) {  // deletes a 2d array by deleting the dynamic arrays within the dynamic array
    for (int i = 0; i < height; ++i) {
        delete[] map[i];
    }
    delete[] map;
}

/* Complete(!) */
/* same as deleteIntMap, but for char** arrays */
void deleteCharMap(char** map, int height) {     
    for (int i = 0; i < height; ++i) {
        delete[] map[i];
    }
    delete[] map;
}


/* Complete(!) */
/* particleRoll is a bit heavier, so it has a lot of comments inside to explain the steps*/
int particleRoll(int** map,int width, int height, int centerX, int centerY, int numParticles, int maxLife,int radius)
{

   for (int i = 0; i < numParticles; i++)
   {                  
        /* looping marks new particle, and repeats until numParticles reaches 0 */
        
        /*To test and debug:
        printMap(map,width,height);
        cout << "Particle: " << i <<endl;*/
        
        
        int dropX,dropY;    // declare dropX,dropY to be modified in dropParticle
        dropParticle(map,width,height,centerX,centerY,radius,numParticles,maxLife,dropX,dropY);     // assigns dropX and dropY random values within the dropZone radius around the center coordinates
        int x = dropX;  // set the x to be changed during the life
        int y = dropY;  // set the y to be changed during the life
        if(maxLife>0)
        {
            map[y][x]++;    // increment initial dropX,dropY
        }
        int particleLife = maxLife-1; // how many lives in each particle, subtracting one life for the initial dropZone increment which I am counting as a life
                    /* ^ because if the life was 0 the dropZone wouldn't be incremented, and if the life was 1 it would just be the dropZone with a value */
        
        bool canMove = moveExists(map,width,height,x,y);    // moves exist for this aprticle?
        while (canMove && particleLife>0)   // looping marks new life, same particle
        {                                          

        /* In this loop, which repeats until the particle runs out of lives or moves, a new coordinate pair is randomly chosen from the array of valid coordinates
            made in the function possibleMoves, and the current coordinates are set to those new ones, incrementing at this new spot and reevaluating 'canMove' before repeating
        */
            int** moves = possibleMoves(map, width,height,x,y); // array of valid coordinate pairs that particle can move to
            int moveNum = numMoves(map, width, height, x, y); // number of moves in order to uses as a max for randomize
            int randIndex = (int)(moveNum*frand()); // random index between 0 and the number of moves possible - 1 (because the array is 0-indexed)
            y = moves[randIndex][0];   // y change
            x = moves[randIndex][1];   // x change
            map[y][x]++;    // increment the new location
            canMove = moveExists(map,width,height,x,y); // changes the truth value of canMove to the new location's canMove
            deleteIntMap(moves,moveNum);    // deallocate memory
            particleLife--;    // lose a life on each loop
        }   
        /* If no moves were available after the initial drop, it immediately skips the while and loses a life, moving on to the next particle*/
    }
    return 0;
}


/*
TODO:
Add description and usage to the header
(?) Do color extra credit?
*/

/* Complete(!) */
/* Initialize each value in the int** map to 0  */
void instatiateMap(int** map,int width, int height)
{   
    for(int i = 0; i < height; i++) // iterate through height
    {
        for(int j = 0; j< width; j ++) // iterate through width
        {
            map[i][j]=0;    // set index value to 0
        }
    }
}

/* Complete(!) */
/* main function completes the UI and calls the other above functions as necessary, further comments in the function below */
int main (int argc, char** argv)        
{
    /* ~ Command-Line Arg Check Section and srand-setting ~ */
    int checkArgs = changeSrand(argc,argv); // returns -1 if the command line args were invalid
    if(checkArgs==-1)   // checks if the args ^ were invalid, in which case it prints a usage/syntax statement 
    {
        cerr << "Usage: <exe> <-s> <seed>" << endl; // i don't think cerr is any different visually from cout, but i think i am supposed to use it for errors so i tried it
    }
    else
    {
        /* ~ UI Section and initial info retrieval ~ */
        cout << "Welcome to scnyk's island generator!\n" << endl;
        cout << "Enter grid width: " << endl;
        int width;
        cin >> width;

        /*
        The following while-loop structure is repeated several times in this main function, so this comment is for all of them:

        If the cin throws an error due to an incorrect data-type input, the while loop will reprompt the user to enter the correct data-type
        It uses cin.clear to erase the error messages and cin.ignore to ignore invalid input
        If the newly entered cin value is still not the correct data type (integer) then the loop repeats
        Each while-loop is the same except for the variable being assigned (i.e. width, height, x-coordinate, etc.)
        */

        while ( !cin || width<=0)   //   this while loop checks for errors and makes sure the width is a nonnegative and nonzero integer
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter a positive integer for grid width: " << endl;
            cin >> width;    
        }
        cout << "Enter grid height: " << endl;
        int height;
        cin >> height;
        while ( !cin || height<=0)  // this while loop checks for errors and makes sure the height is a nonnegative and nonzero integer
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter a positive integer for grid height: " << endl;
            cin >> height;    
        }
        cout << "Enter drop-zone x-coordinate (0-" << (width-1) << "): " << endl;
        int centerX;
        cin >> centerX;
        while ( !cin || centerX<0 || centerX>=width ) // this while loop monitors for both errors and inputs outside of the valid X range
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter an integer (0-"<< (width-1) <<") for the the x-coordinate: " << endl;
            cin >> centerX;    
        }
        cout << "Enter drop-zone y-coordinate (0-" << (height-1) << "): " << endl;
        int centerY;
        cin >> centerY;
        while ( !cin || centerY<0 || centerY>=height) // this while loop monitors for both errors and inputs outside of the valid Y range
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter an integer (0-"<< (height-1) <<") for the y-coordinate: " << endl;
            cin >> centerY;    
        }
        int radius;
        cout << "Enter drop-zone radius (minimum 2): " << endl;
        cin >> radius;
        while ( !cin || radius<2)   // this while loop monitors for both errors and inputs of under 2, since the radius has a minimum
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter an integer (minimum 2) for the radius: " << endl;
            cin >> radius;    
        }
        cout << "Enter number of particles to drop: " << endl;
        int numParticles;
        cin >> numParticles;
        while ( !cin || numParticles<0 )    // checks for errors and makes sure numParticles is 0 or greater
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter a nonnegative integer for the number of particles: " << endl;
            cin >> numParticles;    
        }
        cout << "Enter max life of particles: " << endl;
        int maxLife;
        cin >> maxLife;
        while ( !cin || maxLife<0 )    // checks for errors and makes sure maxLife is 0 or greater
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter a nonnegative integer for the maximum particle life: " << endl;
            cin >> maxLife;    
        }
        cout << "Enter value for waterline (40-200): " << endl;
        int waterLine;
        cin >> waterLine;
        while ( !cin || waterLine>200 || waterLine<40)  // checks for errors and makes sure waterLine is within the appropriate range
        {
            cin.clear ();
            cin.ignore ( 1000 , '\n' );    
            cout << "Please enter an integer (40-200) for the waterline: " << endl;
            cin >> waterLine;    
        } 

        /*  Print statements to check the input variables when debugging:
        cout << "Width: " << width << endl;
        cout << "height: " << height << endl;
        cout << "centerX: " << centerX << endl;
        cout << "centerY: " << centerY << endl;
        cout << "radius: " << radius << endl;
        cout << "nump: " << numParticles << endl;
        cout << "maxl: " << maxLife << endl;
        */

        int** map = new int*[height];   // intializes the map with the specified height and width
        for (int i = 0; i < height; ++i) {
            map[i] = new int[width];
        }    

        const char* filename = "island.txt";    // I only intend to only write to "island.txt" so i hard coded it, but an input name can be used to make it open-ended
        ofstream outfile;
        outfile.open(filename,fstream::app);    // i opened the files in append mode in order to write all of the text and islands with each other and prevent overwriting

        instatiateMap(map,width,height);    // initializes map with all 0's

        particleRoll(map,width,height,centerX,centerY,numParticles,maxLife,radius); // rolls the particles onto the map, full map generation in this function

        cout << "\nRaw Island:" << endl; 
        outfile << "\nRaw Island:" << endl; 
        printMap(map,width,height,filename);     // print raw original map before normalization

        cout << "\nNormalized:" << endl;
        outfile << "\nNormalized:" << endl; 
        normalize(map,width,height);    // normalize map

        printMap(map,width,height,filename);     // print normalized map

        char** polmap;  
        polmap = polish(map,width,height,waterLine);    // polish map

        cout << "\nFinal:" << endl;
        outfile  << "\nFinal:" << endl;
        printCharMap(polmap,width,height,filename);  // print polished map

    
        outfile.close();
        deleteCharMap(polmap,height);   // return memory from both dynamic 2D arrays
        deleteIntMap(map,height);       // ^
        return 0;   // return an int for int main
    }
}
