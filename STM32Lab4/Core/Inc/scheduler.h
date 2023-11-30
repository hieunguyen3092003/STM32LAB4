/*
 * scheduler.h
 *
 *  Created on: Nov 28, 2023
 *      Author: hieun
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h> // now we able to use unsigned int 32 bit

#define SCH_MAX_TASKS	40
#define FREQ_OF_TIM		100

typedef struct // cấu trúc dữ liệu cơ bản cần có
{
	void (*pTask)(void); // con trỏ hàm
	uint32_t	Delay;
	uint32_t	Period;
	uint8_t		RunMe; // giống như timer_flag khi RunMe > 0 thì thực hiện tasks

	uint32_t	TaskID; // hiện thực các tác vụ thêm trong bài lab
}sTasks;

void SCH_Init(void);

void SCH_Add_Task(void (*pFunction)(),
						uint32_t DELAY,
						uint32_t PERIOD); // giống như set 1 cái timer

void SCH_Update(void); // giống với timer run (được gọi trong ISR

void SCH_Dispatch_Tasks(void); // giống với kiểm tra flag (được gọi trong while 1)

void SCH_Delete_Task(uint32_t ID);

#endif /* INC_SCHEDULER_H_ */
