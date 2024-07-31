#include <stdio.h>
#include <stdlib.h>

// Define a structure for your dataset
typedef struct {
    int feature1;
    int feature2;
    // Add more features as needed
    int label;  // Binary class label (0 or 1)
} DataInstance;

// Function to train the Naive Bayes classifier
void train(DataInstance *data, int numInstances, double *priorProbabilities, double **conditionalProbabilities) {
    // Count occurrences of each class
    int classCounts[2] = {0}; // Assuming binary classification
    
    // Count occurrences of each feature value given each class
    int featureCounts[2][2][2] = {0}; // Assuming features are binary
    
    // Count occurrences
    for (int i = 0; i < numInstances; ++i) {
        int label = data[i].label;
        classCounts[label]++;
        featureCounts[label][0][data[i].feature1]++;
        featureCounts[label][1][data[i].feature2]++;
        // Add more features as needed
    }
    
    // Calculate prior probabilities
    for (int c = 0; c < 2; ++c) {
        priorProbabilities[c] = (double)classCounts[c] / numInstances;
    }
    
    // Calculate conditional probabilities with Laplace smoothing
    for (int c = 0; c < 2; ++c) {
        for (int f = 0; f < 2; ++f) {
            for (int v = 0; v < 2; ++v) {
                conditionalProbabilities[c][f * 2 + v] = (double)(featureCounts[c][f][v] + 1) / (classCounts[c] + 2);
            }
        }
    }
}

// Function to classify a new instance
int classify(int feature1, int feature2, double *priorProbabilities, double **conditionalProbabilities) {
    double posteriorProbabilities[2];
    
    // Calculate posterior probabilities
    for (int c = 0; c < 2; ++c) {
        posteriorProbabilities[c] = priorProbabilities[c] *
                                    conditionalProbabilities[c][0 * 2 + feature1] *
                                    conditionalProbabilities[c][1 * 2 + feature2];
        // Add more features as needed
    }
    
    // Determine the class with the highest posterior probability
    if (posteriorProbabilities[0] > posteriorProbabilities[1]) {
        return 0;
    } else {
        return 1;
    }
}

int main() {
    // Example dataset
    DataInstance data[] = {
        {1, 0, 0},  // Example instance 1: feature1=1, feature2=0, label=0
        {0, 1, 1},  // Example instance 2: feature1=0, feature2=1, label=1
        // Add more instances as needed
    };
    int numInstances = sizeof(data) / sizeof(data[0]);
    
    // Prior and conditional probabilities
    double priorProbabilities[2];
    double *conditionalProbabilities[2];
    
    for (int i = 0; i < 2; ++i) {
        conditionalProbabilities[i] = (double *)malloc(4 * sizeof(double)); // 4 for 2 features with 2 possible values each
    }
    
    // Train the model
    train(data, numInstances, priorProbabilities, conditionalProbabilities);
    
    // Example classification
    int newFeature1 = 1;
    int newFeature2 = 0;
    int predictedClass = classify(newFeature1, newFeature2, priorProbabilities, conditionalProbabilities);
    
    printf("Predicted class for (feature1=%d, feature2=%d): %d\n", newFeature1, newFeature2, predictedClass);
    
    // Free allocated memory
    for (int i = 0; i < 2; ++i) {
        free(conditionalProbabilities[i]);
    }
    
    return 0;
}
