import subprocess as sub
import matplotlib.pyplot as plt
import numpy as num

Height = []

Nodes = []
Leaves = []

Leaf_size_avg = []
Leaf_size_std = []

Split_2_avg =[]
Split_2_std =[]

Split_3_avg =[]
Split_3_std =[]

dimension =3
N=1000
clusters = 2
input_data=''
percentages = []
for z in range(99):
    percentages.clear()
    percentages.append(z+1)
    percentages.append(99-z)
    
    input_data=''
    input_data += str(dimension)+'\n'+str(N)+'\n'+str(clusters)+'\n'+str(percentages[0])+'\n'+str(percentages[1])+'\n'+'Y\n'

    compile_generator= sub.run(['gcc', 'datapoint_generator.c', '-o', 'out','-lm'])
    run_generator= sub.run(['./out'], input= input_data.encode(), capture_output=True)
    buff = run_generator.stdout.decode("utf-8")
    # print(buff)

    compile_VarMed_Tree= sub.run(['gcc', 'VarMed_GHT.c', '-o', 'out','-lm'])
    run_VarMed_Tree= sub.run(['./out'], input= input_data.encode(), capture_output=True)
    output= run_VarMed_Tree.stdout.decode("utf-8")
    
    x= 27
    while output[x] != '\n':
        x +=1

    Height.append(int(output[27:x]))

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
    # print(Height, Nodes, Leaves, Leaf_size_avg, Leaf_size_std, Split_2_avg,Split_2_std,Split_3_avg,Split_3_std,sep='\n')
    # print(output)
    if z%10 ==0:
        print(z/10)


x = range(len(Height))  # Match the x-axis length with the data lists

plt.plot(x, Height, color='red', label='Height')
plt.plot(x, Nodes, color='blue', label='No. of nodes')
plt.plot(x, Leaves, color='green', label='No. of leaves')
plt.plot(x, Leaf_size_avg, color='purple', label='Avg leaf size')
plt.plot(x, Leaf_size_std, color='orange', label='Std of leaf size')
plt.plot(x, Split_2_avg, color='cyan', label='Avg Split-2')
plt.plot(x, Split_2_std, color='magenta', label='Std Split-2')
plt.plot(x, Split_3_avg, color='brown', label='Avg Split-3')
plt.plot(x, Split_3_std, color='grey', label='Std Split-3')

plt.axhline(y=0, color='black', linewidth=0.5)
plt.axvline(x=0, color='black', linewidth=0.5)

plt.xlabel('percentage distribution')
plt.legend()
plt.show()
