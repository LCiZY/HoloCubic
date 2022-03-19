import os

c_file_dir = "cfiles"


def mergec(path):
    name = os.path.basename(path)
    c_file_path = os.path.join(os.path.abspath(path), c_file_dir)
    num = len(os.listdir(c_file_path))
    varList = ""
    with open(os.path.join(os.path.abspath(path), name + ".c"), 'w') as f:
        f.write("#include \"%s.h\"\r\n" % name)
        f.write("#ifndef LV_ATTRIBUTE_MEM_ALIGN\r\n#define LV_ATTRIBUTE_MEM_ALIGN\r\n#endif\r\n")
        for i in range(num):
            var_name = "&" + name + "_%d" % i
            if i != 0:
                var_name = ", " + var_name
            varList += var_name
            src = os.path.join(c_file_path, name + "_%d" % i + '.c')
            with open(src, 'r') as sub:
                content = sub.read()
                content = content[content.index("#ifndef", content.index("#ifndef") + 2):]
                f.write(content + "\r\n")

        f.write("\r\nconst void *%ss[] = {%s};\r\nstruct Gif gif_obj_%s = {\r\n.frames = %ss,\r\n.frame_num = %d,\r\n.speed = 100,\r\n.last_tick = 0,\r\n.curr_frame = 0,\r\n};" % (name, varList, name, name, num))

    with open(os.path.join(os.path.abspath(path), name + ".h"), 'w') as f:
        with open('./header.h', 'r') as template:
            f.write(template.read() % (name.upper(), name.upper(), name))





