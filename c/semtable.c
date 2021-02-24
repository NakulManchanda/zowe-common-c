#include <sys/sem.h>
#include "logging.h"
#include "semtable.h"
#include "isgenq.h"



void initSemTable() {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start initSemTable\n");
    /* Create semaphore table for datasets */
  for(int i=0; i < N_SEM_TABLE_ENTRIES; i++) {
    sem_table[i].sem_ID = 0;  /* initialise */
  }
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end initSemTable\n");
}

int setAndIncrementSemaphore(int semaphoreID) {
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start setAndIncrementSemaphore\n");
    /*******/
    /* set */
    /*******/
    // semctl() changes permissions and other characteristics of a semaphore set
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 1;
    int semnum = 0;    /* array index zero */
    int semaphoreRetcode;

    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "semctl\n");
    semaphoreRetcode = semctl(semaphoreID, semnum, SETVAL, arg); /* set the value of our semaphore */
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "semctl %d\n", semaphoreRetcode);
    return semaphoreRetcode;
}

int postSemaphore(int entryId) {/* the HTTP GET that consumes this message insists that the response body be JSON */
    // TODO: Don't why it returns here earlier, was rest of code unreachable, after we call respondWithMessage
    // respondWithMessage(response, HTTP_STATUS_OK,
    //            "{\"records\":[\"Enqueue dataset successful\"]}"
    //            ); 

    /********/
    /* wait */
    /********/
    /* WAIT! */
    int semaphoreRetcode;
    int semaphoreID = sem_table[entryId].sem_ID;
    /* define semaphore operation to be performed */
    struct sembuf semaphoreBuffer[1]; /* just one semaphore in the array */
    struct sembuf *semaphoreOps = &semaphoreBuffer[0];
    semaphoreBuffer[0].sem_num = 0; /* index of first and only semaphore */
    semaphoreBuffer[0].sem_op  = 0; /* 0 = wait */
    semaphoreBuffer[0].sem_flg = 0; /* 0 = sychronous + don't undo */

    /* wait for semaphore to be posted */
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "semop - wait for semphore posting\n");
    semaphoreRetcode = semop(semaphoreID, semaphoreOps, 1); /* 1 = one entry */
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "semop %d - semaphore posted\n", semaphoreRetcode);
    /* we are now waiting for our semaphore to be posted ... */
    
    if (semaphoreRetcode == 0 ) {
      /* destroy our semaphore */
      zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "semctl destroy semaphore\n");
      semaphoreRetcode = semctl(semaphoreID, 0, IPC_RMID);
      if (semaphoreRetcode != -1 ) {
          sem_table[entryId].sem_ID = 0;  /* mark as deleted */
      }
      zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "semctl destroy semaphore %d\n", semaphoreRetcode);
    }

    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end setAndIncrementSemaphore\n");
    return semaphoreRetcode;
}

int decrementSemaphore(int semaphoreID) {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start decrementSemaphore\n");
  /* post semaphore semaphoreOps */
  size_t semArrayEntries = 1;
  struct sembuf semaphoreBuffer[1];
  struct sembuf *semaphoreOps = &semaphoreBuffer[0];
  semaphoreBuffer[0].sem_num = 0;
  semaphoreBuffer[0].sem_op  = -1;    /* decrement */
  semaphoreBuffer[0].sem_flg = 0;

  int semaphoreRetcode;
  semaphoreRetcode = semop(semaphoreID, semaphoreOps, semArrayEntries); /* 0=wait, 1=increment */
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end decrementSemaphore\n");
  return semaphoreRetcode;
}


int makeLockToken(Dsn_Member *dsn_member) {
  RName rname_parm; /* resource name; the name of the resource to acquire; the dataset name + optional member */
     
  static const QName MajorQNAME  = {"SPFEDIT "}; 
      
  ENQToken lockToken;
  int lockRC = 0, lockRSN = 0;

  memcpy(rname_parm.value, dsn_member, sizeof(*dsn_member) ); /* copy in dsn+membername */
  rname_parm.length = 52;

/* test lock */    
/* Does not wait forever because CONTENTIONACT=FAIL */
  lockRC = isgenqTryExclusiveLock(
      &MajorQNAME, 
      &rname_parm, 
      ISGENQ_SCOPE_SYSTEMS, 
      &lockToken, 
      &lockRSN);   

  if ( lockRC != 0) {
    return lockRC;
  }
/* test lock end */

  lockRC = isgenqGetExclusiveLock( 
      &MajorQNAME, 
      &rname_parm, 
      ISGENQ_SCOPE_SYSTEMS, 
      &lockToken, 
      &lockRSN);   
  /* This function WAITs in case of contention.  This should not be a problem because it's preceeded by isgenqTryExclusiveLock */
  /* the lock is ALWAYS released when the C program exits. */
  return lockRC;
}


int makeSemaphore(int entryId) {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start makeSemaphore\n");
  int semaphoreID;
  pid_t pid = getpid();

  /* create semaphore */
  key_t key = pid + entryId;  /* key to pass to semget() */
  int nsems = 1;  /* number of semaphores in each 'array' of semaphores to be created */
  semaphoreID = semget(
        key,                  /* key value */
        nsems,                /* number of entries */
        IPC_CREAT | 0666      /* create a new semaphore with perms rw-rw-rw   */
        );

  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end makeSemaphore\n");
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end makeSemaphore semaphoreID %d\n", semaphoreID);
  return semaphoreID;
}

int fillSemTableEntry(int entryId, int semaphoreID, Dsn_Member *dsn_member, char* username) {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start fillSemTableEntry\n");
  /* got semaphore */
  /* fill the table slot */
  sem_table[entryId].sem_ID = semaphoreID;
  memcpy(sem_table[entryId].mem, dsn_member->membername, 8);   /* copy in member */
  memcpy(sem_table[entryId].dsn, dsn_member->dsn, 44); /* copy in dsn */
  memcpy(sem_table[entryId].usr, username, 8); /* copy in user id */
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end fillSemTableEntry\n");
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end fillSemTableEntry userid %s semaphoreID %d dsn %s membername %s \n", username, semaphoreID, dsn_member->dsn, dsn_member->membername);
  return 0;
}

int findSemTableEntry(Dsn_Member *dsn_member, char* username) {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start findSemTableEntry\n");
  int i;
  for(i=0; i<N_SEM_TABLE_ENTRIES; i++)
  {  
    if (sem_table[i].sem_ID != 0                                          /* semaphore exists */      
        && memcmp(sem_table[i].dsn, dsn_member->dsn, 44) == 0                  /* DSN matches */
        && memcmp(sem_table[i].mem, dsn_member->membername, 8) == 0                /* member matches */
        && memcmp(sem_table[i].usr, username, 8) == 0  /* user id matches */
        ) /* present */
    {
      /* dsn found in table at row  i */  
      zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, " end found findSemTableEntry %d\n", i);
      return i;
    }
  }
  
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end findSemTableEntry %d\n", i);
  return i;
}

int trySemTableEmptySlot(Dsn_Member *dsn_member, char* username) {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start trySemTableEmptySlot\n");
  int i;
  for(int i=0; i < N_SEM_TABLE_ENTRIES; i++)
  {  

    if (sem_table[i].sem_ID != 0                                  /* semaphore exists */      
        && memcmp(sem_table[i].dsn, dsn_member->dsn, 44) == 0          /* DSN matches */
        && memcmp(sem_table[i].mem, dsn_member->membername, 8) == 0   /* member matches */
        ) /* already present */
    {
      return -1;
    }
    if (sem_table[i].sem_ID == 0) /* found empty slot in table */
    {
      return i;
    }
      
  } /* end FOR loop */

  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "end trySemTableEmptySlot\n");
  return N_SEM_TABLE_ENTRIES;
}

int semTableEnqueue(Dsn_Member *dsn_member, char *username, int* retEntryId) {
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "start semTableEnqueue\n");
  int lockRC = makeLockToken(dsn_member);
  if ( lockRC != 0) {
    if (lockRC == 4) {
      return LOCK_RESOURCE_CONFLICT;
    }
    else {
      return LOCK_EXCLUSIVE_ERROR;
    }
  }

  int semaphoreID;
  int entryId = trySemTableEmptySlot(dsn_member, username);

  if (entryId >= N_SEM_TABLE_ENTRIES) {
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "SEMTABLE_CAPACITY_ERROR\n");
    return SEMTABLE_CAPACITY_ERROR;
  }

  if(entryId==-1) { 
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "SEMTABLE_EXISTING_DATASET_LOCKED\n");
    return SEMTABLE_EXISTING_DATASET_LOCKED;
  } 
 
  /* not already present, do nothing if present */
  if(sem_table[entryId].sem_ID == 0) {      
    semaphoreID = makeSemaphore(entryId);
    if (semaphoreID == -1 ) {
      return SEMTABLE_SEMGET_ERROR;
    }
    fillSemTableEntry(entryId, semaphoreID, dsn_member, username);
    *retEntryId=entryId;
    int semaphoreRetcode=setAndIncrementSemaphore(semaphoreID);
    if (semaphoreRetcode == -1 ) {
      return SEMTABLE_UNABLE_SET_SEMAPHORE;
    }
    zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "SEMTABLE_SUCCESS");
    return SEMTABLE_SUCCESS;
  }

  // we never reach here
  zowelog(NULL, LOG_COMP_DATASERVICE, ZOWE_LOG_INFO, "SEMTABLE_UNKNOWN_ERROR");
  return SEMTABLE_UNKNOWN_ERROR;
}

int semTableDequeue(Dsn_Member *dsn_member, char *username) {

    int i = findSemTableEntry(dsn_member, username);
    
    if (i >= N_SEM_TABLE_ENTRIES) {
      return SEMTABLE_ENTRY_NOT_FOUND; 
    }
    
    /* POST! */
    int semaphoreID;
    semaphoreID = sem_table[i].sem_ID;  /* use semaphore saved for that dataset */

    int semaphoreRetcode = decrementSemaphore(semaphoreID);
    if (semaphoreRetcode == -1 ) {
      return SEMTABLE_SEM_DECREMENT_ERROR;
    }
    
    return SEMTABLE_SUCCESS;         
}
/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
  
  SPDX-License-Identifier: EPL-2.0
  
  Copyright Contributors to the Zowe Project.
*/