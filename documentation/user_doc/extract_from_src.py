import pprint
import sys
import re
import os
import pathlib


#print ("Number of arguments: " + str(len(sys.argv)))
#print ('Argument List: ' + str(sys.argv))

# argv[0]: this script extract_from_src.py
# argv[1]: the source file path
# argv[2]: ${CMAKE_SOURCE_DIR}
# argv[3]: ${CMAKE_CURRENT_BINARY_DIR}

print("argv[0]: " + sys.argv[0])
print("argv[1]: " + sys.argv[1])
print("argv[2]: " + sys.argv[2])
print("argv[3]: " + sys.argv[3])

print(sys.argv[1].replace(sys.argv[2],''))
print(sys.argv[3] + sys.argv[1].replace(sys.argv[2],''))
print(os.path.dirname(sys.argv[3] + sys.argv[1].replace(sys.argv[2],'')))
filedir_output = os.path.dirname(sys.argv[3] + sys.argv[1].replace(sys.argv[2],''))
pathlib.Path(filedir_output).mkdir(parents=True, exist_ok=True)
#"""

label_doc = ('//begin{usr_doc}','//end{usr_doc}')
label_comment = ('//begin{comment}','//end{comment}')
label_code = ('//begin{code}','//end{code}')

def check_block(nums_begin, nums_end, label):
    if len(nums_begin) != len(nums_end):
        sys.exit("the number of '" + label[0] + "' is not equal to the number of '" + label[1] + "'")
    nb_blocks =  len(nums_begin)
    if nb_blocks == 0:
        sys.exit("No block is found")
    check_list = []
    nb_test = nb_blocks*2
    nums_block = []
    for i in range(0,nb_blocks):
        check_list.append(nums_begin[i])
        check_list.append(nums_end[i])
        nums_block.append((nums_begin[i],nums_end[i]))
    for i in range(1,nb_test):
        if check_list[i] < check_list[i-1]:
            sys.exit("The begin and end label in block between line " + str(check_list[i-1] + 1) + " and line " + str(check_list[i] + 1) + " is not correct")
    print("All blocks between " + label[0] + " and " + label[1] + " are OK")
    return nb_blocks, nums_block

def find_function_name(num_c_b, lines):
    for i in range(num_c_b[0],num_c_b[1]):
        line = lines[i]
        if '::' in line:
            name = re.findall(r"[a-zA-Z0-9]*::[a-zA-Z0-9]*", line)
            break
    filename = name[0].replace('::','_')
    print(filename)
    return filename



print("try to extract the file: " + str(sys.argv[1]))
fin = open(sys.argv[1], 'r')
lines=fin.readlines()
fin.close()

nums_doc_begin=[]
nums_doc_end=[]

for i in range(0,len(lines)-1):#
    line = lines[i].lstrip().rstrip() #"// begin{usr_doc}"
    if line == label_doc[0]:
        nums_doc_begin.append(i)
    elif line == label_doc[1]:
        nums_doc_end.append(i)
    else:
        continue

nb_doc_block, nums_doc_blocks = check_block(nums_doc_begin, nums_doc_end, label_doc)


for i in range(0,nb_doc_block):
    nums_comment_begin = []
    nums_comment_end = []
    nums_code_begin = []
    nums_code_end = []
    for j in range (nums_doc_blocks[i][0],nums_doc_blocks[i][1]):
        line = lines[j].lstrip().rstrip()
        if line == label_comment[0]:
            nums_comment_begin.append(j)
        elif line == label_comment[1]:
            nums_comment_end.append(j)
        elif line == label_code[0]:
            nums_code_begin.append(j)
        elif line == label_code[1]:
            nums_code_end.append(j)
        else:
            continue
    nb_comment_block, nums_comment_blocks = check_block(nums_comment_begin, nums_comment_end, label_comment)
    nb_code_block, nums_code_blocks = check_block(nums_code_begin, nums_code_end, label_code)
    print("nb_comment_block = " + str(nb_comment_block))
    print("nb_comment_block = " + str(nums_comment_blocks))
    print("nb_code_block = " + str(nb_code_block))
    print("nums_code_blocks = " + str(nums_code_blocks))



    base_name = os.path.basename(sys.argv[1])
    base_name = base_name.replace(".", "_")
    function_name = find_function_name(nums_code_blocks[0], lines)
    filename_base = base_name + "_" + function_name
    #print("file_output: " + filedir_output)
    #print("filename_base: " + filename_base)
    file_output_base = filedir_output + "\\" + filename_base\

    #print(file_output)
    fo = open(file_output_base + ".out_code", 'w')
    for j in range(nums_code_blocks[0][0]+1,nums_code_blocks[0][1]):
        fo.write(lines[j])
    fo.close()

    fo = open(file_output_base + ".out_comment", 'w')
    for j in range(nums_comment_blocks[0][0]+2, nums_comment_blocks[0][1]-1):
        fo.write(lines[j])
    fo.close()


#"""


