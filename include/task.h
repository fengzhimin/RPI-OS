#ifndef __TASK_H__
#define __TASK_H__

#include "base.h"

#define	MAX_rank	 3		//	共四个优先级: 0 1 2 3

//任务状态
typedef enum
{
	STATUS_NULL ,		//空
	READY,						//就绪
	RUN, 							//正在运行
	WAIT,							//阻塞
	SUSPEND,				//挂起
	DEAD							//未加入调度函数管理
}TASK_STATUS_ENUM;

//任务信息
typedef struct {
		unsigned char	rank;								 /*优先级*/
		TASK_STATUS_ENUM status;	         /*任务状态*/
		unsigned int task_func;						/*函数地址*/
		unsigned char	TRID;								/*Task Ready ID*/
}TASK_INFO;

TASK_INFO task_info[17];			    /*任务信息*/

struct context{
  unsigned int r0;
  unsigned int r1;
  unsigned int r2;
  unsigned int r3;
  unsigned int r4;
	unsigned int r5;
  unsigned int r6;
  unsigned int r7;
  unsigned int r8;
  unsigned int r9;
  unsigned int r10;
  unsigned int r11;
  unsigned int r12;
  unsigned int sp;
  unsigned int lr;
  unsigned int pc;
	unsigned int cpsr;
  unsigned int spsr;
};

struct tcb{
  unsigned int kstack;
  unsigned int tid;
  int          schedule_lock;
};

struct context task_table[17];
unsigned char task_stack[17][1024];

void task_init();
unsigned char task_create(unsigned char rank, unsigned int task_func);							//创建任务
unsigned char task_get_id();
unsigned char task_run(unsigned char TID);																//加入调度管理
void task_lock_schedule(void);										//锁定调度函数
void task_unlock_schedule(void);								//解锁
void task_idle(void);
void task_delete();
void task_schedule(void);

#endif
