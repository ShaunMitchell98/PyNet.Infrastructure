from PyNetwork import PyNetwork
import tensorflow as tf
import matplotlib.pyplot as plt


fashion_mnist = tf.keras.datasets.fashion_mnist

(train_images, train_labels), (test_images, test_labels) = fashion_mnist.load_data()

class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
               'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']


train_images = train_images / 255.0

test_images = test_images / 255.0

network = PyNetwork(784)
network.add_layer(500)
network.add_layer(129)
network.add_layer(10)

batch_size: int = 5
learning_rate: float = 0.01
number: int = 17

errors = network.train(train_images[6:number], train_labels[6:number], 10, batch_size, learning_rate)
x = list(range(0, train_images[6:number].shape[0]))
plt.title("Change in Error")
plt.xlabel("Iteration")
plt.ylabel("Error")
plt.plot(x, errors)
plt.show()
