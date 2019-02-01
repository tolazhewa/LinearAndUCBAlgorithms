#ifndef ARM_CLASS
#define ARM_CLASS
#include <ctime>
#include <cstdlib>

/**
 * Arm class representing an arm on slot machine with probabilistic characteristics
 **/
class Arm {
    private: 
        // Probability of the arm returning a reward
        double prob;
    public:
        /**
         * Default constructor that regenerates random probability of reward
         **/
        Arm();
        /**
         * Returns the probability of the arm producing a reward
         **/
        double get_prob();
        /**
         * Regenerates the probability associated with this arm
         **/
        void gen_new_prob();
        /**
         * Simulates pulling the arm and returns a 1 (reward) or 0 (no reward)
         **/
        int pull_arm();
};

#endif