#include "together.h"
#include "together_gui.h"
#include "sys/app_controller.h"
#include "common.h"

#define TOGETHER_APP_NAME "We"
#define TIME_API "http://api.m.taobao.com/rest/api3.do?api=mtop.common.gettimestamp"
#define GET_TIME_INTERVAL 1800000 // 每三十分钟自动请求网络时间

// 动态数据，APP的生命周期结束也需要释放它
struct TogetherAppRunData
{
    bool init_get;
};

// 常驻数据，可以不随APP的生命周期而释放或删除
struct TogetherAppForeverData
{
    long long networkTime;
    uint32_t t; // 在得到networkTime时，lv_tick_get返回的值
};

   
static bool hadOpened = false;

// 保存APP运行时的参数信息，理论上关闭APP时推荐在 xxx_exit_callback 中释放掉
static TogetherAppRunData *run_data = NULL;

// 当然你也可以添加恒定在内存中的少量变量（退出时不用释放，实现第二次启动时可以读取）
// 考虑到所有的APP公用内存，尽量减少 forever_data 的数据占用
static TogetherAppForeverData forever_data;

static int together_init(void)
{
    // 初始化运行时的参数
    together_gui_init();
    // 初始化运行时参数
    run_data = (TogetherAppRunData *)calloc(1, sizeof(TogetherAppRunData));
    if(!hadOpened){
        hadOpened = true;
        forever_data.networkTime = 1647613221000;
        forever_data.t = lv_tick_get();
    }
    
    run_data->init_get = false;
    
    // 使用 forever_data 中的变量，任何函数都可以用
    //Serial.print(forever_data.val1);

    // 如果有需要持久化配置文件 可以调用此函数将数据存在flash中
    // 配置文件名最好以APP名为开头 以".cfg"结尾，以免多个APP读取混乱
    //String info = g_flashCfg.readFile("/together.cfg");
    // 解析数据
    // 将配置数据保存在文件中（持久化）
    //g_flashCfg.writeFile("/together.cfg", "value1=100\nvalue2=200");
}

static void together_process(AppController *sys,
                            const Imu_Action *act_info)
{
    if (RETURN == act_info->active)
    {
        sys->app_exit(); // 退出APP
        return;
    }
    if (TURN_RIGHT == act_info->active || TURN_LEFT == act_info->active)
    {
        sys->send_to(TOGETHER_APP_NAME, CTRL_NAME, APP_MESSAGE_WIFI_CONN, NULL, NULL);
        Serial.printf("try to connect wifi, trigger by hand shake.\n");
    }
    

    static uint32_t last_tick = 0;
    uint32_t t = lv_tick_get();
    if(t - last_tick > GET_TIME_INTERVAL || !run_data->init_get) {
        last_tick = t;
        run_data->init_get = true;
        sys->send_to(TOGETHER_APP_NAME, CTRL_NAME, APP_MESSAGE_WIFI_CONN, NULL, NULL);
    }


    display_together(forever_data.networkTime, forever_data.t);
    // 发送请求。如果是wifi相关的消息，当请求完成后自动会调用 together_message_handle 函数
    // sys->send_to(TOGETHER_APP_NAME, CTRL_NAME,
    //              APP_MESSAGE_WIFI_CONN, (void *)run_data->val1, NULL);

    // 程序需要时可以适当加延时
    // delay(300);
}

static int together_exit_callback(void *param)
{
    together_gui_del();
    if(run_data != NULL){
        // 释放资源
        free(run_data);
        run_data = NULL;
    }
    
}


static long long get_timestamp(String url)
{
    if (WL_CONNECTED != WiFi.status())
        return 0;

    HTTPClient http;
    http.setTimeout(1000);
    http.begin(url);

    long long rt = 0;

    int httpCode = http.GET();
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
            int time_index = (payload.indexOf("data")) + 12;
            String millisTime = payload.substring(time_index, payload.length() - 3);
            rt = atoll(millisTime.c_str()); // ms
            // 以网络时间戳为准
            forever_data.networkTime = rt +  2000; //加个延时误差
            forever_data.t = lv_tick_get();
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        // 得不到网络时间戳时
    }
    http.end();

    return rt;
}

static void together_message_handle(const char *from, const char *to,APP_MESSAGE_TYPE type, void *message, void *ext_info){
    switch (type)
    {
        case APP_MESSAGE_WIFI_CONN:
        {
            get_timestamp(TIME_API);
        }
    }
}

APP_OBJ together_app = {TOGETHER_APP_NAME, &app_together_icon, "Author LC\nVersion 1.0.0\n",
                       together_init, together_process,
                       together_exit_callback, together_message_handle};
