/*
 *
 * 文件：			main.c
 * 功能：			主函数入口
 * 作者：			董超
 * 开发板：		STM32F103C8T6
 * 开发工具：	MDK-ARM + vscode + CMSIS-DAP
 * github：		https://github.com/dongchao980612
 * 
 */

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
