1. gen_img_bin_file.py

将一个目录下的所有图片进行缩放操作后再转换为bin文件



2. main.py

将一个gif分解成png后(parse_gif_to_png.py)，把每个png生成为lvgl中可用的c数组(gen_img_c_file.py)，再将这些数组合并到一个c文件中，再在c文件底部将其聚集到一个自定义的`Gif`结构体里(merge_c.py)，然后生成一个声明了上述Gif结构体的h文件

```c++
struct Gif
{
    /* data */
    const void** frames; // 所有png的数据
    short frame_num;
    short speed; // ms
    unsigned int last_tick;
    unsigned short curr_frame;
};
```

​    2.1 gen_img_c_file.py

对[此项目](https://github.com/lvgl/lv_img_conv)进行构建后，使用命令行的形式生成c文件，此命令接收ts脚本的位置和一个包含GIF的目录作为参数，批量在这个目录下的`cfiles`文件夹里生成c文件（注意使用此目录下的cli.ts替换原来的cli.ts）