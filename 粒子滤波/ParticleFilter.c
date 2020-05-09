#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.141592654

int main(int argc, const char * argv[]) {
    void particleFilter(double initialState,double processCov,double measurementCov,int simuLength,double particleLength);
    particleFilter(10, 1, 1, 100, 2);
    return 0;
}

void particleFilter(double initialState,double processCov,double measurementCov,int simuLength,double particleLength)
{
    FILE *fp;
    FILE *fp_1;
    fp=fopen("FilterData.dat","w");
    fp_1=fopen("InitialData.dat", "w");
    srand((int)time(0));
    double homorand(void);
    double gaussrand(void);
    double x=initialState;//initial state of data;
    double pro=processCov;//process noise covariance
    double mea=measurementCov;//measurement noise covariance
    int length=simuLength;//times of simulation
    int N=100;//numbers of particles
    
    double xAll[length+1];
    double yAll[length+1];
    double xMeanParticles[length+1];
    double xParticles[N];
    
    for(int i=0;i<N;i++){
        xParticles[i]=x+sqrt(particleLength)*gaussrand();
    }
    
    xAll[0]=x;
    double y=pow(x,2)/20+sqrt(mea)*gaussrand();//varies according to the condition
    //weight measurement equation
    yAll[0]=y;
    xMeanParticles[0]=x;
    double xpartminus[N];
    double q[N];
    double u;
    double qtemsum;
    for(int k=0;k<length;k++){
        //the following conditions varies according to the exact condition
        //System Simulation
        x=0.5*x+25*x/(1+pow(x,2))+8*cos(1.2*(k-1))+sqrt(pro)*gaussrand();//state equation
        y=pow(x,2)/20+sqrt(mea)*gaussrand();//measurement equation
        
        //Particle filter
        for(int j=0;j<N;j++){
            xpartminus[j]=0.5*xParticles[j]+25*xParticles[j]/(1+pow(xParticles[j],2))+8*cos(1.2*(k-1))+sqrt(pro)*gaussrand();
            double ypart=pow(xpartminus[j],2)/20;
            double differ=y-ypart;
            q[j]=(1/(sqrt(mea*2*PI)))*exp(-pow(differ,2)/(2*mea));
        }
        //normalize
        double sum=0;
        for(int j=0;j<N;j++){
            sum+=q[j];
        }
        for(int j=0;j<N;j++){
            q[j]=q[j]/sum;
        }
        //resample
        for(int j=0;j<N;j++){
            u=homorand();
            qtemsum=0;
            for(int t=0;t<N;t++){
                qtemsum+=q[t];
                if(qtemsum>=u){
                    xParticles[j]=xpartminus[t];
                    break;
                }
            }
        }
        double sumParticles=0;
        for (int j=0; j<N; j++) {
            sumParticles+=xParticles[j];
        }
        xMeanParticles[k+1]=sumParticles/N;
        xAll[k+1]=x;
        yAll[k+1]=y;
        //recording the data
    }
    printf("TrueState  ParticleFilter\n");
    fprintf(fp,"%s","ParticleFilter\n");
    fprintf(fp_1, "%s","TrueStata\n");
    for(int p=0;p<=length;p++){
        printf("%10f  %10f\n",xAll[p],xMeanParticles[p]);
        fprintf(fp,"%10f\n",xMeanParticles[p]);
        fprintf(fp_1, "%10f\n",xAll[p]);
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
    }else{
        Z = sqrt(-2.0 * log(U)) * cos(2.0 * PI * V);
    }
    phase = 1 - phase;
    return Z;
}

double homorand(){
    double j=(double)rand()/(RAND_MAX);
    return j;
}
