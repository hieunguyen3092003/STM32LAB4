/*
 * scheduler.c
 *
 *  Created on: Nov 28, 2023
 *      Author: hieun
 */

#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS]; // khai báo aray SCH_tasks_G có cấu trúc dữ liệu sTasks và có độ lớn là 40;
// array được khai báo trên scheduler.c là vì bao đóng không cho main thay đổi
uint8_t current_index_task = 0; // quản lý task trong array

void SCH_Init(void)
{
	for (int i = 0; i < SCH_MAX_TASKS; i++)
	{
		SCH_Delete_Task(i);
	}

	current_index_task = 0;
	// reset the global error variable
}

void SCH_Add_Task(void (*pFunction)(),
						uint32_t DELAY,
						uint32_t PERIOD)
{
	if(current_index_task < SCH_MAX_TASKS) // kiểm tra xem current_index_task còn nhỏ hơn SCH_MAX_TASKS để bỏ task này vào trong array
	{
		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY * FREQ_OF_TIM / 1000;
		SCH_tasks_G[current_index_task].Period = PERIOD * FREQ_OF_TIM / 1000;
		SCH_tasks_G[current_index_task].RunMe = 0;

		SCH_tasks_G[current_index_task].TaskID = current_index_task; // taskID là lưu trữ vị trí của task trong hệ thống để trong tương lai có thể delete

		current_index_task++; // tăng biến current_index_task lên 1 để lệnh tiếp theo được add vào SCH, tuy nhiên sẽ gặp khó khăn trong tương lai
	}
}

void SCH_Update(void) // nhiệm vụ của hàm này là trừ counter đi
{
	for (int i = 0; i < current_index_task; i++) //  và có nhiều task duyệt vòng lặp for qua các tasks
	{
		if(SCH_tasks_G[i].Delay > 0)
		{
			SCH_tasks_G[i].Delay--;
		}
		else
		{
			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period; // do có 2 thông số là delay và period (chờ và chu kỳ lặp nên sẽ thực hiện lần đầu tiên là chờ thực hiện và các lần sau thực hiện bằng thông số Period)
			SCH_tasks_G[i].RunMe += 1; // counter = 0 thì set RunMe lên 1
		}
	}
}

void SCH_Dispatch_Tasks(void) // lưu ý khi thực hiện 1 task nó có thể block task còn lại nên lưu ý hiện tượng bị đè
{
	for(int i = 0; i < current_index_task; i++)
	{
		if (SCH_tasks_G[i].RunMe > 0) // kiểm tra Flag RunMe có lớn hơn 0 hay không để thực hiện
		{
			SCH_tasks_G[i].RunMe--;
			(*SCH_tasks_G[i].pTask)();	// dùng con trỏ trỏ đến hàm để thực hiện
			if(SCH_tasks_G[i].Period == 0)
			{
				SCH_Delete_Task(i);
			}

		}
	}
}

void SCH_Delete_Task(uint32_t index_task)
{
	SCH_tasks_G[index_task].pTask = 0x0000;
	SCH_tasks_G[index_task].Delay = 0;
	SCH_tasks_G[index_task].Period = 0;
	SCH_tasks_G[index_task].RunMe = 0;
}
