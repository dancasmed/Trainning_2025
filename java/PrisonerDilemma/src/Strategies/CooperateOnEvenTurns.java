package Strategies;

public class CooperateOnEvenTurns implements IStrategy {
        private int turnCounter = 0;

        public boolean execute() {
            turnCounter++;
            return turnCounter % 2 == 1; // Cooperate on odd-numbered turns
        }

        public String toString() {
            return "CooperateOnEvenTurns";
        }

        public void setLastOpponentMove(boolean opponentMove) {
            // No action needed, depends only on turn counter
        }
        public void reset() {
            turnCounter = 0; // Reset turn counter
        }
    }