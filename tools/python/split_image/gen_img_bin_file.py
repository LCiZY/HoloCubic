import os.path
from convertor.core import Convertor
from resize import resize_images_in_dir_to_jpg


def get_img_bin_image(path, outDir):
    if not os.path.exists(outDir):
        os.mkdir(outDir)
    for item in os.listdir(path):
        print("正在转换图片: {} ...".format(os.path.basename(item)))
        img_path = os.path.join(path, item)
        c = Convertor(img_path, outDir)
        c.get_bin_file()
        # c.get_c_code_file()


imgDir = "C:/Users/LC/Desktop/holo_resource/we"
size = 61
suffix = ".png"
out_dir = "a_pngs"
bin_out_dir = "a_bins"

if __name__ == "__main__":
    if not imgDir == "":
        print("step 1: begin to resize images...")
        resize_images_in_dir_to_jpg(imgDir, os.path.join(imgDir, out_dir), size, suffix)

        print("step 2: begin to gen bin files...")
        get_img_bin_image(os.path.join(imgDir, out_dir), os.path.join(imgDir, bin_out_dir))

        print("done.\r\n")
0