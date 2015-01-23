import pylab as pl
import numpy as np

import csv

time = []
bp = []

with open("unfiltered_values.csv", 'rU') as f:
	reader = csv.reader(f)
	next(reader, None)
	for row in reader:
		if row:
			time.append(row[0])
			bp.append(row[1])

filtered_bp = []
with open('filtered_values.csv', 'rU') as f:
	reader = csv.reader(f)
	for row in reader:
		if row:
			filtered_bp.append(row[1])
print(len(time))
print(len(bp))
print(len(filtered_bp))

pl.plot(time, bp, color="blue", linewidth=2.5, linestyle="-", label="cosine")
pl.plot(time, filtered_bp, color="red",  linewidth=2.5, linestyle="-", label="sine")
pl.show()