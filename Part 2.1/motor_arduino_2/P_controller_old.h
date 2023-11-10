#ifndef P_CONTROLLER_H
#define P_CONTROLLER_H

class P_controller {
  public:
    P_controller(double Kp) : Kp(Kp) {}

    double update(double ref, double actual) {
      double error = ref - actual;
      double u = Kp * error;
      return u;
    }

  private:
  double Kp;
};
#endif