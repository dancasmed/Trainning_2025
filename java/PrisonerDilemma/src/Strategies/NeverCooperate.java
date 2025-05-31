package Strategies;

public class NeverCooperate implements IStrategy {

    public boolean execute() {
        // Never cooperate
        return false;
    }

    public String toString() {
        return "NeverCooperate";
    }
    public void setLastOpponentMove(boolean opponentMove) {
        // No action needed, never cooperates
    }
    public void reset() {
        // No state to reset, never cooperates
    }
    
}
