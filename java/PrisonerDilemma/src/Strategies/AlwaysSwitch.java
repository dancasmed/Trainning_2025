package Strategies;

public class AlwaysSwitch implements IStrategy {
    private boolean myLastMove = true;

    public boolean execute() {
        myLastMove = !myLastMove;
        return myLastMove;
    }

    public String toString() {
        return "AlwaysSwitch";
    }

    public void setLastOpponentMove(boolean opponentMove) {
        // No action needed, strategy always switches
    }
    public void reset() {
        myLastMove = true; // Reset to initial state
    }
}
