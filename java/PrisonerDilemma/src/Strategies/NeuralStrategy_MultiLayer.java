package Strategies;

import java.util.Random;

public class NeuralStrategy_MultiLayer implements IStrategy, ILearningStrategy {

    
    // Neurolal Network Parameters
    private final double learningRate = 0.05;

    // Entry layer -> Hidden layer (2 inputs → 3 neurons)
    private double[][] weightsInputToHidden = new double[2][3]; // [entry][hiddenNeuron]
    private double[] biasHidden = new double[3];

    // Hidden layer → Output (3 neurons → 1 output)
    private double[] weightsHiddenToOutput = new double[3];
    private double biasOutput;

    // Short-term memory
    private boolean lastOpponentMove = true;
    private boolean myLastMove = true;

    private Random random = new Random();

    public NeuralStrategy_MultiLayer() {
        // Random initialization of weights
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                weightsInputToHidden[i][j] = random.nextDouble() * 2 - 1;
            }
        }

        for (int j = 0; j < 3; j++) {
            weightsHiddenToOutput[j] = random.nextDouble() * 2 - 1;
            biasHidden[j] = random.nextDouble() * 2 - 1;
        }

        biasOutput = random.nextDouble() * 2 - 1;
    }

    @Override
    public boolean execute() {
        // Normalized inputs
        double inputOpponent = lastOpponentMove ? 1 : -1;
        double inputSelf = myLastMove ? 1 : -1;

        // Forward pass
        double[] hiddenLayer = new double[3];
        for (int h = 0; h < 3; h++) {
            double sum = weightsInputToHidden[0][h] * inputOpponent +
                         weightsInputToHidden[1][h] * inputSelf + biasHidden[h];
            hiddenLayer[h] = sigmoid(sum);
        }

        double outputSum = 0;
        for (int h = 0; h < 3; h++) {
            outputSum += hiddenLayer[h] * weightsHiddenToOutput[h];
        }
        outputSum += biasOutput;

        double prediction = sigmoid(outputSum);

        // Probabilistic decision
        boolean action = prediction > Math.max(0.4, random.nextDouble());
        myLastMove = action;
        return action;
    }

    @Override
    public void setLastOpponentMove(boolean opponentMove) {
        this.lastOpponentMove = opponentMove;
    }

    @Override
    public String toString() {
        return "NeuralStrategy_MultiLayer";
    }

    @Override
    public void reset() {
        this.lastOpponentMove = true;
        this.myLastMove = true;
    }

    // Train the neural network based on the received reward
    public void learn(double reward) {
        double inputOpponent = lastOpponentMove ? 1 : -1;
        double inputSelf = myLastMove ? 1 : -1;

        // Forward pass to store intermediate values
        double[] hiddenLayer = new double[3];
        double[] hiddenLayerActivated = new double[3];
        for (int h = 0; h < 3; h++) {
            double sum = weightsInputToHidden[0][h] * inputOpponent +
                         weightsInputToHidden[1][h] * inputSelf + biasHidden[h];
            hiddenLayer[h] = sum;
            hiddenLayerActivated[h] = sigmoid(sum);
        }

        double outputSum = 0;
        for (int h = 0; h < 3; h++) {
            outputSum += hiddenLayerActivated[h] * weightsHiddenToOutput[h];
        }
        outputSum += biasOutput;

        double prediction = sigmoid(outputSum);
        double error = reward / 5.0 - prediction;
        double gradientOutput = error * sigmoidDerivative(prediction);

        // Gradients for output layer
        double[] deltaHiddenToOutput = new double[3];
        for (int h = 0; h < 3; h++) {
            deltaHiddenToOutput[h] = gradientOutput * hiddenLayerActivated[h];
        }

        // Gradients for hidden layer
        double[] gradientHidden = new double[3];
        for (int h = 0; h < 3; h++) {
            gradientHidden[h] = gradientOutput * weightsHiddenToOutput[h] * sigmoidDerivative(hiddenLayerActivated[h]);
        }

        // Update weights from hidden layer to output
        for (int h = 0; h < 3; h++) {
            weightsHiddenToOutput[h] += learningRate * deltaHiddenToOutput[h];
        }
        biasOutput += learningRate * gradientOutput;

        // Update weights from input to hidden layer
        for (int i = 0; i < 2; i++) {
            for (int h = 0; h < 3; h++) {
                double input = (i == 0) ? inputOpponent : inputSelf;
                weightsInputToHidden[i][h] += learningRate * gradientHidden[h] * input;
            }
        }

        // Update hidden layer bias
        for (int h = 0; h < 3; h++) {
            biasHidden[h] += learningRate * gradientHidden[h];
        }
    }

    // Sigmoid activation function
    private double sigmoid(double x) {
        return 1.0 / (1.0 + Math.exp(-x));
    }

    // Sigmoid derivative
    private double sigmoidDerivative(double x) {
        return x * (1 - x);
    }
}