#ifndef LRAGENT_CLASS
#define LRAGENT_CLASS

#include <vector>
#include <iomanip>
#include "Environment.hpp"
#include <cmath>
#define NUM_SPACE std::setw(5) // Formatting support for printing an array


/**
 * Agent class representing an agent who is playing the slot machine
 **/
class UCBAgent {
    private:
        // Estimated probability of each arm producing a reward
        std::vector<double> est_arm_reward_prob;
        // Number of times arm was pulled
        std::vector<int> times_arm_pulled;
        // Cumulative points
        int points, optm_chosen, iter_number;
        // Upper Confidence Value c
        double c;
        // Current Environment
        Environment curr_env;
        // Label for the type of algorithm
        std::string label;

        /**
         * Returns the index of the element with the highest value in a vector containing doubles
         **/
        int argmax(std::vector<double> vec);

    public:
        /**
         * Default constructor
         **/
        UCBAgent(Environment env, std::string l, double conf = 2);

        /**
         * returns the cumulative rewards collected
         **/
        int get_points();

        /**
         * Prints the agent's probabilities to choose each arm
         **/
        void print_est_arm_reward_probs(std::ostream &file = std::cout);
        
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
        void change_parameters(Environment &env, double conf);
};

#endif