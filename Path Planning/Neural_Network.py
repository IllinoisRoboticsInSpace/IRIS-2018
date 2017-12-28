import numpy as np
import cPickle
import gzip
import random
class Network(object):

    def __init__(self,sizes):
        np.set_printoptions(threshold=np.nan)
        self.numlayers=len(sizes)
        self.sizes=sizes
        self.biases=[np.random.randn(y, 1) for y in sizes[1:]]
        self.weights = [np.random.randn(x, y)
                        for x, y in zip(sizes[:-1], sizes[1:])]
        #self.input=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1],[1,1,0],[1,1,1]])
        #self.output=np.array([[0],[1],[1],[0],[1],[0],[0],[1]])
        self.values=[]
        self.ActivationValues=[]
        self.errors=[]
        self.learning_rate=1.5
        self.input=np.empty((10,784))
        self.output=np.empty((10,10))
        self.epochs=50
        self.mini_batch_size=10
        'self.SGD()'

    def ForwardPropagation(self):

        x=self.input
        self.values.append(self.input)
        self.ActivationValues.append(self.input)

        for i in range(self.numlayers-1):
            self.values.append(np.dot(x,self.weights[i])+(self.biases[i]).T)
            self.ActivationValues.append(self.sigmoid(self.values[i+1]))
            x=(self.ActivationValues[i+1])

    def BackPropagation(self):
        #based on four BackPropagation laws in http://neuralnetworksanddeeplearning.com/chap2.html
        CostDerivative=self.costDerivative()
        self.errors.append(np.multiply(CostDerivative,self.sigmoid_prime(self.values[-1]))) #Outer layer errors
        for i in range(self.numlayers-2):
            self.errors.append(np.multiply(np.dot(self.errors[i],(self.weights[self.numlayers-2-i]).T),self.sigmoid_prime(self.values[self.numlayers-2-i])))

        bias_derivatives=[(error.sum(axis=0,keepdims=1)).T for error in self.errors[::-1]]
        weight_derivatives=[(np.dot(error.T,activation_value)).T for error,activation_value in zip(self.errors,self.ActivationValues[1::-1])]
        weight_derivatives=weight_derivatives[::-1]
        for i in range(self.numlayers-1):
            self.biases[i]=self.biases[i]-self.learning_rate*bias_derivatives[i]
            self.weights[i]=self.weights[i]-self.learning_rate*weight_derivatives[i]

    def costDerivative(self):
        CostDerivative=(self.ActivationValues[-1]-self.output)
        return CostDerivative

    def sigmoid(self,x):
        return 1.0/(1.0+np.exp(-x))

    def sigmoid_prime(self,x):
        return self.sigmoid(x)*(1-self.sigmoid(x))

    def evaluate(self):
        #random.shuffle(test_data)
        i=np.empty((10000,784))
        o=np.empty((10000,10))
        for e,(x,y) in enumerate(test_data):
            i[e]=x
            o[e]=y
            #if(e>=99):
            #    break
        x=i
        val=[]

        for a in range(self.numlayers-1):
            val.append(self.sigmoid(np.dot(x,self.weights[a])+(self.biases[a]).T))
            #self.ActivationValues.append(self.sigmoid(self.values[a+1]))
            x=val[a]
        test_results = [np.argmax(e)
                        for e in x]
        o=[np.argmax(e) for e in o]
        #test_results = [(np.argmax(self.feedforward(x)), y)
        #                for (x, y) in test_data]
        #print test_results
        #print o
        #raw_input("yo")
        count=0
        for e,r in zip(test_results,o):
            if(e==r):
                count+=1
        print count,
        #print (sum(1 for e, r in test_results,o if (e==r)))
        print "/10000"
        if count>9400:
            learning_rate=0.2

        #return sum(int(x == y) for (x, y) in test_results)
        #if(count>96):
        #    self.learning_rate=0.3
        #else:
        #    self.learning_rate=0.5
    def SGD(self):

        self.evaluate()
        for a in xrange(self.epochs):
           #Forward Propagation
            #Back Propagation
            print "Epoch",
            print a
            random.shuffle(training_data)
            mini_batches = [
                training_data[k:k+self.mini_batch_size]
                for k in xrange(0, len(training_data), self.mini_batch_size)]
            for mini_batch in mini_batches:

                #print list(enumerate(mini_batch))
                #raw_input(' )')
                for i,(x,y) in enumerate(mini_batch):
                    self.input[i]=x
                    self.output[i]=y

                #print self.input[0]
                #print self.output
                #raw_input('sd')
                #raw_input("before Propagation")
                self.ForwardPropagation()
                self.BackPropagation()

                self.input=np.empty((10,784))
                self.ouput=np.empty((10,10))
                self.values=[]
                self.ActivationValues=[]
                self.errors=[]
            self.evaluate()
            #self.evaluate(test_data)
        with open("Weights and Biases digit recognition.txt", "w") as output:
            output.write(str(self.weights))
            output.write(str(self.biases))



def load_data():
    """Return the MNIST data as a tuple containing the training data,
    the validation data, and the test data.

    The ``training_data`` is returned as a tuple with two entries.
    The first entry contains the actual training images.  This is a
    numpy ndarray with 50,000 entries.  Each entry is, in turn, a
    numpy ndarray with 784 values, representing the 28 * 28 = 784
    pixels in a single MNIST image.

    The second entry in the ``training_data`` tuple is a numpy ndarray
    containing 50,000 entries.  Those entries are just the digit
    values (0...9) for the corresponding images contained in the first
    entry of the tuple.

    The ``validation_data`` and ``test_data`` are similar, except
    each contains only 10,000 images.

    This is a nice data format, but for use in neural networks it's
    helpful to modify the format of the ``training_data`` a little.
    That's done in the wrapper function ``load_data_wrapper()``, see
    below.
    """
    f = gzip.open('C:\Users\Pavitra\Desktop\Scripts\Neural-networks-and-deep-learning-master\Neural-networks-and-deep-learning-master\data\mnist.pkl.gz', 'rb')
    training_data, validation_data, test_data = cPickle.load(f)
    f.close()
    return (training_data, validation_data, test_data)

def load_data_wrapper():
    """Return a tuple containing ``(training_data, validation_data,
    test_data)``. Based on ``load_data``, but the format is more
    convenient for use in our implementation of neural networks.

    In particular, ``training_data`` is a list containing 50,000
    2-tuples ``(x, y)``.  ``x`` is a 784-dimensional numpy.ndarray
    containing the input image.  ``y`` is a 10-dimensional
    numpy.ndarray representing the unit vector corresponding to the
    correct digit for ``x``.

    ``validation_data`` and ``test_data`` are lists containing 10,000
    2-tuples ``(x, y)``.  In each case, ``x`` is a 784-dimensional
    numpy.ndarry containing the input image, and ``y`` is the
    corresponding classification, i.e., the digit values (integers)
    corresponding to ``x``.

    Obviously, this means we're using slightly different formats for
    the training data and the validation / test data.  These formats
    turn out to be the most convenient for use in our neural network
    code."""
    tr_d, va_d, te_d = load_data()
    training_inputs = [x for x in tr_d[0]]
    training_results = [vectorized_result(y) for y in tr_d[1]]
    #training_inputs = [np.reshape(x, (1,784)) for x in tr_d[0]]
    #training_results = [vectorized_result(y) for y in tr_d[1]]
    training_data = zip(training_inputs, training_results)
    validation_inputs = [np.reshape(x, (1,784)) for x in va_d[0]]
    validation_data = zip(validation_inputs, va_d[1])
    test_inputs = [x for x in te_d[0]]
    test_results=[vectorized_result(y) for y in te_d[1]]
    test_data = zip(test_inputs, test_results)
    #test_inputs = [np.reshape(x, (1,784)) for x in te_d[0]]
    #test_data = zip(test_inputs, te_d[1])
    return (training_data,validation_data,test_data)


