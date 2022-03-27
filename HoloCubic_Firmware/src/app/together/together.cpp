#include "together.h"
#include "together_gui.h"
#include "sys/app_controller.h"
#include "common.h"

#define TOGETHER_APP_NAME "We"
#define TIME_API "http://api.m.taobao.com/rest/api3.do?api=mtop.common.gettimestamp"
#define GET_TIME_INTERVAL 1800000 // 每三十分钟自动请求网络时间
#define DISPLAY_US_INTERVAL 30000 // 每三十秒自动切换us

// 动态数据，APP的生命周期结束也需要释放它
struct TogetherAppRunData
{
    File_Info *image_file;      // 文件夹节点
    File_Info *pfile;           // 指向当前播放的文件节点
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

static File_Info *get_next_file(File_Info *p_cur_file, int direction)
{
    // 得到 p_cur_file 的下一个 类型为FILE_TYPE_FILE 的文件（即下一个非文件夹文件）
    if (NULL == p_cur_file)
        return NULL;

    File_Info *pfile = NULL;
    while (pfile != p_cur_file)
    {
        pfile = direction == 1 ? p_cur_file->next_node : p_cur_file->front_node;
        if(pfile == NULL)
            return NULL;
        if (FILE_TYPE_FILE == pfile->file_type)
            break;
    }
    if (FILE_TYPE_FILE != pfile->file_type)
        return NULL;
    return pfile;
}

static int together_init(void)
{
    // 初始化运行时的参数
    together_gui_init();
    // 初始化运行时参数
    run_data = (TogetherAppRunData *)calloc(1, sizeof(TogetherAppRunData));
    if(!hadOpened){
        hadOpened = true;
        forever_data.networkTime = 1649304000000;
        forever_data.t = lv_tick_get();
    }
    
    run_data->image_file = tf.listDir(WE_IMAGE_PATH);
    run_data->pfile = run_data->image_file;
    // 使用 forever_data 中的变量，任何函数都可以用
    //Serial.print(forever_data.val1);

    // 如果有需要持久化配置文件 可以调用此函数将数据存在flash中
    // 配置文件名最好以APP名为开头 以".cfg"结尾，以免多个APP读取混乱
    //String info = g_flashCfg.readFile("/together.cfg");
    // 解析数据
    // 将配置数据保存在文件中（持久化）
    //g_flashCfg.writeFile("/together.cfg", "value1=100\nvalue2=200");
}

static void display_us(ACTIVE_TYPE active){ // 展示we目录下的bin文件
    if(run_data->image_file != NULL && get_next_file(run_data->image_file, 1) != NULL) { // 
        static uint32_t last_tick = 0;
        uint32_t t = lv_tick_get();
        if (t - last_tick > DISPLAY_US_INTERVAL) {
            last_tick = t;
            static int direction = 1;
            if(TURN_RIGHT == active) {direction = 1;}
            if(TURN_LEFT == active) {direction = -1;}
            File_Info *pfile = get_next_file(run_data->pfile, direction);
            if(pfile != NULL) {
                run_data->pfile = pfile;
                char file_name[PIC_FILENAME_MAX_LEN] = {0};
                snprintf(file_name, PIC_FILENAME_MAX_LEN, "%s/%s", run_data->image_file->file_name, run_data->pfile->file_name);
                Serial.print(F("display us image: "));
                Serial.println(file_name);
                if (NULL != strstr(file_name, ".bin") || NULL != strstr(file_name, ".BIN"))
                    display_us(file_name);
            }
        }
    }
}

static void shake2getTime(AppController *sys, ACTIVE_TYPE active){
    if (TURN_RIGHT == active || TURN_LEFT == active){
        sys->send_to(TOGETHER_APP_NAME, CTRL_NAME, APP_MESSAGE_WIFI_CONN, NULL, NULL);
        Serial.printf("try to connect wifi, trigger by hand shake.\n");
    }
}

static void getTimeTimer(AppController *sys, ACTIVE_TYPE active){
    static uint32_t last_tick = 0;
    uint32_t t = lv_tick_get();
    if(t - last_tick > GET_TIME_INTERVAL || last_tick == 0) {
        last_tick = t;
        sys->send_to(TOGETHER_APP_NAME, CTRL_NAME, APP_MESSAGE_WIFI_CONN, NULL, NULL);
    }
}

static void together_process(AppController *sys,
                            const ImuAction *act_info)
{
    if (RETURN == act_info->active)
    {
        sys->app_exit(); // 退出APP
        return;
    }

    shake2getTime(sys, act_info->active);
    getTimeTimer(sys, act_info->active);
    
    

    display_together(forever_data.networkTime, forever_data.t);
    // display_us(act_info->active); //播放bin文件储存的图片, 有bug
    

    // 发送请求。如果是wifi相关的消息，当请求完成后自动会调用 together_message_handle 函数
    // sys->send_to(TOGETHER_APP_NAME, CTRL_NAME,
    //              APP_MESSAGE_WIFI_CONN, (void *)run_data->val1, NULL);

    // 程序需要时可以适当加延时
    // delay(300);
}

static int together_exit_callback(void *param)
{
    together_gui_del();
    release_file_info(run_data->image_file);
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
