import tensorflow as tf

filename_queue = tf.train.string_input_producer(["case2.csv"])

reader = tf.TextLineReader()
key, value = reader.read(filename_queue)

Y, X1, X2 = tf.decode_csv(value
)
