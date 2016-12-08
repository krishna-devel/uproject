import numpy as np

class Config(object):
    def __init__(self, num_samples, num_queries, num_dimensions, dataset_name):
        self.num_samples = num_samples
        self.num_queries = num_queries
        self.num_dimensions = num_dimensions
        self.data_filename = "%s.csv"%dataset_name
        self.queries_filename = "%s.queries.csv"%dataset_name
    def generate(self):
        S = np.random.rand(self.num_samples, self.num_dimensions)
        Q = np.random.rand(self.num_queries, self.num_dimensions)
        with open(self.data_filename, 'w') as f:
            for r in range(self.num_samples): f.write(",".join(map(str, S[r]))+"\n")
        with open(self.queries_filename, 'w') as f:
            for r in range(self.num_queries): f.write(",".join(map(str, Q[r]))+"\n")

if __name__ == '__main__':
    # Config(num_samples=1000, num_queries=5000, num_dimensions=25, dataset_name="test_25d").generate()
    Config(num_samples=50000, num_queries=5000, num_dimensions=3, dataset_name="test_50K").generate()