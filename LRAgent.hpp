#ifndef LRAGENT_CLASS
#define LRAGENT_CLASS

#include <vector>
#include <iomanip>
#include "Environment.hpp"
#define NUM_SPACE std::setw(5) // Formatting support for printing an array


/**
 * Agent class representing an agent who is playing the slot machine
 **/
class LRAgent {
    private:
        // Probability of agent picking each arm
        std::vector<double> arm_probs;
        // Cumulative points
        int points, optm_chosen , iter_number;
        // Alpha and Beta variables from the L(r-p) and L(r-i) functions
        double alpha, beta;
        // Current Environment
        Environment curr_env;
        // Label for the type of algorithm
        std::string label;

    public:
        /**
         * Default constructor
         **/
        LRAgent(Environment env, std::string l, double beta = 0.1, double alpha = 0.1);

        /**
         * returns the cumulative rewards collected
         **/
        int get_points();

        /**
         * Prints the agent's probabilities to choose each arm
         **/
        void print_arm_sel_probs(std::ostream &file = std::cout);
        
        /**
         * Chooses an arm to pull based on the arm probabilities
         **/
        int choose_arm();

        /**
         * Executes a single round of game and updates arm probabilities
         **/
        void exec_round();

        /**
         * Return the percentage of iterations where reward was received
         **/
        double get_reward_percent();

        /**
         * Return the percentage of iterations where optimal arm was chosen
         **/
        double get_optm_percent();

        /**
         * Prints the agent's statistics
         **/
        void print_agent_stats(std::ostream &file = std::cout);
        /**
         * Resets the agent variables and produces a new set of arm probabilities
         **/
        void change_parameters(Environment &env, double a, double b);
};

#endif