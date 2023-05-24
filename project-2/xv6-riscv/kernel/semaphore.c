#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

struct sem{
	int key; 
	int sem_value;
	int ref_count;			
	struct spinlock lk;
};

struct sem semaphore[NSEM];	
struct spinlock sem_lock;

// Create and initialize the semaphore with id=key. 
// if a semaphore with that key already exists in the system, it should return error.
int sem_create(int key, int init_value){
	
	if(key <= 0){
		return -1;
	}
	
	acquire(&sem_lock);
	int i;
	int free = -1;  

	for (i = 0; i < NSEM; i++) {
		if((semaphore+i)->key == key){ 
			release(&sem_lock);
			return -2;
		}
		 
		if((semaphore+i)->key == 0){
			free = i;
		}
	}
	
	if(free == -1){
		return -3;
	}

	(semaphore+free)->key = key;
	(semaphore+free)->sem_value = init_value;
	
	release(&sem_lock);
	return 1;
} 

// Get a semaphore descriptor with id=key.
// If it does not exist it should return -1.
struct sem* 
sem_get(int key)
{
	int found = -1;
	// acquire(&sem_lock);
	for (int i = 0; i < NSEM; i++){
		acquire(&(semaphore+i)->lk);
		if((semaphore+i)->key == key){		
			found = 1;
			(semaphore+i)->ref_count++;
			// release(&sem_lock);
			release(&(semaphore+i)->lk);
			return semaphore+i;
		}
		release(&(semaphore+i)->lk); 
	}
	if(found == -1){
		return 0;
	}
	return 0;
}

// Atomically decrements the value of the semaphore.
// If it was at zero it should be blocked.
int 
sem_wait(struct sem *s)
{
	
	acquire(&s->lk);
	while(s->sem_value == 0){
		sleep(s, &s->lk);
	}
	s->sem_value--;
	release(&s->lk);
		
	return 0;
}

// Atomically increment the value of the semaphore and
// eventually wake up waiting processes stuck at the semaphore.
int 
sem_signal(struct sem *s)
{
	
	acquire(&s->lk);
	s->sem_value++;
	wakeup(s);
	release(&s->lk);
	
	return 0;
}

// Release the semaphore.
int 
sem_close(struct sem *s)
{

	acquire(&s->lk);
	s->ref_count--;
	release(&s->lk);
	return 0;		
}

// Destroy the semaphore in the system.
// Should return if it doesn't exist or if the semaphore is still in use.
int 
sem_destroy(int key)
{	
	if(key <= 0){
		return -1;
	}

	int i;
	int found = -1;
	for (i = 0; i < NSEM; i++){
		if((semaphore+i)->key == key){
			acquire(&(semaphore+i)->lk);
			found = 1;
			break;
		}
	}
	if(found == -1){
		release(&(semaphore+i)->lk);
		return -2;
	}
	if((semaphore+i)->ref_count != 0){
		release(&(semaphore+i)->lk);
		return -3;	
	}
	(semaphore+i)->sem_value = 0;
	release(&(semaphore+i)->lk);

	return 0;
}
