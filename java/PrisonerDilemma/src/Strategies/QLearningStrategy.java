package Strategies;

public class QLearningStrategy implements IStrategy, ILearningStrategy {

    // States: 0 = opponent cooperated, 1 = opponent defected
    // Actions: 0 = Cooperate, 1 = Defect
    private double[][] qTable = new double[2][2]; // Q-table initialized to 0

    private final double learningRate = 0.1;      // Learning rate
    private final double discountFactor = 0.9;    // Discount factor
    private final double epsilon = 0.1;           // Exploration probability

    private int lastState = 0;   // Last observed state
    private int lastAction = 0;  // Last action taken

    private boolean opponentLastMove = true; // Initialize as cooperation

    public boolean execute() {
        // Map the opponent's last action to a state (0 or 1)
        int state = opponentLastMove ? 0 : 1;

        // Select action using ε-greedy
        int action;
        if (Math.random() < epsilon) {
            // Exploration: random action
            action = Math.random() < 0.5 ? 0 : 1;
        } else {
            // Exploitation: best action according to Q-table
            action = qTable[state][0] > qTable[state][1] ? 0 : 1;
        }

        // Store current state and action for later update
        this.lastState = state;
        this.lastAction = action;

        return action == 0; // 0 = Cooperate, 1 = Defect
    }

    public void setLastOpponentMove(boolean opponentMove) {
        this.opponentLastMove = opponentMove;
    }

    public String toString() {
        return "QLearning";
    }

    public void reset() {
        // Reset the memory of the last move
        this.opponentLastMove = true;
        this.lastState = 0;
        this.lastAction = 0;
    }

    /**
     * Call this method after each round to update the Q-table
     *
     * @param reward Reward received in this round (0, 1, 3 or 5)
     */
    public void learn(double reward) {
        int nextState = opponentLastMove ? 0 : 1;

        // Best future value (max Q-value of the next state)
        double maxFutureQ = Math.max(qTable[nextState][0], qTable[nextState][1]);

        // Q-Learning update
        qTable[lastState][lastAction] += learningRate * (
                reward + discountFactor * maxFutureQ - qTable[lastState][lastAction]
        );
    }
}