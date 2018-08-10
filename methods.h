#ifndef METHODS
#define METHODS

// Included libraries
#include <QDebug> // TODO verify if we need this at the end of the project
#include <stdlib.h>
#include <math.h>

// Defines declarations for the Fuzzy input sets
#define VERY_NEGATIVE  -254.0
#define NEGATIVE       -125.0
#define ZERO            0.0
#define POSITIVE        125.0
#define VERY_POSITIVE   255.0

// Defines declarations for the Fuzzy output sets
#define OUT_NEGATIVE     64.0
#define OUT_ZERO        124.0
#define OUT_POSITIVE    190.0
#define OUT_V_POSITIVE  254.0

#define HEADROOM                5
#define FUZZY_SETS_UPPER_LIMIT  255
#define FUZZY_SETS_LOWER_LIMIT -254

// Function declarations
void setMiddlePoints(unsigned int, unsigned int);
unsigned int FuzzyCtl(unsigned int, unsigned int);
void memDegreeP(int);
void memDegreeI(int);
unsigned int fuzzyRules(void);

// Exported variables
extern unsigned int middlePoints[5][2];

#endif // METHODS

