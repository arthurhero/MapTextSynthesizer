#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <time.h> //removeme
#include "prod_cons.h"
#include "ipc_consumer.h"

sample_t* consume(intptr_t buff, uint64_t* consume_offset, int semid, uint32_t* have_buff_lock) {

  /* For wrapping -- test to see if producer wrapped */
  if(*consume_offset + BASE_CHUNK_SIZE >= SHM_SIZE
     || *(uint64_t*)(buff + *consume_offset) == NO_SPACE_TO_PRODUCE) {
    *consume_offset = START_BUFF_OFFSET;
  }

  /* Nothing available to consume, return NULL */
  if(*(uint64_t*)(buff + *consume_offset) != SHOULD_CONSUME) {
    return NULL;
  }
  
  /* Cache initial buff value */
  intptr_t start_buff = buff;
  
  /* Jump to the next available element (8 is to skip past `ABLE_TO_CONSUME``) */
  buff += *consume_offset + sizeof(uint64_t);
  
  sample_t* spl = (sample_t*)malloc(sizeof(sample_t));
  if(spl == NULL) {
    perror("malloc");
    exit(1);
  }

  // Stored as 64 bit int for mem alignment's sake, but don't need 64 bits
  uint32_t height = (uint32_t)*((uint64_t*)buff);
  buff += sizeof(uint64_t);

  char* label = strdup((char*)buff);
  if(label == NULL) {
    perror("strdup");
    exit(1);
  }

  buff += (MAX_WORD_LENGTH + 1)*sizeof(char);

  uint64_t sz = *((uint64_t*)buff);
  buff += sizeof(uint64_t);

  unsigned char* img_flat = (unsigned char*)malloc(sz);
  if(img_flat == NULL) {
    perror("malloc");
    fprintf(stderr, "Requested %lu bytes.\n", sz);
    exit(1);
  }

  // Copy image data into img_flat
  memcpy(img_flat, (void*)buff, sz);
  buff += sz*sizeof(unsigned char);

  spl->height = height;
  if(sz % height != 0) {
    fprintf(stderr,
	    "invalid image dimensions. size=%lu, height=%u\n", sz, height); }
  spl->width = sz/height; //should be evenly divisible
  spl->caption = label;
  spl->img_data = img_flat;

  // Buff is now consumed!
  *(uint64_t*)(start_buff+*consume_offset) = (uint64_t)ALREADY_CONSUMED;

  // Update consume_offset (local)
  *consume_offset = buff - start_buff;

  // Force 8 byte alignment -- consistent with producer
  *consume_offset += (*consume_offset % 8);
  
  // Update the consume_offset (global)
  *((uint64_t*)(start_buff+sizeof(uint64_t))) = *consume_offset;

  /*
  // Prevent producers from wrapping
  // and producing too close from the back of the consume offset
  // WARNING: Can result in buggy behavior if shm_size is too small
  if(*((uint64_t*)buff) < *consume_offset
     && ((*((uint64_t*)buff) + PRODUCER_LAP_PREVENTION_SIZE) % SHM_SIZE >= *consume_offset
     && !*have_buff_lock)) {
    // do this once
    lock_buff(semid);
    *have_buff_lock = PRODUCER_LAP_PREVENTION_NUM;
  } else if(*have_buff_lock){
    // do this only if locked by consumer
    (*have_buff_lock)--;
  } else {
    // let them produce!
    unlock_buff(semid);
  }
  */
  //DEBUG REMOVEME
  //  sleep(1);
  printf("consumed....%d\n", time(NULL));
  return spl;
}

  
sample_t* ipc_get_sample(void* buff, uint64_t* consume_offset,
		 int semid, uint32_t* have_buff_lock) {

  sample_t* spl = consume((intptr_t)buff, consume_offset,
			  semid, have_buff_lock);
  
  // If this happens, then something broke
  if(*consume_offset >= SHM_SIZE) {
    fprintf(stderr, "Consumer did not wrap appropriately.\n");
    exit(1);
  }

  return spl;
}

/* For debugging */
void print_sample_struct(sample_t* spl) {
  // Contains all of the raw data of a sample
  printf("Height: %ld\n", spl->height);
  printf("Caption: %s\n", spl->caption);
}