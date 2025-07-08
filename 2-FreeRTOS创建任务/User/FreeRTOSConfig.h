#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0

#define configSUPPORT_STATIC_ALLOCATION 1  // 静态创建任务

// 写入实际的CPU内核时钟频率，也就是CPU指令执行频率。
#define configCPU_CLOCK_HZ ((unsigned long)72000000)

// 滴答中断（SYSTICK）1毫秒中断1次。
#define configTICK_RATE_HZ ((TickType_t)1000)

// 配置应用程序最大可用的优先级数目。
#define configMAX_PRIORITIES (5)

//  定义空闲任务使用的堆栈大小。
#define configMINIMAL_STACK_SIZE ((unsigned short)128)

//   配置FreeRTOS任务栈可用的RAM总量。
#define configTOTAL_HEAP_SIZE ((size_t)(8 * 1024)) 

#define configMAX_TASK_NAME_LEN (16)
#define configUSE_TRACE_FACILITY 0
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 0
#define configMAX_CO_ROUTINE_PRIORITIES (2)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_vTaskCleanUpResources 0
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191 /* equivalent to 0xb0, or priority 11. */

/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15

//#define xPortPendSVHandler PendSV_Handler
//#define vPortSVCHandler SVC_Handler
//#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
