#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

#include "Environment.hpp"
#include "Arm.hpp"
#include "LRAgent.hpp"

#define COL_WIDTH std::setw(10) // Formatting support for printing the statistics
#define COL_WIDTH_2 std::setw(12) // To align numerical values with their heading
#define SIDE_OFFSET std::setw(10) // Side offsets for stats


void print_stats(std::vector<std::vector<std::vector<double>>> ab_pairs
                ,std::vector<std::vector<std::vector<double>>> lrp_results 
                ,std::vector<std::vector<double>> lri_results, 
                int size_of_cons_val, std::string stats_file_name);

/**
 * Main function that executes the program
 **/
int main(){

    //-------------------------------------------------------------------------------------------------------//
    //-------------------------------------------------------------------------------------------------------//

    // Dump and stats file names
    std::string dump_file_name = "q2.out";
    std::string stats_file_name = "q2_stats";

    // Should we collect stats?
    bool collect_stats = true;

    // Should we collect iteration data?
    bool collect_iter_data = true;

    // How often (once every how many iterations) should we print stats of the iteration?
    int print_freq = 100;

    // Number of arms the slot machine should have
    int num_of_arms = 10;

    // Array of considered alpha and beta values
    // To test a wide variety of values, use { 0.01, 0.02, 0.05, 0.1, 0.3, 0.5, 0.7, 0.9, 0.95, 0.99 };
    std::vector<double> cons_val = { 0.1 };

    // Number of environments used
    int num_of_envs = 100; 

    // Number of Iterations
    int num_of_iters = 5000;

    // The full number of executions will be cons_val.size()^2 * num_of_envs * num_of_iters

    //------------------------------------DO NOT MODIFY BEYOND THIS POINT------------------------------------//
    //-------------------------------------------------------------------------------------------------------//


    srand(time(NULL));

    // Files to write to (dump file and stats file)
    std::ofstream dump_file;

    if (collect_iter_data){
        // Open the dump file
        dump_file.open(dump_file_name, std::ofstream::trunc);

        // Set up the output style preferred
        dump_file << std::fixed;

        //sets the printing double precision to 2 decimal points
        dump_file << std::setprecision(2); 
    }
    
    // Temporary values to use for getting averages and other operations
    double lrp_point_avg, lrp_optm_avg, lri_optm_avg, lri_point_avg, curr_alpha, curr_beta, lri_tmp_o, lri_tmp_p;

    // Environment Variable to represent the current environment
    Environment curr_env;

    // L(r-p) agent
    LRAgent lrp(curr_env, "L(r-p)", 10); 

    // L(r-i) agent
    LRAgent lri(curr_env, "L(r-i)", 10, 0); 

    // Size of considered values
    int size_of_cons_val = cons_val.size();

    // Array of possible combinations of alpha and beta values
    std::vector<std::vector<std::vector<double>>> ab_pairs;

    // Fill out the possible permutations
    for(int i = 0; i < size_of_cons_val; i++) {
        std::vector<std::vector<double>> v1;
        for(int j = 0; j < size_of_cons_val; j++){
            std::vector<double> v;
            v.push_back(cons_val[i]);
            v.push_back(cons_val[j]);
            v1.push_back(v);
        }
        ab_pairs.push_back(v1);
    }

    // Array containing the results (% optimal arm chosen, % reward collected) for the l(r-p) algorithm
    std::vector<std::vector<std::vector<double>>> lrp_results;
    for(int i = 0; i < size_of_cons_val; i++) {
        std::vector<std::vector<double>> v1;
        for(int j = 0; j < size_of_cons_val; j++){
            std::vector<double> v;
            v1.push_back(v);
        }
        lrp_results.push_back(v1);
    }
    
    // Array containing the results (% optimal arm chosen, % reward collected) for the l(r-i) algorithm
    std::vector<std::vector<double>> lri_results;
    for(int i = 0; i < size_of_cons_val; i++) {
        std::vector<double> v1;
        lri_results.push_back(v1);
    }

    // For each alpha value
    for (int alpha_index = 0; alpha_index < size_of_cons_val; alpha_index++) {
        // For each beta value
        for (int beta_index = 0; beta_index < size_of_cons_val; beta_index++) {
            // Reset the variables 
            lrp_point_avg = 0, lri_point_avg = 0, lrp_optm_avg = 0; lri_optm_avg = 0;
            lri_tmp_o = 0, lri_tmp_p = 0;

            // Get the current values from our array
            curr_alpha = ab_pairs[alpha_index][beta_index][0];
            curr_beta = ab_pairs[alpha_index][beta_index][1];

            // Iterate number of environments indicated times
            for (int env_count = 0; env_count < num_of_envs; env_count++) {
                // Create a new environment with the indicated number of arms
                curr_env = Environment(num_of_arms);

                // Change the parameters of the agents to accomodate for the current configuration
                lrp.change_parameters(curr_env, curr_alpha, curr_beta);
                lri.change_parameters(curr_env, curr_alpha, 0);

                for (int iter_num = 1; iter_num <= num_of_iters; iter_num++) {
                    // Execute an iteration (round) for both agents
                    lrp.exec_round();
                    lri.exec_round();

                    // Print out once very print_freq number of times
                    if (iter_num % print_freq == 0 && collect_iter_data) {
                        lrp.print_agent_stats(dump_file);
                        lri.print_agent_stats(dump_file);
                    }
                }

                lrp_point_avg += lrp.get_reward_percent();
                lrp_optm_avg += lrp.get_optm_percent();

                lri_point_avg += lri.get_reward_percent();
                lri_optm_avg += lri.get_optm_percent();
            }

            // Get the average points percent for L(r-p) (divide by n)
            lrp_point_avg /= num_of_envs; 
            // Get the average percentage of time optimal arm was chosen for L(r-p)
            lrp_optm_avg /= num_of_envs; 

            // Get the average points percent for L(r-i) (divide by n)
            lri_point_avg /= num_of_envs; 
            // Get the average percentage of time optimal arm was chosen for L(r-i)
            lri_optm_avg /= num_of_envs; 

            // Store results in the results arrays
            lrp_results[alpha_index][beta_index].push_back(lrp_optm_avg); 
            lrp_results[alpha_index][beta_index].push_back(lrp_point_avg);
            
            lri_tmp_o += lri_optm_avg;
            lri_tmp_p += lri_point_avg;
        }

        lri_results[alpha_index].push_back(lri_tmp_o);
        lri_results[alpha_index].push_back(lri_tmp_p);
    }
    
    if (collect_iter_data){
        dump_file.close();
    }

    if (collect_stats) {
        print_stats(ab_pairs, lrp_results, lri_results, size_of_cons_val, stats_file_name);
    }
    return 0;
}
/**
 * Prints all statistics to file
 **/
void print_stats(std::vector<std::vector<std::vector<double>>> ab_pairs
                ,std::vector<std::vector<std::vector<double>>> lrp_results 
                ,std::vector<std::vector<double>> lri_results, 
                int size_of_cons_val, std::string stats_file_name){
    // Open the stats file
    std::ofstream stats_file;
    stats_file.open(stats_file_name, std::ofstream::trunc);

    // Set up the output style preferred
    stats_file << std::fixed;
    
    //sets the printing double precision to 2 decimal points
    stats_file << std::setprecision(2); 

    // Prints the header for our L(r-p)
    stats_file << "L(r-p) Statistics\n\n";
    stats_file << SIDE_OFFSET << "alpha" << COL_WIDTH << "beta" << COL_WIDTH_2
              << "% Optimal" << COL_WIDTH << "% Reward" << std::endl;
    stats_file << "----------------------------------------------------------------------------------------------------" << std::endl;
    
    // Print all the values
    // For each alpha value
    for (int alpha_index = 0; alpha_index < size_of_cons_val; alpha_index++) {
        // For each beta value
        for (int beta_index = 0; beta_index < size_of_cons_val; beta_index++) {
            // Print out the values
            stats_file << SIDE_OFFSET << ab_pairs[alpha_index][beta_index][0] 
                      << COL_WIDTH << ab_pairs[alpha_index][beta_index][1] 
                      << COL_WIDTH << lrp_results[alpha_index][beta_index][0] * 100 << "%"
                      << COL_WIDTH << lrp_results[alpha_index][beta_index][1] * 100 << "%" << std::endl;
        }
    }
    stats_file << "----------------------------------------------------------------------------------------------------" << std::endl;

    // Prints the header for our L(r-i)
    stats_file << "\n\nL(r-i) Statistics\n\n";
    stats_file << SIDE_OFFSET << "alpha" << COL_WIDTH_2 << "% Optimal" << COL_WIDTH
              << "% Reward" << std::endl;
    stats_file << "----------------------------------------------------------------------------------------------------" << std::endl;
    // Print all the values
    for (int alpha_index = 0; alpha_index < size_of_cons_val; alpha_index++) {
        stats_file << SIDE_OFFSET << ab_pairs[alpha_index][0][0] 
                  << COL_WIDTH << lri_results[alpha_index][0] * 100 << "%"
                  << COL_WIDTH << lri_results[alpha_index][1] * 100 << "%" << std::endl;
    }
    stats_file << "----------------------------------------------------------------------------------------------------" << std::endl;
    stats_file.close();
}