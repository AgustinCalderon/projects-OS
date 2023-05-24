#include "types.h"
#include "param.h"
#include "kernel/riscv.h"
#include "kernel/spinlock.h"
#include "defs.h"
#include "kernel/proc.h"

static int 
allocsd(struct proc *p)
{

    int i;
    for (i = 0; i < NOSEM; i++){
        if(p->osem[i] == 0){
            return i;
        }
    }
    return -2;
}

uint64 
sys_sem_create(void)
{
    int key, value;
    argint(0, &key);
    argint(1, &value);
    return sem_create(key, value);
}

uint64 
sys_sem_get(void)
{
    int key;
    struct proc *p = myproc();
    int slot = allocsd(p);
    if (slot == -1){
        return -1;
    }
    argint(0, &key);
    struct sem *s = sem_get(key);
    if(s){
        p->osem[slot] = s;
        return slot;
    }
    return -2;
}


uint64 
sys_sem_wait(void)
{
    struct proc *p = myproc();
    int sem; 
    argint(0, &sem);
    if(p->osem[sem] == 0){
        return -1;
    }
    sem_wait(p->osem[sem]);   
    return 0;
}

uint64 
sys_sem_signal(void)
{
    struct proc *p = myproc();
    int sem; 
    argint(0, &sem);
    if(p->osem[sem] == 0){
        return -1;
    }    
    sem_signal(p->osem[sem]);
    return 0;
}

uint64 
sys_sem_close(void)
{
    struct proc *p = myproc();
    int sem; 
    argint(0, &sem);
    if(p->osem[sem] == 0){
        return -1;
    }   
    sem_close(p->osem[sem]);
    p->osem[sem] = 0;
    return 0;
}

uint64 
sys_sem_destroy(void)
{
    int key;
    argint(0, &key);
    return sem_destroy(key);;
}
