#' @export newQBD
#' @export QBD
#' @importFrom methods new

newQBD = function()
{
    return(new(QBD))
}

adaptiveTaylor = function(process, pi0, error, max_time)
{
    return(new(TaylorSeriesAdaptive,process, pi0, error, max_time))
}