#include <Rcpp.h>
#include <RcppEigen.h>

#include "libqbd/inc/libQBD.hpp"

using namespace libQBD;
using namespace Rcpp;
using namespace std;

class QBD_wrappper
{
    private:
    QBD<double> proc;
    public:

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
}


