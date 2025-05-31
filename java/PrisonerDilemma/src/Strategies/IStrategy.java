package Strategies;

public interface IStrategy {
    boolean execute();
    String toString();
    void setLastOpponentMove(boolean opponentMove);
    void reset();
}
