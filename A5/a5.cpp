
/*******************************************************************************
*                           INCLUDE FILES
*******************************************************************************/

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>

#include "ece650-prj.h"
#include "thread/thread_io.h"
#include "thread/thread_vc.h"

/*******************************************************************************
*                           NAMESPACE
*******************************************************************************/

using namespace std;

/*******************************************************************************
*                          ENUMERATION DEFINES
*******************************************************************************/

enum eMAIN_THREAD_STATE {
  eMain_CreateThread = 0,
  eMain_WaitForUser,
  eMain_WaitForResult,
};

/*******************************************************************************
*                           CONSTANT DEFINE
*******************************************************************************/

/*******************************************************************************
*                          STRUCTURE DEFINES
*******************************************************************************/

/*******************************************************************************
*                           VARIABLE  DEFINE
*******************************************************************************/

/*******************************************************************************
*                           FUNCTION PROTOTYPE  DEFINE
*******************************************************************************/

/*******************************************************************************
*                           LOCAL TABLE DEFINE
*******************************************************************************/

/*******************************************************************************

    Routine: main

    Description:
    Main routine

*******************************************************************************/

int main(int argc, char**argv) {

  static enum eMAIN_THREAD_STATE eState = eMain_CreateThread;
  static thread_ctrl_struct thread_ctrl[eTHREAD_NUM];
  static bool bstatus[eTHREAD_NUM] = {false};
  bool bSuccess = true;
  bool bReadFromFile = false;
  int optChar = -1;
  void * (*ptHandler)(void *);

  static vc_input_struct vc_in;
  static vc_output_struct vc_out;
  static io_input_struct io_in;
  static io_output_struct io_out;
 
  while((optChar = getopt(argc, argv, "tf")) != -1 ) {
    switch(optChar) {
    case 't':
      enable_display_time();
      break;
    case 'f':
      bReadFromFile = true;
      enable_display_time();
      enable_file_io();  
      break;
    default:
      break;
    }
  }
  while(bSuccess == true){
    switch(eState) {        /*state machine for the main thread */
    case eMain_CreateThread:
      for (int iThread = 0; iThread < eTHREAD_NUM; iThread++) {
	vc_thread_get((enum eTHREAD_LIST)iThread, &ptHandler);
	io_thread_get((enum eTHREAD_LIST)iThread, &ptHandler);
        thread_ctrl[iThread].status =  eTHREAD_RUNNING;
	bSuccess |= sem_init(&thread_ctrl[iThread].sem, 0, 0); 	// Initialize semaphore
	bSuccess |= pthread_attr_init(&thread_ctrl[iThread].attr); 	// Initialize the thread attribute
	bSuccess |= pthread_create(&thread_ctrl[iThread].id, &thread_ctrl[iThread].attr,        // Create thread 
				   ptHandler, (void*)&thread_ctrl[iThread] );
      }
      eState = eMain_WaitForUser;
      break;
    case eMain_WaitForUser:
      if (bReadFromFile == false) {
	if (thread_ctrl[eTHREAD_IO].status != eTHREAD_BLOCKED) {break;}
	else {io_output_get(eTHREAD_IO, &io_out);}
      }
      else {
	if (thread_ctrl[eTHREAD_FILE_IO].status != eTHREAD_BLOCKED) {break;}
	else {io_output_get(eTHREAD_FILE_IO, &io_out);}
      }
      if (io_out.edge_list.empty()) {
	thread_ctrl[eTHREAD_IO].status = eTHREAD_RUNNING;
	sem_post(&thread_ctrl[eTHREAD_IO].sem);
	break;
      }
      vc_in.vertex_num = io_out.vertex_num;
      vc_in.edge_list = io_out.edge_list;
      for (int iThread = 0; iThread < eTHREAD_NUM; iThread++)
      {
	if (iThread == eTHREAD_CNF_SAT || iThread == eTHREAD_APPROX1 || iThread == eTHREAD_APPROX2) {
	  vc_input_set((enum eTHREAD_LIST)iThread, &vc_in);
	}
      }
      for (int iThread = 0; iThread < eTHREAD_NUM; iThread++) {
	if (iThread == eTHREAD_CNF_SAT || iThread == eTHREAD_APPROX1 || iThread == eTHREAD_APPROX2) {
	  thread_ctrl[iThread].status = eTHREAD_RUNNING;
	  sem_post(&thread_ctrl[iThread].sem);}
      }
      eState = eMain_WaitForResult;
      break;
    case eMain_WaitForResult:
      if (thread_ctrl[eTHREAD_IO].status == eTHREAD_RUNNING) {break;}
      if (bReadFromFile == true && thread_ctrl[eTHREAD_FILE_IO].status == eTHREAD_RUNNING) {break;}
      for (int iThread = 0; iThread < eTHREAD_NUM; iThread++) {
	if (thread_ctrl[iThread].status == eTHREAD_RUNNING) {continue;}
	if (iThread != eTHREAD_CNF_SAT &&
	    iThread != eTHREAD_APPROX1 && iThread != eTHREAD_APPROX2) {continue;}
	if (bstatus[iThread] == true) {continue;}
	bstatus[iThread] = true;
	vc_output_get((enum eTHREAD_LIST)iThread, &vc_out);
	io_in.estatus = thread_ctrl[iThread].status;
	io_in.us_timestamp = thread_ctrl[iThread].us_timestamp;
	io_in.vertex_list = vc_out.vertex_list;
	switch(iThread) {
	case eTHREAD_CNF_SAT:
	  io_input_set(eTHREAD_IO, &io_in, eALG_CNF_VC);
	  io_input_set(eTHREAD_FILE_IO, &io_in, eALG_CNF_VC);
	  break;
	case eTHREAD_APPROX1:
	  io_input_set(eTHREAD_IO, &io_in, eALG_APPROX1);
	  io_input_set(eTHREAD_FILE_IO, &io_in, eALG_APPROX1);
	  break;
	case eTHREAD_APPROX2:
	  io_input_set(eTHREAD_IO, &io_in, eALG_APPROX2);
	  io_input_set(eTHREAD_FILE_IO, &io_in, eALG_APPROX2);
	  break;
	}
      }
      if (bstatus[eTHREAD_CNF_SAT] == true && bstatus[eTHREAD_APPROX1] == true &&
	bstatus[eTHREAD_APPROX2] == true) {
	memset(bstatus, false, eTHREAD_NUM);
	thread_ctrl[eTHREAD_IO].status = eTHREAD_RUNNING;
	thread_ctrl[eTHREAD_FILE_IO].status = eTHREAD_RUNNING;
	sem_post(&thread_ctrl[eTHREAD_IO].sem);
	sem_post(&thread_ctrl[eTHREAD_FILE_IO].sem);
        eState = eMain_WaitForUser;
      }      
      break;
    }
  }
  if (bSuccess == false) {
     std::cerr<< "Error: Unable to initialize thread" << std::endl; //Check for error
  }
  usleep(100000);
  return 0;
}

