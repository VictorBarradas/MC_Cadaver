import matplotlib.pyplot as plt
from pylab import show, plot, grid

with open("forcerecord.txt", "r") as file:
    data = file.readlines()

time_series = []
force_series = []
spike_series = []

for line in data:
    time, force, spike = line.split()
    time_series.append(time)
    force_series.append(force)
    spike_series.append(float(spike))
    	
	
plt.plot(time_series, force_series, 'r', label='Force')
plt.plot(time_series, spike_series, 'b', label='Spikes')
    
plt.legend(loc='upper right', numpoints=1)
grid('on')
plt.show()	
    