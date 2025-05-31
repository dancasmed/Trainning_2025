package Strategies;

import java.util.Random;

public class NeuralStrategy_SingleLayer implements IStrategy, ILearningStrategy {

private double explorationRate = 0.4; // Initial exploration rate
private final double MIN_EXPLORATION_RATE = 0.05;
private final double EXPLORATION_DECAY = 0.9998;

    // Weights of the neuron (now two inputs)
    private double weightOpponent;
    private double weightSelf;
    private double bias;
    private final double learningRate = 0.05;

    // Short-term memory
    private boolean lastOpponentMove = true;
    private boolean myLastMove = true;

    private Random random = new Random();

    public NeuralStrategy_SingleLayer() {
        this.weightOpponent = random.nextDouble() * 2 - 1; // [-1, 1]
        this.weightSelf = random.nextDouble() * 2 - 1;
        this.bias = random.nextDouble() * 2 - 1;
    }

    @Override
    public boolean execute() {
        double inputOpponent = lastOpponentMove ? 1 : -1;
    double inputSelf = myLastMove ? 1 : -1;

    double output = weightOpponent * inputOpponent + weightSelf * inputSelf + bias;
    double prediction = sigmoid(output);

    // Decision with controlled exploration
    boolean action = random.nextDouble() < (explorationRate > prediction ? Math.max(explorationRate, prediction) : prediction);
    myLastMove = action;

    // Reduce exploration with each call
    explorationRate = Math.max(MIN_EXPLORATION_RATE, explorationRate * EXPLORATION_DECAY);

    return action;
    }

    @Override
    public void setLastOpponentMove(boolean opponentMove) {
        this.lastOpponentMove = opponentMove;
    }

    @Override
    public String toString() {
        return "NeuralStrategy_SingleLayer";
    }

    @Override
    public void reset() {
        this.lastOpponentMove = true;
        this.myLastMove = true;
    }
    
    // Train the network based on the received reward.
     
    public void learn(double reward) {
        double inputOpponent = lastOpponentMove ? 1 : -1;
        double inputSelf = myLastMove ? 1 : -1;

        double netInput = weightOpponent * inputOpponent + weightSelf * inputSelf + bias;
        double prediction = sigmoid(netInput);

        double error = reward / 5.0 - prediction;
        double gradient = error * sigmoidDerivative(prediction);

        // Update both weights and bias
        weightOpponent += learningRate * gradient * inputOpponent;
        weightSelf += learningRate * gradient * inputSelf;
        bias += learningRate * gradient;
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