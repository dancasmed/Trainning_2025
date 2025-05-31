package Strategies;

import java.util.Random;

public class DefectWithProbability implements IStrategy {
    private float DEFECT_PROBEBILITY = 0.3f; // Default defect probability, 30%
    private final Random random = new Random();

    public boolean execute() {
        return random.nextDouble() < DEFECT_PROBEBILITY;
    }

    public String toString() {
        return "DefectWithProbability";
    }

    public void setLastOpponentMove(boolean opponentMove) {
        // No action needed, decision is random and independent of the move
    }

    public void reset() {
        // No state to reset, decision is always random
    }
}