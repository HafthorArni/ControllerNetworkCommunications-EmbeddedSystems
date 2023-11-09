class PI_controller {
  public:
    PI_controller(double Kp,double Ti) : Kp(Kp), Ti(Ti) {}

    double update(double ref, double actual) {
      double error = ref - actual;
      if (u<1) E += error*T;
      P = Kp*error;
      I = Kp*E/Ti;
      u = P+I;
      return u;
    }
    double get_integral() {
      return E;
    }
    
  

  private:
    double P;
    double I;
    double Kp;
    double E = 0;
    double T=0.003;
    double u = 0;
    double Ti;
};
