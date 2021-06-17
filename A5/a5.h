/*******************************************************************************/

#ifndef __ECE650_PRJ_H
#define __ECE650_PRJ_H

/*******************************************************************************
*                           INCLUDE FILES
*******************************************************************************/

#include <vector>
#include <semaphore.h>
#include <pthread.h>

/*******************************************************************************
*                           GLOBAL CONSTANT
*******************************************************************************/

#define ALGORITHM_TIMEOUT_US                                     30000000

/*******************************************************************************
*                           STRUCTURE DEFINES
*******************************************************************************/


enum eTHREAD_LIST {
  eTHREAD_CNF_SAT = 0,
  eTHREAD_APPROX1,
  eTHREAD_APPROX2,
  eTHREAD_IO,
  eTHREAD_FILE_IO,
  eTHREAD_NUM,
};

enum eTHREAD_STATUS {
  eTHREAD_BLOCKED = 0,
  eTHREAD_TIMEOUT,
  eTHREAD_RUNNING,
};

enum eRETURN_STATUS {
  eSTATUS_OK = 0,
  eSTATUS_ERROR,
};

typedef struct {
  pthread_t id;
  pthread_attr_t attr;
  sem_t sem;
  long us_timestamp;
  enum eTHREAD_STATUS status;
}thread_ctrl_struct;


/*******************************************************************************
*                           Global Function Prototypes
*******************************************************************************/


#endif
