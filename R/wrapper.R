#' @export QBD
#' @export newQBD
#' @export TaylorSeriesAdaptive
#' @export adaptiveTaylor
#' @export StationaryDistribution
#' @export stationaryAnalysis
#' @export function_of_dists
#' @export function_of_dist
#' @export mean_clients_func
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


function_of_dists = function(dist, fun)
{
  ret = c()
  if(length(dist)>0){
    for(j in 1:length(dist)){
      res = 0
      for(k in 1:length(dist[[j]])){
        vec = fun(k-1, length(dist[[j]][[k]]))
        res = res + sum(vec*dist[[j]][[k]])
      } 
      ret = c(ret,res)
    }
    return(ret)
  }else{
    return(NA)
  }
}

mean_clients_func = function(level, len)
{
  return(rep(level,len))
}

function_of_dist = function(dist, fun)
{
  if(length(dist)>0){
    res = 0
    for(k in 1:length(dist)){
      vec = fun(k-1, length(dist[[k]]))
      res = res + sum(vec*dist[[k]])
    } 
    return(res)
  } else{
    return(NA)
  }
}