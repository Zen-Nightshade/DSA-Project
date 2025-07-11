import subprocess as sub
import matplotlib.pyplot as plt
import numpy as num
import time

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

Naive_A1=[]
Naive_A2=[]
Naive_A3=[]

VarMed_A1=[]
VarMed_A2=[]
VarMed_A3=[]

dimension =50
N=10000
clusters = 50
input_data=''
percentages=[]
for i in range(clusters):
    percentages.append(2)


for z in range(201):
    # clusters = 2+z
    # percentages=[]
    # for i in range(clusters-1):
    #     jojo = int(100/clusters)
    #     percentages.append(jojo)

    # jojojo = 100- int(100/clusters) * (clusters-1)
    # percentages.append(jojojo)
    dimension = z+10
    files = ['VarMed_GHT.c', 'Naive_GHT.c']
    old_dimesion = "dimension " + str(dimension-1)
    new_dimension = "dimension " + str(dimension)

    # Read the contents of the file
    for filename in files:
        with open(filename, 'r') as file:
            content = file.read()
        if z==0:
            content = content.replace("dimension 2", new_dimension, 1)
        else:
            content = content.replace(old_dimesion, new_dimension, 1)

        with open(filename, 'w') as file:
            file.write(content)

    input_data=''
    input_data += str(dimension)+'\n'+str(N)+'\n'+str(clusters)+'\n'+str(50)+'\n'

    for i in range(clusters):
        # print(percentages[i])
        input_data += str(percentages[i])+'\n'
    input_data += 'Y\n'

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
    
    time.sleep(1)
    # print(Naive_output)


    x= 15
    while Naive_output[x] != '\n':
        x +=1
    y=x+10
    x+=10
    
    while Naive_output[x] != '\n':
        x +=1
    Naive_Height.append(int(Naive_output[y:x]))

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

    y=x+24
    x+=24
    while Naive_output[x] != '\n':
        x +=1
    Naive_A1.append(float(Naive_output[y:x]))

    y=x+24
    x+=24
    while Naive_output[x] != '\n':
        x +=1
    Naive_A2.append(float(Naive_output[y:x]))

    y=x+24
    x+=24
    while Naive_output[x] != '\n':
        x +=1
    Naive_A3.append(float(Naive_output[y:x]))
    # print(Height, Nodes, Leaves, Leaf_size_avg, Leaf_size_std, Split_2_avg,Split_2_std,Split_3_avg,Split_3_std,sep='\n')
    # print(Naive_output)
 
    x= 15
    while VarMed_output[x] != '\n':
        x +=1
    y=x+10
    x+=10
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_Height.append(int(VarMed_output[y:x]))

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
    y=x+24
    x+=24
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_A1.append(float(VarMed_output[y:x]))

    y=x+24
    x+=24
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_A2.append(float(VarMed_output[y:x]))

    y=x+24
    x+=24
    while VarMed_output[x] != '\n':
        x +=1
    VarMed_A3.append(float(VarMed_output[y:x]))
    # print(Height, Nodes, Leaves, Leaf_size_avg, Leaf_size_std, Split_2_avg,Split_2_std,Split_3_avg,Split_3_std,sep='\n')
    # print(VarMed_output)
    if z%10 ==0:
        print(z/10)



x = range(len(Naive_Height))  # Match the x-axis length with the data lists

# Plot Naive data with solid lines
plt.plot(x, Naive_Height, color='red', linestyle='-', label='Random Height')
# plt.plot(x, Naive_Nodes, color='blue', linestyle='-', label='Naive No. of nodes')
# plt.plot(x, Naive_Leaves, color='green', linestyle='-', label='Naive No. of leaves')
# plt.plot(x, Naive_Leaf_size_avg, color='purple', linestyle='-', label='Naive Avg leaf size')
# plt.plot(x, Naive_Leaf_size_std, color='orange', linestyle='-', label='Naive Std of leaf size')
# plt.plot(x, Naive_Split_2_avg, color='cyan', linestyle='-', label='Naive Avg Split-2')
# plt.plot(x, Naive_Split_2_std, color='magenta', linestyle='-', label='Naive Std Split-2')
# plt.plot(x, Naive_Split_3_avg, color='brown', linestyle='-', label='Naive Avg Split-3')
# plt.plot(x, Naive_Split_3_std, color='grey', linestyle='-', label='Naive Std Split-3')
# plt.plot(x, Naive_A1, color='darkviolet', linestyle='-', label='Test-1 Accuracy')
# plt.plot(x, Naive_A2, color='orangeRed', linestyle='-', label='Accuracy')
plt.plot(x, Naive_A3, color='limegreen', linestyle='-', label='Test Accuracy for Random')
# Plot VarMed data with dashed lines
plt.plot(x, VarMed_Height, color='red', linestyle='--', label='Variance Height')
# plt.plot(x, VarMed_Nodes, color='blue', linestyle='--', label='VarMed No. of nodes')
# plt.plot(x, VarMed_Leaves, color='green', linestyle='--', label='VarMed No. of leaves')
# plt.plot(x, VarMed_Leaf_size_avg, color='purple', linestyle='--', label='VarMed Avg leaf size')
# plt.plot(x, VarMed_Leaf_size_std, color='orange', linestyle='--', label='VarMed Std of leaf size')
# plt.plot(x, VarMed_Split_2_avg, color='cyan', linestyle='--', label='VarMed Avg Split-2')
# plt.plot(x, VarMed_Split_2_std, color='magenta', linestyle='--', label='VarMed Std Split-2')
# plt.plot(x, VarMed_Split_3_avg, color='brown', linestyle='--', label='VarMed Avg Split-3')
# plt.plot(x, VarMed_Split_3_std, color='grey', linestyle='--', label='VarMed Std Split-3')
# plt.plot(x, VarMed_A1, color='darkviolet', linestyle='--', label='Test-1 Accuracy')
# plt.plot(x, VarMed_A2, color='orangeRed', linestyle='--', label='Accuracy')
plt.plot(x, VarMed_A3, color='limegreen', linestyle='--', label='Test Accuracy for Variance')

plt.axhline(y=0, color='black', linewidth=0.5)
plt.axvline(x=0, color='black', linewidth=0.5)
plt. title("Random Vs Variance 10k pts and 50D")
plt.xlabel('Dimensions')
plt.legend()
plt.show()
