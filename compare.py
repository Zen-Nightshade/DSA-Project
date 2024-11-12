import subprocess as sub
import matplotlib.pyplot as plt
import numpy as num

Naive_Height = []

Naive_Nodes = []
Naive_Leaves = []

Naive_Leaf_size_avg = []
Naive_Leaf_size_std = []

Naive_Split_2_avg =[]
Naive_Split_2_std =[]

Naive_Split_3_avg =[]
Naive_Split_3_std =[]

VarMed_Height = []

VarMed_Nodes = []
VarMed_Leaves = []

VarMed_Leaf_size_avg = []
VarMed_Leaf_size_std = []

VarMed_Split_2_avg =[]
VarMed_Split_2_std =[]

VarMed_Split_3_avg =[]
VarMed_Split_3_std =[]

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
    VarMed_output= run_VarMed_Tree.stdout.decode("utf-8")

    compile_Naive_Tree= sub.run(['gcc', 'Naive_GHT.c', '-o', 'out','-lm'])
    run_Naive_Tree= sub.run(['./out'], input= input_data.encode(), capture_output=True)
    Naive_output= run_Naive_Tree.stdout.decode("utf-8")
    
    x= 27
    while Naive_output[x] != '\n':
        x +=1

    Naive_Height.append(int(Naive_output[27:x]))

    y=x+16
    x +=16
    while Naive_output[x] != '\n':
        x +=1
    Naive_Nodes.append(int(Naive_output[y:x]))

    y =x +21
    x+= 21
    while Naive_output[x] != '\n':
        x +=1
    Naive_Leaves.append(int(Naive_output[y:x]))

    y= x+21
    x+=21
    while Naive_output[x] != '\n':
        x +=1

    Naive_Leaf_size_avg.append(float(Naive_output[y:x]))
    y= x+34
    x+=34
    while Naive_output[x] != '\n':
        x +=1
    Naive_Leaf_size_std.append(float(Naive_output[y:x]))

    y= x+26
    x+=26
    while Naive_output[x] != '\n':
        x +=1
    Naive_Split_2_avg.append(float(Naive_output[y:x]))
    y= x+15
    x+=15
    while Naive_output[x] != '\n':
        x +=1
    Naive_Split_2_std.append(float(Naive_output[y:x]))

    y= x+26
    x+=26
    while Naive_output[x] != '\n':
        x +=1
    Naive_Split_3_avg.append(float(Naive_output[y:x]))

    y = x+15
    x+=15
    while Naive_output[x] != '\n':
        x +=1
    Naive_Split_3_std.append(float(Naive_output[y:x]))
    # print(Height, Nodes, Leaves, Leaf_size_avg, Leaf_size_std, Split_2_avg,Split_2_std,Split_3_avg,Split_3_std,sep='\n')
    # print(Naive_output)

    x= 27
    while VarMed_output[x] != '\n':
        x +=1

    VarMed_Height.append(int(VarMed_output[27:x]))

    y=x+16
    x +=16
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Nodes.append(int(VarMed_output[y:x]))

    y =x +21
    x+= 21
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Leaves.append(int(VarMed_output[y:x]))

    y= x+21
    x+=21
    while VarMed_output[x] != '\n':
        x +=1

    VarMed_Leaf_size_avg.append(float(VarMed_output[y:x]))
    y= x+34
    x+=34
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Leaf_size_std.append(float(VarMed_output[y:x]))

    y= x+26
    x+=26
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Split_2_avg.append(float(VarMed_output[y:x]))
    y= x+15
    x+=15
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Split_2_std.append(float(VarMed_output[y:x]))

    y= x+26
    x+=26
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Split_3_avg.append(float(VarMed_output[y:x]))

    y = x+15
    x+=15
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Split_3_std.append(float(VarMed_output[y:x]))
    # print(Height, Nodes, Leaves, Leaf_size_avg, Leaf_size_std, Split_2_avg,Split_2_std,Split_3_avg,Split_3_std,sep='\n')
    # print(VarMed_output)
    if z%10 ==0:
        print(z/10)



x = range(len(Naive_Height))  # Match the x-axis length with the data lists

# Plot Naive data with solid lines
plt.plot(x, Naive_Height, color='red', linestyle='-', label='Naive Height')
# plt.plot(x, Naive_Nodes, color='blue', linestyle='-', label='Naive No. of nodes')
# plt.plot(x, Naive_Leaves, color='green', linestyle='-', label='Naive No. of leaves')
# plt.plot(x, Naive_Leaf_size_avg, color='purple', linestyle='-', label='Naive Avg leaf size')
# plt.plot(x, Naive_Leaf_size_std, color='orange', linestyle='-', label='Naive Std of leaf size')
# plt.plot(x, Naive_Split_2_avg, color='cyan', linestyle='-', label='Naive Avg Split-2')
# plt.plot(x, Naive_Split_2_std, color='magenta', linestyle='-', label='Naive Std Split-2')
# plt.plot(x, Naive_Split_3_avg, color='brown', linestyle='-', label='Naive Avg Split-3')
# plt.plot(x, Naive_Split_3_std, color='grey', linestyle='-', label='Naive Std Split-3')

# Plot VarMed data with dashed lines
plt.plot(x, VarMed_Height, color='red', linestyle='--', label='VarMed Height')
# plt.plot(x, VarMed_Nodes, color='blue', linestyle='--', label='VarMed No. of nodes')
# plt.plot(x, VarMed_Leaves, color='green', linestyle='--', label='VarMed No. of leaves')
# plt.plot(x, VarMed_Leaf_size_avg, color='purple', linestyle='--', label='VarMed Avg leaf size')
# plt.plot(x, VarMed_Leaf_size_std, color='orange', linestyle='--', label='VarMed Std of leaf size')
# plt.plot(x, VarMed_Split_2_avg, color='cyan', linestyle='--', label='VarMed Avg Split-2')
# plt.plot(x, VarMed_Split_2_std, color='magenta', linestyle='--', label='VarMed Std Split-2')
# plt.plot(x, VarMed_Split_3_avg, color='brown', linestyle='--', label='VarMed Avg Split-3')
# plt.plot(x, VarMed_Split_3_std, color='grey', linestyle='--', label='VarMed Std Split-3')

plt.axhline(y=0, color='black', linewidth=0.5)
plt.axvline(x=0, color='black', linewidth=0.5)

plt.xlabel('percentage distribution')
plt.legend()
plt.show()
