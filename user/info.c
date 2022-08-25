#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int param = 0;
	if (argc >= 2) param = atoi(argv[1]);
	printf("sys into file %d\n", param);
	// switch(param)
	// {
	// case 0: int count = 0;
	// 		for(struct proc *p = proc; p < &proc[NPROC]; p++)
	// 		if(p->state != UNUSED)
	// 		count++;
	// 		printf("total number of active processes %d\n", count);
	// 		break;

	// case 1: printf("total number of system calls");
	// 		break;
	// case 2: printf("total number free memory pages");
	// 		break;
	// default: printf("-1");;
	// }
	sysinfo(param);
	exit(0);
}
