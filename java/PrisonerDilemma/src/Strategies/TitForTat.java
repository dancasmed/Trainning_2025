package Strategies;

public class TitForTat implements IStrategy {

    private boolean lastOpponentMove = true; // Assume opponent cooperates first

    public boolean execute() {
        boolean currentMove = lastOpponentMove;
        return currentMove; // Copy opponent's last move
    }

    public String toString() {
        return "TitForTat";
    }

    public void setLastOpponentMove(boolean opponentMove) {
        this.lastOpponentMove = opponentMove; // Update last opponent's move
    }

    public void reset() {
        lastOpponentMove = true; // Reset to initial state (assume opponent cooperates)
    }

}