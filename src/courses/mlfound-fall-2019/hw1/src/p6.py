import numpy as np
import matplotlib.pyplot as plt
import time

class Data:
    '''
    Object of labeled data
    '''
    def __init__(self, x, y):
        self.x = x
        self.y = y


def read_data(file_name):
    '''
    function that read the data from file and 
    parse them into the form of class Data
    '''
    X = []
    Y = []
    with open(file_name, 'rt') as f:
        for ln in f.readlines():
            ln = ln.split()
            X.append( np.array([1.0] + [ float(x) for x in ln[:-1] ]) )
            Y.append(int(ln[-1]))

    X = np.array(X)
    Y = np.array(Y)

    return np.array([ Data(x, y) for x, y in zip(X, Y) ])


class PLA:
    def __init__(self, file_name, shuffle=False, nn=1):
        self.D = read_data(file_name)
        self.w_f = None
        self.shuffle = shuffle
        self.nn = nn

    @staticmethod
    def sign(x):
        return 1 if x > 0 else -1

    @staticmethod
    def test(d, w):
        return PLA.sign(d.x @ w) == d.y

    def run(self):
        D = np.copy(self.D)
        if self.shuffle: np.random.shuffle(D)
        w = np.zeros(5)
        upd = 0
        done = False
        while not done:
            done = True
            for d in D:
                if not PLA.test(d, w):
                    done = False
                    upd += 1
                    w += self.nn * d.y * d.x

        self.w_f = w
        self.upd = upd


if __name__ == '__main__':
    pla = PLA('hw1_6_train.dat', shuffle=True, nn=1)
    T = 1126
    tot_upd = 0
    max_upd = 0
    upd_time_list = []

    for _ in range(T):
        np.random.seed(int(time.clock() * 1000000000) % (1<<31-1))
        pla.run()
        tot_upd += pla.upd
        max_upd = max(max_upd, pla.upd)
        upd_time_list.append(pla.upd)

    print(f"average # of update: {tot_upd / T}")

    # plot histogram
    fig, ax = plt.subplots()
    num_bins = max_upd
    n, bins, patches = ax.hist(upd_time_list, num_bins, facecolor='purple', alpha=0.5)

    # prettify
    ax.set_title('Figure 1: PLA statistics')
    ax.set_xlabel('number of updates')
    ax.set_ylabel('number frequency')
    fig.tight_layout()
    plt.grid(True)
    plt.show()

