#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.141592654

int main(int argc, const char * argv[]) {
    void particleFilter(double initialState,double processCov,double measurementCov,int simuLength,double initialCov);
    particleFilter(10, 1, 1, 100, 2);
    return 0;
}

void particleFilter(double initialState,double processCov,double measurementCov,int simuLength,double initialCov)
{
    FILE *fp;
    FILE *fp_1;
    fp=fopen("KalmanFilterData.dat", "w");
    fp_1=fopen("InitialData.dat","w");
    srand((int)time(0));
    double homorand(void);
    double gaussrand(void);
    double x=initialState;//initial state of data;
    double pro=processCov;//process noise covariance
    double mea=measurementCov;//measurement noise covariance
    int length=simuLength;//times of simulation
    
    double xAll[length+1];
    double yAll[length+1];
    //KalmanFilter forecasting data
    double xhat=x;
    double xhatAll[length+1];
    xhatAll[0]=xhat;
    
    
    
    xAll[0]=x;
    double y=pow(x,2)/20+sqrt(mea)*gaussrand();//varies according to the condition
    //weight measurement equation
    yAll[0]=y;
    
    
    for(int k=0;k<length;k++){
        //the following conditions varies according to the exact condition
        //System Simulation
        x=0.5*x+25*x/(1+pow(x,2))+8*cos(1.2*(k-1))+sqrt(pro)*gaussrand();//state equation
        y=pow(x,2)/20+sqrt(mea)*gaussrand();//measurement equation
        
        //Kalman filter
        double F=0.5+25*(1-pow(xhat,2))/pow(1+pow(xhat,2),2);
        initialCov=F*initialCov*F+pro;
        double H=xhat/10.0;
        double K=initialCov*H*pow(H*initialCov*H+mea,-1);
        xhat=0.5*xhat+25*xhat/(1+pow(xhat,2))+8*cos(1.2*(k-1));
        xhat=xhat+K*(y-pow(xhat,2)/20);
        initialCov=(1-K*H)*initialCov;
        xhatAll[k+1]=xhat;
        xAll[k+1]=x;
        yAll[k+1]=y;
        //recording the data
    }
    printf("TrueState  KalmanFilter\n");
    fprintf(fp,"TrueState  KalmanFilter\n");
    for(int p=0;p<=length;p++){
        printf("%10f  %10f\n",xAll[p],xhatAll[p]);
        fprintf(fp, "%10f\n",xhatAll[p]);
        fprintf(fp,"%10f\n", xAll[p]);
    }
}

//Gaussian distrubution function generator,mith mean at 0,covariance at 1
double gaussrand()
{
    static double U, V;
    static int phase = 0;
    double Z;
    if(phase == 0)
    {
        U = rand() / (RAND_MAX + 1.0);
        V = rand() / (RAND_MAX + 1.0);
        Z = sqrt(-2.0 * log(U))* sin(2.0 * PI * V);
    }
    else
    {
        Z = sqrt(-2.0 * log(U)) * cos(2.0 * PI * V);
    }
    phase = 1 - phase;
    return Z;
}
double homorand(){
    double j=(double)rand()/(RAND_MAX);
    return j;
}



