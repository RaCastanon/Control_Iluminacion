#include "methods.h"

// Global variables declarations
unsigned int middlePoints[5][2];

// Controller variables
int p_err = 0;
int i_err = 0;

double mu_e_vn = 0.0;
double mu_e_n = 0.0;
double mu_e_z = 0.0;
double mu_e_p = 0.0;
double mu_e_vp = 0.0;

double mu_ei_vn = 0.0;
double mu_ei_n = 0.0;
double mu_ei_z = 0.0;
double mu_ei_p = 0.0;
double mu_ei_vp = 0.0;

double mu_u_voff = 0.0;
double mu_u_off = 0.0;
double mu_u_on = 0.0;
double mu_u_von = 0.0;

/* Function name: FuzzyCtl(uint, uint)
 * Developer:     Raul Castañon
 * Details:       Get control signal
 */
unsigned int FuzzyCtl(unsigned int intensityLevel, unsigned int setPoint)
{
    // Local variable declarations
    unsigned int ut = 0; // This will be the return control variable

    // Get the proportional error
    p_err = setPoint - intensityLevel;
    // Verify limits of the proportional error
    if(p_err >= 255)
    {
        p_err = 250;
    }
    else if (p_err <= 0)
    {
        p_err = 5;
    }
    else
    {
        // do nothing
    }
    // Get the integral error
    i_err += p_err;
    // Verify limits of the integral error
    if(i_err >= 255)
    {
        i_err = 250;
    }
    else if (i_err <= 0)
    {
        i_err = 5;
    }
    else
    {
        // do nothing
    }
    // Set the membership degree variables for p_err
    memDegreeP(p_err);
    // Set the membership degree variables for i_err
    memDegreeI(i_err);
    // Apply the fuzzy rules
    ut = fuzzyRules();
    // Get the output from the controler
    return ut;
}

/* Function name: fuzzyRules(void)
 * Developer:     Raul Castañon
 * Details:       Apply fuzzy rules
 */
unsigned int fuzzyRules(void)
{
    // Local variable declarations
    double u_voff[6], u_off[6], u_on[6], u_von[6];
    double ut_voff = 0.0;
    double ut_off = 0.0;
    double ut_on = 0.0;
    double ut_von = 0.0;

    double ut = 0.0;
    // Local variable initialization
    memset(u_voff, 0x00, sizeof(u_voff));
    memset(u_off, 0x00, sizeof(u_off));
    memset(u_on, 0x00, sizeof(u_on));
    memset(u_von, 0x00, sizeof(u_von));
    // Rule one
    u_voff[0] = std::min(mu_e_vn, mu_ei_vn);
    // Rule two
    u_voff[1] = std::min(mu_e_vn, mu_ei_n);
    // Rule three
    u_voff[2] = std::min(mu_e_vn, mu_ei_z);
    // Rule four
    u_voff[3] = std::min(mu_e_vn, mu_ei_p);
    // Rule five
    u_voff[4] = std::min(mu_e_vn, mu_ei_vp);
    // Rule six
    u_voff[5] = std::min(mu_e_n, mu_ei_vn);
    // Rule seven
    u_off[0] = std::min(mu_e_n, mu_ei_n);
    // Rule eight
    u_off[1] = std::min(mu_e_n, mu_ei_z);
    // Rule nine
    u_off[2] = std::min(mu_e_n, mu_ei_p);
    // Rule ten
    u_off[3] = std::min(mu_e_n, mu_ei_vp);
    // Rule eleven
    u_off[4] = std::min(mu_e_z, mu_ei_vn);
    // Rule twelve
    u_off[5] = std::min(mu_e_z, mu_ei_n);
    // Rule thirteen
    u_on[0] = std::min(mu_e_z, mu_ei_z);
    // Rule fourteen
    u_on[1] = std::min(mu_e_z, mu_ei_p);
    // Rule fiveteen
    u_on[2] = std::min(mu_e_z, mu_ei_vp);
    // Rule sixteen
    u_on[3] = std::min(mu_e_p, mu_ei_vn);
    // Rule seventeen
    u_on[4] = std::min(mu_e_p, mu_ei_n);
    // Rule eighteen
    u_on[5] = std::min(mu_e_p, mu_ei_z);
    // Rule nineteen
    u_von[0] = std::min(mu_e_p, mu_ei_p);
    // Rule twenty
    u_von[1] = std::min(mu_e_p, mu_ei_vp);
    // Rule twentyone
    u_von[2] = std::min(mu_e_vp, mu_ei_vn);
    // Rule twentytwo
    u_von[3] = std::min(mu_e_vp, mu_ei_n);
    // Rule twentythree
    u_von[4] = std::min(mu_e_vp, mu_ei_z);
    // Rule twentyfour
    u_von[5] = std::min(mu_e_vp, mu_ei_p);

    // Get the module of each output membership
    ut_voff = sqrt(pow(u_voff[0], 2.0) + pow(u_voff[1], 2.0)+ pow(u_voff[2], 2.0)+ pow(u_voff[3], 2.0)+ pow(u_voff[4], 2.0)+ pow(u_voff[5], 2.0));
    ut_off = sqrt(pow(u_off[0], 2.0) + pow(u_off[1], 2.0)+ pow(u_off[2], 2.0)+ pow(u_off[3], 2.0)+ pow(u_off[4], 2.0)+ pow(u_off[5], 2.0));
    ut_on = sqrt(pow(u_on[0], 2.0) + pow(u_on[1], 2.0)+ pow(u_on[2], 2.0)+ pow(u_on[3], 2.0)+ pow(u_on[4], 2.0)+ pow(u_on[5], 2.0));
    ut_von = sqrt(pow(u_von[0], 2.0) + pow(u_von[1], 2.0)+ pow(u_von[2], 2.0)+ pow(u_von[3], 2.0)+ pow(u_von[4], 2.0)+ pow(u_von[5], 2.0));

    ut = (unsigned int) ((ut_voff * NEGATIVE) +(ut_off * ZERO) + (ut_on * POSITIVE) + (ut_von * VERY_POSITIVE));
    return ut;

}

/* Function name: memDegreeI(int)
 * Developer:     Raul Castañon
 * Details:       Get membership degree
 */
void memDegreeI(int i_err)
{
    // Function from the VERY_NEGATIVE area
    if((VERY_NEGATIVE <= i_err) && (NEGATIVE > i_err))
    {
        mu_ei_vn = (NEGATIVE - i_err)/(NEGATIVE - VERY_NEGATIVE);
    }
    else
    {
        mu_ei_vn = 0.0;
    }
    // Funtion of the NEGATIVE area
    if((VERY_NEGATIVE <= i_err) && (NEGATIVE > i_err))
    {
        mu_ei_n = (i_err - VERY_NEGATIVE)/(NEGATIVE - VERY_NEGATIVE);
    }
    else if((NEGATIVE <= i_err) && (ZERO > i_err))
    {
        mu_ei_n = (ZERO - i_err)/(ZERO - NEGATIVE);
    }
    else
    {
        mu_ei_n = 0.0;
    }
    // Function of the ZERO area
    if((NEGATIVE <= i_err) && (ZERO> i_err))
    {
        mu_ei_z = (i_err - NEGATIVE)/(ZERO - NEGATIVE);
    }
    else if((ZERO <= i_err) && (POSITIVE > i_err))
    {
        mu_ei_z = (POSITIVE - i_err)/(POSITIVE - ZERO);
    }
    else
    {
        mu_ei_z = 0.0;
    }
    // Function of the POSITIVE area
    if((ZERO <= i_err) && (POSITIVE> i_err))
    {
        mu_ei_p = (i_err - ZERO)/(POSITIVE - ZERO);
    }
    else if((POSITIVE <= i_err) && (VERY_POSITIVE > i_err))
    {
        mu_ei_p = (VERY_POSITIVE - i_err)/(VERY_POSITIVE - POSITIVE);
    }
    else
    {
        mu_ei_p = 0.0;
    }
    // Function of the VERY_POSITIVE area
    if((POSITIVE <= i_err) && (VERY_POSITIVE> i_err))
    {
        mu_ei_p = (i_err - POSITIVE)/(VERY_POSITIVE - POSITIVE);
    }
    else
    {
        mu_ei_p = 0.0;
    }

}

/* Function name: memDegreeP(int)
 * Developer:     Raul Castañon
 * Details:       Get membership degree
 */
void memDegreeP(int p_err)
{
    // Function from the VERY_NEGATIVE area
    if((VERY_NEGATIVE <= p_err) && (NEGATIVE > p_err))
    {
        mu_e_vn = (NEGATIVE - p_err)/(NEGATIVE - VERY_NEGATIVE);
    }
    else
    {
        mu_e_vn = 0.0;
    }
    // Funtion of the NEGATIVE area
    if((VERY_NEGATIVE <= p_err) && (NEGATIVE > p_err))
    {
        mu_e_n = (p_err - VERY_NEGATIVE)/(NEGATIVE - VERY_NEGATIVE);
    }
    else if((NEGATIVE <= p_err) && (ZERO > p_err))
    {
        mu_e_n = (ZERO - p_err)/(ZERO - NEGATIVE);
    }
    else
    {
        mu_e_n = 0.0;
    }
    // Function of the ZERO area
    if((NEGATIVE <= p_err) && (ZERO> p_err))
    {
        mu_e_z = (p_err - NEGATIVE)/(ZERO - NEGATIVE);
    }
    else if((ZERO <= p_err) && (POSITIVE > p_err))
    {
        mu_e_z = (POSITIVE - p_err)/(POSITIVE - ZERO);
    }
    else
    {
        mu_e_z = 0.0;
    }
    // Function of the POSITIVE area
    if((ZERO <= p_err) && (POSITIVE> p_err))
    {
        mu_e_p = (p_err - ZERO)/(POSITIVE - ZERO);
    }
    else if((POSITIVE <= p_err) && (VERY_POSITIVE > p_err))
    {
        mu_e_p = (VERY_POSITIVE - p_err)/(VERY_POSITIVE - POSITIVE);
    }
    else
    {
        mu_e_p = 0.0;
    }
    // Function of the VERY_POSITIVE area
    if((POSITIVE <= p_err) && (VERY_POSITIVE> p_err))
    {
        mu_e_p = (p_err - POSITIVE)/(VERY_POSITIVE - POSITIVE);
    }
    else
    {
        mu_e_p = 0.0;
    }
}

/* Function name: setMiddlePoints(uint, uint)
 * Developer:     Raul Castañon
 * Details:       Get middle points from the data of the image
 */
void setMiddlePoints(unsigned int cols, unsigned int rows)
{
    // Initialize global variable
    memset(middlePoints, 0, sizeof(middlePoints));

    // Get middle points
    middlePoints[0][0] = (unsigned int)(cols/2.0);
    middlePoints[0][1] = (unsigned int)(rows/2.0);

    middlePoints[1][0] = (unsigned int)(cols/4.0);
    middlePoints[1][1] = (unsigned int)(rows/4.0);

    middlePoints[2][0] = (unsigned int)(3*(cols/4.0));
    middlePoints[2][1] = (unsigned int)(rows/4.0);

    middlePoints[3][0] = (unsigned int)(cols/4.0);
    middlePoints[3][1] = (unsigned int)(3*(rows/4.0));

    middlePoints[4][0] = (unsigned int)(3*(cols/4.0));
    middlePoints[4][1] = (unsigned int)(3*(rows/4.0));
}
