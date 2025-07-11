import subprocess as sub
import matplotlib.pyplot as plt
import numpy as num
import time
Height = []

Nodes = []
Leaves = []

Leaf_size_avg = []
Leaf_size_std = []

Split_2_avg =[]
Split_2_std =[]

Split_3_avg =[]
Split_3_std =[]

A1=[]
A2=[]
A3=[]

dimension =3
N=1000
clusters = 3
input_data=''

percentages=[30, 40, 30]


filename = 'Naive_GHT.c'
input_data=''

for z in range(21):
    # clusters = 2+z
    # for i in range(clusters-1):
    #     jojo = int(100/clusters)
    #     percentages.append(jojo)

    # jojojo = 100- int(100/clusters) * (clusters-1)
    # percentages.append(jojojo)
    # dimension = z+10
    # filename = 'Naive_GHT.c'
    # old_dimesion = "dimension " + str(dimension-1)
    # new_dimension = "dimension " + str(dimension)

    # # Read the contents of the file
    # with open(filename, 'r') as file:
    #     content = file.read()
    # if z==0:
    #     content = content.replace("dimension 10", new_dimension, 1)
    # else:
    #     content = content.replace(old_dimesion, new_dimension, 1)

    # with open(filename, 'w') as file:
    #     file.write(content)
    # percentages.clear()
    # percentages.append(z+1)
    # percentages.append(99-z)

    input_data = str(dimension)+'\n'+str(N)+'\n'+str(clusters)+'\n'+str(50)+'\n'

    for i in range(clusters):
        input_data += str(percentages[i])+'\n'
    input_data += 'Y\n'

    # print(buff)

    compile_generator= sub.run(['gcc', 'datapoint_generator.c', '-o', 'out','-lm'])
    run_generator= sub.run(['./out'], input= input_data.encode(), capture_output=True)
    buff = run_generator.stdout.decode("utf-8")

    

    compile_Naive_Tree= sub.run(['gcc', 'Naive_GHT.c', '-o', 'out','-lm'])
    run_Naive_Tree= sub.run(['./out'], input= input_data.encode(), capture_output=True)
    output= run_Naive_Tree.stdout.decode("utf-8")

    time.sleep(1)


    # print(output)
    x= 15
    while output[x] != '\n':
        x +=1
    y=x+10
    x+=10
    while output[x] != '\n':
        x +=1
    Height.append(int(output[y:x]))

    y=x+16
    x +=16
    while output[x] != '\n':
        x +=1
    Nodes.append(int(output[y:x]))

    y =x +21
    x+= 21
    while output[x] != '\n':
        x +=1
    Leaves.append(int(output[y:x]))

    y= x+21
    x+=21
    while output[x] != '\n':
        x +=1

    Leaf_size_avg.append(float(output[y:x]))
    y= x+34
    x+=34
    while output[x] != '\n':
        x +=1
    Leaf_size_std.append(float(output[y:x]))

    y= x+26
    x+=26
    while output[x] != '\n':
        x +=1
    Split_2_avg.append(float(output[y:x]))
    y= x+15
    x+=15
    while output[x] != '\n':
        x +=1
    Split_2_std.append(float(output[y:x]))

    y= x+26
    x+=26
    while output[x] != '\n':
        x +=1
    Split_3_avg.append(float(output[y:x]))

    y = x+15
    x+=15
    while output[x] != '\n':
        x +=1
    Split_3_std.append(float(output[y:x]))

    y=x+24
    x+=24
    while output[x] != '\n':
        x +=1
    A1.append(float(output[y:x]))

    y=x+24
    x+=24
    while output[x] != '\n':
        x +=1
    A2.append(float(output[y:x]))

    y=x+24
    x+=24
    while output[x] != '\n':
        x +=1
    A3.append(float(output[y:x]))
    # print(Height, Nodes, Leaves, Leaf_size_avg, Leaf_size_std, Split_2_avg,Split_2_std,Split_3_avg,Split_3_std,sep='\n')
    # print(output)
    if z%10 ==0:
        print(z/10)


x = range(10, 10 + len(Height))  # Match the x-axis length with the data lists

plt.plot(x, Height, color='red', label='Height')
plt.plot(x, Nodes, color='blue', label='No. of nodes')
plt.plot(x, Leaves, color='green', label='No. of leaves')
plt.plot(x, Leaf_size_avg, color='purple', label='Avg leaf size')
plt.plot(x, Leaf_size_std, color='orange', label='Std of leaf size')
# plt.plot(x, Split_2_avg, color='cyan', label='Avg Split-2')
# plt.plot(x, Split_2_std, color='magenta', label='Std Split-2')
# plt.plot(x, Split_3_avg, color='brown', label='Avg Split-3')
# plt.plot(x, Split_3_std, color='darkgrey', label='Std Split-3')
# plt.plot(x, A1, color='darkviolet', label='Test-1 Accuracy')
# plt.plot(x, A2, color='orangeRed', label='Accuracy')
plt.plot(x, A3, color='limegreen', label='Test Accuracy')

plt.axhline(y=0, color='black', linewidth=0.5)
plt.axvline(x=0, color='black', linewidth=0.5)
title= str(N)+" data points and 50 dimensions"
plt.title(title)
plt.xlabel('no. of clusters')
plt.legend()
plt.show()

# root count = 10007
# Height = 13
# no. of nodes = 335
