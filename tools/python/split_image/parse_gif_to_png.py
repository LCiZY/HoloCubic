import os
from PIL import Image, ImageSequence

split_png_dir = "pngs"


def parse_gif_to_png(path, w, h):
    name = os.path.basename(path)
    gifname = os.path.join(os.path.abspath(path), name + ".gif")
    split_png_path = os.path.join(os.path.abspath(path), split_png_dir)

    for item in os.listdir(path):
        if item.endswith(".gif") or item.endswith(".GIF"):
            gifname = os.path.join(os.path.abspath(path), item)
            break

    # 将gif解析为图片
    # 读取GIF
    im = Image.open(gifname)
    # GIF图片流的迭代器
    iter = ImageSequence.Iterator(im)
    # 获取文件名
    index = 0
    # 判断目录是否存在
    mkdirlambda = lambda x: os.makedirs(
        x) if not os.path.exists(x) else True  # 目录是否存在,不存在则创建
    mkdirlambda(split_png_path)
    # 遍历图片流的每一帧
    for frame in iter:
        # print("image %d: mode %s, size %s" % (index, frame.mode, frame.size))
        pngname = "%s_%d.png" % (name, index)
        src = os.path.join(split_png_path, pngname)
        W, H = frame.size
        if W > H:
            h = w * H / (W * 1.0)
        if W < H:
            w = W * h / (H * 1.0)
        try:
            frame = frame.resize((round(w), round(h)))
            frame.save(src)
        except BaseException as e:
            print("error:", e.__class__.__name__, e)
            continue
        index += 1
    im.close()


parse_gif_to_png("C:/Users/LC/Desktop/test_merge_c/test", 160, 160)
