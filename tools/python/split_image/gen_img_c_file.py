import os

c_file_dir = "cfiles"

ts_script_path = "F:/projects/vscodeProjects/lv_img_conv/lib/cli.ts"


def gen_c_file(path):
    c_file_path = os.path.join(os.path.abspath(path), c_file_dir)
    if not os.path.exists(c_file_path):
        os.mkdir(c_file_path)
    gen_c_result = os.popen("ts-node %s %s -f" % (ts_script_path, path)).read()  # set-ExecutionPolicy RemoteSigned
    print("step2 result:", gen_c_result)
