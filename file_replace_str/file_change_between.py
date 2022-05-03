import os

def file_copy(fsrc, fdst):
    src = open(fsrc, 'r')
    dst = open(fdst, 'w')
    dst.writelines(src.readlines())
    dst.close()
    src.close()

def file_change_between(fpath, str_before, str_after, str_new):
    file_object = open(fpath, 'r')
    ofs = open(fpath+'.new', 'w')

    while True:
        line = file_object.readline()
        if not line:
            break
        pos = line.find(str_before)
        if pos != -1:
            pos_end = line.find(str_after)
            if pos_end == -1:
                ofs.write(line)
            else:
                line_tail = line[pos_end:]
                line_head = line[:pos + len(str_before)]
                ofs.write(line_head)
                ofs.write(str_new)
                ofs.write(line_tail)
        else:
            ofs.write(line)
    ofs.close()
    file_object.close()
    file_copy(fpath+".new", fpath)
    os.remove(fpath+".new")
    return 0
