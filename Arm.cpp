#include "Arm.hpp"

/**
 * Default constructor that regenerates random probability of reward
 **/
Arm::Arm(){ gen_new_prob(); }
/**
 * Returns the probability of the arm producing a reward
 **/
double Arm::get_prob(){ return prob; }
/**
 * Regenerates the probability associated with this arm
 **/
void Arm::gen_new_prob(){ prob = rand()/double(RAND_MAX); }
/**
 * Simulates pulling the arm and returns a 1 (reward) or 0 (no reward)
 **/
int Arm::pull_arm(){ 
    return (rand()/double(RAND_MAX) <= prob) ? 1 : 0; 
}