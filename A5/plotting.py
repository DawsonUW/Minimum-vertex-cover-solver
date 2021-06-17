import matplotlib.pyplot as plt
import statistics
a1,a2,a3,a4,a5,a6,a7,a8=[],[],[],[],[],[],[],[]
cnf_time,cnf_time_2,cnf_time_3=[],[],[]
vertices_plot=[5,10,15]
mean_plot,stdev_plot=[],[]

for line in open('output_from_dawson.txt','r'):
    values=[float(s) for s in line.split(',')]
    a1.append(values[0])
    a2.append(values[1])#number of vertices
    a3.append(values[2])
    a4.append(values[3])#cnf_time
    a5.append(values[4])
    a6.append(values[5])
    a7.append(values[6])
    a8.append(values[7])

for k in range(100):
    cnf_time.append(a4[k])
for m in range(100,200):
    cnf_time_2.append(a4[m])
for n in range(200,300):
    cnf_time_3.append(a4[n])
# print(statistics.mean(cnf_time))
# print(statistics.stdev(cnf_time))
# print(statistics.mean(cnf_time_2))
# print(statistics.stdev(cnf_time_2))
# print(statistics.mean(cnf_time_3))
# print(statistics.stdev(cnf_time_3))
mean_plot.append(statistics.mean(cnf_time))
mean_plot.append(statistics.mean(cnf_time_2))
mean_plot.append(statistics.mean(cnf_time_3))
stdev_plot.append(statistics.stdev(cnf_time))
stdev_plot.append(statistics.stdev(cnf_time_2))
stdev_plot.append(statistics.stdev(cnf_time_3))

plt.errorbar(vertices_plot,mean_plot,stdev_plot,marker='o',linestyle='--',capsize=5)
plt.show()



# print(a4[1])
# plt.errorbar(a2, a3, a4, linestyle='None', marker='^')
# plt.show()


