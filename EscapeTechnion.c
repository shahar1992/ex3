

/**====================Include files==============================*/
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "EscapeTechnion.h"
/**================================================================*/



/**======================Macros===================================*/

#define FACULTY_NUM 19

/**====================End of Macros==============================*/




/**====================Main entity decleration==============================*/
struct system_t{
    CompanySet companies;
    OrderList orders;
    EscaperSet escapers;
    long day;
    long faculty_profits[FACULTY_NUM];
};
/**====================end of entity decleration==============================*/









/**=====================Static functions declarations=========================*/
/**
 *
 */
static bool sysMailCheck(char *mail);
static bool facultyCheck(TechnionFaculty faculty);
static MtmErrorCode getSystemTime(char *time_format,long* hour,long* days_left);

/**===================End of static function declarations.====================*/

/**===================Escaper ADT functions implementation====================*/


/**===========================SystemCreate====================================*/



/**===================Static functions implementaion==========================*/
static bool sysMailCheck(char* mail){
    char* ptr=mail;
    int counter =0;
    while(*ptr!=0){
        if(*ptr=='@') counter++;
    }
    return (counter==1)?true:false;//return true if only 1 @;

}

