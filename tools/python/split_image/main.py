import os

from parse_gif_to_png import parse_gif_to_png
from merge_c import mergec
from gen_img_c_file import gen_c_file

# ---heart
#    --- heart.gif
#    --- heart.h
#    --- heart.c
#    --- cfiles
#    --- pngs

paths = [
    # "C:/Users/LC/Desktop/test_merge_c/top_right",
    # "C:/Users/LC/Desktop/test_merge_c/top_left",
    # "C:/Users/LC/Desktop/test_merge_c/bottom_right",
    # "C:/Users/LC/Desktop/test_merge_c/bottom_left",

    # "C:/Users/LC/Desktop/test_merge_c/cloud",
    # "C:/Users/LC/Desktop/test_merge_c/wing",
    # "C:/Users/LC/Desktop/test_merge_c/love_word",
    # "C:/Users/LC/Desktop/test_merge_c/smile_rabbit",
    # "C:/Users/LC/Desktop/test_merge_c/twinkle",
    # "C:/Users/LC/Desktop/test_merge_c/rainbow",
    # "C:/Users/LC/Desktop/test_merge_c/flowers_5",
    # "C:/Users/LC/Desktop/test_merge_c/photo_frame",
    # "C:/Users/LC/Desktop/test_merge_c/note",
    # "C:/Users/LC/Desktop/test_merge_c/love_7",
    # "C:/Users/LC/Desktop/test_merge_c/flowers_3",
    # "C:/Users/LC/Desktop/test_merge_c/love",
    "C:/Users/LC/Desktop/test_merge_c/fanli1",
    "C:/Users/LC/Desktop/test_merge_c/fanli2",
    "C:/Users/LC/Desktop/test_merge_c/fanli3",
    "C:/Users/LC/Desktop/test_merge_c/fanli4",
    "C:/Users/LC/Desktop/test_merge_c/pp1",
    "C:/Users/LC/Desktop/test_merge_c/pp2",
    "C:/Users/LC/Desktop/test_merge_c/pp3",
    "C:/Users/LC/Desktop/test_merge_c/pp4",
    "C:/Users/LC/Desktop/test_merge_c/pp5",
    "C:/Users/LC/Desktop/test_merge_c/yangbaobao1",
    "C:/Users/LC/Desktop/test_merge_c/yangbaobao2",
    "C:/Users/LC/Desktop/test_merge_c/yangbaobao3",
    "C:/Users/LC/Desktop/test_merge_c/yangbaobao4",
    "",
    "",
    "",

]

size = 96  # w and h

if __name__ == "__main__":
    for path in paths:
        if path == "":
            continue
        print("step 1: begin to parse png...")
        parse_gif_to_png(path, size, size)

        print("step 2: begin to gen c files...")
        gen_c_file(path)

        print("step 3: begin to merge c files...")
        mergec(path)

        print("done.\r\n")
