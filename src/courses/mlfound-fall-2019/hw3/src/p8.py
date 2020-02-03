import numpy as np
import matplotlib.pyplot as plt

training_data_file_name = "hw3_train.dat"
test_data_file_name = "hw3_test.dat"

T = 2000
dim = 20

class Problem8:
    def __init__(self):
        self.N = 0
        self.X = None
        self.Y = None
        self.Ntest = 0
        self.Xtest = None
        self.Ytest = None
    
    @staticmethod
    def read(fname, X, Y):
        with open(fname, "r") as f:
            for ln in f.readlines():
                ln = ln.split()
                X.append([1.0,] + [ float(x) for x in ln[:-1] ])
                Y.append([int(ln[-1]),])

    def read_data(self):
        self.X = []
        self.Y = []
        self.read(training_data_file_name, self.X, self.Y)
        self.N = len(self.X)
        self.X = np.array(self.X)
        self.Y = np.array(self.Y)

        self.Xtest = []
        self.Ytest = []
        self.read(test_data_file_name, self.Xtest, self.Ytest)
        self.Ntest = len(self.Xtest)
        self.Xtest = np.array(self.Xtest)
        self.Ytest = np.array(self.Ytest)

    @staticmethod
    def theta(s):
        return 1.0 / (1.0 + np.exp(-s))

    def calc_err(self, X, Y, W):
        P = X @ W
        err = 0
        for y, p in zip(Y, P):
            p = self.theta(p)
            yp = 1 if p > 0.5 else -1
            if yp != y:
                err += 1
        return err / self.Ntest

    def trainGD(self):
        w = np.zeros(dim + 1)
        record = []
        for _ in range(T):
            grad = np.zeros(dim + 1)
            for x, y in zip(self.X, self.Y):
                grad += -1.0 / self.N * self.theta(-y * np.dot(w, x)) * (y * x)
            w -= 0.01 * grad
            record.append(self.calc_err(self.Xtest, self.Ytest, w))
        self.GD_Eout = np.array(record)

    def trainSGD(self):
        w = np.zeros(dim + 1)
        record = []
        samples = [n % self.N for n in range(T)]
        for n in samples:
            grad = -self.theta(-self.Y[n] * np.dot(w, self.X[n])) * (self.Y[n] * self.X[n])
            w -= 0.001 * grad
            record.append(self.calc_err(self.Xtest, self.Ytest, w))
        self.SGD_Eout = np.array(record)

    def plot(self):
        time = np.array([t for t in range(1, T+1)])
        #np.save("t_axis.npy", time)
        #np.save("gd_eout.npy", self.GD_Eout)
        #np.save("sgd_eout.npy", self.SGD_Eout)
        fig, ax = plt.subplots()
        ax.plot(time, self.GD_Eout, label=r"GD  ($\eta=0.01$)", color="purple", alpha=0.7, linewidth=2)
        ax.plot(time, self.SGD_Eout, label=r"SGD($\eta=0.001$)", color="orchid", alpha=0.7, linewidth=2)
        plt.title(r"Comparison of $E_{out}(\mathbf{w}_t): $Gradient Descent(GD) v.s. Stochastic Gradient Descent(SGD)")
        plt.xlabel(r"$t$")
        plt.ylabel(r"$E_{out}(\mathbf{w}_t)$")
        plt.legend()
        plt.show()

def main():
    trainer = Problem8()
    trainer.read_data()
    trainer.trainGD()
    trainer.trainSGD()
    trainer.plot()

if __name__ == '__main__':
    main()
