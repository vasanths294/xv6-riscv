#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_hello(void) // hello syscall def
{
	int n;
	argint(0, &n);
	print_hello(n);
	return 0;
}

uint64 sys_sysinfo(void) // sysinfo syscall def
{
	int n;
	argint(0, &n);
	switch(n)
	{
	case 0: return total_process_count();
			break;

	case 1: return totalsyscount;
			break;

	case 2: return kfreepages();
			break;

	default: printf("-1");;
	}
	return -1;
}

uint64 sys_procinfo(void)
{
  uint64 ptr;
  argaddr(0, &ptr);
  int temp;
  struct proc *p = myproc();
  int parentPid = copyout(p->pagetable, ptr, (char*)&(p->parent->pid), sizeof(p->parent->pid));
  int count = copyout(p->pagetable, ptr+4, (char*)&(p->syscallCount), sizeof(p->syscallCount));

  if ( parentPid < 0 || count < 0 )
    return -1;

  if((p->sz)%PGSIZE == 0)
    temp = p->sz/PGSIZE;
  else
    temp = p->sz/PGSIZE + 1;

  int pages = copyout(p->pagetable, ptr+8, (char*)&temp, sizeof(p->sz));
  if ( pages < 0 )
    return -1;
  return 0;
}