/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: tgust
 *
 * Created on November 29, 2021, 9:10 PM
 */

#include <cstdlib>
#include<limits>
#include<vector>
#include<ctime>
#include<cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/*
 * 
 */

float leave_one_out_cross_validation(vector<vector<float>>&,vector<int>,int);
float leave_one_out_cross_validation_backward(vector<vector<float>>&,vector<int>,int);
vector<int> forward_search_demo(vector<vector<float>>&);
vector<int> backward_search_demo(vector<vector<float>>&);
bool isempty_intersect(vector<int>, int);
float distance_from_object(vector<vector<float>>&, vector<int>,int,int);
int main(int argc, char** argv) {
    //read data from file
    srand(time(0));
    ifstream data;
    string rowString;
    vector<vector<float>>dataSet;
    vector<int> classifiers;
    //cout<<Enter 1 for Small data
    data.open("Ver_2_CS170_Fall_2021_Small_data__57.txt");
    while(getline(data,rowString)){
        vector<float>rowData;
        stringstream row(rowString);
        float num;
        while(row>>num)rowData.push_back(num);
        dataSet.push_back(rowData);
    }
    /*classifiers=forward_search_demo(dataSet);
    cout<<"The best features are {";
    for(int i=0;i<classifiers.size();i++){
        cout<<classifiers[i]<<" ";
    }
    cout<<"}\n";*/
    classifiers=backward_search_demo(dataSet);
    cout<<endl;
    cout<<"The best features are {";
    for(int i=0;i<classifiers.size();i++){
        cout<<classifiers[i]<<" ";
    }
    cout<<"}";
    return 0;
}

bool isempty_intersect(vector<int> feat,int check){
    for (int i=0; i<feat.size();i++){
        if (feat[i]==check)return false;
    }
    return true;
}

float distance_from_object(vector<vector<float>>& data, vector<int> features,int vertex,int d){
    float dist=0;
    for(int j=0;j<features.size();j++){
            dist+=(data[vertex][features[j]]-data[d][features[j]])*(data[vertex][features[j]]-data[d][features[j]]);
    }
   
    return dist;
}

float leave_one_out_cross_validation(vector<vector<float>>&data,vector<int>current_set,int feature_to_add){
    float accuracy;
    int number_correctly_classified=0;
    vector<int> test=current_set;
    test.push_back(feature_to_add);
    for(int i=0; i<data.size();i++){
        vector<float>object_to_classify=data[i];
        float label_object_to_classify=data[i][0];
        float nearest_neighbor_distance=numeric_limits<float>::max();
        float nearest_neighbor_label;
        int nearest_neighbor_location=numeric_limits<int>::max();
        for(int k=0;k<data.size();k++){
            if(k!=i){
                float distance=distance_from_object(data,test,i,k);//implement distance calculation function and assign to distance
                if(distance<nearest_neighbor_distance){
                    nearest_neighbor_distance=distance;
                    nearest_neighbor_location=k;
                    nearest_neighbor_label=data[nearest_neighbor_location][0];
                }
            }
        }
        if(label_object_to_classify==nearest_neighbor_label){
            number_correctly_classified++;
        }

    }
    accuracy=(float)number_correctly_classified/(float)data.size();
    return accuracy;

}

vector<int> forward_search_demo(vector<vector<float>>& data){
    vector<int> current_set_of_features;
    vector<int> best_set_of_features;
    float best_accuracy=0;
    for (int i=1;i<data[i].size();i++){
        cout<<"On level "<<i<<" of the search tree\n";
        int feature_to_add_at_this_level;
        float best_so_far_accuracy=0;
        for(int k=1; k<data[i].size();k++){
            if(isempty_intersect(current_set_of_features,k)){//need to implement is empty and intersect
                cout<<"--Considering adding the "<<k<<" feature\n";
                float accuracy=leave_one_out_cross_validation(data,current_set_of_features,k);
                if(accuracy>best_so_far_accuracy){
                    best_so_far_accuracy=accuracy;
                    feature_to_add_at_this_level=k;
                    //cout<<feature_to_add_at_this_level<<endl;
                }
                cout<<"The accuracy is "<<accuracy<<endl;
            }
        }
        current_set_of_features.push_back(feature_to_add_at_this_level);
        cout<<"On level "<<i<<" I added feature "<<feature_to_add_at_this_level<<" to current set\n";
        cout<<"The current set is {";
        for(int i=0;i<current_set_of_features.size();i++)cout<<current_set_of_features[i]<<" ";
        cout<<"}\n";
        cout<<"This gives an accuracy of "<<best_so_far_accuracy<<endl;;
        if(best_accuracy<best_so_far_accuracy){
            best_accuracy=best_so_far_accuracy;
            best_set_of_features=current_set_of_features;
        }
        //else if(best_so_far_accuracy<best_accuracy)return best_set_of_features;
    }
    return best_set_of_features;

}
vector<int> backward_search_demo(vector<vector<float>>& data){
    vector<int> current_set_of_features;
    for(int i=1;i<data[i].size();i++)current_set_of_features.push_back(i);
    vector<int> best_set_of_features;
    float best_accuracy=0;
    for (int i=1;i<data[i].size();i++){
        cout<<"On the "<<i<<"th level of the search tree\n";
        int feature_to_remove_at_this_level;
        float best_so_far_accuracy=0;
        for(int k=1; k<data[i].size();k++){
            if(!isempty_intersect(current_set_of_features,k)){//need to implement is empty and intersect
                cout<<"--Considering removing the "<<k<<" feature\n";
                float accuracy=leave_one_out_cross_validation_backward(data,current_set_of_features,k);
                if(accuracy>best_so_far_accuracy){
                    best_so_far_accuracy=accuracy;
                    feature_to_remove_at_this_level=k;
                    //cout<<feature_to_add_at_this_level<<endl;
                }
                cout<<"The accuracy is "<<accuracy<<endl;
            }
        }
        for(int i=0;i<current_set_of_features.size();i++)if(current_set_of_features[i]==feature_to_remove_at_this_level)current_set_of_features.erase(current_set_of_features.begin()+i);
        cout<<"On level "<<i<<" I removed feature "<<feature_to_remove_at_this_level<<" from current set\n";
        cout<<"The current set is {";
        for(int i=0;i<current_set_of_features.size();i++)cout<<current_set_of_features[i]<<" ";
        cout<<"}\n";
        cout<<"This gives an accuracy of "<<best_so_far_accuracy<<endl;;
        if(best_accuracy<best_so_far_accuracy){
            best_accuracy=best_so_far_accuracy;
            best_set_of_features=current_set_of_features;
        }
        //else if(best_so_far_accuracy<best_accuracy)return best_set_of_features;
    }
    return best_set_of_features;

}
float leave_one_out_cross_validation_backward(vector<vector<float>>&data,vector<int>current_set,int feature_to_remove){
    float accuracy;
    int number_correctly_classified=0;
    vector<int> test=current_set;
    for(int i=0;i<test.size();i++){
        if(test[i]==feature_to_remove)test.erase(test.begin()+i);
    }
    for(int i=0; i<data.size();i++){
        vector<float>object_to_classify=data[i];
        float label_object_to_classify=data[i][0];
        float nearest_neighbor_distance=numeric_limits<float>::max();
        float nearest_neighbor_label;
        int nearest_neighbor_location=numeric_limits<int>::max();
        for(int k=0;k<data.size();k++){
            if(k!=i){
                float distance=distance_from_object(data,test,i,k);//implement distance calculation function and assign to distance
                if(distance<nearest_neighbor_distance){
                    nearest_neighbor_distance=distance;
                    nearest_neighbor_location=k;
                    nearest_neighbor_label=data[nearest_neighbor_location][0];
                }
            }
        }
        if(label_object_to_classify==nearest_neighbor_label){
            number_correctly_classified++;
        }

    }
    accuracy=(float)number_correctly_classified/(float)data.size();
    return accuracy;


}