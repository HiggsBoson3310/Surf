#include <iostream> 
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <complex>
#include <random> //Random number library
using namespace std;
const double PI=3.14159265359, h = 6.626e-34, mue = 928.5e-26, z=2*PI*(mue/h);
//Matrix-Vector Multiplication
vector<complex<double>> MM(vector<complex<double>> M, vector<complex<double>> a){
    vector<complex<double>> r{complex<double> (0,0),complex<double> (0,0),complex<double> (0,0)};
    r.at(0)=M.at(0)*a.at(0)+M.at(1)*a.at(1)+M.at(2)*a.at(2);
    r.at(1)=M.at(3)*a.at(0)+M.at(4)*a.at(1)+M.at(5)*a.at(2);
    r.at(2)=M.at(6)*a.at(0)+M.at(7)*a.at(1)+M.at(8)*a.at(2);
    return r;
}
//Inner product
complex<double> inner(vector<complex<double>> b, vector<complex<double>> a){
    complex<double> r=0;
    for(int j=0; j<b.size(); j++){
        r+=conj(b.at(j))*a.at(j);
    }
    return r;
}
double inner(vector<double> b, vector<double> a){
    double r=0;
    for(int j=0; j<a.size(); j++){
        r+=b.at(j)*a.at(j);
    }
    return r;
}
//Matrix-Matrix multiplication
vector<complex<double>> Msq(vector<complex<double>> M, vector<complex<double>> A){
    vector<complex<double>> r(A.size(), 0);
    r.at(0)=M.at(0)*A.at(0)+M.at(1)*A.at(3)+M.at(2)*A.at(6);
    r.at(1)=M.at(0)*A.at(1)+M.at(1)*A.at(4)+M.at(2)*A.at(7);
    r.at(2)=M.at(0)*A.at(2)+M.at(1)*A.at(5)+M.at(2)*A.at(8);
    r.at(3)=M.at(3)*A.at(0)+M.at(4)*A.at(3)+M.at(5)*A.at(6);
    r.at(4)=M.at(3)*A.at(1)+M.at(4)*A.at(4)+M.at(5)*A.at(7);
    r.at(5)=M.at(3)*A.at(2)+M.at(4)*A.at(5)+M.at(5)*A.at(8);
    r.at(6)=M.at(6)*A.at(0)+M.at(7)*A.at(3)+M.at(8)*A.at(6);
    r.at(7)=M.at(6)*A.at(1)+M.at(7)*A.at(4)+M.at(8)*A.at(7);
    r.at(8)=M.at(6)*A.at(2)+M.at(7)*A.at(5)+M.at(8)*A.at(8);
    return r;
}
//Vector addition
vector<complex<double>> add_M(vector<complex<double>> b, vector<complex<double>> a){
    vector<complex<double>> r(a.size(), 0);
    for(int i=0; i<a.size();i++){
        r.at(i)=b.at(i)+a.at(i);
    }
    return r;
}
vector<double> add_M(vector<double> b, vector<double> a){
    vector<double> r(a.size(), 0);
    for(int i=0; i<a.size();i++){
        r.at(i)=b.at(i)+a.at(i);
    }
    return r;
}
//Scalar multiplication
vector<complex<double>> MS(complex<double> b, vector<complex<double>> a){
    vector<complex<double>> r(a.size(), 0);
    for(int i=0; i<a.size();i++){
        r.at(i)=b*a.at(i);
    }
    return r;
}
vector<double> MS(double b, vector<double> a){
    vector<double> r(a.size(), 0);
    for(int i=0; i<a.size();i++){
        r.at(i)=b*a.at(i);
    }
    return r;
}
//Cross product
vector<double> cross(vector<double> a, vector<double> b){
    vector<double> r(3,0);
    r.at(0)=a.at(1)*b.at(2)-b.at(1)*a.at(2);
    r.at(1)=a.at(2)*b.at(0)-b.at(2)*a.at(0);
    r.at(2)=a.at(0)*b.at(1)-b.at(0)*a.at(1);
    return r;
}
//function that obtains an orthogonal unitary vector to B
vector<double> ortho(vector<double> B){
    vector<double> b_unit, L, T, a={0,0,1};
    b_unit = MS(1/sqrt(inner(B,B)),B);
    double dot = inner(a,b_unit);
    L = MS(-dot,b_unit);
    T = add_M(a,L);
    return MS(1/sqrt(inner(T,T)),T);
}
//Function that generates a hamiltonian given a field.
vector<complex<double>> Hamil(vector<complex<double>> B){
    //Spin matrices of S=1
    complex<double> i(0,1);
    vector<complex<double>> Sx={0,1/sqrt(2),0,1/sqrt(2),0,1/sqrt(2),0,1/sqrt(2),0},Sy={0,-i/sqrt(2),0,i/sqrt(2),0,-i/sqrt(2),0,i/sqrt(2),0},Sz={1,0,0,0,0,0,0,0,-1},H;
    H=add_M(MS(mue*B.at(0),Sx),MS(mue*B.at(1),Sy)); H=add_M(H,MS(mue*B.at(2),Sz));
    return H;
}
//Expectation value
complex<double> obs(vector<complex<double>> O, vector<complex<double>> v){
    return inner(v,MM(O,v));
}
//Spin projection in the direction of the field
vector<complex<double>> Sp1(vector<complex<double>> b){
    complex<double> i(0,1);
    vector<complex<double>> Sx={0,1/sqrt(2),0,1/sqrt(2),0,1/sqrt(2),0,1/sqrt(2),0},Sy={0,-i/sqrt(2),0,i/sqrt(2),0,-i/sqrt(2),0,i/sqrt(2),0},Sz={1,0,0,0,0,0,0,0,-1}, Sp;
    Sp = add_M(MS(b.at(0),Sx),MS(b.at(1),Sy)); Sp=add_M(Sp,MS(b.at(2),Sz));
    return Sp;
    
}
//Time varyiing field
vector<complex<double>> B_field(vector<double> x, double Bo, double Bq, double Bc){
    vector<complex<double>> r={0,0,0};
    r.at(0)=Bo*(4*pow(x.at(0),3)-12*pow(x.at(1),2)*x.at(0))-Bq*(6*x.at(2)*x.at(0));
    r.at(1)=Bo*(-12*pow(x.at(0),2)*x.at(1)+4*pow(x.at(1),3))-Bq*(6*x.at(1)*x.at(2));
    r.at(2)=Bc+Bq*(6*x.at(2)*x.at(2)-3*(x.at(1)*x.at(1)+x.at(0)*x.at(0)));
    return r;
}
//-------------------------------------------Time Steps-----------------------------------------------------
vector<complex<double>> RK(vector<complex<double>> v, vector<complex<double>> Ht, vector<complex<double>> Hdt, double dt){
    complex<double> D(0,-dt);   
    vector<complex<double>> Tdt=MS(D,Hdt), Tt=MS(D,Ht), T2=MS(0.5,Msq(Tdt,Tt));
    return  add_M(add_M(v,MM(Tdt,v)),MM(T2,v));
}
//Crank-Nicolson
vector<complex<double>> CN(vector<complex<double>> v, vector<complex<double>> H, double dt){
    complex<double> D(0,0.5*dt*2*PI*(1/h));
    vector<complex<double>> I={1,0,0,0,1,0,0,0,1}, A=add_M(I,MS(D,H)), phi={0,0,0}, psi=MS(-1,v);
    phi.at(1)=v.at(1)-(A.at(5)/A.at(8))*v.at(2)-(A.at(3)/A.at(0))*v.at(0);
    phi.at(1)=phi.at(1)/(A.at(4)-(A.at(3)/A.at(0))*A.at(1)-(A.at(5)/A.at(8))*A.at(7));
    phi.at(0)=(v.at(0)-A.at(1)*phi.at(1))/(A.at(0));
    phi.at(2)=(v.at(2)-A.at(7)*phi.at(1))/(A.at(8));
    return add_M(MS(2,phi),psi);
}
int main(){
    cout << setprecision(9) << scientific;
    //Uniformly distributed random numbers in the interval [0,1].
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0,1.0);
    //Variables in which the random number is stored.
    //For initial position (e.i. the velocity direction)
    double cthr=0, phr=0;
    //For the impact parameter
    double alpha=0, imp_N=0;
    //Maximum Montecarlo tries:
    int N_Mc=25000;
    //Integrations: one if for the montecarlo and the otherone for the approximate expression b_mx^2*PI*<P>
    double mont=0, app_i=0, mont1=0;
    //Output files
    /*ofstream  temp_f, field, pos, lulz;
    lulz.open("lulz.txt");
    pos.open("pos1.txt"), field.open("field1.txt"), temp_f.open("temp1.txt");
    lulz << setprecision(9) << scientific;
    pos<< setprecision(9) << scientific;
    field<< setprecision(9) << scientific;
    temp_f << setprecision(9) << scientific;
    ofstream probu, probz ,probd , speed, prob;
    prob.open("prob1.txt");
    probu.open("probu.txt");
    probz.open("probz.txt");
    probd.open("probd.txt");
    speed.open("speed1.txt");
    prob << setprecision(9) << scientific;
    probu << setprecision(9) << scientific;
    probz << setprecision(9) << scientific;
    probd << setprecision(9) << scientific; 
    speed << setprecision(9) << scientific;*/
    //Imaginary unit
    complex<double> i(0,1);
    //Octopole and linear amplitudes
    double Bo=8595.766069, Bq=10, Bc=0;
    //Velocity and initial position directions and magnitudes
    double Vo=70;
    //Position, Velocity and impact prameter vectors
    vector<double> v, xo, imp;
    //Initial B-fields
    vector<complex<double>> B, b, Sph, HCNh;
    //Initial projections
    //Sph=SpH(b);
    //B field direction and magnitude
    double BM, th, ph;
    //Landau-Zener factors
    double wN, A, C, K;
    vector<double> Bv,Bimp, uBv, CB;
    //Definition of the initial states as the up direction eigenstate 
    vector<complex<double>> psihCN, psiCN2, psiCN3, psihf, psihf2, psihf3;
    //Time constants
    double Rp=4e-3, Tf, dt, b_max=0.1e-3, l, uu=0, ud=0, uz=0, dd=0, dz=0, zz=0;
    vector<double> e1, e2, temp;
    int k; 
    cout << "fine" << endl;
    for(int m=0; m<N_Mc; m++){
        //Generation of the random position parameters
        //Random polar angle
        cthr = 2*dis(gen)-1;
        //Random azimuthal angle
        phr = 2*PI*dis(gen);
        //Randon cylindrical angle for the impact parameter
        alpha = 2*PI*dis(gen);
        //Random magnitude of the impact parameter
        imp_N = b_max*sqrt(dis(gen));
        //Direction oposed to the velocity.n);
        temp = {-sqrt(1-cthr*cthr)*cos(phr),-sqrt(1-cthr*cthr)*sin(phr),-cthr};
        //Orthogonal vectors that form a basis with the direction of movement as the z direction.
        e1 = ortho(temp);
        e2 = cross(e1,temp);
        //Initial position
        xo = MS(1.5*Rp,temp);
        imp = add_M(MS(imp_N*cos(alpha),e1),MS(imp_N*sin(alpha),e2));
        xo = add_M(xo,imp);
        //Velocity
        v = MS(-Vo,temp);
        //Final time and time-step.
        Tf=sqrt(inner(xo,xo))/Vo;
        //Initial field and unit vector in the field direction
        B=B_field(xo,Bo,Bq,Bc);
        b=MS(1/sqrt(real(inner(B,B))),B);
        dt=0.5*(0.01)*(h/(sqrt(real(inner(B,B)))*mue*2*PI));
        cout << Tf << " "<< dt << endl;
        k= -2*Tf/dt;
        cout << -2*k << endl;
        //Instantaneous spin projections
        Sph=Sp1(b);
        //Spherical coordinates of the field.
        BM=real(sqrt(inner(B,B)));
        th=acos(real(B.at(2))/BM);
        if(real(B.at(1))>=0){ph=acos(real(B.at(0))/(BM*sin(th)));}
        else{ph=2*PI-acos(real(B.at(0))/(BM*sin(th)));}
        //Initial states. We start the system in the up state of the local initial field. 
        psihCN={0.5*(1+cos(th))*exp(-i*ph),(1/sqrt(2))*sin(th),0.5*(1-cos(th))*exp(i*ph)};
        psiCN2 = {-(1/sqrt(2))*sin(th)*exp(-i*ph),cos(th),(1/sqrt(2))*sin(th)*exp(i*ph)};
        psiCN3 = {0.5*(1-cos(th))*exp(-i*ph),-(1/sqrt(2))*sin(th),0.5*(1+cos(th))*exp(i*ph)};
        //Bv = {-0.5*v.at(0),-0.5*v.at(1),v.at(2)};
        //Bimp = {-0.5*imp.at(0),-0.5*imp.at(1), imp.at(2)};
        //wN=sqrt(inner(Bv,Bv));
        //uBv = MS(1/wN,Bv);
        //CB = {real(B_field(imp,Bo).at(0)),real(B_field(imp,Bo,Bl).at(1)),real(B_field(imp,Bo,Bl).at(2))};
        //CB = add_M(CB,MS(-inner(uBv,CB),uBv));
        //A=Bl*wN;
        //C=sqrt(inner(CB,CB));
        //K = z*C*C/A;
        //Time evolution:
        while(k<=2*Tf/dt){
            //Position update
            //if((k%1000)%150==0){pos <<k*dt<<" "<< xo.at(0)*1e3 <<" "<< xo.at(1)*1e3<<" "<< xo.at(2)*1e3<< endl;}
            xo.at(0)+=v.at(0)*dt;
            xo.at(1)+=v.at(1)*dt;
            xo.at(2)+=v.at(2)*dt;
            //Speed update
            //if((k%1000)%150==0){speed <<k*dt<<" "<< sqrt(real(inner(v,v))) << endl;}
            l=Vo*0.5*(1+erf(2e5*(k*dt+1.5*Tf)))-Vo*0.5*(1+erf(2e5*(k*dt-1.5*Tf)));
            v=MS(-l , temp);
            //Field Update
            //if((k%1000)%150==0){field <<k*dt<<" "<< real(B.at(0)) <<" "<< real(B.at(1))<<" "<< real(B.at(2))<< endl;}
            B=B_field(xo,Bo,Bq,Bc);
            //b=MS(1/sqrt(real(inner(B,B))),B);
            //Update of the projections
            //Sph=SpH(b);
            //Update of the Hamiltonians
            HCNh=Hamil(B);
            //Time evolution of the states
            //if((k%1000)%150==0){temp_f <<k*dt<<" "<< real(obs(Sph,psihCN))<< endl;}
            psihCN=CN(psihCN,HCNh,dt);
            psiCN2=CN(psiCN2,HCNh,dt);
            psiCN3=CN(psiCN3,HCNh,dt);
            //Counter update.
            //BM=real(sqrt(inner(B,B)));
            //th=acos(real(B.at(2))/BM);
            //if(real(B.at(1))>=0){ph=acos(real(B.at(0))/(BM*sin(th)));}
            //else{ph=2*PI-acos(real(B.at(0))/(BM*sin(th)));}
            //Here we find the final down eigenstates
            //psihf={0.5*(1-cos(th))*exp(-i*ph),-(1/sqrt(2))*sin(th),0.5*(1+cos(th))*exp(i*ph)}; 
            //psihf2 = {-(1/sqrt(2))*sin(th)*exp(-i*ph),cos(th),(1/sqrt(2))*sin(th)*exp(i*ph)};
            //psihf3 = {0.5*(1+cos(th))*exp(-i*ph),(1/sqrt(2))*sin(th),0.5*(1-cos(th))*exp(i*ph)};
            //Here we print the probability of a spin flip as the norm of the projection between the final state and the final down eigenstate.
            //if((k%1000)%150==0){lulz << k*dt <<" " << norm(inner(psihCN,psihf2)) <<" "<< norm(inner(psihCN,psihf3)) << " " << norm(inner(psihCN,psihf)) << endl;}
            k+=1;
        }
        //Here we obatin the spherical coordinates for the final magnetic field 
        BM=real(sqrt(inner(B,B)));
        th=acos(real(B.at(2))/BM);
        if(real(B.at(1))>=0){ph=acos(real(B.at(0))/(BM*sin(th)));}
        else{ph=2*PI-acos(real(B.at(0))/(BM*sin(th)));}
        //Here we find the final down eigenstates
        psihf={0.5*(1-cos(th))*exp(-i*ph),-(1/sqrt(2))*sin(th),0.5*(1+cos(th))*exp(i*ph)}; 
        psihf2 = {-(1/sqrt(2))*sin(th)*exp(-i*ph),cos(th),(1/sqrt(2))*sin(th)*exp(i*ph)};
        psihf3 = {0.5*(1+cos(th))*exp(-i*ph),(1/sqrt(2))*sin(th),0.5*(1-cos(th))*exp(i*ph)};
        //Here we print the probability of a spin flip as the norm of the projection between the final state and the final down eigenstate.
        /*probu << norm(inner(psihCN,psihf2)) <<" "<< norm(inner(psihCN,psihf3)) << " " << norm(inner(psihCN,psihf)) << endl;
        probz << norm(inner(psiCN2,psihf2)) <<" "<< norm(inner(psiCN2,psihf3)) << " " << norm(inner(psiCN2,psihf)) << endl;
        probd << norm(inner(psiCN3,psihf2)) <<" "<< norm(inner(psiCN3,psihf3)) << " " << norm(inner(psiCN3,psihf)) << endl;*/
        uu+=b_max*b_max*PI*norm(inner(psihCN,psihf3));
        ud+=b_max*b_max*PI*norm(inner(psihCN,psihf));
        uz+=b_max*b_max*PI*norm(inner(psihCN,psihf2));
        zz+=b_max*b_max*PI*norm(inner(psiCN2,psihf2));
        dz+=b_max*b_max*PI*norm(inner(psiCN2,psihf));
        dd+=b_max*b_max*PI*norm(inner(psiCN3,psihf));
        /*
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " up-->down " << norm(inner(psihCN,psihf)) << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " up-->zero " << norm(inner(psihCN,psihf2))<< endl ;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " up-->up " << norm(inner(psihCN,psihf3))<<endl ;
        prob << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " zero-->down " << norm(inner(psiCN2,psihf)) << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " zero-->zero " << norm(inner(psiCN2,psihf2)) << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " zero-->up " << norm(inner(psiCN2,psihf3))<< endl;
        prob << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " down-->down " << norm(inner(psiCN3,psihf)) << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " down-->zero " << norm(inner(psiCN3,psihf2)) << endl;
        prob << imp_N*cos(alpha)*1e3 << " " << imp_N*1e3*sin(alpha) << " down-->up " << norm(inner(psiCN3,psihf3)) << endl;*/
    /*
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 1-->3 " << (1-exp(-0.5*PI*K))*(1-exp(-0.5*PI*K)) << endl;
        
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 1-->2 " << 2*exp(-0.5*PI*K)*(1-exp(-0.5*PI*K)) << endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 1-->1 " << exp(-PI*K) << endl;
        temp_f<< endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 2-->3 " << 2*exp(-0.5*PI*K)*(1-exp(-0.5*PI*K)) << endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 2-->2 " << (1-2*exp(-0.5*PI*K))*(1-2*exp(-0.5*PI*K)) << endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 2-->1 " << 2*exp(-0.5*PI*K)*(1-exp(-0.5*PI*K)) << endl;
        temp_f << endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 3-->3 " << exp(-PI*K) << endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 3-->2 " << 2*exp(-0.5*PI*K)*(1-exp(-0.5*PI*K)) << endl;
        temp_f << imp_N*cos(alpha)*1e3 << " " << imp_N*sin(alpha)*1e3 << " 3-->1 " << (1-exp(-0.5*PI*K))*(1-exp(-0.5*PI*K)) << endl;*/

        
        //Integration steps:
        //app_i += norm(inner(psihCN,psihf));
        //mont += PI*2*PI*2*PI*b_max*imp_N*norm(inner(psihCN,psihf))*sin(cthr);

        
    }
    
    cout <<"Cross section (ud): " << ud/N_Mc << endl;
    cout <<"Cross section (uz): " << uz/N_Mc << endl;
    cout <<"Cross section (dz): " << dz/N_Mc << endl;
    
    return 0;
}