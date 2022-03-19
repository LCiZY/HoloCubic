# Created by 牧野 CSDN
from PIL import Image

img_path = './1482.gif'
split_img_width = 64
split_img_height = 64

img_path = img_path.replace("\\", "/")
img_name = img_path[img_path.rindex('/') + 1:]
img_name_dot_idx = img_name.rindex('.')
img = Image.open(img_path)

width, height = img.size
img.close()

curr = 0
i = 1
while curr < width:
    img = Image.open(img_path)
    sub_img_name = "_%d" % i
    # 前两个坐标点是左上角坐标
    # 后两个坐标点是右下角坐标
    # width在前， height在后
    box = (curr, 0, curr + split_img_width, split_img_height)
    region = img.crop(box)
    region.save("./splits/"+img_name[:img_name_dot_idx] + sub_img_name + '.png')
    i += 1
    curr += split_img_width
    img.close()
