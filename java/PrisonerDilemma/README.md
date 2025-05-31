# Prisoner's Dilemma Simulator with Evolutionary Strategies

A Java-based simulation of the **Iterated Prisoner's Dilemma**, featuring classical strategies, reinforcement learning, and neural network-based approaches. The system simulates an **evolutionary tournament**, where the weakest strategy is eliminated each generation, mimicking natural selection in strategic behavior.

This project was inspired by Robert Axelrod’s research on cooperation and competition in repeated games, and explores how cooperation can emerge even among self-interested agents — just like in nature, diplomacy, or everyday life.

---

## 🔍 What is the Prisoner's Dilemma?

The **Prisoner's Dilemma** is a classic problem in Game Theory that illustrates why two rational individuals might not cooperate, even if it appears to be in their best interest to do so.

In this simulation:
- Two players choose to either **cooperate** or **defect**
- Payoffs are defined as:
  - Both cooperate → 3 points each
  - Both defect → 1 point each
  - One cooperates, one defects → Defector gets 5 points, cooperator gets 0

### 🧠 Why It Matters
When players are forced to interact repeatedly (not just once), cooperation becomes **not only possible but optimal**.

This mirrors real-life scenarios such as:
- International relations and arms races
- Environmental agreements between countries
- Daily interactions like sharing chores or business deals
- Biological systems like mutual grooming in impalas or cleaner fish cleaning sharks

---

## 🧬 Best Performing Strategies

From Axelrod’s tournaments and our own simulations, we observe that successful strategies share **four key characteristics**:

| Characteristic | Description |
|----------------|-------------|
| ✅ **Nice** | Never defects first; starts with cooperation |
| 💡 **Forgiving** | Retaliates when betrayed, but forgives quickly if opponent changes |
| 🔥 **Retaliatory** | Doesn’t let betrayal go unpunished |
| 🧮 **Clear** | Has predictable behavior that others can adapt to |

These traits make **TitForTat** the most famous winning strategy:  
It begins by cooperating and then copies its opponent’s last move.

Axelrod found that **nice strategies dominate** over time in evolutionary settings — even when surrounded by selfish actors.

---

## ⚙️ How It Works

### Simulation Flow
1. Run multiple **tournaments** per generation.
2. Each pair of strategies plays a match of many rounds.
3. Track global scores and wins.
4. At the end of each generation:
   - Eliminate the worst-performing strategy.
   - Repeat until one strategy remains.

### Key Features
- ✅ Support for **learning strategies**: Q-Learning & Neural Networks
- ✅ Evolutionary elimination based on combined score (`globalScore + wins`)
- ✅ Reset mechanism that preserves learned behavior for adaptive strategies
- ✅ Customizable parameters:
  - Number of tournaments per generation
  - Number of rounds per match
  - Number of generations

---

## 📦 Project Structure

```
/src
    /Strategies
        AlwaysCooperate.java
        AlwaysSwitch.java
        CooperateOnEvenTurns.java
        CooperateUntilBetrayed.java
        DefectWithProbability.java
        ILearningStrategy.java
        IStrategy.java
        NeuralStrategy_SingleLayer.java
        NeuralStrategy_MultiLayer.java
        NeverCooperate.java
        QLearningStrategy.java
        Random.java
        RandomOnEvenTurns.java
        TitForTat.java
        
PrisonerDilemma.java
README.md
```

---

## 🏁 Getting Started

### Prerequisites
- Java 8+

### Running the Simulation
```bash
javac -d bin src/PrisonerDilemma.java src/**/*.java
java -cp bin PrisonerDilemma
```

Or use your favorite IDE (IntelliJ, Eclipse, etc.)

---

## 📈Results and Insights 

The simulation shows that: 

   - Simple strategies like TitForTat often outperform complex ones 
   - Cooperation emerges naturally when interactions are repeated
   - Being "nice", "forgiving", "retaliatory", and "clear" leads to long-term success
   - Neural strategies show potential but require training

---     

## 🧪 Training Neural Strategies 

Neural strategies are pre-trained against strong baseline opponents: 

    - TitForTat
    - AlwaysCooperate
    - NeverCooperate
    - CooperateUntilBetrayed
     

They also support in-tournament learning , though current results show better performance when this is disabled initially. 

---

## 🤖 Implemented Strategies 

Each strategy follows a unique decision-making logic: 

| Strategy                    | Description                                      |
|----------------------------|--------------------------------------------------|
| AlwaysCooperate            | Always cooperates                                |
| NeverCooperate             | Always defects                                   |
| Random                     | Randomly chooses action with equal probability   |
| RandomOnEvenTurns          | Cooperates randomly on even-numbered turns      |
| DefectWithProbability      | Defects with a fixed probability                 |
| TitForTat                  | Starts with cooperation, then mirrors opponent's last move |
| CooperateUntilBetrayed     | Cooperates until betrayed, then always defects  |
| CooperateOnEvenTurns       | Cooperates only on odd-numbered turns           |
| AlwaysSwitch               | Alternates between cooperation and defection every round |
| QLearningStrategy          | Learns optimal actions using Q-Learning algorithm |
| NeuralStrategy_SingleLayer | Simple feedforward neural net with sigmoid activation |
| NeuralStrategy_MultiLayer  | Multilayer perceptron with hidden layer and backpropagation |

---

## 📊 Simulation on screen Results

```bash
Pre-training neural strategies...
Pre-training completed.

Starting Prisoner's Dilemma Simulation...


=== Generation 1 ===
Final Global Scores:
--------------------------------------------------
NeverCooperate => 4887272
QLearning => 4610779
CooperateUntilBetrayed => 4500804
NeuralStrategy_MultiLayer => 4471029
RandomOnEvenTurns => 4430308
DefectWithProbability => 4269420
NeuralStrategy_SingleLayer => 4132006
TitForTat => 4109244
CooperateOnEvenTurns => 4102360
AlwaysSwitch => 4047909
Random => 3827427
AlwaysCooperate => 3207657


Total Wins:
--------------------------------------------------
NeverCooperate => 10378
CooperateUntilBetrayed => 8636
NeuralStrategy_MultiLayer => 8450
QLearning => 6611
RandomOnEvenTurns => 6021
DefectWithProbability => 5553
NeuralStrategy_SingleLayer => 2734
Random => 2270
TitForTat => 2149
CooperateOnEvenTurns => 2004
AlwaysSwitch => 1938
AlwaysCooperate => 0


Eliminating worst strategy: AlwaysCooperate
--------------------------------------------------
```

## 🌐 Real-World Applications

The insights from this simulation extend far beyond theoretical game theory.

### 🕊️ Diplomacy and International Relations
Just like in the Cold War arms race, nations face a version of the prisoner's dilemma:
- Build more weapons = short-term gain
- Mutual disarmament = long-term peace
But without trust, both sides keep building arms.

**Repeated interaction allows cooperation to arise**, just as U.S. and USSR did in the 1980s through gradual disarmament and verification.

### 🦌 Biology and Evolution
Impalas groom each other to remove ticks, but it costs them saliva, time, and attention — resources they could use for survival.

Yet, they still cooperate because:
- They see each other again tomorrow
- If one stops grooming, the other retaliates
- Cooperation pays off in the long run

### 🧬 Evolution of Cooperation
Robert Axelrod famously demonstrated that cooperation can emerge among purely self-interested agents, especially through repeated interactions using strategies like Tit-for-Tat.

In the final generation of each simulation in this project, we often observe results like the following, strong evidence that AI can learn to cooperate without relying on exploitative or abusive tactics:
```bash
=== Generation 11 ===
Final Global Scores:
--------------------------------------------------
NeuralStrategy_SingleLayer => 482183
TitForTat => 479658
```

This suggests that cooperation doesn't require altruism — just smart rules encoded in DNA or learned behavior.

---

## 🚀 What's Next? (Features to Add)

Here are some powerful enhancements to improve the project and highlight its social relevance:

### 📊 1. Add Graphical Visualization
- Plot evolution of scores over generations
- Show dominance of strategies visually
- Use libraries like:
  - **JFreeChart** (Java)
  - **Matplotlib** (Python post-processing)
  - **Plotly** or **Seaborn** (for web dashboards)

### 📁 2. Export Data to CSV/JSON
Allow users to:
- Save final results
- Analyze trends
- Compare different runs

This makes the simulator useful for educational purposes or academic research.

### 🧬 3. Introduce Noise and Forgiveness
Simulate real-world uncertainty:
- Sometimes a cooperative move is misinterpreted as betrayal
- Implement probabilistic forgiveness to recover from errors

This reflects real-life situations like diplomatic misunderstandings.

### 🧬 4. Genetic Algorithms and Mutation
Let surviving strategies mutate slightly to create new variants.
- Simulate true evolution
- See how new strategies adapt to changing environments

### 🧠 5. Deep Q-Networks (DQN)
Replace simple Q-learning with deep RL:
- Learn from long sequences of moves
- Use memory buffers and neural networks

### 🎯 6. Reinforcement Learning Improvements
Fine-tune hyperparameters:
- Epsilon decay for exploration/exploitation trade-off
- Reward shaping for long-term planning
- Experience replay for deep Q-learning

---

## 📚 Credits & References

This project draws inspiration from:
- Robert Axelrod’s *The Evolution of Cooperation*
- Anatol Rapoport’s TitForTat strategy
- Game theory and evolutionary simulations

Special thanks to Axelrod’s work that shows how cooperation can emerge without trust or morality — just repeated interaction and clear rules.

---

## 🛡 License

MIT License — feel free to use, modify, and distribute!

---

## 📬 Feedback & Contributions

Have ideas for new strategies or improvements? Open an issue or submit a PR!