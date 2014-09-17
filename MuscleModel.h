#pragma once
#include <stdio.h>

class MuscleModel
{
	static const int HILL_TYPE_MODEL = 1;

	static int past_spike_1;
	static int past_spike_2;
	static double past_h_1;
	static double past_h_2;

	int sample();
	static double s(double);
	double active_state(double, double, double, double, double);
	int* spike_train(double, int);
	double* gen_h_diff_eq(int);
	double* plotForce(int, double*);
	double* gen_waveform(double, double, double);
	static double d_force(double, double, double, double);

	
	//double (*algorithm)(double,double,double,double);

public:
	MuscleModel(void);
	MuscleModel(int);
	~MuscleModel(void);

	//const int MUSCLE_MODEL_1 = 1;
	
	static double stepping_model(double, double, double, double);
	double advance_simulation(double, double, double, double);
	double test(double, double, double, double);
	double get_past_spike_1(void);
	double get_past_spike_2(void);
	double get_past_h_1(void);
	double get_past_h_2(void);
	void set_past_h_1(double);
	void set_past_h_2(double);
	void set_past_spike_1(double);
	void set_past_spike_2(double);
	double past_T;
	double T;	
	double dT;
};

