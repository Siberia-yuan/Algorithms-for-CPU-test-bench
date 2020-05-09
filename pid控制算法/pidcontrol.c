#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pidcontrol.h"

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    double homorand(void);
    double modrest(double,double);
    double u_1=0.0;
    double u_2=0.0;
    double u_3=0.0;
    double r_1=homorand();
    double y_1=0.0;
    double y_2=0.0;
    double y_3=0.0;
    double x_1=0.0;
    double x_2=0.0;
    double x_3=0.0;
    double error_1=0;
    double sigTime[simuLength];
    double rin[simuLength];
    double vr[simuLength];
    double u[simuLength];
    //initialize pid parameters
    double kp=1;
    double ki=2;
    double kd=0.01;
    double yout[simuLength];
    double error[simuLength];
    //recording data
    FILE *fp;
    FILE *fp_1;
    fp=fopen("pidControlData.dat","w");
    fp_1=fopen("targetData.dat","w");
    if(signalSource==3){
        fprintf(fp, "randomSignal pidControl\n");
        fprintf(fp_1, "randomSignal\n");
    }
    if(signalSource==1){
        fprintf(fp,"triangleSignal pidControl\n");
        fprintf(fp_1,"triangleSignal\n");
    }
    if(signalSource==2){
        fprintf(fp,"sawtoothSignal pidControl\n");
        fprintf(fp_1, "sawtoothSignal\n");
    }
    for(int k=1;k<=simuLength;k++){
        sigTime[k-1]=k*ts;
        //random signal
        if(signalSource==3){
            rin[k-1]=homorand();
            vr[k-1]=(rin[k-1]-r_1)/ts;
            while(fabs(vr[k-1])>=5.0){
                rin[k-1]=homorand();
                vr[k-1]=(rin[k-1]-r_1)/ts;
            }
        }
        //triangle signal
        if(signalSource==1){
            if(modrest(sigTime[k-1], 2)<1){
                rin[k-1]=modrest(sigTime[k-1],1);
            }else{
                rin[k-1]=1-modrest(sigTime[k-1], 1);
            }
            rin[k-1]=rin[k-1]-0.5;
        }
        //sawtooth signal
        if(signalSource==2){
            rin[k-1]=modrest(sigTime[k-1], 1);
        }
        //pid controller
        u[k-1]=kp*x_1+kd*x_2+ki*x_3;
        //restricting the out put of the controller
        if(u[k-1]>=10){
            u[k-1]=10;
        }
        if(u[k-1]<=-10){
            u[k-1]=-10;
        }
        //linear model
        yout[k-1]=2.9063*y_1-2.8227*y_2+0.9164*y_3+0.0000853*u_1+0.0003338*u_2+0.0000817*u_3;
        error[k-1]=rin[k-1]-yout[k-1];
        r_1=rin[k-1];
        u_3=u_2;
        u_2=u_1;
        u_1=u[k-1];
        y_3=y_2;
        y_2=y_1;
        y_1=yout[k-1];
        
        x_1=error[k-1];
        x_2=(error[k-1]-error_1)/ts;
        x_3=x_3+error[k-1]*ts;
        error_1=error[k-1];
        printf("%f %f\n",rin[k-1],yout[k-1]);
        fprintf(fp,"%f\n",yout[k-1]);
        fprintf(fp_1, "%f\n",rin[k-1]);
    }
    return 0;
}

//Uniform distrubution function generator
double homorand(){
    double j=(double)rand()/(RAND_MAX);
    return j;
}

double modrest(double x,double y){
    while(x>=y){
        x=x-y;
    }
    return x;
}

