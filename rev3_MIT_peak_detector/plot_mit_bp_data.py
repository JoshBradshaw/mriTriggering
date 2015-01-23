import pylab as pl
import numpy as np

import csv

time = []
bp = []
markers_on = []

with open("test_data/filtered_values.csv", 'rU') as f:
	reader = csv.reader(f)
	for ii, row in enumerate(reader):
		if row:
			time.append(row[0])
			bp.append(row[1])
			if int(row[2]):
				markers_on.append(ii)

unfiltered_bp =[]
with open("test_data/unfiltered_values.csv", 'rU') as f:
	reader = csv.reader(f)
	next(reader, None)
	for row in reader:
		if row:
			unfiltered_bp.append(row[1])
			

print(len(time))
print(len(bp))
print(len(unfiltered_bp))


pl.plot(time, bp, color="blue", linewidth=2.5, linestyle="-")
pl.plot(time, unfiltered_bp, color="red",  linewidth=2.5, linestyle="-")

pl.plot(np.array(time)[markers_on], np.array(bp)[markers_on], 'rD')

pl.show()