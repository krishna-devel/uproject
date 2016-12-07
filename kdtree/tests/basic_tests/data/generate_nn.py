import numpy as np

# dataName = "dummy_data"
dataName = "test_5d"

samplesFileName = "%s.csv"%dataName
queriesFileName = "%s.queries.csv"%dataName
nnFileName = "%s.nn.csv"%dataName

samples = map(
    lambda l: map(lambda i: float(i), l),
    map(lambda l: l.strip().split(','), open(samplesFileName))
  )

queries = map(
    lambda l: map(lambda i: float(i), l),
    map(lambda l: l.strip().split(','), open(queriesFileName))
)

S = np.array(samples)
Q = np.array(queries)

output = ""
for query in Q:
    euclideanDistance = np.sqrt(np.sum((S-query)*(S-query), axis=1))
    nearestNeighborIndex = np.argmin(euclideanDistance)
    output += ",".join(map(str, [nearestNeighborIndex, euclideanDistance[nearestNeighborIndex]])) + "\n"

with open(nnFileName, 'w') as f:
    f.write(output)