# Try to do something a specified number of times, waiting a specified period each time, and breaking the loop
#  on first success.
function succeed {
  echo success
  return 0
}
function fail {
  echo fail
  return 1
}
numAttempts=100
waitPeriod=5
function tryThing {
  sleep $waitPeriod
  # pick one
  ########
  succeed
  #fail
  ########
  return
}
for i in $(seq 1 $numAttempts); do
  if tryThing; then
    break
  fi
done
