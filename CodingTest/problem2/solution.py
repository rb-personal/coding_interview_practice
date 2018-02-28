import csv
from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
from sklearn import svm
from sklearn.metrics import mean_squared_error, r2_score
from mpl_toolkits.mplot3d import Axes3D

#mnist = input_data.read_data_sets('/tmp/data/', one_hot=False)

# Data container
Y_train = list()
Y_test = list()
X_train = list()
X_test = list()

scale = 1000000.0

# Neural-Net hyper-parameters
learning_rate = 0.1
num_steps = 1000
batch_size = 128
display_step = 100

n_hidden_1 = 256
n_hidden_2 = 256
num_features = 2
num_classes = 1


def get_data():
    # Read the data
    Ys = list()
    Xs = list()

    with open ('case3.csv', 'r') as inputfile:
        reader = csv.reader(inputfile)
        next(reader)
        counter = 0
        for row in reader:
            Ys.append(float(row[0]))
            Xs.append([float(row[1]), float(row[2])])

    # with open ('case3.csv', 'r') as inputfile:
    #     reader = csv.reader(inputfile)
    #     next(reader)
    #     counter = 0
    #     for row in reader:
    #         y = float(row[0])
    #         x0 = float(row[1])
    #         x1 = float(row[2])
    #         scaled = int(y * scale)
    #         if counter % 10 == 0:
    #             Y_test.append(y)
    #             X_test.append([x0, x1])
    #         else:
    #             Y_train.append(y)
    #             X_train.append([x0, x1])
    #         counter += 1

    # x_np = np.array(X0s)
    # y_np = np.array(X1s)
    # z_np = np.array(Ys)

    # fig = plt.figure()
    # ax = fig.add_subplot(111, projection='3d')
    # ax.scatter(x_np, y_np, z_np)
    # ax.set_xlabel('X0')
    # ax.set_ylabel('X1')
    # ax.set_zlabel('Y')
    # plt.show()

    y = np.array(Ys)
    #print(y)
    x = np.array(Xs)
    #print(x)

    svr_rbf = svm.SVR(kernel='rbf')
    svr_lin = svm.SVR(kernel='linear')
    svr_poly = svm.SVR(kernel='poly')


def main():
    get_data()

    # print(mnist.train.images)
    # print(mnist.train.labels)
    # print(mnist.train.images.shape)
    # print(mnist.train.labels.shape)

    # get_data()
    # X_train_np = np.array(X_train, dtype='float')
    # print(X_train_np)
    # Y_train_np = np.array(Y_train, dtype='int')
    # print(Y_train_np)

    # model = tf.estimator.Estimator(model_fn)

    # input_fn = tf.estimator.inputs.numpy_input_fn(
    #     x={'train':X_train_np}, y=Y_train_np,
    #     batch_size = batch_size,
    #     num_epochs = None,
    #     shuffle=True)
    # model.train(input_fn, steps=num_steps)

    # input_fn = tf.estimator.inputs.numpy_input_fn(
    #     x=X_test, y=Y_test,
    #     batch_size = batch_size,
    #     shuffle=False)

    # e = model.evaluate(input_fn)

    # print("Testing Accuracy:", e['accuracy'])


if __name__ == "__main__":
    main()
