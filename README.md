# STM32移植FreeRTOS v9.0

开发板：STM32F103C8T6

FreeRTOS版本为v9.0.0

## 1、工程模板

使用电协的STM32教程的工程模板，下载地址：<https://pan.baidu.com/s/1jH75y9y>

## 2、FreeRTOS版本

FreeRTOS版本为v9.0.0，为正点原子的FreeRTOS教程配套版本。

## 3、移植过程

### 3.1 解压工程模板

将工程模板解压到任意目录，如：FreeRTOSBasedOnTM32，并重命名为`1-STM32移植FreeRTOS`。

### 3.2 解压FreeRTOS源码

将FreeRTOS源码解压到任意目录，如：FreeRTOS，并重命名为`FreeRTOS_V9.0.0`。

### 3.4 复制FreeRTOS源码到工程模板

新建FreeRTOS文件夹，并将源码中的`include`、`portable`、和`*c`文件复制过去.

对于`portable`文件夹，仅保留`Keil`、`MemMang`、`RVDS`三个文件夹，并保留`MemMang`中的`heap_4.c`文件，保留`RVDS`中的`ARM_CM3`文件夹。

### 3.5 添加keil分组

新建keil工程分组FreeRTOS_CORE和FreeRTOS_PORTABLE，并分别添加文件。
FreeRTOS_CORE添加文件：

- croutine.c
- event_groups.c
- list.c
- queue.c
- tasks.c
- timers.c

FreeRTOS_PORTABLE添加文件：

- port.c
- heap_4.c

### 3.6 添加头文件路径

在keil工程的`Include Paths`中添加FreeRTOS的头文件路径：

- .\FreeRTOS
- .\FreeRTOS\include
- .\FreeRTOS\portable\RVDS\ARM_CM3

### 3.7 添加宏

在FreeRTOSConfig.h中添加宏：

```c
#define xPortPendSVHandler  PendSV_Handler
#define vPortSVCHandler     SVC_Handler
#define xPortSysTickHandler SysTick_Handler
```

并在 `stm32f10x_it.c` 中注释掉这几个中断服务函数

### 3.8 编译并下载

示例代码：

```c
#include "stm32f10x.h" // Device header
#include "stm32f10x_gpio.h"

#include "freertos.h"
#include "task.h"

#define TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1)

void myTask(void *arg);
TaskHandle_t myTaskHandler;

int main(void)
{
    // GPIO初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 创建任务
    xTaskCreate(myTask,          // 任务函数
                "myTask",        // 任务名称
                TASK_STACK_SIZE, // 任务堆栈大小
                NULL,            // 任务参数
                TASK_PRIORITY,   // 任务优先级
                &myTaskHandler); // 任务句柄

    vTaskStartScheduler(); // 启动任务调度器
    while (1)
    {
    }
}

void myTask(void *arg)
{

    while (1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(500);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(500);
    }
}

```

## 4、FreeRTOS创建任务

### 4.1 动态创建 ：由系统分配

```c
BaseType_t xTaskCreate(TaskFunction_t pxTaskCode,
                        const char * const pcName,
                        const uint16_t usStackDepth,
                        void * const pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t * const pxCreatedTask ) 
```

### 4.2 静态创建：由程序员分配

```c
TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                UBaseType_t uxPriority,
                                StackType_t * const puxStackBuffer,
                                StaticTask_t * const pxTaskBuffer )
```

示例代码：

```c

#include "stm32f10x.h" // Device header
#include "stm32f10x_gpio.h"

#include "freertos.h"
#include "task.h"

#define TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1)

void myTask(void *arg);
// TaskHandle_t myTaskHandler;

// 动态创建任务
StackType_t myTaskStack[TASK_STACK_SIZE];
StaticTask_t myTaskTCB;

int main(void)
{
    // GPIO初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 静态创建任务
    xTaskCreateStatic(myTask,         // 任务函数
                      "myTask",       // 任务名称
                      TASK_STACK_SIZE, // 任务栈大小
                      NULL,           // 传递给任务函数的参数
                      TASK_PRIORITY,  // 任务优先级
                      myTaskStack,    // 任务栈
                      &myTaskTCB);    // 任务控制块

    vTaskStartScheduler(); // 启动任务调度器
    while (1)
    {
    }
}

void myTask(void *arg)
{

    while (1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(300);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(300);
    }
}

StaticTask_t IdleTaskTCB;
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = IdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
```

## 5、FreeRTOS编程风格

### 数据类型
```c
typedef unsigned long UBaseType_t;

#define configUSE_16_BIT_TICKS 1

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffU

```

### 变量名和函数名

变量名:

|前缀|FreeRTOS中的含义|
|---|---|
|c|short|
|s|short|
|l| long|
|x|FreeRTOS structure|
|u|unsigned|
|p|pointer|
|uc|unsigned char|
|pc|char   pointer|

函数名:
BaseType_t xTaskCreate():
|前缀|FreeRTOS中的含义|
|---|---|
|x|FreeRTOS structure|
|Task|在task.h中定义的结构体|
|Create|函数功能|

void vTaskDelete():

|前缀|FreeRTOS中的含义|
|---|---|
|v|没有返回值|
|Task|在task.h中定义的结构体|
|Create|函数功能|

static void prvInitialiseNewTask():

|前缀|FreeRTOS中的含义|
|---|---|
|prv|private|
### 宏定义

|前缀|FreeRTOS中的含义|
|---|---|
|port|在portable或者portmacro.h中定义的宏|
|config|在FreeRTOSConfig.h中定义的宏|
|task|在task.h中定义的宏|
|pd|projdefs.h中定义的宏|
|err|projdefs.h中定义的宏|
