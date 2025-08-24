#include <Rcpp.h>
#include <RcppEigen.h>

#include "libqbd/inc/libQBD.hpp"

using namespace libQBD;
using namespace Rcpp;
using namespace std;

class QBD_wrappper
{
    public:
    QBD<double> proc;

    QBD_wrappper()
    {

    }

    Eigen::MatrixXd get_A_plus(size_t level)
    {
        return proc.get_A_plus(level);
    }

    Eigen::MatrixXd get_A_0(size_t level)
    {
        return proc.get_A_0(level);
    }

    Eigen::MatrixXd get_A_minus(size_t level)
    {
        return proc.get_A_minus(level);
    }

    void add_zero_level(Eigen::MatrixXd A0, Eigen::MatrixXd Aplus)
    {
        proc.add_zero_level(A0, Aplus);
    }

    void add_level(Eigen::MatrixXd Aminus, Eigen::MatrixXd A0, Eigen::MatrixXd Aplus)
    {
        proc.add_level(Aminus, A0, Aplus);
    }

    void add_final_level(Eigen::MatrixXd Aminus, Eigen::MatrixXd A0)
    {
        proc.add_final_level(Aminus, A0);
    }

    void fix_diagonal()
    {
        proc.fix_diagonal();
    }
};


class TaylorSeriesAdaptive_wrapper
{
    private:
    TaylorSeriesAdaptive<double> series;

    inline List dist_conversion(const vector<vector<Eigen::VectorXd>> &dist)
    {
        List res;
        for(auto it = dist.begin(); it != dist.end(); it++){
            List dist;
            for(auto it_inner = it->begin(); it_inner != it->end(); it_inner++){
                dist.push_back(*it_inner);
            }
            res.push_back(dist);
        }
        return res;
    }
    public:

    TaylorSeriesAdaptive_wrapper(QBD_wrappper proc, List pi0, double error, double max_time)
    {
        std::vector<Eigen::VectorXd> pi0v;
        for(unsigned long k = 0; k < pi0.size(); k++){
            pi0v.push_back(pi0[k]);
        }
        series.bind(proc.proc,  pi0v, error, max_time);
    }
    
    vector<double> get_reference_times()
    {
        return series.get_reference_times();
    }

    List get_reference_dists()
    {
        return dist_conversion(series.get_reference_dists());
    }

    List get_dist (vector<double> times)
    {
        vector<double> errors;
        vector<vector<Eigen::VectorXd>> dist;
        series.get_dist(dist, times, &errors);
        return List::create(Named("errors") = errors, _["distribution"] = dist_conversion(dist));
    }

    vector<double> get_reference_mean_clients()
    {
        return series.get_reference_mean_clients();
    }
};

class StationaryDistribution_wrapper
{
    private:
    StationaryDistribution<double> sd;
    
    List dist_conversion(vector<Eigen::VectorXd> dist)
    {
        List res;
        for(auto it = dist.begin(); it != dist.end(); it++){
            res.push_back(*it);
        }
        return res;
    }
    
    public:
    StationaryDistribution_wrapper(QBD_wrappper proc)
    {
        sd.bind(proc.proc);
    }

    double get_rho()
    {
        return sd.get_rho();
    }

    Eigen::MatrixXd get_R()
    {
        return sd.get_R();
    }

    Eigen::MatrixXd get_G()
    {
        return sd.get_G();
    }

    List get_dist(unsigned int max_level)
    {
        vector<Eigen::VectorXd> dist;
        dist = sd.get_dist(max_level);
        return dist_conversion(dist);
    }

    List get_pi_0_c()
    {
        return dist_conversion(sd.get_pi_0_c());
    }
    
    double get_mean_clients()
    {
        return sd.get_mean_clients();
    }

    Eigen::VectorXd get_sum_from_c_to_inf()
    {
        return sd.get_sum_from_c_to_inf();
    }
};


RCPP_EXPOSED_CLASS(QBD_wrappper)

RCPP_MODULE(master){
    using namespace Rcpp ;

    class_<QBD_wrappper>("QBD")

    .constructor<>()
    
    .method("get_A_plus", &QBD_wrappper::get_A_plus, "")
    .method("get_A_0", &QBD_wrappper::get_A_0, "")
    .method("get_A_minus", &QBD_wrappper::get_A_minus, "")
    .method("add_zero_level", &QBD_wrappper::add_zero_level, "")
    .method("add_level", &QBD_wrappper::add_level, "")
    .method("add_final_level", &QBD_wrappper::add_final_level, "")
    .method("fix_diagonal", &QBD_wrappper::fix_diagonal, "")
    ;

    class_<TaylorSeriesAdaptive_wrapper>("TaylorSeriesAdaptive")

    .constructor<QBD_wrappper, List, double, double>()
    
    .property("get_reference_times", &TaylorSeriesAdaptive_wrapper::get_reference_times, "")
    .property("get_reference_dists", &TaylorSeriesAdaptive_wrapper::get_reference_dists, "")
    .property("get_reference_mean_clients", &TaylorSeriesAdaptive_wrapper::get_reference_mean_clients, "")
    .method("get_dist", &TaylorSeriesAdaptive_wrapper::get_dist, "")
    ;

    class_<StationaryDistribution_wrapper>("StationaryDistribution")

    .constructor<QBD_wrappper>()

    .property("get_rho", &StationaryDistribution_wrapper::get_rho, "")
    .property("get_R", &StationaryDistribution_wrapper::get_R, "")
    .property("get_G", &StationaryDistribution_wrapper::get_G, "")
    .property("get_pi_0_c", &StationaryDistribution_wrapper::get_pi_0_c, "")
    .property("get_mean_clients", &StationaryDistribution_wrapper::get_mean_clients, "")
    .property("get_sum_from_c_to_inf", &StationaryDistribution_wrapper::get_sum_from_c_to_inf, "")
    .method("get_dist", &StationaryDistribution_wrapper::get_dist, "")
    ;
}


