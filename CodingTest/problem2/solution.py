import csv
import tensorflow as tf

# Data container
Y_train = list()
Y_test = list()
X_train = list()
X_test = list()

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
    with open ('case2.csv', 'r') as inputfile:
        reader = csv.reader(inputfile)
        next(reader)
        counter = 0
        for row in reader:
            if counter % 10 == 0:
                Y_test.append(row[0])
                X_test.append([row[1],row[2]])
                counter += 1
            else:
                Y_train.append(row[0])
                X_train.append([row[1],row[2]])


def neural_net(features):
    layer_1 = tf.layers.dense(features, n_hidden_1)
    layer_2 = tf.layers.dense(layer_1, n_hidden_2)
    out_layer = tf.layers.dense(layer2, num_classes)
    return out_layer


def model_fn(features, labels, mode):
    logits = neural_net(features)

    pred_classes = tf.argmax(logits, axis=1)
    pred_probas = tf.nn.softmax(logits)

    if mode == tf.estimator.ModeKeys.PREDICT:
        return tf.estimator.EstimatorSpec(mode, predictions=pred_classes)

    loss_op = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(
        logits=logits, labels=tf.cast(labels, dtype=tf.float32)))
    optimizer = tf.train.GradientDescentOptimizer(learning_rate=learning_rate)
    train_op = optimizer.minimize(loss_op, global_step=tf.train.get_global_step())

    acc_op = tf.metrics.accuracy(labels=labels, predictions=pred_classes)

    estim_specs = tf.estimator.EstimatorSpec(
        mode=mode, predictions=pred_classes, loss=loss_op, train_op=train_op, eval_metric_ops={'accuracy':acc_op})

    return estim_specs


def main():
    get_data()

    # model = tf.estimator.Estimator(model_fn)

    # input_fn = tf.estimator.inputs.numpy_input_fn(
    #     x=X_train, y=Y_train,
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
