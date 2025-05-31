import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import Strategies.IStrategy;
import Strategies.ILearningStrategy;

import Strategies.AlwaysCooperate;
import Strategies.NeverCooperate;
import Strategies.QLearningStrategy;
import Strategies.Random;
import Strategies.RandomOnEvenTurns;
import Strategies.TitForTat;
import Strategies.AlwaysSwitch;
import Strategies.CooperateOnEvenTurns;
import Strategies.CooperateUntilBetrayed;
import Strategies.DefectWithProbability;
import Strategies.NeuralStrategy_SingleLayer;
import Strategies.NeuralStrategy_MultiLayer;

public class PrisonerDilemma {
    public static void main(String[] args) {
        List<IStrategy> strategies = new ArrayList<>();
        Map<String, Integer> globalScores = new HashMap<>();
        Map<String, Integer> totalWins = new HashMap<>();

        // Pre-training neural strategies
        System.out.println("Pre-training neural strategies...");

        IStrategy neuralStrategySingleLayer = new NeuralStrategy_SingleLayer();
        IStrategy neuralStrategyMultiLayer = new NeuralStrategy_MultiLayer();

        pretrainStrategy(neuralStrategySingleLayer);
        pretrainStrategy(neuralStrategyMultiLayer);

        System.out.println("Pre-training completed.\n");

        // Add strategies to the tournament list
        strategies.add(new AlwaysCooperate());
        strategies.add(new NeverCooperate());
        strategies.add(new Random());
        strategies.add(new TitForTat());
        strategies.add(new AlwaysSwitch());
        strategies.add(new CooperateUntilBetrayed());
        strategies.add(new CooperateOnEvenTurns());
        strategies.add(new RandomOnEvenTurns());
        strategies.add(new DefectWithProbability());
        strategies.add(new QLearningStrategy());
        strategies.add(neuralStrategySingleLayer);
        strategies.add(neuralStrategyMultiLayer);

        System.out.println("Starting Prisoner's Dilemma Simulation...");

        int NUMBER_GENERATIONS = strategies.size() - 1; // Number of generations for the evolutionary process
        int NUMBER_TOURNAMENTS = 1000; // Number of tournaments to run
        int NUMBER_ROUNDS = 200; // Number of rounds for each strategy pair

        for (int g = 0; g < NUMBER_GENERATIONS; g++) {
            System.out.println("\n\n=== Generation " + (g + 1) + " ===");
            for (IStrategy iStrategy : strategies) {
                globalScores.put(iStrategy.toString(), 0);
                totalWins.put(iStrategy.toString(), 0);
            }

            for (int t = 0; t < NUMBER_TOURNAMENTS; t++) {
                for (int i = 0; i < strategies.size(); i++) {
                    for (int j = i + 1; j < strategies.size(); j++) {
                        IStrategy strategyA = strategies.get(i);
                        IStrategy strategyB = strategies.get(j);

                        int totalScoreA = 0;
                        int totalScoreB = 0;

                        for (int round = 0; round < NUMBER_ROUNDS; round++) {
                            boolean resultA = strategyA.execute();
                            boolean resultB = strategyB.execute();

                            if (resultA && resultB) {
                                totalScoreA += 3;
                                totalScoreB += 3;
                            } else if (!resultA && !resultB) {
                                totalScoreA += 1;
                                totalScoreB += 1;
                            } else if (resultA && !resultB) {
                                totalScoreA += 0;
                                totalScoreB += 5;
                            } else if (!resultA && resultB) {
                                totalScoreA += 5;
                                totalScoreB += 0;
                            }
                            // Set the last opponent move for each strategy
                            strategyA.setLastOpponentMove(resultB);
                            strategyB.setLastOpponentMove(resultA);
                        }

                        // Store scores in the map
                        globalScores.merge(strategyA.toString(), totalScoreA, Integer::sum);
                        globalScores.merge(strategyB.toString(), totalScoreB, Integer::sum);

                        // Store total wins for each strategy
                        if (totalScoreA > totalScoreB) {
                            totalWins.merge(strategyA.toString(), 1, Integer::sum);
                        } else if (totalScoreA < totalScoreB) {
                            totalWins.merge(strategyB.toString(), 1, Integer::sum);
                        }

                        if (strategyA instanceof QLearningStrategy) {
                            ((QLearningStrategy) strategyA).learn(totalScoreA);
                        }
                        if (strategyB instanceof QLearningStrategy) {
                            ((QLearningStrategy) strategyB).learn(totalScoreB);
                        }
                    }
                }
                // Reset strategies for the next tournament
                for (IStrategy strategy : strategies) {
                    if (!(strategy instanceof ILearningStrategy)) {
                        strategy.reset();
                    }
                }
            }

            System.out.println("Final Global Scores:");
            System.out.println("--------------------------------------------------");

            // Convert to List of Map.Entry
            List<Map.Entry<String, Integer>> globalScoreList = new ArrayList<>(globalScores.entrySet());
            globalScoreList.sort(Map.Entry.<String, Integer>comparingByValue().reversed());

            // Print sorted entries
            for (Map.Entry<String, Integer> entry : globalScoreList) {
                System.out.println(entry.getKey() + " => " + entry.getValue());
            }

            System.out.println("\n\nTotal Wins:");
            System.out.println("--------------------------------------------------");

            List<Map.Entry<String, Integer>> globalWinList = new ArrayList<>(totalWins.entrySet());
            globalWinList.sort(Map.Entry.<String, Integer>comparingByValue().reversed());

            // Print sorted entries
            for (Map.Entry<String, Integer> entry : globalWinList) {
                System.out.println(entry.getKey() + " => " + entry.getValue());
            }

            Map<String, Double> combinedScore = new HashMap<>();
            for (String name : globalScores.keySet()) {
                double score = globalScores.get(name);
                double wins = totalWins.getOrDefault(name, 0);
                combinedScore.put(name, 0.7 * score + 0.3 * wins);
            }

            Map.Entry<String, Double> worst = combinedScore.entrySet()
                    .stream()
                    .min(Map.Entry.comparingByValue())
                    .get();

            // Remove the worst strategy from the global scores and total wins
            globalScores.remove(worst.getKey());
            totalWins.remove(worst.getKey());
            strategies.removeIf(strategy -> strategy.toString().equals(worst.getKey()));

            System.out.println("\n\nEliminating worst strategy: " + worst.getKey());
            System.out.println("--------------------------------------------------");

        }

        System.out.println("End of Prisoner's Dilemma Simulation");
    }

    private static void pretrainStrategy(IStrategy strategy) {
        IStrategy[] opponents = {
                new TitForTat(),
                new AlwaysCooperate(),
                new NeverCooperate(),
                new CooperateUntilBetrayed()
        };

        for (IStrategy opponent : opponents) {
            for (int i = 0; i < 10000; i++) {
                boolean actionMe = strategy.execute();
                boolean actionOpponent = opponent.execute();

                int reward;
                if (actionMe && actionOpponent)
                    reward = 3;
                else if (!actionMe && !actionOpponent)
                    reward = 1;
                else if (actionMe && !actionOpponent)
                    reward = 0;
                else
                    reward = 5;

                ((ILearningStrategy) strategy).learn(reward);

                strategy.setLastOpponentMove(actionOpponent);
                opponent.setLastOpponentMove(actionMe);
            }
        }
    }
}
