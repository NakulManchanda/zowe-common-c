/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
  
  SPDX-License-Identifier: EPL-2.0
  
  Copyright Contributors to the Zowe Project.
*/

#ifndef __SEMTABLE__
#define __SEMTABLE__ 1
  
#define SEMTABLE_UNKNOWN_ERROR  0x01
#define SEMTABLE_CAPACITY_ERROR  0x02
#define SEMTABLE_SEMGET_ERROR  0x03
#define SEMTABLE_EXISTING_DATASET_LOCKED  0x04
#define SEMTABLE_UNABLE_SET_SEMAPHORE  0x05
#define SEMTABLE_ENTRY_NOT_FOUND  0x06
#define SEMTABLE_SEM_DECREMENT_ERROR  0x08
#define LOCK_RESOURCE_CONFLICT  0x09
#define LOCK_EXCLUSIVE_ERROR  0x0A
#define SEMTABLE_SUCCESS  0x00


#define N_SEM_TABLE_ENTRIES 100

/* declare the table that maps datasets to semaphores */
typedef struct Sem_Table_type {
  char dsn [44];
  char mem [8];
  char usr [8];
  int sem_ID;
} Sem_Table;

typedef struct Dsn_Member_tag {
  char  dsn[44];
  char  membername[8];
} Dsn_Member;

Sem_Table sem_table [N_SEM_TABLE_ENTRIES];

void initSemTable();
int postSemaphore(int entryId);
int semTableEnqueue(Dsn_Member *dsn_member, char *username,  int* retEntryId);
int semTableDequeue(Dsn_Member *dsn_member, char *username);
#endif
/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
  
  SPDX-License-Identifier: EPL-2.0
  
  Copyright Contributors to the Zowe Project.
*/