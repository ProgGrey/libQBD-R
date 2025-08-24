#' @export QBD
#' @export newQBD
#' @export TaylorSeriesAdaptive
#' @export adaptiveTaylor
#' @export StationaryDistribution
#' @export stationaryAnalysis
#' @importFrom methods new

newQBD = function()
{
    return(new(QBD))
}

adaptiveTaylor = function(process, pi0, error, max_time)
{
    return(new(TaylorSeriesAdaptive,process, pi0, error, max_time))
}

stationaryAnalysis = function(process)
{
    return(new(StationaryDistribution,process))
}