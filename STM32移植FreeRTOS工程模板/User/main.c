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