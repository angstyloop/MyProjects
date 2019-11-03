function succeed {
  echo succeeded
  return 0
}
function fail {
  echo failed
  return 1
}
waitPeriod=1         # Initial waiting time. Double each time for exponential backoff.
numAttempts=7        # 7 attempts, doubling each time, works out to ~2 min.
function tryThing {
  echo $waitPeriod
  sleep $waitPeriod
  ((waitPeriod*=2))  # The (()) syntax is called arithmetic expansion.
  # Pick one...
  #######
  #fail
  succeed
  #######
  return             # Returns the most recent exit code (i.e. for `aws s3 cp`)
}
for i in $(seq 1 $numAttempts); do
  if tryThing; then # break loop if successful. 
    break
  fi
done
