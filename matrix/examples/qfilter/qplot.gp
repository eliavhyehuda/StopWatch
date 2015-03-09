# Plot two quaternion time series.

# set ticslevel 0
plot '$0' every ::0 using 1:2 title "$0: q0" with lines \
     , "" every ::0 using 1:3 title "q1" with lines \
     , "" every ::0 using 1:4 title "q2" with lines \
     , "" every ::0 using 1:5 title "q3" with lines \
     , '$1' every ::0 using 1:2 title "$1: q0" with lines \
     , "" every ::0 using 1:3 title "q1" with lines \
     , "" every ::0 using 1:4 title "q2" with lines \
     , "" every ::0 using 1:5 title "q3" with lines

