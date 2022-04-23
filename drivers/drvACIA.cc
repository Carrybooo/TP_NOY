/* \file drvACIA.cc

   \brief Routines of the ACIA device driver

//

//      The ACIA is an asynchronous device (requests return

//      immediately, and an interrupt happens later on).

//      This is a layer on top of the ACIA.

//      Two working modes are to be implemented in assignment 2:

//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting

//      mode implements a synchronous IO whereas IOs are asynchronous

//      IOs are implemented in the Interrupt mode (see the Nachos

//      roadmap for further details).

//

//  Copyright (c) 1999-2000 INSA de Rennes.

//  All rights reserved.

//  See copyright_insa.h for copyright notice and limitation

//  of liability and disclaimer of warranty provisions.

//

*/



/* Includes */



#include "kernel/system.h"         // for the ACIA object

#include "kernel/synch.h"

#include "machine/ACIA.h"

#include "drivers/drvACIA.h"



//-------------------------------------------------------------------------

// DriverACIA::DriverACIA()

/*! Constructor.

  Initialize the ACIA driver.

  In the ACIA Interrupt mode,

  initialize the reception index and semaphores and allow

  reception interrupts.

  In the ACIA Busy Waiting mode, simply inittialize the ACIA

  working mode and create the semaphore.

  */

//-------------------------------------------------------------------------



DriverACIA::DriverACIA()

{
  #ifndef ETUDIANTS_TP
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
  #endif

  #ifdef ETUDIANTS_TP
  //initialisations des index et des semaphores
  if(g_cfg->ACIA = ACIA_INTERRUPT){
      ind_send, ind_rec = 0;
      g_machine->acia->SetWorkingMode(SEND_INTERRUPT | REC_INTERRUPT);
      send_sema = new Semaphore((char*) "Send", 1);
      receive_sema = new Semaphore((char*) "Receive", 0);
  }else if(g_cfg->ACIA = ACIA_BUSY_WAITING){
      ind_send, ind_rec = 0;
      send_sema = new Semaphore((char*) "Send", 1);
      receive_sema = new Semaphore((char*) "Receive", 1);
  }
  #endif
}



//-------------------------------------------------------------------------

// DriverACIA::TtySend(char* buff)

/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)

  */

//-------------------------------------------------------------------------



int DriverACIA::TtySend(char* buff)

{
  #ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
  #endif

  #ifdef ETUDIANTS_TP

  //on garde le précédent état pour pouvoir le restaurer plus tard
  //int previousWorkingMode = g_machine->acia->GetWorkingMode();

  if(g_cfg->ACIA == ACIA_BUSY_WAITING){
    //on définit l'état sur occupé
    //g_machine->acia->SetWorkingMode(BUSY_WAITING);

    //tant qu'il reste du contenu à transmettre
    while(buff[ind_send] != 0){
      //on attend activement si le registre de sortie est plein
      while(g_machine->acia->GetOutputStateReg() == FULL);
      //on ajoute au registre
      g_machine->acia->PutChar(buff[ind_send]);
      ind_send++;
    }

    //on remet l'état précédant la transmission
    //g_machine->acia->SetWorkingMode(previousWorkingMode);

    //on renvoie le nombre de caractères transmis
    return ind_send;

  }else if (g_cfg->ACIA == ACIA_INTERRUPT){
      //on réquisitionne un canal d'émission (ouvert par TtyReceive)
      send_sema->P();
      ind_send = 0;

      //on remplit le buffer d'envoi à partir du buffer donné en paramètre
	    while (ind_send < BUFFER_SIZE) {
		      send_buffer[ind_send] = buff[ind_send];
          if (buff[ind_send] == '\0') {
              break;
          }
		      ind_send++;
	   }

     //on sauvegarde le nombre de caractères envoyés et on le reset à 0 + vidage buffer
     int res_ind = ind_send;
	   ind_send = 0;
     //on envoie
     g_machine->acia->PutChar(send_buffer[ind_send]);

     //on renvoie le nombre de caractères transmis.
	   return res_ind;
  }

  return 0;
  #endif
}



//-------------------------------------------------------------------------

// DriverACIA::TtyReceive(char* buff,int length)

/*! Routine to reveive a message through the ACIA

//  (Busy Waiting and Interrupt mode).

  */

//-------------------------------------------------------------------------



int DriverACIA::TtyReceive(char* buff,int lg)
{
  #ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
  #endif

  #ifdef ETUDIANTS_TP

  //version attente active
  if(g_cfg->ACIA == ACIA_BUSY_WAITING){
    bool endTransmission = false;
    ind_rec = 0;
    while(!endTransmission && ind_rec < lg){
      //on attend activement tant que le buffer est vide
      while(g_machine->acia->GetInputStateReg() == EMPTY);
      //on reçoit
      receive_buffer[ind_rec] = g_machine->acia->GetChar();
      //si on arrive au bout de la chaine à recevoir, on met fin à la transmission.
      if(receive_buffer[ind_rec] == '\0') endTransmission = true;

      ind_rec++;
    }
    //on copie dans "buff" ce qui a été reçu dans receive_buffer
    memcpy(buff, receive_buffer, ind_rec);
    //on revoie le nombre de caractères reçus
    return ind_rec;


    //version interruptions
  }else if(g_cfg->ACIA == ACIA_INTERRUPT){
    receive_sema->P();
    memcpy(buff, receive_buffer, ind_rec);
    int res = ind_rec;
    ind_rec = 0;
    //on ouvre un canal donc on incrémente le semaphore.
    send_sema->V();
    g_machine->acia->SetWorkingMode(REC_INTERRUPT | SEND_INTERRUPT);
    //on renvoie le nombre de caractères reçus
    return res;
  }
  #endif

}





//-------------------------------------------------------------------------

// DriverACIA::InterruptSend()

/*! Emission interrupt handler.

  Used in the ACIA Interrupt mode only.

  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.

  */

//-------------------------------------------------------------------------



void DriverACIA::InterruptSend()

{
  #ifndef ETUDIANTS_TP
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
  #endif

  #ifdef ETUDIANTS_TP
  printf("--------Envoi du caractère %d : %c ----------\n", ind_send, send_buffer[ind_send]);
  if(send_buffer[ind_send] != 0){
    ind_send++;
    g_machine->acia->PutChar(send_buffer[ind_send]);
  }else{
    send_sema->V();
  }
  #endif
}



//-------------------------------------------------------------------------

// DriverACIA::Interrupt_receive()

/*! Reception interrupt handler.

  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA.

  Releases the receive_sema semaphore and disables reception

  interrupts when the last character of the message is received

  (character '\0').

  */

//-------------------------------------------------------------------------



void DriverACIA::InterruptReceive()

{
  #ifndef ETUDIANTS_TP
  printf("**** Warning: receive interrupt handler not implemented yet\n");
  exit(-1);
  #endif

  #ifdef ETUDIANTS_TP
  receive_buffer[ind_rec] = g_machine->acia->GetChar();
  printf("--------reception du caractère %d : %c ----------\n", ind_rec, receive_buffer[ind_rec]);
  if( (receive_buffer[ind_rec] == '\0') || (ind_rec+1 == BUFFER_SIZE)){
    receive_sema->V();
    g_machine->acia->SetWorkingMode(SEND_INTERRUPT);
  }else{
    ind_rec++;
  }
  #endif
}
