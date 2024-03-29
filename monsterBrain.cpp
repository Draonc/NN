// monsterBrain.cpp : Defines the entry point for the console application.
//

//Author Candice Sandefur

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

struct node {
	float* weights;
	float input;
	float output;
	float error;
	float desiredOutput;
	float errorGradient;
	float outputs;

	node(int O) {
		outputs = O;

		srand(time(0));

		weights = new float[outputs];
		for (int i = 0; i < outputs; i++) {
			weights[i] = (float)rand() / (float)RAND_MAX;
		}

		input = 0;
		output = 0;
		error = 0;
		desiredOutput = 0;
		errorGradient = 0;
	}
};

struct layer {
	float numberNodes;
	float numberOutputs;
	node* nodes;
	
	layer (int numberN, int numberO) {
		numberNodes = numberN;
		numberOutputs = numberO;
		nodes = (node*)malloc(sizeof(node) * numberNodes);
		for (int i = 0; i < numberNodes; i++) {
			nodes[i] = node(numberOutputs);
		}
	}	
};

layer inputs(34, 10);
layer hidden(10, 17);
layer output(17, 0);


float e = 2.71828;
float alpha = .8;
float theta = 0;
float netError = 1;


//Possible inputs that this could recieve
float caught[] = {5, 0, 0, 1, 3, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 1, 0};
float alert[] = {5, 6, 0, 9, 0, 10, 10, 10, 2, 6, 7, 8, 9, 1, 0, 5, 9};

void train();
void init(float* capture, float* alert);
void calculate(int number);
void calculateError(float hp);
void errorGradient(int number);
void updateWeights(int number);
float sigmoid(float input);
void normalize(layer layer);
void caculateDesiredOutput();
void netErrorC();

int main()
{
	init(caught, alert);
	caculateDesiredOutput();
	int iterations = 0;

	while (netError > .5 && iterations < 100) {
		train();

		printf("================= %d =====================\n", iterations);
		iterations++;


	}
	
    return 0;
}

void train() {
	calculate(1);
	calculate(2);
	calculateError(16);
	errorGradient(2);
	updateWeights(2);
	errorGradient(1);
	updateWeights(1);
}

void init(float* capture, float* alert) {

	for (int i = 0; i < inputs.numberNodes / 2; i++) {
		inputs.nodes[i].input = capture[i];
		inputs.nodes[i + 17].input = alert[i];
	}

	normalize(inputs);
}

void calculate(int number) {
	switch (number) {
	case 0:
		break;
	case 1:
		for (int i = 0; i < hidden.numberNodes; i++) {
			hidden.nodes[i].input = 0;
			for (int j = 0; j < inputs.numberNodes; j++) {
				hidden.nodes[i].input += (inputs.nodes[j].output * inputs.nodes[j].weights[i]);
			}
		}

		normalize(hidden);

		break;
	case 2:
		for (int i = 0; i < output.numberNodes; i++) {
			output.nodes[i].input = 0;
			for (int j = 0; j < hidden.numberNodes; j++) {
				output.nodes[i].input += hidden.nodes[j].output * hidden.nodes[j].weights[i];
			}
		}

		for (int i = 1; i < output.numberNodes; i++) {
		}
		normalize(output);
		break;
	}
}

void calculateError(float hp) {
		for (int i = 0; i < output.numberNodes; i++) {
			output.nodes[i].error = output.nodes[i].desiredOutput - output.nodes[i].output;
			
			//This is the error used in the game.
			//output.nodes[i].error = ((hp - inputs.nodes[i].output)/ hp);
		}

		for (int i = 0; i < output.numberNodes; i++) {
			printf("error at %d is %f,   %f   %f \n" , i, output.nodes[i].error, output.nodes[i].desiredOutput, output.nodes[i].output);
		}
}

void errorGradient(int number) {
	float errorSum = 0;
	switch (number) {
	case 0:
		break;
	case 1:
		for (int i = 0; i < hidden.numberNodes; i++) {
			errorSum = 0;
			for (int j = 0; j < output.numberNodes; j++) {
				errorSum += (output.nodes[j].errorGradient * hidden.nodes[i].weights[j]);
			}
			hidden.nodes[i].errorGradient = hidden.nodes[i].output * (1 - hidden.nodes[i].output) * output.nodes[i].error * errorSum;
		}
		break;
	case 2:
		for (int i = 0; i < output.numberNodes; i++) {
			output.nodes[i].errorGradient = (output.nodes[i].output * (1 - output.nodes[i].output) * output.nodes[i].error);
		}
		break;
	}
}

void updateWeights(int number) {
	float weightChange;

	switch (number) {
	case 0:
		break;
	case 1:
		for (int j = 0; j < inputs.numberNodes; j++) {
			for (int i = 0; i < hidden.numberNodes; i++) {
				weightChange = alpha * inputs.nodes[j].output * hidden.nodes[i].errorGradient;
				inputs.nodes[j].weights[i] += weightChange;
			}
		}
		break;
	case 2:
		for (int j = 0; j < hidden.numberNodes; j++) {
			for (int i = 0; i < output.numberNodes; i++) {
				weightChange = alpha * hidden.nodes[j].output * output.nodes[i].errorGradient;
				hidden.nodes[j].weights[i] += weightChange;
			}
		}
		break;
	}

}

float sigmoid(float input) {
	int sol = 0;
	float power = input - theta;

	sol = 1 / (1 + pow(e, -power));

	return sol;
}

void normalize(layer layer) {
	float max = 0;

	for (int n = 0; n < layer.numberNodes; n++) {
		layer.nodes[n].output = fmod(layer.nodes[n].input, .95);
	}

}

void caculateDesiredOutput() {
	float sum = 0;

	for (int j = 0; j < 34; j++) {
		sum += inputs.nodes[j].input;
	}

	for (int i = 1; i < 17; i++) {
		output.nodes[i].desiredOutput = (inputs.nodes[i].input + inputs.nodes[i + 17].input) / sum;
	}
}

void netErrorC() {
	netError = 0;
	for (int i = 0; i < output.numberNodes; i++) {
		netError += output.nodes[i].error;
	}
}


