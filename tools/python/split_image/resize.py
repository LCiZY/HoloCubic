import os.path
from PIL import Image


def resize_images_in_dir_to_jpg(path, outDir, size, suffix):
    index = 0
    if not os.path.exists(outDir):
        os.mkdir(outDir)
    for item in os.listdir(path):
        try:
            w = size
            h = size
            img2open = os.path.join(path, item)
            if not os.path.isfile(img2open):
                continue
            im = Image.open(img2open)
            newImgName = "%d%s" % (index, suffix)

            src = os.path.join(outDir, newImgName)
            W, H = im.size
            if W > H:
                h = w * H / (W * 1.0)
            if W < H:
                w = W * h / (H * 1.0)
            try:
                im = im.resize((round(w), round(h)))
                im.save(src)
                im.close()
                index += 1
            except BaseException as e:
                print("error2:", e.__class__.__name__, e)
                continue
        except BaseException as e:
            print("error1:", e.__class__.__name__, e)
            continue


imgDir = "C:/Users/LC/Desktop/holo_resource/we"
size = 200
suffix = ".jpg"
jpg_out_dir = "a_jpgs_%d"

resize_images_in_dir_to_jpg(imgDir, os.path.join(imgDir, jpg_out_dir % size), size, suffix)
