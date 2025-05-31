package Strategies;

public class CooperateUntilBetrayed implements IStrategy {
        private boolean hasBeenBetrayed = false;

        public boolean execute() {
            if (hasBeenBetrayed) return false; // Defect after being betrayed
            return true; // Cooperate initially
        }

        public String toString() {
            return "CooperateUntilBetrayed";
        }

        public void setLastOpponentMove(boolean opponentMove) {
            if (!opponentMove) hasBeenBetrayed = true;
        }
        public void reset() {
            hasBeenBetrayed = false; // Reset state for a new game
        }
    }
