#ifndef METHODS
#define METHODS

// Included libraries
#include <QDebug> // TODO verify if we need this at the end of the project
#include <stdlib.h>
#include <math.h>

// Defines declarations
#define VERY_NEGATIVE 0.0
#define NEGATIVE 65.0
#define ZERO 125.0
#define POSITIVE 190.0
#define VERY_POSITIVE 255.0

// Function declarations
void setMiddlePoints(unsigned int, unsigned int);
unsigned int FuzzyCtl(unsigned int, unsigned int);
void memDegreeP(int);
void memDegreeI(int);
unsigned int fuzzyRules(void);

// Exported variables
extern unsigned int middlePoints[5][2];

#endif // METHODS

