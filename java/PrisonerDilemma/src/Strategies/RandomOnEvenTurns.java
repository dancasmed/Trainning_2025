package Strategies;

public class RandomOnEvenTurns implements IStrategy {
        private int turnCounter = 0;

        public boolean execute() {
            turnCounter++;
            return (turnCounter % 2 == 0 && Math.random() < 0.5); // Random on even turns
        }

        public String toString() {
            return "RandomOnEvenTurns";
        }

        public void setLastOpponentMove(boolean opponentMove) {
            // No action needed, depends only on turn counter and randomization
        }
        public void reset() {
            turnCounter = 0; // Reset turn counter
        }
    }