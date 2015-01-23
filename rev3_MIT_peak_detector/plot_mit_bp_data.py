import pylab as pl
import numpy as np

import csv

time = []
bp = []

with open("bp_test_data.csv", 'rU') as f:
	reader = csv.reader(f)
	next(reader, None)
	for row in reader:
		if row:
			time.append(row[0])
			bp.append(row[1])

pl.plot(time, bp)
pl.show()