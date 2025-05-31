package Strategies;

public class AlwaysCooperate implements IStrategy {   
    public boolean execute() {
        // Always cooperate
        return true;
    }

    public String toString() {
        return "AlwaysCooperate";
    }
    public void setLastOpponentMove(boolean opponentMove) {
        // No action needed, always cooperates
    }
    public void reset() {
        // No state to reset, always cooperates
    }
    
}
