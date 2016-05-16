#include "../include/task.h"
#include "../include/uart.h"
#include "../include/timer.h"

struct tcb task_global;

void task_init()
{
    int i;
    for(i = 0; i < 17; i++)
    {
      task_info[i].rank = NULL;
      task_info[i].status = NULL;
      task_info[i].task_func = NULL;
      task_info[i].TRID = 17;
    }
		unsigned char rank = MAX_rank ;
		unsigned int task_func = (unsigned int)task_idle;
		unsigned char TID = task_create( rank , task_func);
		task_run(TID);

		//初始化task_global
		task_global.schedule_lock = false;
		task_global.tid = TID;
		task_global.kstack = (unsigned int) &task_table[TID];
}

void task_idle(void)
{
  while(1)
  {
    uart_puts("init task success!\r\n");
    sleep(100);
  }
}

unsigned char task_create(unsigned char rank, unsigned int task_func)
{
		/*1.检查参数合法性*/
		if( rank > MAX_rank + 1)
		{
				return NULL;
		}

		/*2.设置TASK_INFO*/
		unsigned char TID = task_get_id();

		TASK_INFO task_info_this;
		task_info_this.rank = rank;
		task_info_this.status = DEAD ;
		task_info_this.task_func = task_func ;
		task_info[TID] = task_info_this;
		//task_info.TRID 到 task_run 中设置

		/*3.设置tasktable*/
    task_table[TID].r0 = 0;
    task_table[TID].r1 = 0;
    task_table[TID].r2 = 0;
    task_table[TID].r3 = 0;
    task_table[TID].r4  = 0;
    task_table[TID].r5 = 0;
    task_table[TID].r6 = 0;
    task_table[TID].r7 = 0;
    task_table[TID].r8 = 0;
    task_table[TID].r9 = 0;
    task_table[TID].r10 = 0;
    task_table[TID].r11 = 0;
    task_table[TID].r12 = 0;
		task_table[TID].sp = (unsigned int)task_stack[TID] + 1024;
		task_table[TID].lr = (unsigned int)task_delete;
		task_table[TID].pc =(unsigned int) task_func;
    task_table[TID].cpsr = 0x53;//禁止fiq , svc模式
		task_table[TID].spsr = 0x53;//禁止fiq , svc模式
		return TID;
}

unsigned char task_get_id()
{
		int id = 0;
		for(id = 0; id < 17; id++)
		{
			if( (task_info[id].rank == NULL) && (task_info[id].status == NULL) && (task_info[id].task_func == NULL ) && (task_info[id].TRID == 17 ) )
			{
				return id;
			}
		}
		return 0;
}

unsigned char task_run(unsigned char TID)
{
		/*1.参数验证, run 的进程状态必须是DEAD,其他状态的进程不可以被run*/
		if(task_info[TID].status != DEAD)
		{
				return NULL;
		}

		/*2.1 申请加入task_ready链表, 返回0失败*/
		task_info[TID].TRID = TID;//ll_add_by_order(task_ready, TID, task_info[TID].rank);

		/*2.2 验证返回值是否有效
		if( NULL == task_info[TID].TRID)
		{
				task_info[TID].TRID = NULL;
				task_info[TID].rank = NULL ;
				task_info[TID].status = NULL;
				task_info[TID].task_func = NULL;
		}
		else
		{
				task_info[TID].status = READY;
		}*/
    task_info[TID].status = READY;
		return task_info[TID].TRID;
}

void  task_schedule(void)
{
		unsigned char current_TID , current_TRID;
		current_TID = current_TRID = task_info[task_global.tid].TRID;										//获取当前TRID
/*
		do{
			current_TRID = ll_get_next_id(task_ready , current_TRID);										//得到下一个TRID
			current_TID = (unsigned char) task_ready->node[current_TRID].value;
		}while(0  == current_TRID || task_info[current_TID].status !=  READY);
*/
    int i;
    for(i = 0; i < 17; i++)
    {
      unsigned int _temp = task_info[(current_TRID+i+1)%17].TRID;
      if(17 != _temp)
      {
        current_TID = _temp;
        break;
      }
    }

		task_global.tid = current_TID;			//设置当前TID													//
		task_global.kstack = (unsigned int) &task_table[task_global.tid];	//设置当前tasktable

		return ;
}

void task_delete()
{
	unsigned char TID = task_global.tid;
	task_info[TID].TRID = NULL;
	task_info[TID].rank = NULL ;
	task_info[TID].status = NULL;
	task_info[TID].task_func = NULL;

	task_table[TID].r0 = 0;
	task_table[TID].r1 = 0;
	task_table[TID].r2 = 0;
	task_table[TID].r3 = 0;
	task_table[TID].r4  = 0;
	task_table[TID].r5 = 0;
	task_table[TID].r6 = 0;
	task_table[TID].r7 = 0;
	task_table[TID].r8 = 0;
	task_table[TID].r9 = 0;
	task_table[TID].r10 = 0;
	task_table[TID].r11 = 0;
	task_table[TID].r12 = 0;
	task_table[TID].sp = 0;
	task_table[TID].lr = 0;
	task_table[TID].pc = 0;
	task_table[TID].spsr = 0;
}

void task_lock_schedule(void)
{
	task_global.schedule_lock = true;
}

void task_unlock_schedule(void)
{
	task_global.schedule_lock = false;
}
