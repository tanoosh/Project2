#include <fstream>
#include<bits/stdc++.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

//go back and fill out later
double get_distance(const std::vector<std::string>& file, const std::string object_to_classify, const std::string object_to_check, const int num_features, const std::vector<int>& feature_set){
	int burner;
	double val1;
	double val2;
	double sum = 0;
	std::stringstream ss_1(object_to_classify); ss_1 >> burner; //ignore first col
	std::stringstream ss_2(object_to_check); ss_2 >> burner; //ignore first col

	for (unsigned int i = 0; i <= num_features; i++){
		ss_1 >> val1;
		ss_2 >> val2;
		if (std::find(feature_set.begin(), feature_set.end(), i) != feature_set.end()) { //only for features in the feature set
		//ouput check
			//std::cout << "\tFor feature " << i << "\tValue 1: " << val1 << "\tValue 2: " << val2 << std::endl;
			sum += pow((val2-val1), 2.0);
		}
	}

//	std::cout << "\n\tdistance: " << sqrt(sum) << std::endl;
	return sqrt(sum);
}

double cross_validation(const std::vector<std::string>& file, const std::vector<int>& current_set, const int feature_to_add, const int num_features, const int feature_to_remove){
	//create set of all features (including feature to add)
	std::vector<int>feature_set;

	if (feature_to_add != 0){
		feature_set = current_set;
		feature_set.push_back(feature_to_add);
	}
	else {
		for (int feat : current_set){
			if (feat != feature_to_remove){
				feature_set.push_back(feat);
			}
		}
	}
	//output check
	
	/*	
		std::cout << " under feature set{";
		for (int feat : feature_set){
			std::cout << feat << ", ";
		}
		std::cout << "}";
	*/	
	

	int number_correctly_classified = 0;
	
	//looop through all data, classify each
	for (unsigned int i = 0; i < file.size(); i++){
		std::string object_to_classify = file.at(i);
		int label_object_to_classify;

		std::stringstream ss(object_to_classify);
		ss >> label_object_to_classify;

		double nearest_neighbor_distance = std::numeric_limits<double>::infinity(); 
		int nearest_neighbor_location; 
		int nearest_neighbor_label;

		for (unsigned int k = 0; k < file.size(); k++){
			if (k!=i){
				//output check
				//std::cout << "\nComparing object " << i << " AND " << k << std::endl;
				double distance = get_distance(file, object_to_classify, file.at(k), num_features, feature_set);
				
				//output check
				//std::cout << "\n\tDistance: " << distance << std::endl;
				if (distance < nearest_neighbor_distance) {
					nearest_neighbor_distance = distance;
					nearest_neighbor_location = k;

					std::stringstream ss_1(file.at(nearest_neighbor_location));
					ss_1 >> nearest_neighbor_label;
				}
			}
		}
		
		//output check
		//	std::cout << "Object " << i << " is class " << label_object_to_classify << std::endl;
		//	std::cout << "Its nearest neighbor is " << nearest_neighbor_location << " of class " << nearest_neighbor_label << std::endl;
		
		//update if object is labelled correctly	
		if (label_object_to_classify == nearest_neighbor_label) {
			number_correctly_classified++;
		} 
	}

	//std::cout<< " number correctly classified: " << number_correctly_classified << std::endl;
	double accuracy = number_correctly_classified/(double)file.size();
	return accuracy*100.0;
}

void backwards (const std::vector<std::string>& file, const int num_features){
	/******************Initialize******************/
	std::vector<int> current_set_of_features;
	for (unsigned int i = 1; i <= num_features; i++){
		current_set_of_features.push_back(i);
	}

	int feature_to_remove_at_this_level;
	std::vector<int>::iterator location_of_feature;

	//loop through all features
	for (unsigned int i = 0; i< num_features; i++){

	double worst_so_far_accuracy = std::numeric_limits<double>::infinity(); 
	//std::cout << "On the " << i << "th level of the search tree" << std::endl;
		
		std::cout << "Base Case accuracy: " << cross_validation(file, current_set_of_features, 0, num_features, 0) << std::endl; 
		//for all remaining features, find worst one
		for (std::vector<int>::iterator it1 = current_set_of_features.begin(); it1 != current_set_of_features.end(); it1++){
			//std::cout << "Considering removing the " << *it1 << "th feature";

			double accuracy = cross_validation(file, current_set_of_features, 0, num_features, *it1);

			//std::cout << " with accuracy " << accuracy << std::endl;

			if (accuracy < worst_so_far_accuracy){
				location_of_feature = it1;
				feature_to_remove_at_this_level = *it1;
				worst_so_far_accuracy = accuracy;
			}
		}

		current_set_of_features.erase(location_of_feature);

		std::cout << "On level " << i << ", removed feature " << feature_to_remove_at_this_level << " with " << worst_so_far_accuracy << "\% acc" << std::endl; 
	}
}

void feature_search(const std::vector<std::string>& file, const int num_features){
	std::vector<int> current_set_of_features; //initialize empty set


	for (unsigned int i = 1; i <= num_features; i++){
		//output check
		//std::cout << "On the " << i << "th level of the search tree" << std::endl;

		//initialzie
		int feature_to_add_at_this_level;
		double best_so_far_accuracy = 0.0;

		for (unsigned int k = 1; k <= num_features; k++){
			if (std::find(current_set_of_features.begin(), current_set_of_features.end(), k) == current_set_of_features.end()){ //if feature not in set
				//output check
				//std::cout << "\tConsidering adding the " << k << "th feature";
				
				//get accuracy
				double accuracy = cross_validation(file, current_set_of_features, k, num_features, 0);
				//double accuracy = rand() % 100;
				
				//more output check
			//	std::cout << " \twith " << accuracy << "\% accuracy" << std::endl;	

				//if accuracy of adding this feature is better than best so far, then update
				if (accuracy > best_so_far_accuracy){
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = k;
				}
			}
		}
		
		current_set_of_features.push_back(feature_to_add_at_this_level);

		//output check
		std::cout << "On level " << i << ", added feature " << feature_to_add_at_this_level << " with " << best_so_far_accuracy << "\% accuracy\n" << std::endl;
	

	}
}

int main(){
	//initialize
	std::string file;
	std::vector<std::string> data;

	//choose which dataset
	int file_type;
	std::cout << "Enter \"1\" for Big File, \"2\" for Small File" << std::endl;
	std::cin >> file_type;
	
	//open dataset
	if (file_type == 1){
		file = "large.txt";
	}
	else if (file_type == 2) { file = "small.txt"; }
	else { file = "test.txt"; }

	std::ifstream myFile(file); 	

	//convert to vector for ease
	std::string str;
	while (std::getline(myFile, str)){
		data.push_back(str);
	}

	//check output
	/*
	std::string label;
	for (std::string temp : data){
		std::stringstream tester(temp);
		tester >> label;
		std::cout << label << std::endl;
	}
	*/

	//get number of features (number of columns)
	int cols = -1; //first column is not a feature
	std::string s;
	std::string temp;
	s = data.front();
	std::stringstream ss(s);
	while (ss >> temp){
		cols++;
	}

	std::vector<std::string> small_data;
	for (unsigned int i = 0; i < 200; i++){
		small_data.push_back(data.at(i));
	}

	/**********************RUN***********************/
	//clock stuff
	std::clock_t start;
    double duration;

	//algo
	//	feature_search(data, cols);

	backwards(data, cols);

	//more clock stuff
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << "Time: " << duration << std::endl;
	
	/****************C'EST FINI*********************/
	//close file	
	myFile.close();
}
