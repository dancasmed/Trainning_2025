package Strategies;

public class Random implements IStrategy {
    private float EXPLORATION_RATE = 0.5f; // Initial exploration rate

    public boolean execute() {
        // Randomly decide to cooperate or defect
        return Math.random() < EXPLORATION_RATE;
    }

    public String toString() {
        return "Random";
    }
    public void setLastOpponentMove(boolean opponentMove) {
        // No action needed, random strategy does not depend on opponent's last move
    }
    public void reset() {
        // No state to reset, random strategy does not maintain any state
    }
    
}
