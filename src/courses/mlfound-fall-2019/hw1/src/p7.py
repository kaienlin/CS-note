import numpy as np
import matplotlib.pyplot as plt
import time, random

class Data:
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


class Verifier:
    def __init__(self, file_name):
        self.test_set = read_data(file_name)

    def verify(self, w):
        '''
        return the error rate of w on self.test_set
        '''
        err_cnt = 0
        for d in self.test_set:
            if not PLA.test(d, w):
                err_cnt += 1
        return err_cnt / len(self.test_set)


class PLA:
    def __init__(self, file_name, upd_time=50):
        self.D = read_data(file_name)
        self.w_pocket = None
        self.w_n = None
        self.upd_time = upd_time
        
    @staticmethod
    def sign(x):
        return 1 if x > 0 else -1
    
    @staticmethod
    def test(d, w):
        return PLA.sign(d.x @ w) == d.y

    def get_err_cnt(self, w):
        err_w = 0
        for d in self.D:
            if not PLA.test(d, w):
                err_w += 1
        return err_w

    def run(self):
        w = np.zeros(5)
        w_pocket = np.zeros(5)

        err_wp = self.get_err_cnt(w_pocket)

        upd = 0
        while upd < self.upd_time:
            d = random.choice(self.D)
            if not PLA.test(d, w):
                upd += 1
                w += d.y * d.x
                err_w = self.get_err_cnt(w)
                if err_w < err_wp:
                    w_pocket = np.copy(w)
                    err_wp = err_w

        self.w_pocket = w_pocket
        self.w_n = w
        

if __name__ == '__main__':
    pla = PLA('hw1_7_train.dat', upd_time=100)
    vrfy = Verifier('hw1_7_test.dat')
    T = 1126
    tot_err_rate = 0.0
    err_rate_list = []

    for _ in range(T):
        print(_)
        random.seed(int(time.process_time() * 1000000000) % (1<<32-1))
        pla.run()
        err_rate = vrfy.verify(pla.w_pocket)
        tot_err_rate += err_rate
        err_rate_list.append(err_rate)

    print(tot_err_rate / T)

    # plot histogram
    fig, ax = plt.subplots()
    num_bins = 100
    n, bins, patches = ax.hist(err_rate_list, num_bins, facecolor='green', alpha=0.5)

    # prettify
    ax.set_title('Figure 2: Pocket Algorithm statistics of $w_{pocket}$')
    ax.set_xlabel('error rate')
    ax.set_ylabel('frequency')
    fig.tight_layout()
    plt.grid(True)
    plt.show()

